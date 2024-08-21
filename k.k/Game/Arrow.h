#pragma once

#include "ArrowStatus.h"

class Brave;
class ArrowStatus;

/// <summary>
/// ����F�{�E���g���A���[�N���X
/// </summary>
class Arrow:public IGameObject
{
public:

	Arrow();
	~Arrow();

	/// <summary>
	/// �U���p�^�[���X�e�[�g
	/// </summary>
	enum EnShotPatternState
	{
		enNormalShot,		//�ʏ�U��
		enSkillShot,		//�X�L���U��
		enNone,
	};
	/// <summary>
	/// ����̃X�e�[�g
	/// </summary>
	enum EnWeaponState
	{
		enStowed,		//���[���
		enArmed			//�������
	};


	bool Start() override;

	void Update() override;

	void Render(RenderContext& rc) override;

	/// <summary>
	/// �V���b�g�p�^�[����ݒ�
	/// </summary>
	/// <param name="shotPattern"></param>
	void SetShotPatternState(EnShotPatternState shotPattern)
	{
		m_enShotPatternState = shotPattern;
	}

	/// <summary>
	/// ����̃X�e�[�g��ݒ�
	/// </summary>
	/// <param name="setWeaponState"></param>
	void SetWeaponState(EnWeaponState setWeaponState)
	{
		m_enWeaponState = setWeaponState;
	}

	/// <summary>
	/// �����Ƃ��̃p�����[�^�̐ݒ�
	/// </summary>
	/// <param name="shotpatternState">�ʏ�U�����X�L���U����</param>
	/// <param name="forward">�����L�����N�^�[�̑O����</param>
	void SetShotArrowParameters(
		EnShotPatternState shotpatternState,
		Vector3 forward);

	/// <summary>
	/// ���[��Ԃɂ���
	/// </summary>
	void ChangeStowed();
	/// <summary>
	/// ������Ԃɂ���
	/// </summary>
	void ChangeArmed();

	/// <summary>
	/// ���[��Ԃ̈ړ�����
	/// </summary>
	void MoveStowed();

	/// <summary>
	/// �������̈ړ�����
	/// </summary>
	void MoveArmed();

	


private:
	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �V���b�g�p�^�[���ɂ��X�e�[�g�̊Ǘ�
	/// </summary>
	void ManageShotPatternState();

	/// <summary>
	/// �m�[�}���V���b�g�p�^�[���X�e�[�g�̍X�V����
	/// </summary>
	void UpdateNormalShotState();
	/// <summary>
	/// �X�L���V���b�g�p�^�[���X�e�[�g�̍X�V����
	/// </summary>
	void UpdateSkillShotState();
	/// <summary>
	/// �V���b�g�p�^�[���X�e�[�g�Ȃ��̍X�V����
	/// </summary>
	void UpdateNoneState();
	


	/// <summary>
	/// �U�����̖�̃g�����X�t�H�[���̏C��
	/// </summary>
	void FixedAttaackArrowTransform();
	/// <summary>
	/// �����蔻����쐬
	/// </summary>
	/// <param name="shotPatternState">�V���b�g�p�^�[���X�e�[�g</param>
	/// <param name="position">�������W</param>
	/// <param name="rotation">��]</param>
	void CreateCollision(
		EnShotPatternState shotPatternState,
		Vector3 createPosition,
		Quaternion rotation
	);


private:


	Brave* m_brave = nullptr;

	ModelRender m_arrowModelRender;

	CollisionObject* m_arrowCollision = nullptr;	//��̓����蔻��

	Matrix m_arrowMatrix;				//��̃��[���h���W
	Matrix m_arrowCenterMatrix;			//��̒��S�̃��[���h���W

	EnShotPatternState m_enShotPatternState = enNone;		//�V���b�g�p�^�[���X�e�[�g

	EnWeaponState m_enWeaponState = enStowed;

	Vector3 m_forward = g_vec3Zero;		//�O����

	Vector3 m_stowedPosition = { 0.0f,100.0f,0.0f };

	Vector3 m_moveAttackArrowPosition = g_vec3Zero;


	std::map<EnShotPatternState, ArrowStatus> m_statusMap;


	//�������������Ƃ��̃{�[��ID
	int m_armedArrowBoonId = -1;

	int m_arrowCentorBoonId = -1;		//��̐^�񒆂̃{�[��ID
	int m_arrowTipBoonId = -1;			//��̐�[�̃{�[��ID


	float m_deleteTimer = 0.0f;


};

