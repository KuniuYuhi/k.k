#pragma once
#include "ISummonerState.h"

class SummonerState_Command :public ISummonerState
{
public:
	SummonerState_Command(Summoner* summoner)
		:ISummonerState(summoner, enActionName_Command)
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
