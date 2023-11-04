#pragma once
#include "Actor.h"

class Actor;
class Game;

class Brave;

class Player:public IGameObject
{
public:

	Player();
	~Player();
	bool Start();
	void Update();

	/// <summary>
	/// 行動不可能かどうかの判定可能
	/// </summary>
	/// <returns>trueで不可能、falseで可能</returns>
	bool IsInaction();

	/// <summary>
	/// プレイヤーがやられたかの判定
	/// </summary>
	bool IsDeadPlayer();

	/// <summary>
	/// 座標の取得。現在のアクターの座標
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_nowActor->GetPosition();
	}

	const Vector3& GetMoveSpeed() const
	{
		return m_moveSpeed;
	}

	/// <summary>
	/// 現在のアクターのステータスを返す
	/// </summary>
	/// <returns></returns>
	Status GetNowActorStatus()
	{
		return m_nowActor->GetStatus();
	}

	/// <summary>
	/// 現在のアクターの前方向の取得。正規化済み
	/// </summary>
	/// <returns></returns>
	const Vector3& GetForward() const
	{
		return m_nowActor->GetForward();
	}

	/// <summary>
	/// 現在のキャラの死んだかのフラグ取得
	/// </summary>
	/// <returns></returns>
	const bool GetNowActorDieFlag() const
	{
		return m_nowActor->GetDieFlag();
	}
		
	/// <summary>
	/// 現在のキャラクターの攻撃力を返す
	/// </summary>
	/// <returns></returns>
	int GetAtk();

	/// <summary>
	/// 現在のコンボステートとダメージを受けた時のコンボステートが違うか
	/// </summary>
	/// <returns>違うならtrue,同じならfalse</returns>
	bool IsComboStateSame();
	
	/// <summary>
	/// ダメージを受けた時のコンボステートを設定
	/// </summary>
	/// <param name="damagedcombostate"></param>
	void SetDamagedComboState(Actor::EnComboState damagedcombostate);
	
	/// <summary>
	/// 現在のキャラクターのコンボステートを取得
	/// </summary>
	/// <returns></returns>
	Actor::EnComboState GetNowComboState() const;

	bool GetGameEndFlag()
	{
		return m_gameEndFlag;
	}

	/// <summary>
	/// 行動しないフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetDontActionFlag(bool flag)
	{
		m_dontActionFlag = flag;
	}
	/// <summary>
	/// 行動しないフラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetmDontActionFlag() const
	{
		return m_dontActionFlag;
	}
	/// <summary>
	///　ノックバック攻撃フラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetKnockBackAttackFlag() const
	{
		return m_nowActor->GetKnockBackAttackFalg();
	}

	/// <summary>
	/// 攻撃の中心座標を設定
	/// </summary>
	/// <param name="attackPosition"></param>
	void SetAttackPosition(Vector3 attackPosition)
	{
		m_attackPosition = attackPosition;
	}
	/// <summary>
	/// 攻撃の中心座標を取得
	/// </summary>
	/// <returns></returns>
	const Vector3& GetAttackPosition() const
	{
		return m_attackPosition;
	}

	/// <summary>
	/// ノックバックパワーを取得
	/// </summary>
	/// <returns></returns>
	const float& GetKnockBackPower() const
	{
		return m_nowActor->GetKnockBackPower();
	}

private:
	Game*						m_game = nullptr;
	Brave*						m_brave = nullptr;

	FontRender					m_mpFont;
	FontRender					m_hpFont;

	Actor*						m_nowActor = nullptr;							//現在のアクター

	Vector3						m_position = g_vec3Zero;						//現在のキャラクターの座標

	Vector3						m_moveSpeed = g_vec3Zero;

	Vector3						m_attackPosition = g_vec3Zero;

	bool						m_informGameFlag = false;

	bool						m_gameEndFlag = false;							//勝ち負け関わらずバトルが終わったときにtrueにする
	bool						m_VictoryAnimFlag = false;						//勝利アニメーション再生フラグ
	bool						m_idleAnimFlag = false;							//アイドルアニメーション再生フラグ

	bool						m_dontActionFlag = true;	//行動しないフラグ。trueで行動しない

	bool						m_decisionOutComeFlag = false;



};

