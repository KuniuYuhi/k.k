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
	virtual void Attack() override {}
	/// <summary>
	/// Update()�������~�߂邩
	/// </summary>
	/// <returns></returns>
	virtual bool IsStopProcessing() override { return false; }

	/// <summary>
	/// ����̃A�j���[�V�������Đ�����
	/// </summary>
	/// <returns></returns>
	virtual bool isAnimationEntable() const override = 0;
	/// <summary>
	/// ��]�\��
	/// </summary>
	/// <returns></returns>
	virtual bool isRotationEntable() const override = 0;
	/// <summary>
	/// �U���\��
	/// </summary>
	/// <returns></returns>
	virtual bool IsAttackEntable() const override = 0;
	/// <summary>
	/// ��_���[�W������
	/// </summary>
	virtual void Damage(int attack) override = 0;

	/// <summary>
	/// �_���[�W�t�H���g����
	/// </summary>
	virtual void CreateDamageFont(int damage);

	/// <summary>
	/// ��_���[�W�p�����蔻��
	/// </summary>
	virtual void DamageCollision(CharacterController& characon){}

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
	/// </summary>
	/// <param name="distance">�v���C���[�������邱�Ƃ��ł������̋���</param>
	/// <returns>��������������true��Ԃ�</returns>
	bool IsFindPlayer(float distance);












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


};

