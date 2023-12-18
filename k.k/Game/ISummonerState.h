#pragma once

#include "IBossStateMachine.h"

class Summoner;

using namespace SummonerActions;

/// <summary>
/// �{�X�̃X�e�[�g�̊��N���X
/// </summary>
class ISummonerState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="bossBase">�{�X�̃C���X�^���X</param>
	/// <param name="actionName">�A�N�V�����̖��O</param>
	ISummonerState(Summoner* bossBase, EnActionName actionName)
	{
		m_summoner = bossBase;
		m_actionName = actionName;
	}

	virtual ~ISummonerState(){}

	/// <summary>
	/// ��Ԃ��Ǘ�����
	/// </summary>
	virtual void ManageState() = 0;
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	virtual void PlayAnimation() = 0;

	/// <summary>
	/// �A�N�V�����̗D��x�̎擾���@�ύX
	/// </summary>
	/// <returns></returns>
	const int GetPriority() const
	{
		return m_priority;
	}

	/// <summary>
	/// �s���̖��O���擾
	/// </summary>
	/// <returns></returns>
	const EnActionName GetActionName()
	{
		return m_actionName;
	}

protected:
	/// <summary>
	/// �A�N�V�����̗D��x�̐ݒ�
	/// </summary>
	/// <param name="priority"></param>
	void SetPriority(const int priority)
	{
		m_priority = priority;
	}
	
	/// <summary>
	/// ���g�̃A�N�V�����̖��O��ݒ�
	/// </summary>
	/// <param name="actionName"></param>
	void SetActionName(EnActionName actionName)
	{
		m_actionName = actionName;
	}


protected:
	Summoner* m_summoner = nullptr;

	int m_priority = 0;		//�A�N�V�����̗D��x
	EnActionName m_actionName;

};

