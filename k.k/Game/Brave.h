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
class WeaponBase;



class Brave : public CharacterBase
{
public:
	~Brave();


	bool Start() override;

	void Update() override;

	void Render(RenderContext& rc) override;

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

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
	/// 現在の武器のアニメーションの最初の番号を取得
	/// </summary>
	/// <returns></returns>
	const int GetCurrentAnimationStartIndexNo()
	{
		return m_braveAnimClip.get()->GetCurrentAnimationStartIndexNo();
	}

	/// <summary>
	/// 
	/// </summary>
	void SetCurrentAnimationStartIndexNoForMainWeaponType();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="moveSpeed"></param>
	void CharaConExecute(Vector3 moveSpeed);

	/// <summary>
	/// アクション中にする
	/// </summary>
	void ActionActive()
	{
		m_isActionFlag = true;
	}
	/// <summary>
	/// アクションを終わる
	/// </summary>
	void ActionDeactive()
	{
		m_isActionFlag = false;
	}
	/// <summary>
	/// アクション中か
	/// </summary>
	/// <returns></returns>
	const bool IsAction() const
	{
		return m_isActionFlag;
	}

	/// <summary>
	/// 装備状態の武器を取得
	/// </summary>
	/// <returns></returns>
	WeaponBase* GetArmedWeapon()
	{
		return m_armedWeapon;
	}

	/// <summary>
	/// プレイヤーコントローラーを取得
	/// </summary>
	/// <returns></returns>
	PlayerController* GetPlayerController()
	{
		return m_playerContoller;
	}
	/// <summary>
	/// プレイヤームーブメントを取得
	/// </summary>
	/// <returns></returns>
	PlayerMovement* GetPlayerMovement()
	{
		return m_playerMovement;
	}



	bool IsButtonAction();

	/// <summary>
	/// 共通のステート処理
	/// </summary>
	void ProcessCommonStateTransition();

	/// <summary>
	/// 通常攻撃処理
	/// </summary>
	void NormalAttackProcess();

	/// <summary>
	/// 攻撃アクションを抜け出す
	/// </summary>
	void ExitAttackAction();

	/// <summary>
	/// ステートを切り替える
	/// </summary>
	/// <param name="nextState">次のステート</param>
	void ChangeBraveState(BraveState::EnBraveState nextState);

private:

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
	/// ボタンアクションの処理
	/// </summary>
	void ButtonAction();

	/// <summary>
	/// 攻撃処理
	/// </summary>
	void AttackAction();

	/// <summary>
	/// 回避、防御処理
	/// </summary>
	void DefensiveAction();

	/// <summary>
	/// 武器切り替え
	/// </summary>
	void ChangeWeaponAction();

	/// <summary>
	/// 硬直時間か
	/// </summary>
	/// <returns>硬直中ならtrue、硬直中でないならfalse</returns>
	bool IsStarkTime();



private:

	Status_Player m_status;					//プレイヤーのステータス

	std::unique_ptr<LoadBraveAnimationClips> m_braveAnimClip = nullptr;		//アニメーションクリップロード

	PlayerMovement* m_playerMovement = nullptr;			//移動コンポーネントインスタンス

	PlayerController* m_playerContoller = nullptr;		//プレイヤーコントローラーインスタンス

	std::unique_ptr<BraveStateContext> m_braveStateCotext = nullptr;	//ステートコンテキスト

	WeaponBase* m_armedWeapon = nullptr;		//装備している武器

	


	bool m_isActionFlag = false;		//アクションt中かのフラグ
	

	float m_starkTimer = 0.0f;			//硬直タイマー

};

