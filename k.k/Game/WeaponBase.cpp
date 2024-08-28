#include "stdafx.h"
#include "WeaponBase.h"

#include "MobEnemyBase.h"
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
	//エネミーリストを取得
	std::vector<MobEnemyBase*> tempList = EnemyManager::GetInstance()->GetMobEnemyList();

	//リストの中身がないなら処理をしない
	if (tempList.size() == 0) return newDirection;

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
		float dot = Dot(nowDirection, diff);
		//引数の内積より似ていたら新しい座標とする
		if (dot >= maxDot)
		{
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
