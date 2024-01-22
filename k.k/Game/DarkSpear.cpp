#include "stdafx.h"
#include "DarkSpear.h"
#include "DarkSpearObj.h"
#include "CharactersInfoManager.h"
#include "InitEffect.h"

#include "GameManager.h"

namespace {
	const float DELETE_TIMER_LIMMIT = 2.0f;
}

DarkSpear::DarkSpear()
{
}

DarkSpear::~DarkSpear()
{
	//エフェクトの削除
	DeleteRangeEffect();
}

bool DarkSpear::Start()
{
	//ダークスピアを生成する座標を決める
	DecideCreatePosition();

	//範囲エフェクトを生成
	PlayRangeEffect();

	return true;
}

void DarkSpear::Update()
{
	//勝敗が決まったら削除
	if (GameManager::GetInstance()->GetOutComeState() !=
		GameManager::enOutComeState_None)
	{
		DeleteGO(this);
		return;
	}

	//生成開始フラグがセットされていないなら処理しない
	if (m_createStartFlag != true)
	{
		return;
	}

	//生成仕切るまで
	if (m_rowsCount < SPEAR_ROWS)
	{
		//生成するタイミングなら
		if (IsCreateTiming() == true)
		{
			CreateDarkSpearObj();

			//生成カウントを加算
			//これが列の数
			m_rowsCount++;
		}
	}
	else
	{
		//消去する時間に達したら
		if (m_timer > DELETE_TIMER_LIMMIT)
		{
			//自身を削除
			DeleteGO(this);
		}
		else
		{
			//タイマーを加算
			m_timer += g_gameTime->GetFrameDeltaTime();
		}
	}

	
}

void DarkSpear::DecideCreatePosition()
{
	//プレイヤーの座標
	Vector3 playerPos= CharactersInfoManager::GetInstance()->GetPlayerInstance()->GetPosition();
	//自身(サモナー)からプレイヤーに向かうベクトルを計算
	Vector3 diff = playerPos - m_position;
	diff.y = 0.0f;
	diff.Normalize();

	//全方位に槍を生成するとき
	Vector3 center = m_position;
	//一周
	float en = 360.0f;
	//生成する場所で割って角度を求める
	en /= SPEAR_CREATE_PLACE;
	//ダークスピアを生成する座標に足す長さを設定
	float addLength = FIRST_CREATE_POS_LENGTH;

	//生成する場所の数繰り返す
	//生成する座標を決める
	for (int i = 1; i <= SPEAR_CREATE_PLACE; i++)
	{
		//角度をラジアンに変換
		float theta = Math::PI / 180 * en * i;
		//生成する座標を求めるために角度を求める
		Vector3 pos = g_vec3Zero;
		pos.x = center.x + FIRST_CREATE_POS_LENGTH * cos(theta);
		pos.z = center.z + FIRST_CREATE_POS_LENGTH * sin(theta);

		//決めた角度の座標から中心に向かうベクトルを計算
		Vector3 finalPos = m_position - pos;
		//正規化
		finalPos.Normalize();

		//列の数繰り返す
		for (int j = 0; j < SPEAR_ROWS; j++)
		{
			//生成する座標を設定
			m_createPosition[i-1][j] = m_position + finalPos * addLength;
			//次に生成する座標の距離を伸ばす
			addLength += ADD_CREATE_POS_LENGTH;
		}
		//加算する長さを最初に戻す
		addLength = FIRST_CREATE_POS_LENGTH;
	}

}

void DarkSpear::CreateDarkSpearObj()
{
	//一列ごとに生成する
	for (int num2 = m_rowsCount; num2 <= m_rowsCount; num2++)
	{
		//生成場所の数繰り返す
		for (int num = 0; num < SPEAR_CREATE_PLACE; num++)
		{
			//ダークスピアオブジェクトの生成
			m_darkSpearObj[num][num2] = NewGO<DarkSpearObj>(0, "darkspearobj");
			//座標の設定
			m_darkSpearObj[num][num2]->SetPosition(
				m_createPosition[num][num2]
			);
			//ダークスピアの最大サイズを決める。
			//列が増えるとサイズも大きくなる
			m_darkSpearObj[num][num2]->SetMaxScale(m_rowsCount);

			//生成した場所の範囲エフェクトを削除
			m_rangeEffect[num][num2]->Stop();
		}
	}
}

bool DarkSpear::IsCreateTiming()
{
	//一秒ごとに生成する
	if (m_createTimer > CREATE_TIMER_LIMMIT)
	{
		m_createTimer = 0.0f;
		return true;
	}
	else
	{
		//タイマーを加算
		m_createTimer += g_gameTime->GetFrameDeltaTime();
	}

	return false;
}

void DarkSpear::PlayRangeEffect()
{
	//一列ごとに生成する
	for (int num2 = 0; num2 < SPEAR_ROWS; num2++)
	{
		float size = num2 + 1;
		//生成場所の数繰り返す
		for (int num = 0; num < SPEAR_CREATE_PLACE; num++)
		{
			Vector3 createPos = m_createPosition[num][num2];
			createPos.y += 3.0f;

			//生成した場所の範囲エフェクトを削除
			m_rangeEffect[num][num2] = NewGO<EffectEmitter>(0);
			m_rangeEffect[num][num2]->Init(InitEffect::enEffect_Meteo_Range);
			m_rangeEffect[num][num2]->Play();
			m_rangeEffect[num][num2]->SetScale(g_vec3One * (size * 7.0f));
			m_rangeEffect[num][num2]->SetPosition(createPos);
			m_rangeEffect[num][num2]->Update();
		}
	}
	//エフェクト生成をセット
	m_playRangeEffectFlag = true;
}

void DarkSpear::DeleteRangeEffect()
{
	//エフェクトが生成されていないなら処理しない
	if (m_playRangeEffectFlag != true)
	{
		return;
	}

	for (int num2 = 0; num2 < SPEAR_ROWS; num2++)
	{
		//生成場所の数繰り返す
		for (int num = 0; num < SPEAR_CREATE_PLACE; num++)
		{
			if (m_rangeEffect[num][num2] != nullptr)
			{
				//範囲エフェクトを削除
				m_rangeEffect[num][num2]->Stop();
			}
		}
	}
}