#pragma once
#include "ISummonerState.h"

class SummonerState_Warp :public ISummonerState
{
public:
	SummonerState_Warp(Summoner* summoner)
		:ISummonerState(summoner, enActionName_Warp)
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
