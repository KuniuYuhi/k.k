#pragma once
#include "Actor.h"
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

	/// <summary>
	/// ���݂̃L�����N�^�[�̍U���͂�Ԃ�
	/// </summary>
	/// <returns></returns>
	int GetAtk();

	/// <summary>
	/// ���݂̃L�����N�^�[�̃X�L���̍U���͂�Ԃ�
	/// </summary>
	/// <returns></returns>
	int GetSkillAtk();
	
	/// <summary>
	/// ���݂̃R���{�X�e�[�g�ƃ_���[�W���󂯂����̃R���{�X�e�[�g���Ⴄ��
	/// </summary>
	/// <returns>�Ⴄ�Ȃ�true,�����Ȃ�false</returns>
	bool IsComboStateSame();
	
	/// <summary>
	/// �_���[�W���󂯂����̃R���{�X�e�[�g��ݒ�
	/// </summary>
	/// <param name="damagedcombostate"></param>
	void SetDamagedComboState(Actor::EnComboState damagedcombostate);
	
	/// <summary>
	/// ���݂̃L�����N�^�[�̃R���{�X�e�[�g���擾
	/// </summary>
	/// <returns></returns>
	Actor::EnComboState GetNowComboState() const;

	

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

