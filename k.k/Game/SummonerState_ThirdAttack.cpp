#include "stdafx.h"
#include "SummonerState_ThirdAttack.h"
#include "Summoner.h"

void SummonerState_ThirdAttack::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(
		enSummonerAnimClip_ThirdAttack, 0.2f
	);
}

void SummonerState_ThirdAttack::Entry()
{
	m_summoner->ActionActive();
	//�_���[�W����ݒ肷��
	m_summoner->SettingDamageInfo(enSkillType_ComboThird);
}

void SummonerState_ThirdAttack::Ubdate()
{
	
	//�A�j���[�V�������I�������
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//�X�e�[�g�}�V����؂�ւ���̂��`�F�b�N
		m_summoner->CheckAndTransitionStateMachine();
		//���ʃX�e�[�g
		m_summoner->ProcessCommonTranstion();
	}
}

void SummonerState_ThirdAttack::Exit()
{
	m_summoner->ActionDeactive();
}
