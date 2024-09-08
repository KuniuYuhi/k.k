#pragma once
#include "IComponent.h"
#include "CommonEnemyStatus.h"

class CharacterBase;
class CommonEnemyStatus;

/// <summary>
/// ���u�G�l�~�[�̈ړ��p�R���|�[�l���g
/// </summary>
class MobEnemyMovement : public IComponent
{
public:
	MobEnemyMovement();
	~MobEnemyMovement();

	void Start() override;

	void UpdateComponent() override;

	
	/// <summary>
	/// �L�����N�^�[��ǂ�����������̌v�Z
	/// </summary>
	/// <param name="status">�X�e�[�^�X</param>
	/// <param name="chaseCharacter">�ǂ�������L�����N�^�[�̍��W</param>
	/// <param name="moveSpeed">�ړ����x</param>
	/// <returns></returns>
	Vector3 CalcChaseCharacterVerocity(
		CommonEnemyStatus status,
		Vector3 targetPositon,
		Vector3 m_currentPosition,
		Vector3 moveSpeed
	);

private:

};

