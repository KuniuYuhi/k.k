#pragma once

#include "ISummonerState.h"

class DarkMeteo;

class SummonerState_DMeteo_Main :public ISummonerState
{
public:
	SummonerState_DMeteo_Main(Summoner* summoner)
		:ISummonerState(summoner, enActionName_DarkMeteo_Main)
	{
		Init();
	}

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
	void OnDarkMeteoAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	void ProcessShotMeteo();

private:
	DarkMeteo* m_darkMeteo = nullptr;

};