#include "stdafx.h"
#include "WeaponBase.h"

#include "MobEnemyBase.h"
#include "BossEnemyBase.h"

#include "EnemyManager.h"


void WeaponBase::ChangeStowedState()
{
	SetCurrentWeaponState(enStowed);
}

void WeaponBase::ChangeArmedState()
{
	SetCurrentWeaponState(enArmed);
}

void WeaponBase::AttackImpactProcess(bool startOrEnd)
{
	//キャンセルアクションフラグを設定
	m_isImpossibleancelAction = startOrEnd;

	//コリジョンの設定など派生クラスで実装

}

Vector3 WeaponBase::CalcAutoAimAtTarget(
	Vector3 origin, Vector3 nowDirection, float searchRadius, float maxDot)
{
	Vector3 newDirection = nowDirection;
	newDirection.y = 0.0f;

	//モブエネミーリストをプレイヤーに近い順にいれかえる
	EnemyManager::GetInstance()->SortEnemiesByDistanceToTarget(origin);
	//ソートしたエネミーリストを取得
	std::vector<MobEnemyBase*> tempList = EnemyManager::GetInstance()->GetMobEnemyList();


	float mobDot = -1.0f;


	//リストの中身がないなら処理をしない
	if (tempList.size() != 0)
	{
		for (auto enemy : tempList)
		{
			//プレイヤーから敵に向かうベクトルを計算
			Vector3 diff = enemy->GetPosition() - origin;
			diff.y = 0.0f;
			//ベクトルの長さが調べる範囲外なら
			if (diff.Length() > searchRadius)
			{
				//もう近くに敵がいないのでfor文をぬける
				break;
			}
			//ベクトルを正規化
			diff.Normalize();
			//今の方向と計算したベクトルの内積を調べ、どのくらいベクトルが似ているか調べる
			mobDot = Dot(nowDirection, diff);
			//引数の内積より似ていたら新しい座標とする
			if (mobDot >= maxDot)
			{
				newDirection = diff;
			}
		}
	}

	//ボスエネミーのインスタンスがあるかチェック。
	//ないならここで終わる
	if (EnemyManager::GetInstance()->IsNullptrBossEnemyInstance()) return newDirection;

	//モブエネミーの一番近いやつとボスエネミーの座標で比べる
	//ベクトル計算
	Vector3 diff = EnemyManager::GetInstance()->GetBossEnemyInstance()->GetPosition() - origin;
	//ベクトルが調べる範囲内なら
	if (diff.Length() < searchRadius)
	{
		diff.y = 0.0f;
		diff.Normalize();
		//内積を計算
		float bossDot = Dot(nowDirection, diff);
		//ボスの内積がモブの内積より似ていたら
		if (bossDot >= mobDot && bossDot >= maxDot)
		{
			//ベクトルを変更
			newDirection = diff;
		}
	}

	return newDirection;
}

int WeaponBase::ConvertComboStateToNumber(EnComboState comboState)
{
	switch (comboState)
	{
	case WeaponBase::enCombo_First:
		return 0;
	case WeaponBase::enCombo_Second:
		return 1;
	case WeaponBase::enCombo_Third:
		return 2;
	default:
		std::abort();
		break;
	}
}
