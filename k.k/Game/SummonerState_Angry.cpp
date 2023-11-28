#include "stdafx.h"
#include "SummonerState_Angry.h"
#include "Summoner.h"

void SummonerState_Angry::ManageState()
{
	//�{�胂�[�h�ł��鎞�Ԃ��I�������
	if (CalcAngryModeTimer() == true)
	{
		//�{�胂�[�h�I���B
		//��Ԃ��m�[�}���ɖ߂�
		m_summoner->ChangeSpecialState(false);
		m_summoner->ProcessCommonStateTransition();
	}
	//�{�胂�[�h�Ȃ炱���艺�̏������Ȃ�
	if (m_angryModeFlag == true)
	{
		return;
	}
	//�A�j���[�V�������I�������{�胂�[�h�ɕύX
	if (m_summoner->GetModelRender().IsPlayingAnimation() == false)
	{
		//�{�胂�[�h�ɐ؂�ւ�
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

	//�^�C�}�[��0�ȉ��ɂȂ�����
	if (m_angryModeTimer < 0.0f)
	{
		//����̒l�ɖ߂�
		m_angryModeTimer = m_angryModeLimmitTime;
		return true;
	}
	else
	{
		//�^�C�}�[�����Z
		m_angryModeTimer -= g_gameTime->GetFrameDeltaTime();
		return false;
	}
}
