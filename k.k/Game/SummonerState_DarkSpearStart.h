#pragma once
#include "ISummonerState.h"

class SummonerState_DarkSpearStart :public ISummonerState
{
public:
	SummonerState_DarkSpearStart(Summoner* summoner)
		:ISummonerState(summoner, enActionName_DarkSpear_Start)
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
