#pragma once
#include "ISummonerState.h"

class Summoner;
class SummonerStateContext;

class SummonerState_ShockWave : public ISummonerState
{
public:
	SummonerState_ShockWave(Summoner* summoner, SummonerStateContext* context)
		:ISummonerState(summoner, context)
	{
	}

	~SummonerState_ShockWave() {}

	/// <summary>
	/// �A�j���[�V�������Đ�
	/// </summary>
	void PlayAnimation() override;
	/// <summary>
	/// ���������̏���
	/// </summary>
	void Entry() override;
	/// <summary>
	/// �X�V����
	/// </summary>
	void Ubdate() override;
	/// <summary>
	/// �I�����̏���
	/// </summary>
	void Exit() override;



private:

};

