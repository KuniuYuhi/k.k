#pragma once
#include "Actor.h"

class Hero;
class Wizard;
class Actor;
class Game;

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
	/// �L�����N�^�[�؂�ւ�����
	/// </summary>
	void Change();

	/// <summary>
	/// �L�����N�^�[��؂�ւ���
	/// </summary>
	/// <param name="nextCharacter">���̃L�����N�^�[</param>
	void ChangeCharacter(EnCharacters nextCharacter);

	/// <summary>
	/// �L�����N�^�[�������I�ɐ؂�ւ���
	/// </summary>
	bool ForcedChange();

	/// <summary>
	/// �S�ł�����
	/// </summary>
	bool IsAnnihilation();

	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	const Vector3& GetMoveSpeed() const
	{
		return m_moveSpeed;
	}

	CharacterController& GetCharacterController()
	{
		return m_charaCon;
	}

	/// <summary>
	/// ���݂̃A�N�^�[�̃X�e�[�^�X��Ԃ�
	/// </summary>
	/// <returns></returns>
	Status GetNowActorStatus()
	{
		return m_nowActor->GetStatus();
	}

	/// <summary>
	/// ���݂̃A�N�^�[�̑O�����̎擾�B���K���ς�
	/// </summary>
	/// <returns></returns>
	const Vector3& GetForward() const
	{
		return m_nowActor->GetForward();
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

	bool GetGameEndFlag()
	{
		return m_gameEndFlag;
	}

private:
	Game* m_game = nullptr;
	Hero* m_hero = nullptr;
	Wizard* m_wizard = nullptr;

	FontRender m_mpFont;
	FontRender m_hpFont;

	//�L�����R������g��
	CharacterController m_charaCon;

	RigidBody rigitBody;

	Actor* actor[enCharacter_num];	//�L�����N�^�[�̐����̃A�N�^�[
	Actor* m_nowActor = nullptr;	// ���݂̃A�N�^�[
	Actor* m_subActor = nullptr;    // �T�u�̃A�N�^�[


	EnCharacters m_enActiveCharacter= enHero;

	Vector3 m_position = Vector3::Zero;		//���݂̃L�����N�^�[�̍��W

	Vector3 m_moveSpeed = Vector3::Zero;

	const float m_dieToChangeTime = 1.0f;

	float m_dieToChangeTimer = 0.0f;

	bool m_informGameFlag = false;



	bool m_gameEndFlag = false;		//���������ւ�炸�o�g�����I������Ƃ���true�ɂ���

};

