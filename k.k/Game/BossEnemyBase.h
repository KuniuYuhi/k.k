#pragma once
#include "EnemyBase.h"
#include "KnockBackInfo.h"


class DamageProvider;
class BossEnemyMovement;
class Brave;

using namespace DamageInformaiton;
using namespace KnockBackInfo;

/// <summary>
/// �{�X�G�l�~�[�x�[�X�N���X
/// </summary>
class BossEnemyBase : public EnemyBase
{
public:

	void SetArrivedApproachDistance(bool flag)
	{
		m_isArrivedApproachDistance = flag;
	}

	bool IsArrivedApproachDistance()
	{
		return m_isArrivedApproachDistance;
	}

	/// <summary>
	/// ���G�ɂ���
	/// </summary>
	void InvicibleActive()
	{
		m_isInvincible = true;
	}
	/// <summary>
	/// ���G������
	/// </summary>
	void InvicibleDeactive()
	{
		m_isInvincible = false;
	}
	/// <summary>
	/// ���G���H
	/// </summary>
	/// <returns></returns>
	bool IsInvincible()
	{
		return m_isInvincible;
	}



	/// <summary>
	/// �_���[�W����ݒ肷��
	/// </summary>
	/// <param name="damageId"></param>
	/// <param name="attackPower"></param>
	/// <param name="knockBackTimeScale"></param>
	/// <param name="knockBackPattern"></param>
	/// <param name="weaponAttribute"></param>
	void SetDamageInfo(
		int damageId,int attackPower,float knockBackTimeScale,
		EnKnockBackPattern knockBackPattern,
		EnWeaponAttribute weaponAttribute = enWeaponAttribute_None
	);



protected:

	/// <summary>
	/// ���g���I�u�W�F�N�g�v�[���ɖ߂�
	/// </summary>
	virtual void ReleaseThis() = 0;

	/// <summary>
	/// �����蔻��̏��������Ȃ�����
	/// </summary>
	/// <returns></returns>
	virtual bool IgnoreCollision() = 0;

	/// <summary>
	/// ��{�I�ȃR���|�[�l���g���Z�b�e�B���O
	/// </summary>
	virtual void SettingDefaultComponent() override;

	/// <summary>
	/// ����ɒǉ�����R���|�[�l���g���Z�b�e�B���O
	/// </summary>
	virtual void AddMoreComponent() {}

	/// <summary>
	/// �_���[�W���󂯂����̏���
	/// </summary>
	virtual void ProcessHit(DamageInfo damageInfo) {}

	/// <summary>
	/// ���g�̓����蔻��m�F
	/// </summary>
	void CheckSelfCollision();


	/// <summary>
	/// �ǂ�������ړ�����
	/// </summary>
	/// <param name="targetPosition">�^�[�Q�b�g�̍��W</param>
	void ChaseMovement(Vector3 targetPosition);


	/// <summary>
	/// �^�[�Q�b�g�̂ق��Ɍ���
	/// </summary>
	void TurnToTarget();
	
	/// <summary>
	/// �U�����̒ǂ�������ړ�
	/// </summary>
	/// <param name="targetPosition">�^�[�Q�b�g�̍��W</param>
	/// <param name="speed">����</param>
	void AttackChaseMovement(Vector3 targetPosition,float speed);


protected:


	DamageProvider* m_damageProvider = nullptr;
	BossEnemyMovement* m_movement = nullptr;

	Brave* m_player = nullptr;			//�v���C���[�̃C���X�^���X

	std::vector<CurvePoint> m_curvePointList;			//�m�b�N�o�b�N�J�[�u�|�C���g���X�g

	Vector2 m_knockBackSpeed = g_vec2Zero;

	EnKnockBackPattern m_hitKnockBackPattern;
	float m_knockBackTimeScale = 0.0f;

	bool m_isArrivedApproachDistance = false;		//�ڋ߂ł��鋗���ɓ��B������


	int oldAttackId = -1;				//�O�t���[���̍U��ID


	bool m_isStopMove = false;			//�ړ�����̂��~�߂�

	bool m_isInvincible = false;		//���G���H

	bool m_isCreateCollision = false;	//�����蔻��𐶐����邩�H




};

