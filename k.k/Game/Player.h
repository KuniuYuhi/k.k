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

	void Render(RenderContext& rc);

	/// <summary>
	/// �L�����N�^�[��؂�ւ���
	/// </summary>
	/// <param name="nextCharacter">���̃L�����N�^�[</param>
	void ChangeCharacter(EnCharacters nextCharacter);

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

	CharacterController& GetCharacterController()
	{
		return m_charaCon;
	}

private:
	Hero* m_hero = nullptr;
	Wizard* m_wizard = nullptr;

	FontRender m_MpFont;

	//�L�����R������g��
	CharacterController m_charaCon;

	Actor* actor[enCharacter_num];
	Actor* m_nowActor = nullptr;	// ���݂̃A�N�^�[
	EnCharacters m_enActiveCharacter= enHero;

	Vector3 m_position = Vector3::Zero;		//���݂̃L�����N�^�[�̍��W

	Vector3 m_moveSpeed = Vector3::Zero;
};

