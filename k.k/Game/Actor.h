#pragma once
#include "Status.h"
#include "DamageFont.h"
#include "KnockBack.h"

class AIActor;

class Actor:public IGameObject
{
public:

	Actor();
	virtual ~Actor();

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
	void SetTransForm(Vector3 position, Quaternion rotation,Vector3 scale=Vector3::One);

	/// <summary>
	/// ���W�ݒ�
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

	///////////////////////////////////////////////////////////////
	//���z�֐��A�������z�֐�
	///////////////////////////////////////////////////////////////

	/// <summary>
	/// �R���W�����I�u�W�F�N�g�ɓ����������̏���
	/// </summary>
	/// <param name="characon"></param>
	virtual void DamageCollision(CharacterController& characon);

	/// <summary>
	/// ��_���[�W����
	/// </summary>
	/// <param name="attack">����̍U����</param>
	virtual void Damage(int attack) = 0;
	/// <summary>
	/// ����̃A�j���[�V�������Đ�����
	/// </summary>
	/// <returns></returns>
	virtual bool isAnimationEntable() const = 0;
	/// <summary>
	/// �����蔻��\�ȃA�j���[�V������
	/// </summary>
	/// <returns></returns>
	virtual bool isCollisionEntable() const = 0;
	/// <summary>
	/// ��]�\�ȃA�j���[�V�������Đ�����
	/// </summary>
	/// <returns></returns>
	virtual bool isRotationEntable() const = 0;
	/// <summary>
	/// �������̏���
	/// </summary>
	virtual void ProcessWin() = 0;

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
	/// �X�L���̎g�p���Ȃǂ̈ړ��͂��Ȃ�����]�͂������Ƃ��Ɏg��
	/// </summary>
	virtual bool RotationOnly() = 0;

	/// <summary>
	/// �O�i����t���O�̎擾
	/// </summary>
	/// <returns></returns>
	virtual const bool& GetMoveForwardFlag() const
	{
		return false;
	}

	/// <summary>
	/// �O�������ݒ�ł��������
	/// </summary>
	/// <returns>true�ŉ\�Afalse�ŕs�\</returns>
	virtual const bool& IsSetForwardCondition() const
	{
		return true;
	}

	///////////////////////////////////////////////////////////////
	//���̑��̊֐�
	///////////////////////////////////////////////////////////////

	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="status">�X�e�[�^�X</param>
	/// <returns>moveSpeed</returns>
	Vector3 calcVelocity(Status status);
	/// <summary>
	/// �O�����̌v�Z
	/// </summary>
	/// <param name="moveSpeed"></param>
	void CalcForward(Vector3 moveSpeed);

public:
	/// <summary>
	/// ���G���Ԃ̌v�Z
	/// </summary>
	bool CalcInvincibleTime();

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
	/// <summary>
	/// �X�e�[�^�X�̎擾
	/// </summary>
	/// <returns></returns>
	const Status& GetStatus() const
	{
		return m_status;
	}
	/// <summary>
	/// �O�����̎擾
	/// </summary>
	/// <returns></returns>
	const Vector3& GetForward() const
	{
		return m_forward;
	}
	/// <summary>
	/// moveSpeed�̎擾
	/// </summary>
	/// <returns></returns>
	const Vector3& GetMoveSpeed() const
	{
		return m_moveSpeed;
	}

	/// <summary>
	/// ���g�����ꂽ���̃t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetDieFlag(bool flag)
	{
		m_dieFlag = flag;
	}
	/// <summary>
	/// ���g�����ꂽ���̃t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool GetDieFlag() const
	{
		return m_dieFlag;
	}

	/// <summary>
	/// ���G���ԃt���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetInvicibleTimeFlag(bool flag)
	{
		m_invincibleTimeFlag = flag;
	}
	/// <summary>
	/// ���G���ԃt���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetInvincibleTimeFlag()
	{
		return m_invincibleTimeFlag;
	}

	/// <summary>
	/// ���G��ԃt���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetInvicibleFlag(bool flag)
	{
		m_invicibleFlag = flag;
	}
	/// <summary>
	/// ���G��ԃt���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetInvicibleFlag() const
	{
		return m_invicibleFlag;
	}

	/// <summary>
	/// �m�b�N�o�b�N�p���[��ݒ�
	/// </summary>
	/// <param name="power"></param>
	void SetKnockBackPower(float power)
	{
		m_knockBackPower = power;
	}
	/// <summary>
	/// �m�b�N�o�b�N�p���[���擾
	/// </summary>
	/// <returns></returns>
	const float& GetKnockBackPower() const
	{
		return m_knockBackPower;
	}
	/// <summary>
	/// �m�b�N�o�b�N�q�b�g�t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetHitKnockBackFlag(bool flag)
	{
		m_hitKnockBackFlag = flag;
	}
	/// <summary>
	/// �m�b�N�o�b�N�q�b�g�t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool& GetHitKnockBackFlag() const
	{
		return m_hitKnockBackFlag;
	}

	/// <summary>
	/// ���f���h���[�t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetModelDrawFlag(bool flag)
	{
		m_modelDrawFlag = flag;
	}

protected:

	/// <summary>
	/// �_�ł��邩�B
	/// </summary>
	/// <returns>true�Ŕ�\���Afalse�ŕ\��</returns>
	bool IsFlashing();

	/// <summary>
	/// �ړ����̉�]
	/// </summary>
	/// <returns></returns>
	Quaternion Rotation(float rotSpeed, float rotOnlySpeed);

	/// <summary>
	/// �_�b�V������X�e�[�g �g��Ȃ��Ȃ邩��
	/// </summary>
	enum EnDashInvicibleState
	{
		enDashInvicibleState_None,
		enDashInvicibleState_On,
		enDashInvicibleState_Off
	};

	/// <summary>
	/// �_�b�V������X�e�[�g�̐ݒ�
	/// </summary>
	/// <param name="enDashInvicibleState"></param>
	void SetInvicibleDashState(EnDashInvicibleState enDashInvicibleState)
	{
		m_enDashInvicibleState = enDashInvicibleState;
	}
	/// <summary>
	/// �_�b�V������X�e�[�g�̎擾
	/// </summary>
	/// <returns></returns>
	const EnDashInvicibleState& GetInvicibleDashState() const
	{
		return m_enDashInvicibleState;
	}

	/// <summary>
	/// �_�b�V�������Ƃ��̖��G���Ԃ̌v�Z
	/// </summary>
	/// <returns></returns>
	bool CalcInvicibleDash();
	
	/// <summary>
	/// �_���[�W�t�H���g����
	/// </summary>
	/// <param name="damage"></param>
	void CreateDamageFont(int damage);
	
	/// <summary>
	/// �����蔻����Ƃ邩
	/// </summary>
	/// <returns>�����蔻����Ƃ�Ƃ���true�A�Ƃ�Ȃ��Ƃ���false</returns>
	bool IsDecisionCollision();

	
	
protected:

	Status							m_status;
	AIActor*						m_atttackAIActor = nullptr;							//�U�����Ă���AIActor(�����X�^�[)
	
	EnDashInvicibleState			m_enDashInvicibleState = enDashInvicibleState_None;	//�_�b�V�������Ƃ��̖��G���Ԃ̂��߂̃X�e�[�g

	Vector3							m_forward = { 0.0f,0.0f,1.0f };						//�O����
	Vector3							m_position = Vector3::Zero;							//���g�̍��W
	Vector3							m_moveSpeed = Vector3::Zero;						//�ړ�����

	Vector3							m_SaveMoveSpeed = Vector3::Zero;					//��]�̂ݎg�p���鎞�Ɏg��
	Vector3							m_rotMove = g_vec3Zero;								//��]�̌���

	Quaternion						m_rotation = Quaternion::Identity;
	Vector3							m_scale = Vector3::One;

	bool							m_dashFlag = false;									//�_�b�V�����邩�̃t���O

	bool							m_dieFlag = false;									//���ꂽ��true�ɂ���
	
	bool							m_invicibleFlag = false;							//���G��ԃt���O(�A�N�V�������ɖ��G��Ԃɂ������Ƃ��g��)

	bool							m_invincibleTimeFlag = false;						//���G���Ԃł��邩�̃t���O(�_���[�W���󂯂���ȂǂɎg��)
	const float						m_invincbleTime = 1.5f;
	float							m_invincbleTimer = 0.0f;

	const float						m_invincbleDashTime = 0.3f;							//���G��ԃ_�b�V���̎���
	float							m_invincbledDashTimer = 0.0f;

	bool							m_modelDrawFlag = false;							//���f����`�悷�邩�̃t���O

	int								m_hitDamage = 0;									//�U�����ꂽ�Ƃ��̃_���[�W���L��

	float							m_knockBackPower = 0.0f;							//�m�b�N�o�b�N�̋���

	bool							m_hitKnockBackFlag = false;							//���g���m�b�N�o�b�N���邩


};

