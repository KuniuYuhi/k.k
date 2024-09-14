#pragma once

#include "ArrowStatus.h"

class Brave;
class ArrowStatus;
class DamageProvider;
class Bow;

class UseEffect;

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
	/// �|�̃C���X�^���X��ݒ�
	/// </summary>
	/// <param name="bow"></param>
	void SetBowInstance(Bow* bow)
	{
		m_bow = bow;
	}

	/// <summary>
	/// �_���[�W�v���o�C�_�[���擾
	/// </summary>
	/// <returns></returns>
	DamageProvider* GetDamageProvider()
	{
		return m_damageProvider;
	}

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
	/// �X�L���U�����̏��̐ݒ�
	/// </summary>
	/// <param name="power">�X�L���U����</param>
	/// <param name="interval">�U�����X�V�C���^�[�o��</param>
	void SetSkillShotInfo(int power, int interval)
	{
		SetDefaultSkillAttackPower(power);
		SetAttackInfoUpdateTimeLimit(interval);
	}
	/// <summary>
	/// ��{�X�L���U���͂�ݒ�
	/// </summary>
	/// <param name="power"></param>
	void SetDefaultSkillAttackPower(int power)
	{
		m_defaultSkillAttackPower = power;
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="timeLimit"></param>
	void SetAttackInfoUpdateTimeLimit(float timeLimit)
	{
		m_attackInfoUpdateTimeLimit = timeLimit;
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
	/// �R���|�[�l���g��ݒ�
	/// </summary>
	void InitComponent();

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
	/// �����ꂽ��̈ړ�
	/// </summary>
	void ShotArrowMove(EnShotPatternState shotPattern);



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


	/// <summary>
	/// �R���W�����ɉ����q�b�g������
	/// </summary>
	/// <returns></returns>
	bool IsHitCollision();


	/// <summary>
	/// ��̃G�t�F�N�g�𐶐�
	/// </summary>
	void PlayArrowEffect(EnShotPatternState shotpatternState);

	/// <summary>
	/// 
	/// </summary>
	void DeleteArrow();


private:

	UseEffect* m_arrowEffect = nullptr;

	Brave* m_brave = nullptr;

	Bow* m_bow = nullptr;

	ModelRender m_arrowModelRender;

	DamageProvider* m_damageProvider = nullptr;

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

	int m_defaultSkillAttackPower = 0;

	float m_attackInfoUpdateTimeLimit = 0.0f;
	float m_attackInfoUpdateTimer = 0.0f;


};

