#pragma once
#include "ISummonerState.h"

class SumonerState_NAttack_3 :public ISummonerState
{
public:
	SumonerState_NAttack_3(Summoner* summoner)
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