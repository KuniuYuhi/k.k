#pragma once
#include "Actor.h"

class Hero;
class Wizard;
class Actor;
class Game;

class Brave;

class Player:public IGameObject
{
public:

	enum EnCharacters {
		enHero,
		enWizard,
		enBrave,
		enCharacter_num
	};

	Player();
	~Player();
	bool Start();
	void Update();

	/// <summary>
	/// �s���s�\���ǂ����̔���\
	/// </summary>
	/// <returns>true�ŕs�\�Afalse�ŉ\</returns>
	bool IsInaction();

	/// <summary>
	/// ���s�����肵����
	/// </summary>
	/// <returns></returns>
	bool IsWinnerDecision();

	bool GameClear();

	bool GameOver();

	bool TimeUp();

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
	/// �L�����؂�ւ��̃N�[���^�C���̌v�Z
	/// </summary>
	/// <returns>true�Ő؂�ւ��\</returns>
	bool ChangeCharacterTime();

	/// <summary>
	/// �S�ł�����
	/// </summary>
	bool IsAnnihilation();

	/// <summary>
	/// ���W�̌v�Z
	/// </summary>
	void CalcPosition(Vector3 moveSpeed, float deltaTime);

	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

	/// <summary>
	/// ���W�̎擾�B���݂̃A�N�^�[�̍��W
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_nowActor->GetPosition();
	}

	const Vector3& GetMoveSpeed() const
	{
		return m_moveSpeed;
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
	/// �T�u�̃A�N�^�[�̃X�e�[�^�X��Ԃ�
	/// </summary>
	/// <returns></returns>
	Status GetSubActorStatus()
	{
		return m_subActor->GetStatus();
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
	/// ���݂̃A�N�e�B�u�ȃL�����N�^�[��Ԃ�
	/// </summary>
	/// <returns></returns>
	const EnCharacters& GetActiveCharacter() const
	{
		return m_enActiveCharacter;
	}

	/// <summary>
	/// ���݂̃L�����̎��񂾂��̃t���O�擾
	/// </summary>
	/// <returns></returns>
	const bool GetNowActorDieFlag() const
	{
		return m_nowActor->GetDieFlag();
	}
	/// <summary>
	/// �T�u�̃L�����̎��񂾂��̃t���O�擾
	/// </summary>
	/// <returns></returns>
	const bool GetSubActorDieFlag() const
	{
		return m_subActor->GetDieFlag();
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

	/// <summary>
	/// �L�����؂�ւ��t���O�̐ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetChangCharacterFlag(bool flag)
	{
		m_ChangCharacterFlag = flag;
	}
	/// <summary>
	/// �L�����؂�ւ��t���O�̎擾
	/// </summary>
	/// <returns></returns>
	const bool& GetChangCharacterFlag() const
	{
		return m_ChangCharacterFlag;
	}
	/// <summary>
	/// �L�����؂�ւ���̌v�Z���̃^�C�}�[�̒l��Ԃ�
	/// </summary>
	/// <returns></returns>
	const float& GetChangeCharacterTimer() const
	{
		return m_ChangeCharacterTimer;
	}

	/// <summary>
	/// GameUI�p�̃L�����؂�ւ��t���O�̐ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetChangCharacterFlagForGameUI(bool flag)
	{
		m_ChangCharacterForGameUIFlag = flag;
	}
	/// <summary>
	/// GameUI�p�̃L�����؂�ւ��t���O�̎擾
	/// </summary>
	/// <returns></returns>
	const bool& GetChangCharacterFlagForGameUI() const
	{
		return m_ChangCharacterForGameUIFlag;
	}

	/// <summary>
	/// �s�����Ȃ��t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetDontActionFlag(bool flag)
	{
		m_dontActionFlag = flag;
	}
	/// <summary>
	/// �s�����Ȃ��t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetmDontActionFlag() const
	{
		return m_dontActionFlag;
	}

private:
	Game*						m_game = nullptr;
	Hero*						m_hero = nullptr;
	Wizard*						m_wizard = nullptr;
	Brave*						m_brave = nullptr;

	FontRender					m_mpFont;
	FontRender					m_hpFont;

	//RigidBody					rigitBody;

	Actor*						actor[enCharacter_num];							//�L�����N�^�[�̐����̃A�N�^�[
	Actor*						m_nowActor = nullptr;							//���݂̃A�N�^�[
	Actor*						m_subActor = nullptr;							//�T�u�̃A�N�^�[


	EnCharacters				m_enActiveCharacter= enHero;

	Vector3						m_position = Vector3::Zero;						//���݂̃L�����N�^�[�̍��W

	Vector3						m_moveSpeed = Vector3::Zero;

	const float					m_dieToChangeTime = 1.0f;

	float						m_dieToChangeTimer = 0.0f;

	bool						m_informGameFlag = false;

	bool						m_ChangCharacterFlag = false;					//�L�����؂�ւ��t���O
	bool						m_ChangCharacterForGameUIFlag = false;			//�L�����؂�ւ��t���O�BgameUI�p

	const float					m_ChangeCharacterTime = 3.0f;
	float						m_ChangeCharacterTimer = 3.0f;					//�L�����؂�ւ��̃N�[���^�C��
	

	bool						m_gameEndFlag = false;							//���������ւ�炸�o�g�����I������Ƃ���true�ɂ���
	bool						m_VictoryAnimFlag = false;						//�����A�j���[�V�����Đ��t���O
	bool						m_idleAnimFlag = false;							//�A�C�h���A�j���[�V�����Đ��t���O

	bool						m_dontActionFlag = true;						//�s�����Ȃ��t���O�Btrue�ōs�����Ȃ�

};

