#pragma once
#include "EffectItemBase.h"
class Brave;
class UseEffect;

/// <summary>
/// �v���C���[�̃X�^�~�i���񕜂�����R���|�[�l���g
/// </summary>
class StaminaRecoveryBuffItem : public EffectItemBase
{
public:

	StaminaRecoveryBuffItem();
	~StaminaRecoveryBuffItem();


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

	bool isRecoveryPlayerStamina = false;

};

