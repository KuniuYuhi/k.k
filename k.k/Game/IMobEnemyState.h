#pragma once

/// <summary>
/// ���u�G�l�~�[�̃X�e�[�g�̊��N���X(�C���^�[�t�F�[�X)
/// </summary>
class IMobEnemyState
{
public:

	virtual ~IMobEnemyState() {}

	/// <summary>
	/// �A�j���[�V�������Đ�
	/// </summary>
	virtual void PlayAnimation() = 0;
	/// <summary>
	/// ���������̏���
	/// </summary>
	virtual void Entry() = 0;
	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Ubdate() = 0;
	/// <summary>
	/// �I�����̏���
	/// </summary>
	virtual void Exit() = 0;

protected:


};

