#pragma once

#include "ISummonerState.h"

class SummonerState_DMeteo_Main :public ISummonerState
{
public:
	SummonerState_DMeteo_Main(Summoner* summoner)
		:ISummonerState(summoner, enActionName_DarkMeteo_Main)
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