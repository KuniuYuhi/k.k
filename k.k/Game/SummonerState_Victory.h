#pragma once
#include "ISummonerState.h"

class SummonerState_Victory :public ISummonerState
{
public:
	SummonerState_Victory(Summoner* summoner)
		:ISummonerState(summoner, enActionName_Victory)
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
