#pragma once
#include "IComponent.h"

class IMagicBall;

/// <summary>
/// ���@���ړ��p�R���|�[�l���g
/// </summary>
class MagicBallMovement : public IComponent
{
public:
	MagicBallMovement();
	~MagicBallMovement();

	void Start() override;

	/// <summary>
	/// �X�V�����B�����ōs���Ă����B���������Ȃ�GameObjectManager��
	/// </summary>
	void UpdateComponent() override;



	/// <summary>
	/// ���@���C���X�^���X��ݒ�
	/// </summary>
	/// <param name="magicBall"></param>
	void SetMagicBallInstance(IMagicBall* magicBall)
	{
		m_magicBall = magicBall;
	}

	/// <summary>
	/// ���i�ړ�
	/// </summary>
	void MoveStraight();

	/// <summary>
	/// ������ړ�
	/// </summary>
	void MoveFall();


	/// <summary>
	/// �ǂ�������ړ�
	/// </summary>
	/// <param name="targetPosition">�^�[�Q�b�g�̍��W</param>
	/// <param name="radius">���a�B(�^�[�Q�b�g�ɋ߂����鎞�ɂ��������Ȃ��悤�ɂ���)</param>
	void MoveChase(
		Vector3 targetPosition, float radius,
		float yUp, bool setYUp = false
	);

private:

	IMagicBall* m_magicBall = nullptr;

	Vector3 m_direction = g_vec3Zero;
};

