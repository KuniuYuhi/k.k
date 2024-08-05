#pragma once
#include "EnemyBase.h"

class MobEnemyMovement;

class Brave;


/// <summary>
/// ���u�G�l�~�[�p�̃x�[�X�N���X
/// ���u�G�l�~�[�ŋ��ʂ��鏈������������
/// </summary>
class MobEnemyBase : public EnemyBase
{
public:

	//virtual ~MobEnemyBase() = default;


	
	/// <summary>
	/// ��_���[�W����
	/// </summary>
	virtual void Damage() override {}

	/// <summary>
	/// ���S������
	/// </summary>
	virtual void Dead() override {}


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
	/// �ǂ�������ړ�����
	/// </summary>
	/// <param name="targetPosition">�^�[�Q�b�g�̍��W</param>
	void ChaseMovement(Vector3 targetPosition);

	void Rotation();



protected:

	Vector3 m_deactivatePosition = { 0.0f, 100.0f, 0.0f };

	//�ړ��R���|�[�l���g
	MobEnemyMovement* m_movement = nullptr;
	//




	Brave* m_player = nullptr;			//�v���C���[�̃C���X�^���X

};

