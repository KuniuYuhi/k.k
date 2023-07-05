#pragma once
#include "Status.h"
#include "Player.h"

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

	void DeleteCharaCon()
	{
		//delete m_charaCon
	}

	/// <summary>
	/// �L�����R���̍��W�ݒ�
	/// </summary>
	/// <param name="position"></param>
	void SetCharaConPosition(Vector3 position)
	{
		//m_charaCon.SetPosition(position);
	}

	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="status">�X�e�[�^�X</param>
	/// <returns>moveSpeed</returns>
	Vector3 calcVelocity(Status status);

	/// <summary>
	/// ���W�̎擾
	/// </summary>
	/// <returns></returns>
	const Vector3 GetPosition()
	{
		return m_position;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const Vector3 GetScale()
	{
		return m_scale;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const Quaternion GetRotation()
	{
		return m_rotation;
	}

	const Status GetStatus()
	{
		return m_status;
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

	

protected:
	Status m_status;
	Player* m_player = nullptr;

	//CharacterController m_charaCon;

	Vector3 m_position = Vector3::Zero;
	Vector3 m_moveSpeed = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;

	bool m_dashFlag = false;		//�_�b�V�����邩�̃t���O

};

