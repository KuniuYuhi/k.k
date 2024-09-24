#pragma once
#include "ISummonerState.h"

class Summoner;
class SummonerStateContext;

class SummonerState_SecondAttack : public ISummonerState
{
public:

	SummonerState_SecondAttack(Summoner* summoner, SummonerStateContext* context)
		:ISummonerState(summoner, context)
	{
	}

	~SummonerState_SecondAttack() {}

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

