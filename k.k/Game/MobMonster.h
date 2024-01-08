#pragma once
#include "MonsterBase.h"
#include "KnockBack.h"


#include "MobMonsterActionList.h"

class IMobStateMachine;

class MobMonster:public MonsterBase
{
public:

	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="charaCon">�ړ����������I�u�W�F�N�g�̃L�����N�^�[�R���g���[���[</param>
	void MoveMonster(CharacterController& charaCon);

	/// <summary>
	/// �p�g���[�����̈ړ�����
	/// </summary>
	/// <param name="charaCon"></param>
	void MovePatrol(CharacterController& charaCon);
	
	/// <summary>
	/// �v���C���[��ǂ�������Ƃ��̈ړ�����
	/// </summary>
	/// <param name="charaCon"></param>
	void MoveChasePlayer(CharacterController& charaCon);

	/// <summary>
	/// �m�b�N�o�b�N����
	/// </summary>
	void ProcessKnockBack(CharacterController& charaCon);

	/// <summary>
	/// �������x�N�g����ݒ�
	/// </summary>
	/// <param name="range"></param>
	/// <returns></returns>
	Vector3 SetRamdomDirection(int range);

	/// <summary>
	/// ��(�X)�ɂԂ�������
	/// </summary>
	/// <returns></returns>
	virtual bool IsBumpedForest(float pos2Length);

	/// <summary>
	/// �����X�^�[�ɂԂ�������
	/// </summary>
	/// <returns></returns>
	bool IsBumpedMonster();

	/// <summary>
	/// �͂����p���[������������
	/// </summary>
	void SubPassPower();

	/// <summary>
	/// ����̃A�j���[�V�������Đ�����
	/// </summary>
	/// <returns></returns>
	virtual bool isAnimationEnable() const = 0;

	/// <summary>
	/// ��]�\��
	/// </summary>
	/// <returns></returns>
	virtual bool isRotationEnable() const = 0;

	/// <summary>
	/// �U���\��
	/// </summary>
	/// <returns></returns>
	virtual bool IsAttackEnable() const = 0;

	/// <summary>
	/// ��_���[�W������
	/// </summary>
	virtual void Damage(int attack) = 0;

	/// <summary>
	/// ���ꂽ�Ƃ��̏���
	/// </summary>
	/// <param name="seFlag">���ꂽ�Ƃ��̌��ʉ����Đ����邩�̃t���O</param>
	void ProcessDead(bool seFlag = true) override;

	/// <summary>
	/// �q�b�g�G�t�F�N�g����
	/// </summary>
	void CreateHitEffect();

	/// <summary>
	/// �m�b�N�o�b�N�t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetKnockBackFlag(bool flag)
	{
		m_knockBackFlag = flag;
	}
	/// <summary>
	/// �m�b�N�o�b�N�t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetKnockBackFlag() const
	{
		return m_knockBackFlag;
	}

	/// <summary>
	/// �m�b�N�o�b�N�ɂ��d������
	/// </summary>
	/// <returns>�d�����Ȃ�true</returns>
	const bool& IsKnockBackStiffness();

	///////////////////////////////////////////////////////////
	///�擾�A�ݒ�֐�
	///////////////////////////////////////////////////////////

	/// <summary>
	/// �v���C���[���߂��ɂ��邩�t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetPlayerNearbyFlag(bool flag)
	{
		m_playerNearbyFlag = flag;
	}
	/// <summary>
	/// �v���C���[���߂��ɂ��邩�t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetPlayerNearbyFlag() const
	{
		return m_playerNearbyFlag;
	}


	/// <summary>
	/// �G����������
	/// �I�[�o�[���C�h���Ȃ���Ύ���p����Ŕ��f
	/// </summary>
	/// <returns>��������true</returns>
	virtual bool IsFoundPlayerFlag();

	/// <summary>
	/// �U���͈͓��Ƀv���C���[�����邩�B
	/// </summary>
	/// <returns>������true</returns>
	virtual bool IsPlayerInAttackRange();
	
	/// <summary>
	/// �U���\���B���Ԋu�ōU�����邽��
	/// </summary>
	/// <returns></returns>
	bool IsProcessAttackEnable();
	
	/// <summary>
	/// �X�L���g�p�\��
	/// </summary>
	/// <returns>�g�p�\�Ȃ�true</returns>
	virtual bool IsSkillUsable();
	
	/// <summary>
	/// �X�L���̃C���^�[�o���̌v�Z
	/// </summary>
	void CalcSkillAttackIntarval();

	/// <summary>
	/// ���̃A�j���[�V�����X�e�[�g���쐬����B
	/// </summary>
	/// <param name="nextState"></param>
	virtual void SetNextAnimationState(MobMonsterInfo::EnAnimationState nextState){}

	/// <summary>
	/// ���̃X�e�[�g�}�V�����쐬����
	/// </summary>
	/// <param name="nextStateMachine"></param>
	virtual void SetNextStateMachine(MobMonsterInfo::EnStateMachineState nextStateMachine){}

protected:

	/// <summary>
	/// ���f���̏�����
	/// </summary>
	virtual void InitModel() = 0;

	/// <summary>
	/// �A�j���[�V�������Đ�
	/// </summary>
	virtual void PlayAnimation() = 0;

	/// <summary>
	/// �X�e�[�g�Ǘ�
	/// </summary>
	virtual void ManageState() = 0;

	/// <summary>
	/// ��]�݂̂��s����������
	/// </summary>
	/// <returns></returns>
	virtual bool RotationOnly();


protected:

	IMobStateMachine* m_mobStateMachine = nullptr;
	
	Vector3					m_passPower = g_vec3One;		//�͂����Ƃ��̗�

	Vector3					m_direction = Vector3::Zero;	//�ړ���������x�N�g��

	int					m_angleChangeTime = 0;		//�������x�N�g�����v�Z����^�C�}�[

	float					m_distanceToPlayer = 0.0f;		//�v���C���[�Ƃ̋���
	float					m_attackRange = 0.0f;			//�U���ł���͈�
	float					m_skillAttackRange = 0.0f;		//�X�L���U�����ł���͈�
	float					m_stayRange = 0.0f;				//�ړ����Ȃ��͈�

	float					m_ramdomAngle = 0.0f;			//�����_���ȃA���O���B�ړ����������߂�Ƃ��Ɏg��

	float					m_attackIntervalTime = 0.0f;	//�U��������̃C���^�[�o��

	int						m_angleRange = 2;				//�ړ�����A���O���͈̔�

	float					m_pos2Length = 0.0;				//�ǂƔ�

	bool                    m_chasePlayerFlag = false;		//�v���C���[��ǂ������邩�̃t���O

	bool					m_knockBackFlag = false;		//�m�b�N�o�b�N���邩�̃t���O

	float					m_knockBackTimer = 0.0f;		//�m�b�N�o�b�N�ňړ����鎞��

	bool					m_playerNearbyFlag = false;

	float m_knockBackStiffnessTimer = 0.0f;


	//�X�L���g�p�\���̃^�C�}�[
	float m_skillUsableLimmit = 5.0f;
	float m_skillUsableTimer = 0.0f;
	bool m_skillUsableFlag = true;


};

