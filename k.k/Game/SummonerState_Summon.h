#pragma once
#include "ISummonerState.h"

class SummonerState_Summon :public ISummonerState
{
public:
	SummonerState_Summon(Summoner* summoner)
		:ISummonerState(summoner, enActionName_Summon)
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
