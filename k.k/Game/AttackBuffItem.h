#pragma once
#include "EffectItemBase.h"

class Brave;
class UseEffect;

/// <summary>
/// �v���C���[�̍U���͂𑝉�������R���|�[�l���g
/// </summary>
class AttackBuffItem : public EffectItemBase
{
public:

	AttackBuffItem();
	~AttackBuffItem();


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

	bool IsDelete();

private:

	Brave* m_brave = nullptr;

	UseEffect* m_effect = nullptr;

	float m_deleteTimer = 0.0f;

};

