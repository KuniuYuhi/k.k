#include "stdafx.h"
#include "SummonerState_DMeteo_Main.h"
#include "Summoner.h"

#include "DarkMeteo.h"


//���[�v��������static�̃��Z�b�g
//�_�[�N���e�I�̒��g��2��ڂ�null�ɂȂ�
//���߂Ă���r���ł�߂�Ƃ��܂�debugBreak

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
	//�S�Ẵ��e�I�������I�������A�j���[�V�����I��
	if (m_darkMeteo !=nullptr && m_darkMeteo->GetEndFlag() == true)
	{
		//�_�[�N���e�I�̍폜
		DeleteGO(m_darkMeteo);
		m_summoner->ProcessCommonStateTransition();
		return;
	}

	//if (m_darkMeteo == nullptr)
	//{
	//	m_darkMeteo = FindGO<DarkMeteo>("darkmeteo");
	//	//�X�e�[�g���V���b�g���e�I�X�e�[�g�ɕύX
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
	//���e�I������
	if (wcscmp(eventName, L"createMeteo") == 0)
	{
		//ProcessShotMeteo();
	}
}

void SummonerState_DMeteo_Main::ProcessShotMeteo()
{
	m_darkMeteo = FindGO<DarkMeteo>("darkmeteo");
	//�X�e�[�g���V���b�g���e�I�X�e�[�g�ɕύX
	m_darkMeteo->SetNextDarkMeteoState(
		DarkMeteo::enDarkMeteoState_ShotMeteo);
}
