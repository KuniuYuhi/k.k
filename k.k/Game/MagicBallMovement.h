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
	/// �ǂ�������
	/// </summary>
	/// <param name="targetPosition"></param>
	void MoveChase(Vector3 targetPosition);

private:

	IMagicBall* m_magicBall = nullptr;


};

