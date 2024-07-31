#pragma once
#include "CharacterBase.h"

#include "Status_Player.h"



class Status_Player;
class PlayerMovement;
class PlayerController;


class Brave : public CharacterBase
{
public:
	~Brave();


	bool Start() override;

	void Update() override;

	void Render(RenderContext& rc) override;


	/// <summary>
	/// 更新処理
	/// </summary>
	void UpdateCharacter() override;

	/// <summary>
	/// 回転するための方向ベクトルを設定
	/// </summary>
	/// <param name="rotateDirection"></param>
	void SetRotateDirection(Vector3 rotateDirection)
	{
		m_rotateDirection = rotateDirection;
	}


protected:

	/// <summary>
	/// 基本的なコンポーネントをセッティング
	/// </summary>
	void SettingDefaultComponent() override;


	/// <summary>
	/// 移動処理
	/// </summary>
	void Movement();
	/// <summary>
	/// 回転処理
	/// </summary>
	void Rotation();
	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack();

private:

	Status_Player m_status;

	CharacterController m_charaCon;

	PlayerMovement* m_playerMovement = nullptr;

	PlayerController* m_playerContoller = nullptr;

	Vector3 m_moveSpeed = Vector3::Zero;

	Vector3 m_rotateDirection = Vector3::Zero;
};

