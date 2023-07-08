#pragma once

class Hero;

/// <summary>
/// �q�[���[�̃X�e�[�g�̊��N���X
/// </summary>
class IHeroState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	IHeroState(Hero* hero)
	{
		m_hero = hero;
	}
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~IHeroState() {}
	/// <summary>
	/// ��Ԃ��Ǘ�����
	/// </summary>
	virtual void ManageState() = 0;
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	virtual void PlayAnimation() = 0;
protected:
	Hero* m_hero = nullptr; // �v���C���[
};

