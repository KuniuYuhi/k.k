#pragma once
#include "ISummonerState.h"

class SummonerState_KnockBack :public ISummonerState
{
public:
	SummonerState_KnockBack(Summoner* summoner)
		:ISummonerState(summoner, enActionName_KnockBack)
	{
	}

	~SummonerState_KnockBack();

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
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	/// <summary>
	/// �m�b�N�o�b�N����
	/// </summary>
	void ProcessKnockBack();

};

