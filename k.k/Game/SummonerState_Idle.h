#pragma once
#include "ISummonerState.h"

namespace {
	const int PRIORITY = 1;
}

class SummonerState_Idle:public ISummonerState
{
public:
	SummonerState_Idle(Summoner* summoner)
		:ISummonerState(summoner,enActionName_Idle)
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

};

