#pragma once
#include "ISummonerStateMachine.h"
#include "SummonerInfo.h"

class Summoner;
class SummonerSMContext;

class Brave;

using namespace SummonerStates;

/// <summary>
/// �U���T�u�X�e�[�g�}�V��
/// </summary>
class SummonerSSM_Offensive : public ISummonerStateMachine
{
public:

	SummonerSSM_Offensive(Summoner* summoner, SummonerSMContext* context)
		:ISummonerStateMachine(summoner, context)
	{
	}


	void Entry() override;

	void Update() override;

	//virtual void Exit() {}

	bool IsEnableChangeStateMachine() override;

private:

	/// <summary>
	/// �����ɂ���ĉ��������ߋ����U�����I��
	/// </summary>
	void SelectAttackBasedOnDistance();

	/// <summary>
	/// �ߋ����U���̒�����I��
	/// </summary>
	void SelectMeleeAttack();
	/// <summary>
	/// �������U���̒�����I��
	/// </summary>
	void SelectRangedAttack();


	/// <summary>
	/// ���̃R���{�U���ɂ���
	/// </summary>
	bool ChangeNextComboAttackState();



	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool CheckChangeDarkBallState();
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool CheckChangeDarkMeteoriteState();



private:

	Brave* m_player = nullptr;


	const int m_MaxUseDarkMeteoriteCount = 4;
	int m_darkMeteoriteRemainingCount = m_MaxUseDarkMeteoriteCount;		//�c���

	float m_darkMeteoriteCoolTimer = 0.0f;
};

