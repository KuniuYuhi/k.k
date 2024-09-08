#include "stdafx.h"
#include "SummonerState_SecondAttack.h"
#include "Summoner.h"

void SummonerState_SecondAttack::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(
		enSummonerAnimClip_SecondAttack, 0.2f
	);
}

void SummonerState_SecondAttack::Entry()
{
	m_summoner->ActionActive();
	//�_���[�W����ݒ肷��
	m_summoner->SettingDamageInfo(enSkillType_ComboSecond);

}

void SummonerState_SecondAttack::Ubdate()
{
	m_summoner->UpdateComboAttackProcess();

	//�A�j���[�V�������I�������
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//�X�e�[�g�}�V����؂�ւ���̂��`�F�b�N
		m_summoner->CheckAndTransitionStateMachine();
		//���ʃX�e�[�g
		m_summoner->ProcessCommonTranstion();
	}
}

void SummonerState_SecondAttack::Exit()
{
	m_summoner->ActionDeactive();
}
