#pragma once
#include "ISummonerState.h"

class SummonerState_Die :public ISummonerState
{
public:
	SummonerState_Die(Summoner* summoner)
		:ISummonerState(summoner, enActionName_Die)
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
