#pragma once
#include "Status.h"
#include "Player.h"

class AIActor:public IGameObject
{
public:
	AIActor();
	virtual ~AIActor();

	/// <summary>
	/// ���s�X�e�[�g
	/// </summary>
	enum EnOutCome
	{
		enOutCome_Win,
		enOutCome_Lose,
		enOutCome_None
	};

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
	/// �U�����̉����Đ�
	/// </summary>
	virtual void PlayAttackSound();

	/// <summary>
	/// �X�e�[�^�X�̎擾
	/// </summary>
	/// <returns></returns>
	const Status& GetStatus()
	{
		return m_status;
	}

	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}
	/// <summary>
	/// ��]��ݒ�
	/// </summary>
	/// <param name="rotation"></param>
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
	/// <summary>
	/// �O�������擾
	/// </summary>
	/// <returns></returns>
	const Vector3& GetForward() const
	{
		return m_forward;
	}

	/// <summary>
	/// �����t���O�̐ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetWinFlag(bool flag)
	{
		m_winFlag = flag;
	}
	/// <summary>
	/// �����t���O�̎擾
	/// </summary>
	/// <returns></returns>
	const bool& GetWinFlag() const
	{
		return m_winFlag;
	}

	/// <summary>
	/// ���s�X�e�[�g�̐ݒ�
	/// </summary>
	/// <param name="outCome"></param>
	void SetEnOutCome(EnOutCome outCome)
	{
		m_enOutCome = outCome;
	}
	/// <summary>
	/// ���s�X�e�[�g�̎擾
	/// </summary>
	/// <returns></returns>
	const EnOutCome& GetEnOutCome() const
	{
		return m_enOutCome;
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
	/// �^�[�Q�b�g�̍��W��m_targetPsition�ɑ������todo�@�Ȃ���
	/// </summary>
	void SetTargetPosition();

	/// <summary>
	/// ����̃A�j���[�V�������Đ�����
	/// </summary>
	/// <returns></returns>
	virtual bool isAnimationEnable() const = 0;



	Player*				m_player = nullptr;

	Status						m_status;

	Vector3						m_targetPosition = Vector3::Zero;		//�^�[�Q�b�g�̍��W���i�[

	Vector3						m_toTarget= Vector3::Zero;				//�^�[�Q�b�g�Ɍ������x�N�g��

	Vector3						m_position = Vector3::Zero;
	Vector3						m_rotMove = Vector3::Zero;
	Vector3						m_moveSpeed = Vector3::Zero;

	Vector3						m_forward = Vector3::Zero;				//�O����

	Vector3						m_SaveMoveSpeed = Vector3::Zero;		//��]�̂ݎg�p���鎞�Ɏg��

	Quaternion					m_rotation = Quaternion::Identity;
	Vector3						m_scale = Vector3::One;

	EnOutCome					m_enOutCome = enOutCome_None;


	bool						m_winFlag = false;						//���������̃t���O

};

