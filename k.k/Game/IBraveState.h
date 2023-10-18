#pragma once

class Brave;

/// <summary>
/// �E�҂̃X�e�[�g�̊��N���X
/// </summary>
class IBraveState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="brave"></param>
	IBraveState(Brave* brave)
	{
		m_brave = brave;
	}
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~IBraveState(){}

	/// <summary>
	/// ��Ԃ��Ǘ�����
	/// </summary>
	virtual void ManageState() = 0;
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	virtual void PlayAnimation() = 0;
protected:
	Brave* m_brave = nullptr; // �v���C���[

};

