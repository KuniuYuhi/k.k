#pragma once
#include "ISummonerState.h"

class SummonerState_DarkSpearEnd :public ISummonerState
{
public:
	SummonerState_DarkSpearEnd(Summoner* summoner)
		:ISummonerState(summoner, enActionName_DarkSpear_End)
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
