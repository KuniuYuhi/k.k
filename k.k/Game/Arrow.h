#pragma once

#include "WeaponBase.h"

class Bow;

class Arrow:public WeaponBase
{
public:
	Arrow();
	~Arrow();

	enum EnShotPatternState
	{
		enShotPatternState_Normal,
		enShotPatternState_Skill
	};

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// ����̈ړ�����
	/// </summary>
	void MoveWeapon() override;

	/// <summary>
	/// ����𑕔����Ă��鎞�̈ړ�����
	/// </summary>
	void MoveArmed() override;
	/// <summary>
	/// ��������[���Ă��鎞�̈ړ�����
	/// </summary>
	void MoveStowed() override;

	/// <summary>
	/// ����̓����蔻��
	/// </summary>
	/// <returns></returns>
	bool IsHitCollision() override;

	/// <summary>
	/// �|�̃C���X�^���X��ݒ�
	/// </summary>
	/// <param name="bow"></param>
	void SetBow(Bow* bow)
	{
		m_bow = bow;
	}

	/// <summary>
	/// ��̃��[���h���W��ݒ�
	/// </summary>
	/// <param name="matrix"></param>
	void SetArrowMatrix(const Matrix matrix)
	{
		m_arrowMatrix = matrix;
	}
	/// <summary>
	/// ��̏����X�V
	/// </summary>
	void ArrowUpdate()
	{
		m_modelArrow.Update();
	}

	/// <summary>
	/// �ˌ��t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetShotFlag(bool flag)
	{
		m_shotFlag = flag;
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
	/// �ˌ��J�n���̐ݒ�
	/// </summary>
	/// <param name="startPosition">��𔭎˂�����W</param>
	/// <param name="deg">�p�x</param>
	void SetShotStartPosition(Vector3 startPosition,float angle)
	{
		m_shotStartPosition = startPosition;
		m_angle = angle;
	}
	/// <summary>
	/// ��̍��W���擾
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_arrowPos;
	}
	/// <summary>
	/// ���[���h���W�����[�J�����W�ɓK��
	/// </summary>
	void ApplyMatrixToLocalPosition()
	{
		m_arrowPos = g_vec3Zero;
		m_arrowMatrix.Apply(m_arrowPos);
		m_rotation.SetRotation(m_arrowMatrix);
	}

	/// <summary>
	/// ��������̐ݒ�
	/// </summary>
	/// <param name="shotFlag">�V���b�g�t���O</param>
	/// <param name="forward">������L�����̑O����</param>
	/// <param name="angle">���˂���p�x</param>
	/// <param name="shotStartPosition">���ˊJ�n���W</param>
	void SetShotArrowSetting(
		bool shotFlag,
		Vector3 forward,
		Vector3 shotStartPosition,
		float angle,
		EnShotPatternState shotPatternState
	);
	/// <summary>
	/// �������n�_�̐ݒ�
	/// </summary>
	void SetTargetPosition();

	/// <summary>
	/// �V���b�g�p�^�[���X�e�[�g��ݒ�
	/// </summary>
	/// <param name="shotPatternState"></param>
	void SetShotPatternState(EnShotPatternState shotPatternState)
	{
		m_enShotPatternState = shotPatternState;
	}

private:
	/// <summary>
	/// ���f���̏�����
	/// </summary>
	void InitModel() override;
	/// <summary>
	/// �V���b�g�p�^�[���ɂ���ē����蔻���������
	/// </summary>
	/// <param name="shotPatternState">�����������������蔻��̃X�e�[�g</param>
	void SelectInitCollision(EnShotPatternState shotPatternState);
	
	/// <summary>
	/// �ʏ�U���̓����蔻��̏�����
	/// </summary>
	/// <param name="collisionName">�����蔻��̖��O</param>
	/// <param name="createPos">�������������W</param>
	/// <param name="rotation">��]</param>
	/// <param name="collisionSize">�����蔻��̃T�C�Y(�{�b�N�X)</param>
	void InitCollision(
		const char* collisionName,
		Vector3 createPos,
		Quaternion rotation,
		Vector3 collisionSize
		);

	/// <summary>
	/// �������U������
	/// </summary>
	void ProcessLongRangeAttack();
	/// <summary>
	/// �ʏ�U��
	/// </summary>
	void NormalShot();
	/// <summary>
	/// �X�L���U��
	/// </summary>
	void SkillShot();

	/// <summary>
	/// ���W���s��ɓK��
	/// </summary>
	/// <param name="matrix">�s��</param>
	/// <param name="position">���W</param>
	void ApplyVector3ToMatirx(Matrix& baseMatrix, Vector3 position);

	
	Bow* m_bow = nullptr;

	ModelRender m_modelArrow;		//��f��
	CollisionObject* m_arrowCollision = nullptr;

	Vector3 m_arrowPos = g_vec3Zero;
	Vector3 m_oldArrowPos = g_vec3Zero;
	Vector3 m_forward = g_vec3Zero;
	Vector3 m_shotStartPosition = g_vec3Zero;
	Vector3 m_targetPosition = g_vec3Zero;			//�������n�_
	Quaternion m_rotation = g_quatIdentity;

	Matrix m_arrowMatrix = g_matIdentity;

	EnShotPatternState m_enShotPatternState = enShotPatternState_Normal;
	

	bool m_shotFlag = false;			//��𔭎˂��邩�̃t���O

	float m_angle = 0.0f;
	float m_flightDuration = 0.0f;		//�������܂ł̎���
	float m_deleteTimer = 0.0f;

	struct ShotArrowVerocity
	{
		float Vx = 0.0f;
		float Vy = 0.0f;
	};
	ShotArrowVerocity m_shotArrowVerocity;

};

