#pragma once

class Wizard;

/// <summary>
/// �E�B�U�[�h�̃X�e�[�g�̊��N���X
/// </summary>
class IWizardState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	IWizardState(Wizard* wizard)
	{
		m_wizard = wizard;
	}
	virtual ~IWizardState() {}
	/// <summary>
	/// ��Ԃ��Ǘ�����
	/// </summary>
	virtual void ManageState() = 0;
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	virtual void PlayAnimation() = 0;

protected:
	Wizard* m_wizard = nullptr;
};

