#pragma once
#include "ISummonerState.h"

class SummonerState_DarkWall :public ISummonerState
{
public:
	SummonerState_DarkWall(Summoner* summoner)
		:ISummonerState(summoner, enActionName_DarkWall)
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