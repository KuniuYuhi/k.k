#pragma once
#include "ISummonerState.h"

class SummonerState_DMeteo_Start :public ISummonerState
{
public:
	SummonerState_DMeteo_Start(Summoner* summoner)
		:ISummonerState(summoner, enActionName_DarkMeteo_Start)
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
