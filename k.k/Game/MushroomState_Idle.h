#pragma once
#include "IMobEnemyState.h"
#include "MushroomStateContext.h"

class Mushroom;
class MushroomStateContext;

class MushroomState_Idle : public IMobEnemyState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="slime">�L�m�R�̃C���X�^���X</param>
	/// <param name="stateContest">�X�e�[�g�R���e�L�X�g</param>
	MushroomState_Idle(Mushroom* mushroom, MushroomStateContext* stateContest)
	{
		m_mushroom = mushroom;
		m_stateCotext = stateContest;
	}

	~MushroomState_Idle();

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
	Mushroom* m_mushroom = nullptr;
	MushroomStateContext* m_stateCotext = nullptr;
};
