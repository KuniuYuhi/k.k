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

	void SetTargetPosition();

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

	float m_distance = 0.0f;	//�v���C���[�𔭌��ł��鋗��

};

