#pragma once
#include "ISummonerState.h"

class SummonerState_DarkBall :public ISummonerState
{
public:
	SummonerState_DarkBall(Summoner* summoner)
		:ISummonerState(summoner,enActionName_Darkball)
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
