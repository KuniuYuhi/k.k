#pragma once
#include "Status.h"
#include "Player.h"
#include "FireBall.h"
#include "FlamePillar.h"
#include "DamageFont.h"

class AIActor:public IGameObject
{
public:
	AIActor();
	virtual ~AIActor();

	/// <summary>
	/// ���f���̍��W�A��]�A�T�C�Y�̐ݒ�
	/// </summary>
	/// <param name="modelRender"></param>
	void SetTransFormModel(ModelRender& modelRender);

	/// <summary>
	/// ���W�A��]�A�T�C�Y�̐ݒ�
	/// </summary>
	/// <param name="position"></param>
	/// <param name="scale"></param>
	/// <param name="rotation"></param>
	void SetTransForm(Vector3 position, Quaternion rotation, Vector3 scale = Vector3::One);

	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="status">�X�e�[�^�X</param>
	/// <param name="targetposition">�����������^�[�Q�b�g�̍��W</param>
	/// <returns>moveSpeed</returns>
	Vector3 CalcVelocity(Status status,Vector3 targetposition);

	//�^�[�Q�b�g�Ɍ������x�N�g��

	/// <summary>
	/// ����p����
	/// </summary>
	/// <param name="toPlayerDir">���g����^�[�Q�b�g�Ɍ������x�N�g��</param>
	/// <param name="forward">�O����</param>
	/// <param name="angle">����p</param>
	/// <returns>����p�̒��Ȃ�true,���Ȃ�������false</returns>
	bool IsInFieldOfView(Vector3 toPlayerDir,Vector3 forward, float angle);

	/// <summary>
	/// �U������
	/// </summary>
	virtual void Attack();

	/// <summary>
	/// �^�[�Q�b�g�̍��W��m_targetPsition�ɑ������
	/// </summary>
	void SetTargetPosition();

	/// <summary>
	/// �U�����Ă��玟�̍U�����s���܂ł̃C���^�[�o��
	/// </summary>
	/// <returns></returns>
	bool AttackInterval(const float attackintarvaltime);

	/// <summary>
	/// �_���[�W���󂯂���̃_���[�W���󂯂�܂ł̃C���^�[�o��
	/// </summary>
	/// <returns></returns>
	bool DamageInterval(const float damageintarvaltime);

	/// <summary>
	/// �������x�N�g����ς���C���^�[�o���̌v�Z
	/// </summary>
	/// <returns></returns> 
	bool AngleChangeTimeIntarval(float LimitTime);

	/// <summary>
	/// �v���C���[��������
	/// </summary>
	/// <param name="distance">�v���C���[�������邱�Ƃ��ł������̋���</param>
	/// <returns>��������������true��Ԃ�</returns>
	bool IsFindPlayer(float distance);

	/// <summary>
	/// ����̃A�j���[�V�������Đ�����
	/// </summary>
	/// <returns></returns>
	virtual bool isAnimationEntable() const = 0;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	virtual bool isRotationEntable() const = 0;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	virtual bool IsAttackEntable() const = 0;

	/// <summary>
	/// ��_���[�W������
	/// </summary>
	virtual void Damage(int attack) = 0;

	/// <summary>
	/// �_���[�W�t�H���g����
	/// </summary>
	virtual void CreateDamageFont(int damage);

	/// <summary>
	/// ��_���[�W�p�����蔻��
	/// </summary>
	virtual void DamageCollision(CharacterController& characon);
	//�ʏ�U���ɓ����������̏���
	virtual void HitNormalAttack();
	//�q�[���[�̃X�L���ɓ����������̏���
	virtual void HitHeroSkillAttack();
	/// <summary>
	/// �E�B�U�[�h�̃t�@�C���[�{�[���ɓ����������̏����B�h���N���X�Ŏ���
	/// </summary>
	virtual void HitFireBall();
	/// <summary>
	/// �E�B�U�[�h�̃t���C���s���[�ɓ����������̏����B�h���N���X�Ŏ���
	/// </summary>
	virtual void HitFlamePillar();

	/// <summary>
	/// �X�e�[�^�X�̎擾
	/// </summary>
	/// <returns></returns>
	const Status& GetStatus()
	{
		return m_status;
	}

	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

	void SetRotation(Quaternion rotation)
	{
		m_rotation = rotation;
	}

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// <summary>
	/// �g�嗦�̎擾
	/// </summary>
	/// <returns></returns>
	const Vector3& GetScale() const
	{
		return m_scale;
	}

	/// <summary>
	/// ��]�̎擾
	/// </summary>
	/// <returns></returns>
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}

	const Vector3& GetForward() const
	{
		return m_forward;
	}

	void SetWinFlag(bool flag)
	{
		m_winFlag = flag;
	}

	const bool& GetWinFlag() const
	{
		return m_winFlag;
	}

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
	/// �ړ����̉�]
	/// </summary>
	/// <returns></returns>
	Quaternion Rotation();

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	virtual bool RotationOnly() = 0;

	Player* m_player = nullptr;

	Status m_status;

	Vector3 m_targetPosition = Vector3::Zero;		//�^�[�Q�b�g�̍��W���i�[

	Vector3 m_toTarget= Vector3::Zero;				//�^�[�Q�b�g�Ɍ������x�N�g��

	Vector3 m_position = Vector3::Zero;
	Vector3 m_oldPosition= Vector3::Zero;
	Vector3 m_oldMoveSpeed = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;

	Vector3 m_forward = Vector3::Zero;				//�O����

	Vector3 m_SaveMoveSpeed = Vector3::Zero;		//��]�̂ݎg�p���鎞�Ɏg��

	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;

	int m_damage = 0;				//�󂯂��_���[�W��������

	bool m_dashFlag = false;		//�_�b�V�����邩�̃t���O
	bool m_attackFlag = true;		//�U�����Ă������̃t���O�Bfalse�ōU���\
	bool m_damageFlag = false;		//�_���[�W���󂯂��邩�̃t���O�Bfalse�Ŕ�_���[�W�\

	float m_attackIntervalTimer = 0.0f;		//�A�^�b�N�̃C���^�[�o���^�C�}�[
	float m_damageIntervalTimer = 0.0f;		//�_���[�W�̃C���^�[�o���^�C�}�[

	float m_distance = 0.0f;	//�v���C���[�𔭌��ł��鋗��

	float m_angle = 0.0f;			//����p

	bool m_winFlag = false;

	float m_angleChangeTimer = 0.0f;
	bool m_angleChangeTimeFlag = false;

	bool m_elaseListFlag = false;			//���g�����X�g����폜�������̃t���O

};

