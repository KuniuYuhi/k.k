#pragma once
#include "ISummonerState.h"
class SummonerState_Angry : public ISummonerState
{
public:
	SummonerState_Angry(Summoner* summoner)
		:ISummonerState(summoner, enActionName_Angry)
	{
	}
	/// <summary>
	/// ��ԑJ�ڊǗ�
	/// </summary>
	void ManageState();
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimation();


private:
	/// <summary>
	/// �{�胂�[�h�ł��鎞�Ԃ̌v�Z
	/// </summary>
	/// <returns>true�œ{�胂�[�h�I��</returns>
	bool CalcAngryModeTimer();

private:

	const float m_angryModeLimmitTime = 10.0f;	//�{�胂�[�h�ł�����b��
	float m_angryModeTimer = m_angryModeLimmitTime;

	bool m_angryModeFlag = false;				//�{�胂�[�h�ł��邩�̃t���O

};

