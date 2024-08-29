#pragma once
#include "IMobEnemyState.h"
#include "CactusStateContext.h"

class Cactus;
class CactusStateContext;

class CactusState_Victory : public IMobEnemyState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="slime">�T�{�e���̃C���X�^���X</param>
	/// <param name="stateContest">�X�e�[�g�R���e�L�X�g</param>
	CactusState_Victory(Cactus* cactus, CactusStateContext* stateContest)
	{
		m_cactus = cactus;
		m_stateCotext = stateContest;
	}

	~CactusState_Victory();

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
	Cactus* m_cactus = nullptr;
	CactusStateContext* m_stateCotext = nullptr;
};

