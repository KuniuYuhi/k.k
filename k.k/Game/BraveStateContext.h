#pragma once

#include "BraveAnimClipAndStateInfo.h"

using namespace BraveState;

class Brave;
class IBraveState;

/// <summary>
/// �E�҂̃X�e�[�g���Ǘ�����N���X
/// </summary>
class BraveStateContext
{
public:
	BraveStateContext();
	~BraveStateContext();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="brave">�E�҂̃C���X�^���X</param>
	/// <param name="startState">�ŏ��̃X�e�[�g</param>
	void Init(Brave* brave,EnBraveState startState);

	/// <summary>
	/// �X�e�[�g��؂�ւ���
	/// </summary>
	/// <param name="changeState">�؂�ւ������X�e�[�g</param>
	void ChangeBraveState(EnBraveState changeState);

	/// <summary>
	/// �X�e�[�g��؂�ւ���
	/// </summary>
	/// <param name="changeState">�؂�ւ������X�e�[�g�ԍ�</param>
	void ChangeBraveState(int changeStateNum);

	/// <summary>
	/// ���݂̃X�e�[�g���X�V
	/// </summary>
	void UpdateCurrentState();

	/// <summary>
	/// ���݂̃X�e�[�g�̃A�j���[�V�������Đ�
	/// </summary>
	void PlayAnimationCurrentState();
	

private:

	std::map<EnBraveState, IBraveState*> m_braveStateMap;


	EnBraveState m_EnCurrentBraveState = enBraveState_Idle;		//���݂̃X�e�[�g������
	EnBraveState m_EnPreviousBraveState = enBraveState_Idle;	//�O�̃X�e�[�g������

	IBraveState* m_currentBraveState = nullptr;		//���ݓ����Ă���X�e�[�g
};

