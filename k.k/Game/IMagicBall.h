#pragma once
#include "MagicBallStatus.h"


class MagicBallStatus;
class CharacterBase;
class DamageProvider;

/// <summary>
/// ���@���̐e�N���X
/// </summary>
class IMagicBall:public IGameObject
{
public:

	virtual ~IMagicBall() {}

	/// <summary>
	/// ���[�J�����W��ݒ�
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}
	/// <summary>
	/// ���[�J�����W���擾
	/// </summary>
	/// <returns></returns>
	Vector3 GetPosition()
	{
		return m_position;
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
	/// ��]���擾
	/// </summary>
	/// <returns></returns>
	Quaternion GetRotation()
	{
		return m_rotation;
	}

	/// <summary>
	/// �g�嗦��ݒ�
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(Vector3 scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// �g�嗦���擾
	/// </summary>
	/// <returns></returns>
	Vector3 GetScale()
	{
		return m_scale;
	}

	/// <summary>
	/// �O������ݒ�
	/// </summary>
	/// <param name="forward"></param>
	void SetForward(Vector3 forward)
	{
		m_forward = forward;
	}
	/// <summary>
	/// �O�������擾
	/// </summary>
	/// <returns></returns>
	Vector3 GetForward()
	{
		return m_forward;
	}

	/// <summary>
	/// �ړ����x��ݒ�
	/// </summary>
	/// <param name="moveSpeed"></param>
	void SetMoveSpeed(Vector3 moveSpeed)
	{
		m_moveSpeed = moveSpeed;
	}
	/// <summary>
	/// �ړ����x���擾
	/// </summary>
	/// <returns></returns>
	Vector3 GetMoveSpeed()
	{
		return m_moveSpeed;
	}
	
	/// <summary>
	/// �X�s�[�h�p�����[�^�̎擾
	/// </summary>
	/// <returns></returns>
	float GetSpeedParameter()
	{
		return m_status.GetSpeed();
	}

	
	/// <summary>
	/// ���@��������̃p�����[�^�̐ݒ�
	/// </summary>
	/// <param name="startPosition">�J�n���W</param>
	/// <param name="forward">�������i�O����</param>
	/// <param name="chaseCharacter">�ǂ�������L�����N�^�[�B�i���Ȃ��Ȃ炻��ł����j</param>
	void SetShotMagicBallParameters(
		Vector3 startPosition,
		Vector3 forward,
		CharacterBase* chaseCharacter = nullptr
	);


protected:
	/// <summary>
	/// ��{�I�ȃR���|�[�l���g�̐ݒ�
	/// </summary>
	void DefaultSettingComponents();

	/// <summary>
	/// �ǉ��Őݒ肷��R���|�[�l���g
	/// </summary>
	virtual void AddSettingComponents() {}


protected:

	MagicBallStatus m_status;

	DamageProvider* m_damageProvider = nullptr;		//�_���[�W�񋟎҃C���X�^���X

	CharacterBase* m_chaseCharacter = nullptr;		//�ǂ������鎞�̃L�����N�^�[�C���X�^���X
	

	Vector3 m_position = g_vec3Zero;				//���g�̍��W
	Quaternion m_rotation = Quaternion::Identity;	//��]
	Vector3 m_scale = g_vec3One;					//�g�嗦

	Vector3 m_forward = g_vec3Zero;					//�O����
	Vector3 m_moveSpeed = g_vec3Zero;				//�ړ����x

	Vector3 m_startPosition = g_vec3Zero;			//�J�n���W



};

