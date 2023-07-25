#pragma once
#include "Status.h"
#include "Player.h"

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
	/// <returns>moveSpeed</returns>
	Vector3 calcVelocity(Status status);

	/// <summary>
	/// 
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
	/// ��_���[�W������
	/// </summary>
	virtual void Damage(int attack) = 0;

	/// <summary>
	/// ��_���[�W�p�����蔻��
	/// </summary>
	virtual void DamageCollision(CharacterController& characon);
	//�ʏ�U���ɓ����������̏���
	virtual void HitNormalAttack();
	//�q�[���[�̃X�L���ɓ����������̏���
	virtual void HitHeroSkillAttack();
	//�E�B�U�[�h�̃t�@�C���[�{�[���ɓ����������̏���
	virtual void HitFireBall();
	//�E�B�U�[�h�̃t���C���s���[�ɓ����������̏���
	virtual void HitFlamePillar();

	/// <summary>
	/// �X�e�[�^�X�̎擾
	/// </summary>
	/// <returns></returns>
	const Status& GetStatus()
	{
		return m_status;
	}

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns></returns>
	const Vector3 GetPosition()
	{
		return m_position;
	}

	/// <summary>
	/// �g�嗦�̎擾
	/// </summary>
	/// <returns></returns>
	const Vector3 GetScale()
	{
		return m_scale;
	}

	/// <summary>
	/// ��]�̎擾
	/// </summary>
	/// <returns></returns>
	const Quaternion GetRotation()
	{
		return m_rotation;
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

	Vector3 m_position = Vector3::Zero;
	Vector3 m_oldMoveSpeed = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;

	Vector3 m_SaveMoveSpeed = Vector3::Zero;		//��]�̂ݎg�p���鎞�Ɏg��

	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;

	bool m_dashFlag = false;		//�_�b�V�����邩�̃t���O
	bool m_attackFlag = false;		//�U�����Ă������̃t���O�Bfalse�ōU���\
	bool m_damageFlag = false;		//�_���[�W���󂯂��邩�̃t���O�Bfalse�Ŕ�_���[�W�\

	float m_attackIntervalTimer = 0.0f;		//�A�^�b�N�̃C���^�[�o���^�C�}�[
	float m_damageIntervalTimer = 0.0f;		//�_���[�W�̃C���^�[�o���^�C�}�[

	float m_distance = 0.0f;	//�v���C���[�𔭌��ł��鋗��

};

