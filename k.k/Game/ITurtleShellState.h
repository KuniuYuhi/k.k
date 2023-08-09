#pragma once

class TurtleShell;
/// <summary>
/// �Ƃ��b���X�e�[�g�̊��N���X
/// </summary>
class ITurtleShellState
{
public:
	ITurtleShellState(TurtleShell* turtleShell)
	{
		m_turtleShell = turtleShell;
	}

	virtual ~ITurtleShellState() {}

	/// <summary>
	/// ��Ԃ��Ǘ�����
	/// </summary>
	virtual void ManageState() = 0;
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	virtual void PlayAnimation() = 0;


protected:
	TurtleShell* m_turtleShell = nullptr;

};

