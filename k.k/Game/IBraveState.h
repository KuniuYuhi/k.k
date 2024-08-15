#pragma once

#include "BraveAnimClipAndStateInfo.h"

class Brave;
class BraveStateContext;

/// <summary>
/// �v���C���[�̃X�e�[�g�̊��N���X
/// </summary>
class IBraveState
{
public:

	IBraveState(Brave* brave, BraveStateContext* stateContext)
	{
		m_brave = brave;
		m_stateCotext = stateContext;
	}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~IBraveState() {}

	/// <summary>
	/// �A�j���[�V�������Đ�
	/// </summary>
	virtual void PlayAnimation() = 0;
	/// <summary>
	/// ���������̏���
	/// </summary>
	virtual void Entry() = 0;
	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Ubdate() = 0;
	/// <summary>
	/// �I�����̏���
	/// </summary>
	virtual void Exit() = 0;


protected:

	//�v���C���[�̃C���X�^���X
	Brave* m_brave = nullptr;
	//�X�e�[�g�R���e�L�X�g�̃C���X�^���X
	BraveStateContext* m_stateCotext = nullptr;

};
