#include "stdafx.h"
#include "SummonerState_DMeteo_Start.h"
#include "Summoner.h"

#include "DarkMeteo.h"

void SummonerState_DMeteo_Start::ManageState()
{
	//�A�j���[�V�������I�������`���[�W�B
	//�`���[�W���I������玟�̃A�j���[�V�����A�X�e�[�g

	if (m_darkMeteo != nullptr)
	{
		//�`���[�W���I�������
		if (m_darkMeteo->GetChargeCompleteFlag() == true)
		{
			//���e�I�����A�j���[�V�����ɐi��
			m_summoner->SetNextAnimationState(
				Summoner::enAnimationState_Attack_DarkMeteorite_main);
		}
		return;
	}



	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//�A�j���[�V�������I������̂ŁA�_�[�N���e�I�̃`���[�W�J�n
		m_darkMeteo = NewGO<DarkMeteo>(0, "darkmeteo");
		m_darkMeteo->SetDarkMeteoPosition(m_summoner->GetPosition());

	}
}

void SummonerState_DMeteo_Start::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(Summoner::enAnimClip_Attack_DarkMeteorite_start);
}
