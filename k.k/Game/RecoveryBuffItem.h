#pragma once
#include "EffectItemBase.h"

class Brave;
class UseEffect;

/// <summary>
/// �v���C���[��HP���񕜂�����
/// </summary>
class RecoveryBuffItem : public EffectItemBase
{
public:

	RecoveryBuffItem();
	~RecoveryBuffItem();


	/// <summary>
	/// �R���|�[�l���g��ǉ��������̍ŏ��̏���
	/// </summary>
	void Start() override;

	/// <summary>
	/// �X�V�����B�����ōs���Ă����B���������Ȃ�GameObjectManager��
	/// </summary>
	void UpdateComponent() override;

	/// <summary>
	/// �������B
	/// </summary>
	void Init() override;

	/// <summary>
	/// �폜���鎞�̏���
	/// </summary>
	void DeleteProcess() override;

private:


private:

	Brave* m_brave = nullptr;

	UseEffect* m_effect = nullptr;

	bool isRecoveryPlayerHp = false;


};

