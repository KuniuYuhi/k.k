#pragma once
#include "IComponent.h"
#include "CommonEnemyStatus.h"

class CharacterBase;
class CommonEnemyStatus;

class Summoner;

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
		Vector3 moveSpeed,
		bool isBossPosCheck = true
	);

private:

	/// <summary>
	/// �l��͈͓��Ɏ��߂�
	/// </summary>
	/// <param name="value"></param>
	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <returns></returns>
	float Clamp(float value, float min, float max);


private:
	Summoner* m_summoner = nullptr;

	bool bossMoveFlag = false;

	float m_timer = 0.0f;

	int m_addDistance = 0;

};

