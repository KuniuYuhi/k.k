#pragma once

#include "Brave.h"


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
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position"></param>
	void SetPlayerPosition(Vector3 position)
	{
		m_brave->SetPosition(position);
	}
	/// <summary>
	/// ���W�̎擾�B���݂̃A�N�^�[�̍��W
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_brave->GetPosition();
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
		return m_brave->GetStatus();
	}

	/// <summary>
	/// ���݂̃A�N�^�[�̑O�����̎擾�B���K���ς�
	/// </summary>
	/// <returns></returns>
	const Vector3& GetForward() const
	{
		return m_brave->GetForward();
	}

	/// <summary>
	/// ���݂̃L�����̎��񂾂��̃t���O�擾
	/// </summary>
	/// <returns></returns>
	const bool GetNowActorDieFlag() const
	{
		return m_brave->GetDieFlag();
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

	/// <summary>
	/// �m�b�N�o�b�N�p���[���擾
	/// </summary>
	/// <returns></returns>
	const float& GetKnockBackPower() const
	{
		return m_brave->GetKnockBackPower();
	}
	/// <summary>
	/// �m�b�N�o�b�N�q�b�g�t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetHitKnockBackFlag(bool flag)
	{
		m_brave->SetHitKnockBackFlag(flag);
	}
	/// <summary>
	/// �m�b�N�o�b�N�̏���ݒ�
	/// </summary>
	/// <param name="flag">�m�b�N�o�b�N�q�b�g�t���O</param>
	/// <param name="endPos">�I�_</param>
	/// <param name="KnockBackPower">�m�b�N�o�b�N�p���[</param>
	void SetKnockBackInfo(
		bool flag, Vector3 endPos, float KnockBackPower,float knockBackLimmit=1.0f);
	/// <summary>
	/// �m�b�N�o�b�N�q�b�g�t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetHitKnockBackFlag() const
	{
		return m_brave->GetHitKnockBackFlag();
	}



	/// <summary>
	/// �U�����q�b�g�������̃t���O��ݒ�B�ʏ�U��
	/// </summary>
	/// <param name="flag"></param>
	void SetAttackHitFlag(bool flag)
	{
		m_brave->SetAttackHitFlag(flag);
	}

	/// <summary>
	/// ���i�q�b�g�U�����q�b�g�������̃t���O��ݒ�B
	/// </summary>
	/// <param name="flag"></param>
	void SetHittableFlag(bool flag)
	{
		m_brave->GetWeapon(enMaxWeapons_Main)->SetHittableFlag(flag);
	}
	/// <summary>
	/// ���i�q�b�g�U�����q�b�g�������̃t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetHittableFlag() const
	{
		return m_brave->GetWeapon(enMaxWeapons_Main)->GetHittableFlag();
	}

	/// <summary>
	/// ����؂�ւ������t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetChangeWeaponCompleteFlag(bool flag)
	{
		m_brave->SetChangeWeaponCompleteFlag(flag);
	}
	/// <summary>
	/// ����؂�ւ������t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetChangeWeaponCompleteFlag() const
	{
		return m_brave->GetChangeWeaponCompleteFlag();
	}
	/// <summary>
	/// �؂�ւ��Ώۂ̕�����擾
	/// </summary>
	/// <returns></returns>
	const EnMaxWeapons& GetChangeTargetUseWeapon() const
	{
		return m_brave->GetChangeTargetUseWeapon();
	}

	/// <summary>
	/// �v���C���[�����ꂽ���̃t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetIsPlayerDeadFlag(bool flag)
	{
		m_isPlayerDeadFlag = flag;
	}
	/// <summary>
	/// �v���C���[�����ꂽ���̃t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetmIsPlayerDeadFlag() const
	{
		return m_isPlayerDeadFlag;
	}

	/// <summary>
	/// ���݂̕���̑ϋv�l�̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetNowWeaponEndrance() const
	{
		return m_brave->GetWeapon(enMaxWeapons_Main)->GetStatus().GetEndurance();
	}

	/// <summary>
	/// ���݂̕���̑ϋv�l�̍ő�l�̎擾
	/// </summary>
	/// <returns></returns>
	const int& GetNowWeaponMaxEndrance() const
	{
		return m_brave->GetWeapon(enMaxWeapons_Main)->GetStatus().GetMaxEndrance();
	}
	/// <summary>
	/// �L�����R���̎擾
	/// </summary>
	/// <returns></returns>
	CharacterController& GetCharacterController()
	{
		return m_brave->GetCharacterController();
	}

private:
	/// <summary>
	/// �v���C���[�����ꂽ��̏���
	/// </summary>
	void ProcessPlayerDead();


private:


	Game*						m_game = nullptr;
	Brave*						m_brave = nullptr;

	FontRender					m_mpFont;
	FontRender					m_hpFont;

	Vector3						m_position = g_vec3Zero;						//���݂̃L�����N�^�[�̍��W

	Vector3						m_moveSpeed = g_vec3Zero;

	Vector3						m_attackPosition = g_vec3Zero;

	bool						m_informGameFlag = false;

	bool						m_gameEndFlag = false;							//���������ւ�炸�o�g�����I������Ƃ���true�ɂ���
	bool						m_VictoryAnimFlag = false;						//�����A�j���[�V�����Đ��t���O
	bool						m_idleAnimFlag = false;							//�A�C�h���A�j���[�V�����Đ��t���O

	bool						m_dontActionFlag = true;	//�s�����Ȃ��t���O�Btrue�ōs�����Ȃ�

	bool						m_decisionOutComeFlag = false;

	bool						m_isPlayerDeadFlag = false;		//�v���C���[�����ꂽ���̃t���O

};

