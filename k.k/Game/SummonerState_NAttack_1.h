#pragma once
#include "ISummonerState.h"

class SummonerState_NAttack_1 :public ISummonerState
{
public:
	SummonerState_NAttack_1(Summoner* summoner)
		:ISummonerState(summoner, enActionName_Darkball)
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
