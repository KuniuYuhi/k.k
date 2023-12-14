#include "stdafx.h"
#include "SummonerState_DMeteo_Main.h"
#include "Summoner.h"

#include "DarkMeteo.h"


//ループした時のstaticのリセット
//ダークメテオの中身が2回目でnullになる
//溜めている途中でやめるとたまにdebugBreak

void SummonerState_DMeteo_Main::Init()
{
	//ProcessShotMeteo();

	static bool m_initMeteoFlag = false;

	if (m_initMeteoFlag == true)
	{
		
		return;
	}

	m_summoner->GetModelRender().AddAnimationEvent(
		[&](const wchar_t* clipName, const wchar_t* eventName) {
			OnDarkMeteoAnimationEvent(clipName, eventName);
		});


	m_initMeteoFlag = true;


	
}

void SummonerState_DMeteo_Main::ManageState()
{
	//全てのメテオを撃ち終わったらアニメーション終了
	if (m_darkMeteo !=nullptr && m_darkMeteo->GetEndFlag() == true)
	{
		//ダークメテオの削除
		DeleteGO(m_darkMeteo);
		m_summoner->ProcessCommonStateTransition();
		return;
	}

	//if (m_darkMeteo == nullptr)
	//{
	//	m_darkMeteo = FindGO<DarkMeteo>("darkmeteo");
	//	//ステートをショットメテオステートに変更
	//	m_darkMeteo->SetNextDarkMeteoState(
	//		DarkMeteo::enDarkMeteoState_ShotMeteo);
	//}
}

void SummonerState_DMeteo_Main::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_Attack_DarkMeteorite_main);
}

void SummonerState_DMeteo_Main::OnDarkMeteoAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//メテオを撃つ
	if (wcscmp(eventName, L"createMeteo") == 0)
	{
		//ProcessShotMeteo();
	}
}

void SummonerState_DMeteo_Main::ProcessShotMeteo()
{
	m_darkMeteo = FindGO<DarkMeteo>("darkmeteo");
	//ステートをショットメテオステートに変更
	m_darkMeteo->SetNextDarkMeteoState(
		DarkMeteo::enDarkMeteoState_ShotMeteo);
}
