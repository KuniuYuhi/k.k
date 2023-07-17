#pragma once

class Lich;
/// <summary>
/// ���b�`�X�e�[�g�̊��N���X
/// </summary>
class ILichState
{
public:
	ILichState(Lich* lich)
	{
		m_lich = lich;
	}

	virtual ~ILichState(){}

	/// <summary>
	/// ��Ԃ��Ǘ�����
	/// </summary>
	virtual void ManageState() = 0;
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	virtual void PlayAnimation() = 0;

protected:
	Lich* m_lich = nullptr;
};

