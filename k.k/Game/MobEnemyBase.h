#pragma once
#include "EnemyBase.h"
#include "MobEnemyCommonStatus.h"

#include "KnockBackInfo.h"

class MobEnemyMovement;
class DamageProvider;

class Brave;
class MobEnemyCommonStatus;

using namespace DamageInformaiton;
using namespace KnockBackInfo;


/// <summary>
/// ���u�G�l�~�[�p�̃x�[�X�N���X
/// ���u�G�l�~�[�ŋ��ʂ��鏈������������
/// </summary>
class MobEnemyBase : public EnemyBase
{
public:

	/// <summary>
	/// ���g����^�[�Q�b�g�Ɍ������������擾
	/// </summary>
	float GetDistanceToTargetPositionValue(Vector3 target);

	/// <summary>
	/// �ҋ@���邩�̃t���O��ݒ�
	/// </summary>
	/// <param name="flag"></param>
	void SetWaitingFlag(bool flag)
	{
		m_isWaitingFlag = flag;
	}

	/// <summary>
	/// �O������폜
	/// </summary>
	virtual void DieFlomOutside(bool isPlayEffect = false) {}

	/// <summary>
	/// �������̏���
	/// </summary>
	virtual void WinProcess() {}

	/// <summary>
	/// �������~�߂�v��������Ă��邩�H
	/// </summary>
	/// <returns></returns>
	virtual bool IsStopRequested();

protected:

	/// <summary>
	/// ���g���I�u�W�F�N�g�v�[���ɖ߂�
	/// </summary>
	virtual void ReleaseThis() = 0;

	/// <summary>
	/// ��{�I�ȃR���|�[�l���g���Z�b�e�B���O
	/// </summary>
	virtual void SettingDefaultComponent() override;

	/// <summary>
	/// ����ɒǉ�����R���|�[�l���g���Z�b�e�B���O
	/// </summary>
	virtual void AddMoreComponent() {}

	/// <summary>
	/// �R���W�������쐬����
	/// </summary>
	virtual void CreateCollisionObject(){}

	/// <summary>
	/// �_���[�W���󂯂����̏���
	/// </summary>
	virtual void ProcessHit(DamageInfo damageInfo){}

	

	/// <summary>
	/// �_���[�W�ɂ���Ď��S
	/// </summary>
	virtual void DieFromDamage();

	

	/// <summary>
	/// ���g�̓����蔻��m�F
	/// </summary>
	void CheckSelfCollision();

	/// <summary>
	/// �^�[�Q�b�g�̂ق��Ɍ���
	/// </summary>
	void TurnToTarget();

	/// <summary>
	/// �ǂ�������ړ�����
	/// </summary>
	/// <param name="targetPosition">�^�[�Q�b�g�̍��W</param>
	void ChaseMovement(Vector3 targetPosition);

	/// <summary>
	/// ��]����
	/// </summary>
	void Rotation();

	/// <summary>
	/// �m�b�N�o�b�N����O�̏���
	/// </summary>
	void SettingKnockBackProcess();

	void KnockBackMove(int listNum);

	void KnockBackGravityFall();

	/// <summary>
	/// �U���\���H
	/// </summary>
	/// <returns>�\�Ȃ�true</returns>
	bool IsAttackable();



	/// <summary>
	/// �_���[�W�t�H���g�𐶐�
	/// </summary>
	/// <param name="hitDamage">�q�b�g�_���[�W</param>
	//void CreateDamageFont(int hitDamage);



protected:

	MobEnemyCommonStatus m_commonStatus;

	Vector3 m_deactivatePosition = { 0.0f, 100.0f, 0.0f };

	//�ړ��R���|�[�l���g
	MobEnemyMovement* m_movement = nullptr;
	//
	DamageProvider* m_damageProvider = nullptr;

	bool m_isWaitingFlag = false;		//�v���C���[�ɋ߂Â��߂��������ő҂��̃t���O

	Brave* m_player = nullptr;			//�v���C���[�̃C���X�^���X

	std::vector<CurvePoint> m_curvePointList;			//�m�b�N�o�b�N�J�[�u�|�C���g���X�g

	Vector2 m_knockBackSpeed = g_vec2Zero;

	EnKnockBackPattern m_hitKnockBackPattern;
	float m_knockBackTimeScale = 0.0f;

	int oldAttackId = -1;				//�O�t���[���̍U��ID

	float m_starkTimer = 0.0f;			//�d���^�C�}�[//�S���g�����

	float m_attackIntarvalTimer = 0.0f;			//�U���C���^�[�o���^�C�}�[



};

