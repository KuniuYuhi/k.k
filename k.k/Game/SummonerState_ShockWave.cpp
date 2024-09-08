#include "stdafx.h"
#include "SummonerState_ShockWave.h"
#include "Summoner.h"

void SummonerState_ShockWave::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(
		enSummonerAnimClip_ShockWave, 0.2f
	);
}

void SummonerState_ShockWave::Entry()
{
	m_summoner->ActionActive();
	//�_���[�W����ݒ肷��
	m_summoner->SettingDamageInfo(enSkillType_ShockWave);
}

void SummonerState_ShockWave::Ubdate()
{
	m_summoner->UpdateShockWaveProcess();


	//�A�j���[�V�������I�������
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//�X�e�[�g�}�V����؂�ւ���̂��`�F�b�N
		m_summoner->CheckAndTransitionStateMachine();
		//���ʃX�e�[�g
		m_summoner->ProcessCommonTranstion();
	}
}

void SummonerState_ShockWave::Exit()
{
	m_summoner->ActionDeactive();
}
