#pragma once
#include "AIActor.h"
#include "DamageFont.h"

/// <summary>
/// �����X�^�[�n�̃N���X
/// </summary>
class MonsterBase:public AIActor
{
public:
	MonsterBase();
	virtual ~MonsterBase();

	/////////////////////////////////////////////////////////////////////////////
	// ���z�֐��A�������z�֐��̐錾
	/////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// �U������
	/// </summary>
	virtual void Attack(){}
	/// <summary>
	/// Update()�������~�߂邩
	/// </summary>
	/// <returns></returns>
	virtual bool IsStopProcessing(){ return false; }

	/// <summary>
	/// ����̃A�j���[�V�������Đ�����
	/// </summary>
	/// <returns></returns>
	virtual bool isAnimationEnable() const override = 0;
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
	/// ��_���[�W�p�����蔻��
	/// </summary>
	virtual void DamageCollision(CharacterController& characon);

	//�ʏ�U���ɓ����������̏���
	virtual void HitNormalAttack();

	//�X�L���ɓ����������̏���
	virtual void HitSkillAttack();

	/// <summary>
	/// �����蔻��̏��������邩
	/// </summary>
	virtual bool IsCollisionDetection();

	/// <summary>
	///  �ړ����̉�]
	/// </summary>
	/// <param name="rotSpeed">���`��Ԏ��̉�]���x(�ړ���)</param>
	/// <param name="rotOnlySpeed">���`��Ԏ��̉�]���x(��]�݂̂̎�)</param>
	/// <returns></returns>
	virtual Quaternion Rotation(float rotSpeed, float rotOnlySpeed);

	/// <summary>
	/// ��]�݂̂��s����������
	/// </summary>
	/// <returns></returns>
	virtual bool RotationOnly() = 0;

	/// <summary>
	/// �q�b�g�G�t�F�N�g����
	/// </summary>
	virtual void CreateHitEffect() {}

	/// <summary>
	/// ���ꂽ�Ƃ��̏���
	/// </summary>
	/// <param name="seFlag">���ꂽ�Ƃ��̌��ʉ����Đ����邩�̃t���O</param>
	virtual void ProcessDead(bool seFlag = true) = 0;

	/////////////////////////////////////////////////////////////////////////////
	// ���̑��̊֐�
	/////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="status">���g�̃X�e�[�^�X</param>
	/// <param name="targetposition">�����������^�[�Q�b�g�̍��W</param>
	/// <param name="dashFlag">�_�b�V���t���O�Btrue�Ń_�b�V���B��Ƀv���C���[��ǂ�������Ƃ��Ɏg��</param>
	/// <returns></returns>
	Vector3 CalcVelocity(Status status, Vector3 targetposition, bool dashFlag = false);
	/// <summary>
	/// ����p����
	/// </summary>
	/// <param name="toPlayerDir">���g����^�[�Q�b�g�Ɍ������x�N�g��</param>
	/// <param name="forward">�O����</param>
	/// <param name="angle">����p</param>
	/// <returns>����p�̒��Ȃ�true,���Ȃ�������false</returns>
	bool IsInFieldOfView(Vector3 toPlayerDir, Vector3 forward, float angle);
	/// <summary>
	/// �v���C���[��������
	/// �~�`�Ō���
	/// </summary>
	/// <param name="distance">�v���C���[�������邱�Ƃ��ł������̋���</param>
	/// <returns>��������������true��Ԃ�</returns>
	bool IsFindPlayer(float distance);

	/// <summary>
	/// �_���[�W�t�H���g����
	/// </summary>
	void CreateDamageFont(int damage);



	//todo�@�Ȃ�������
	/// <summary>
	/// �U�����Ă��玟�̍U�����s���܂ł̃C���^�[�o��
	/// </summary>
	/// <returns></returns>
	bool AttackInterval(const float attackintarvaltime);
	/// <summary>
	/// �������x�N�g����ς���C���^�[�o���̌v�Z
	/// </summary>
	/// <returns></returns> 
	bool AngleChangeTimeIntarval(float LimitTime);


protected:
	/// <summary>
	/// ���f���̏�����
	/// </summary>
	virtual void InitModel() override = 0;

	/// <summary>
	/// �A�j���[�V�������Đ�
	/// </summary>
	virtual void PlayAnimation() override = 0;

	/// <summary>
	/// �X�e�[�g�Ǘ�
	/// </summary>
	virtual void ManageState() override = 0;



	int							m_damage = 0;							//�󂯂��_���[�W��������

	bool						m_dashFlag = false;						//�_�b�V�����邩�̃t���O
	bool						m_attackEnableFlag = true;					//�U�����Ă������̃t���O�Bfalse�ōU���\
	bool						m_damageFlag = false;					//�_���[�W���󂯂��邩�̃t���O�Bfalse�Ŕ�_���[�W�\

	float						m_attackIntervalTimer = 0.0f;			//�A�^�b�N�̃C���^�[�o���^�C�}�[
	float						m_damageIntervalTimer = 0.0f;			//�_���[�W�̃C���^�[�o���^�C�}�[

	float						m_distance = 0.0f;						//�v���C���[�𔭌��ł��鋗��

	float						m_angle = 0.0f;							//����p

	float						m_angleChangeTimer = 0.0f;				//�ړ����������������^�C�~���O���v�Z����^�C�}�[
	bool						m_angleChangeTimeFlag = false;			//�ړ���������������邩�̃t���O

	bool						m_elaseListFlag = false;				//���g�����X�g����폜�������̃t���O

	float						m_rotTimer = 0.0f;						//��]�̐��`��ԂŎg���^�C�}�[


};

