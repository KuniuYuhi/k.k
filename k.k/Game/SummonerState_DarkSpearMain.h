#pragma once

#include "ISummonerState.h"

class DarkSpear;

class SummonerState_DarkSpearMain :public ISummonerState
{
public:
	SummonerState_DarkSpearMain(Summoner* summoner)
		:ISummonerState(summoner, enActionName_DarkSpear_Main)
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
	/// <summary>
	/// �_�[�N�X�s�A�𐶐�
	/// </summary>
	void CreateDarkSpear();

private:

	DarkSpear* m_darkSpear = nullptr;

	bool m_createDarkSpearFag = false;
};
