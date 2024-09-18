#pragma once
#include "IComponent.h"
#include "EffectItemStatus.h"

class EffectItemStatus;

/// <summary>
/// �v���C���[�ɉ��炩�́u���ʁv��^����A�C�e���̊��N���X
/// </summary>
class EffectItemBase : public IComponent
{
public:

	/// <summary>
	/// �R���|�[�l���g��ǉ��������̍ŏ��̏���
	/// </summary>
	virtual void Start() override {}

	/// <summary>
	/// �X�V�����B�����ōs���Ă����B���������Ȃ�GameObjectManager��
	/// </summary>
	virtual void UpdateComponent() override {}

	/// <summary>
	/// �������B
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// �폜���鎞�̏���
	/// </summary>
	virtual void DeleteProcess() {}

protected:
	EffectItemStatus m_status;
};

