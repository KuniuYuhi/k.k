#pragma once
#include "ISummonerState.h"

class SummonerState_DarkBall :public ISummonerState
{
public:
	SummonerState_DarkBall(Summoner* summoner)
		:ISummonerState(summoner,enActionName_Darkball)
	{
		//Init();
	}

	~SummonerState_DarkBall();

	/// <summary>
	/// ������
	/// </summary>
	void Init();

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
	/// �A�j���[�V�����C�x���g
	/// </summary>
	/// <param name="clipName"></param>
	/// <param name="eventName"></param>
	//void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	/// <summary>
	/// �_�[�N�{�[���̐���
	/// </summary>
	void CreateDarkBall();

private:

	

};
