#pragma once

#include "SummonerInfo.h"


class Summoner;
class ISummonerState;

using namespace SummonerStates;

class SummonerStateContext
{
public:
	SummonerStateContext();
	~SummonerStateContext();

	void Init(Summoner* summoner, EnSummonerState startState);

	void ChangeState(EnSummonerState changeState);

	/// <summary>
	/// ���݂̃X�e�[�g���X�V
	/// </summary>
	void UpdateCurrentState();

	/// <summary>
	/// ���݂̃X�e�[�g�̃A�j���[�V�������Đ�
	/// </summary>
	void PlayAnimationCurrentState();

private:

	std::map<EnSummonerState, ISummonerState*> m_summonerStateMap;

	ISummonerState* m_currentState = nullptr;

	EnSummonerState m_EnCurrentState = enSummonerState_Idle;
	EnSummonerState m_EnPreviousState = enSummonerState_Idle;
};

