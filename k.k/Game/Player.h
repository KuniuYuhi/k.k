#pragma once
#include "Actor.h"

class Actor;
class Game;

class Brave;

class Player:public IGameObject
{
public:

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
	/// �v���C���[�����ꂽ���̔���
	/// </summary>
	bool IsDeadPlayer();

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
	/// ���݂̃A�N�^�[�̑O�����̎擾�B���K���ς�
	/// </summary>
	/// <returns></returns>
	const Vector3& GetForward() const
	{
		return m_nowActor->GetForward();
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
	/// ���݂̃L�����N�^�[�̍U���͂�Ԃ�
	/// </summary>
	/// <returns></returns>
	int GetAtk();

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
	Brave*						m_brave = nullptr;

	FontRender					m_mpFont;
	FontRender					m_hpFont;

	Actor*						m_nowActor = nullptr;							//���݂̃A�N�^�[

	Vector3						m_position = Vector3::Zero;						//���݂̃L�����N�^�[�̍��W

	Vector3						m_moveSpeed = Vector3::Zero;

	bool						m_informGameFlag = false;

	bool						m_gameEndFlag = false;							//���������ւ�炸�o�g�����I������Ƃ���true�ɂ���
	bool						m_VictoryAnimFlag = false;						//�����A�j���[�V�����Đ��t���O
	bool						m_idleAnimFlag = false;							//�A�C�h���A�j���[�V�����Đ��t���O

	bool						m_dontActionFlag = true;	//�s�����Ȃ��t���O�Btrue�ōs�����Ȃ�

	bool						m_decisionOutComeFlag = false;

};

