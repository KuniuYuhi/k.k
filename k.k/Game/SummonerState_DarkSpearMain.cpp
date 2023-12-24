#include "stdafx.h"
#include "SummonerState_DarkSpearMain.h"
#include "Summoner.h"

#include "DarkSpear.h"

void SummonerState_DarkSpearMain::ManageState()
{
	if (m_createDarkSpearFag == true)
	{
		//�_�[�N�X�s�A���I���A�j���[�V�����Ɉڂ�
		m_summoner->SetNextAnimationState(
			Summoner::enAnimationState_DarkSpear_End);
		return;
	}

	//�A�j���[�V�������I�������
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//�n�ʂ��瑄�𐶐��d�؂�����G���h�ɑJ��
		CreateDarkSpear();
		//
		m_createDarkSpearFag = true;
	}
}

void SummonerState_DarkSpearMain::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_DarkSpear_Main,0.1f);
}

void SummonerState_DarkSpearMain::CreateDarkSpear()
{
	m_darkSpear = FindGO<DarkSpear>("darkspear");
	//���𐶐��J�n
	m_darkSpear->SetCreateStartFlag(true);
}
