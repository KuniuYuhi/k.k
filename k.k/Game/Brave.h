#pragma once
#include "CharacterBase.h"

#include "Status_Player.h"

#include "LoadBraveAnimationClips.h"
#include "BraveStateContext.h"

#include "KnockBackInfo.h"

class Status_Player;
class PlayerMovement;
class PlayerController;
class DamageProvider;
class LoadBraveAnimationClips;
class BraveStateContext;
class WeaponBase;

using namespace DamageInformaiton;
using namespace KnockBackInfo;

class Brave : public CharacterBase
{
public:
	~Brave();


	bool Start() override;

	void Update() override;

	void Render(RenderContext& rc) override;

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	/// <summary>
	/// 現在の攻撃力を取得
	/// </summary>
	/// <returns></returns>
	int GetCurrentPower() override;


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
	/// 無敵有効化
	/// </summary>
	void EnableInvincible()
	{
		m_isInvincible = true;
	}
	/// <summary>
	/// 無敵無効化
	/// </summary>
	void DisableInvincible()
	{
		m_isInvincible = false;
	}
	/// <summary>
	/// 無敵か？
	/// </summary>
	/// <returns></returns>
	const bool IsInvincible() const
	{
		return m_isInvincible;
	}

	/// <summary>
	/// スキルメインに進むかのフラグ
	/// </summary>
	/// <param name="flag"></param>
	void SetProceedSkillMainFlag(bool flag)
	{
		m_isProceedSkillMain = flag;
	}
	/// <summary>
	/// スキルメインに進むのか？
	/// </summary>
	/// <returns></returns>
	bool IsProceedSkillMain()
	{
		return m_isProceedSkillMain;
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
	/// <summary>
	/// ダメージプロバイダーを取得
	/// </summary>
	/// <returns></returns>
	DamageProvider* GetDamageProvider()
	{
		return m_damageProvider;
	}

	/// <summary>
	/// ステータスの取得
	/// </summary>
	/// <returns></returns>
	Status_Player& GetStatus()
	{
		return m_status;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="position"></param>
	void SetDamageProviderPosition(Vector3 position)
	{
		m_damageProviderPosition = position;
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


	void EntryHitActionProcess();

	void UpdateHitActionProcess();

	void ExitHitActionProcess();
	
	/// <summary>
	/// ノックバックの情報をダメージ情報を使って設定
	/// </summary>
	/// <param name="damageInfo"></param>
	void SettingKnockBackInfoForDamageInfo(DamageInfo damageInfo);

private:

	/// <summary>
	/// 基本的なコンポーネントをセッティング
	/// </summary>
	void SettingDefaultComponent() override;


	/// <summary>
	/// ダメージを受けた時の処理
	/// </summary>
	void ProcessHit(DamageInfo damageInfo);

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

	/// <summary>
	/// スタミナの自動回復
	/// </summary>
	void AutoRecoveryStamina();

	/// <summary>
	/// コリジョンのチェック
	/// </summary>
	void CheckSelfCollision();

	/// <summary>
	/// ノックバックする前の準備
	/// </summary>
	void SettingKnockBackProcess();

	void KnockBackMove(int listNum);

	void KnockBackGravityFall();

private:

	Status_Player m_status;					//プレイヤーのステータス

	std::unique_ptr<LoadBraveAnimationClips> m_braveAnimClip = nullptr;		//アニメーションクリップロード

	PlayerMovement* m_playerMovement = nullptr;			//移動コンポーネントインスタンス

	DamageProvider* m_damageProvider = nullptr;

	PlayerController* m_playerContoller = nullptr;		//プレイヤーコントローラーインスタンス

	std::unique_ptr<BraveStateContext> m_braveStateCotext = nullptr;	//ステートコンテキスト

	WeaponBase* m_armedWeapon = nullptr;		//装備している武器

	std::vector<CurvePoint> m_curvePointList;			//ノックバックカーブポイントリスト

	Vector2 m_knockBackSpeed = g_vec2Zero;				//ノックバックのスピード

	Vector3 m_damageProviderPosition = g_vec3Zero;		//ダメージ提供者（攻撃者）の座標

	Vector3 m_saveForward = g_vec3Zero;					//保存用前方向

	EnKnockBackPattern m_hitKnockBackPattern;			//ノックバックのパターン
	float m_knockBackTimeScale = 0.0f;					//ノックバックの進行速度
	
	bool m_isProceedSkillMain = false;					//スキルメインステートに進むのか

	bool m_isActionFlag = false;		//アクションt中かのフラグ
	

	bool m_isInvincible = false;		//無敵か？

	float m_starkTimer = 0.0f;			//硬直タイマー

	float count = 0.0f;

	int oldAttackId = -1;

};

