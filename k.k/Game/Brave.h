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


	bool IsButtonAction();

	/// <summary>
	/// 
	/// </summary>
	void ProcessCommonStateTransition();

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
	/// 武器切り替え
	/// </summary>
	void ChangeWeaponAction();


	//void 



private:

	Status_Player m_status;					//プレイヤーのステータス

	std::unique_ptr<LoadBraveAnimationClips> m_braveAnimClip = nullptr;		//アニメーションクリップロード

	PlayerMovement* m_playerMovement = nullptr;			//移動コンポーネントインスタンス

	PlayerController* m_playerContoller = nullptr;		//プレイヤーコントローラーインスタンス

	std::unique_ptr<BraveStateContext> m_braveStateCotext = nullptr;	//ステートコンテキスト



	WeaponType::EnWeaponType m_mainWeaponType;		//メイン武器タイプ
	WeaponType::EnWeaponType m_subWeaponType;		//サブ武器1
	WeaponType::EnWeaponType m_subWeaponType2;		//サブ武器2
	WeaponType::EnWeaponType m_changeTargetWeaponType;	//切り替え対象の武器タイプ



	bool m_isActionFlag = false;		//アクションt中かのフラグ
	

};

