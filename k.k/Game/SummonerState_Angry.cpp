#include "stdafx.h"
#include "SummonerState_Angry.h"
#include "Summoner.h"

void SummonerState_Angry::ManageState()
{
	//怒りモードである時間が終わったら
	if (CalcAngryModeTimer() == true)
	{
		//怒りモード終了。
		//状態をノーマルに戻す
		m_summoner->ChangeSpecialState(false);
		m_summoner->ProcessCommonStateTransition();
	}
	//怒りモードならこれより下の処理しない
	if (m_angryModeFlag == true)
	{
		return;
	}
	//アニメーションが終わったら怒りモードに変更
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//怒りモードに切り替え
		m_summoner->ChangeSpecialState(true);
		m_angryModeFlag = true;
	}
}

void SummonerState_Angry::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_Angry);
}

bool SummonerState_Angry::CalcAngryModeTimer()
{
	if (m_angryModeFlag != true)
	{
		return false;
	}

	//タイマーが0以下になったら
	if (m_angryModeTimer < 0.0f)
	{
		//上限の値に戻す
		m_angryModeTimer = m_angryModeLimmitTime;
		return true;
	}
	else
	{
		//タイマーを減算
		m_angryModeTimer -= g_gameTime->GetFrameDeltaTime();
		return false;
	}
}
