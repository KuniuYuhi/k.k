#include "stdafx.h"
#include "SummonerState_Die.h"
#include "Summoner.h"

#include "InitEffect.h"
#include "GameManager.h"

void SummonerState_Die::ManageState()
{
	//死亡エフェクトが再生され始めたら
	if (m_isdeadEffectPlayFlag == true)
	{
		//死亡の再生が終わったら
		if (m_deadEffect->IsPlay() != true)
		{
			//全ての処理が終わりもう削除されてもよい
			GameManager::GetInstance()->SetBossDeleteOkFlag(true);
		}
		//以降の処理はする必要がないのでreturn
		return;
	}
	//アニメーションが終わったら
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//やられたら死亡エフェクト再生
		CreateDeadEffect();
		//モデルドローフラグをリセット
		m_summoner->SetIsDrawModelFlag(false);
		//モブモンスターを削除
		m_summoner->DeleteMobMonsters();
	}
}

void SummonerState_Die::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_Die);
}

void SummonerState_Die::CreateDeadEffect()
{
	m_deadEffect = NewGO<EffectEmitter>(0);
	m_deadEffect->Init(InitEffect::enEffect_Mob_Dead);
	m_deadEffect->Play();
	m_deadEffect->SetPosition(m_summoner->GetPosition());
	m_deadEffect->SetScale(g_vec3One * 8.0f);
	m_deadEffect->Update();
	//フレームレートを戻す
	g_engine->SetFrameRateMode(K2EngineLow::enFrameRateMode_Variable, 60);

	//エフェクト再生フラグをセット
	m_isdeadEffectPlayFlag = true;
}
