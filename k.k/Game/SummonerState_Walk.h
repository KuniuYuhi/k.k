#pragma once
#include "ISummonerState.h"

class SummonerState_Walk :public ISummonerState
{
public:
	SummonerState_Walk(Summoner* summoner)
		:ISummonerState(summoner, enActionName_Walk)
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