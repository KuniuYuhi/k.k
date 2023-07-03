#pragma once
#include "Status.h"

class Actor:public IGameObject
{
public:

	Actor();
	virtual ~Actor();

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns></returns>
	const Vector3 GetPosition()
	{
		return m_position;
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
	/// ���W�A��]�A�T�C�Y�̐ݒ�
	/// </summary>
	/// <param name="modelRender"></param>
	void SetTransFormModel(ModelRender& modelRender);

	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="status">�X�e�[�^�X</param>
	/// <returns>moveSpeed</returns>
	Vector3 calcVelocity(Status status);

	/// <summary>
	/// �ړ����̉�]
	/// </summary>
	/// <returns></returns>
	Quaternion Rotation();

	

protected:
	Status m_status;

	Vector3 m_position = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;

	bool m_dashFlag = false;		//�_�b�V�����邩�̃t���O

};

