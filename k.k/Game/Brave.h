#pragma once
#include "CharacterBase.h"

#include "Status_Player.h"

#include "LoadBraveAnimationClips.h"
#include "BraveStateContext.h"

class Status_Player;
class PlayerMovement;
class PlayerController;
class LoadBraveAnimationClips;
class BraveStateContext;

class Brave : public CharacterBase
{
public:
	~Brave();


	bool Start() override;

	void Update() override;

	void Render(RenderContext& rc) override;

	/// <summary>
	/// 被ダメージ処理
	/// </summary>
	virtual void Damage() override;

	/// <summary>
	/// 死亡時処理
	/// </summary>
	virtual void Dead() override;

	/// <summary>
	/// 回転するための方向ベクトルを設定
	/// </summary>
	/// <param name="rotateDirection"></param>
	void SetRotateDirection(Vector3 rotateDirection)
	{
		m_rotateDirection = rotateDirection;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const int GetCurrentAnimationStartIndexNo()
	{
		return m_braveAnimClip.get()->GetCurrentAnimationStartIndexNo();
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

	std::unique_ptr<LoadBraveAnimationClips> m_braveAnimClip = nullptr;

	

	PlayerMovement* m_playerMovement = nullptr;

	PlayerController* m_playerContoller = nullptr;

	std::unique_ptr<BraveStateContext> m_braveStateCotext = nullptr;


	

};

