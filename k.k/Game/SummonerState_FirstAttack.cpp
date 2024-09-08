#include "stdafx.h"
#include "SummonerState_FirstAttack.h"
#include "Summoner.h"

void SummonerState_FirstAttack::PlayAnimation()
{
	m_summoner->GetModelRender().PlayAnimation(
		enSummonerAnimClip_FirstAttack, 0.2f
	);
}

void SummonerState_FirstAttack::Entry()
{
	m_summoner->ActionActive();
	//�_���[�W����ݒ肷��
	m_summoner->SettingDamageInfo(enSkillType_ComboFirst);

	m_isNextAttack = false;
}

void SummonerState_FirstAttack::Ubdate()
{
	m_summoner->UpdateComboAttackProcess();

	//�A�j���[�V�������I�������
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//�X�e�[�g�}�V����؂�ւ���̂��`�F�b�N
		//���̍U���ɍs���Ȃ�t���O������
		//�Ȃ��ɂ��ăX�e�[�g�}�V�����Ō��߂Ă�������������Ȃ�
		m_isNextAttack = m_summoner->CheckAndTransitionStateMachine();

		//���ʃX�e�[�g
		m_summoner->ProcessCommonTranstion();
	}
}

void SummonerState_FirstAttack::Exit()
{
	if (m_isNextAttack) return;

	m_summoner->ActionDeactive();
}
