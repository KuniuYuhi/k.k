#pragma once
#include "ISummonerState.h"

class SummonerState_Die :public ISummonerState
{
public:
	SummonerState_Die(Summoner* summoner)
		:ISummonerState(summoner, enActionName_Die)
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
	/// ���S���̃G�t�F�N�g�Đ�
	/// </summary>
	void CreateDeadEffect();


private:

	EffectEmitter* m_deadEffect = nullptr;	//���S�G�t�F�N�g
	bool m_isdeadEffectPlayFlag = false;
};
