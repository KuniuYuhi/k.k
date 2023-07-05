#pragma once

class Hero;
class Wizard;
class Actor;

class Player:public IGameObject
{
public:

	enum EnCharacters {
		enHero,
		enWizard,
		enCharacter_num
	};

	Player();
	~Player();

	bool Start();
	void Update();


	//��ɂ�����
	/// <summary>
	/// �q�[���[���疂�@�g���ɐ؂�ւ���
	/// </summary>
	void ChangeFromHeroToWizard();

	/// <summary>
	/// ���@�g������q�[���[�ɐ؂�ւ���
	/// </summary>
	void ChangeFromWizardToHero();

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns></returns>
	Vector3 GetPosition() const
	{
		return m_position;
	}

	Vector3 GetMoveSpeed() const
	{
		return m_moveSpeed;
	}

private:
	Hero* m_hero = nullptr;
	Wizard* m_wizard = nullptr;

	//�L�����R������g��
	CharacterController m_charaCon;

	Actor* actor[enCharacter_num];

	EnCharacters m_enActiveCharacter= enHero;

	Vector3 m_position = Vector3::Zero;		//���݂̃L�����N�^�[�̍��W

	Vector3 m_moveSpeed = Vector3::Zero;
};

