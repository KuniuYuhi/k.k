#pragma once
#include "IMagicBall.h"

class UseEffect;
class MagicBallMovement;

/// <summary>
/// �T���i�[�������@���F�_�[�N���e�I
/// </summary>
class DarkMeteorite : public IMagicBall
{
public:
	DarkMeteorite();
	~DarkMeteorite();

	bool Start() override;

	void Update() override;

	void Render(RenderContext& rc) override;

	void InitModel() override;

	/// <summary>
	/// ���e�I�������n�߂�
	/// </summary>
	void ShotStartDarkMeteorite();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="position"></param>
	void PlayRangeEffect(Vector3 position);

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
	bool IsGroundHit();

	/// <summary>
	/// ��������
	/// </summary>
	void Explosion();


private:

	MagicBallMovement* m_magicBallMovement = nullptr;

	CollisionObject* m_collision = nullptr;

	UseEffect* m_mainEffect = nullptr;
	
	UseEffect* m_areaEffect = nullptr;

	bool m_isMove = false;
};

