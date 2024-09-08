#pragma once
#include "IMagicBall.h"

class MagicBallMovement;
//class MagicBallStatus;

/// <summary>
/// BeholderEye�������@��
/// </summary>
class EyeBall : public IMagicBall
{
public:
	EyeBall();
	~EyeBall();

	bool Start() override;

	void Update() override;

	void Render(RenderContext& rc) override;

	void InitModel() override;


	


private:

	/// <summary>
	/// �ǉ��Őݒ肷��R���|�[�l���g
	/// </summary>
	void AddSettingComponents() override;

	/// <summary>
	/// �����蔻�萶��
	/// </summary>
	void CreateCollision();

	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();

	/// <summary>
	/// �������邩�H
	/// </summary>
	/// <returns></returns>
	bool IsDelete();


	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool IsDeleteTime();

private:

	MagicBallMovement* m_magicBallMovement = nullptr;

	CollisionObject* m_collision = nullptr;


	float m_deleteTimer = 0.0f;

};

