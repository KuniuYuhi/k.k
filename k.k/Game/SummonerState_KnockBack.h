#pragma once
#include "ISummonerState.h"

class SummonerState_KnockBack :public ISummonerState
{
public:
	SummonerState_KnockBack(Summoner* summoner)
		:ISummonerState(summoner, enActionName_KnockBack)
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

