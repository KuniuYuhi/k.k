#pragma once
#include "ISummonerState.h"

class SummonerState_Hit :public ISummonerState
{
public:
	SummonerState_Hit(Summoner* summoner)
		:ISummonerState(summoner, enActionName_CriticalHit)
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

