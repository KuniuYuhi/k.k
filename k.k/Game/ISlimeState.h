#pragma once

class Slime;
/// <summary>
/// �X���C���X�e�[�g�̊��N���X
/// </summary>
class ISlimeState
{
public:
	ISlimeState(Slime* slime)
	{
		m_slime = slime;
	}

	virtual ~ISlimeState() {}

	/// <summary>
	/// ��Ԃ��Ǘ�����
	/// </summary>
	virtual void ManageState() = 0;
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	virtual void PlayAnimation() = 0;

protected:
	Slime* m_slime = nullptr;
};

