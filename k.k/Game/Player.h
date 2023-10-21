#pragma once
#include "Actor.h"

class Hero;
class Wizard;
class Actor;
class Game;

class Brave;

class Player:public IGameObject
{
public:

	enum EnCharacters {
		enHero,
		enWizard,
		enBrave,
		enCharacter_num
	};

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
	/// 勝敗が決定したか
	/// </summary>
	/// <returns></returns>
	bool IsWinnerDecision();

	bool GameClear();

	bool GameOver();

	bool TimeUp();

	/// <summary>
	/// キャラクター切り替え処理
	/// </summary>
	void Change();

	/// <summary>
	/// キャラクターを切り替える
	/// </summary>
	/// <param name="nextCharacter">次のキャラクター</param>
	void ChangeCharacter(EnCharacters nextCharacter);

	/// <summary>
	/// キャラクターを強制的に切り替える
	/// </summary>
	bool ForcedChange();

	/// <summary>
	/// キャラ切り替えのクールタイムの計算
	/// </summary>
	/// <returns>trueで切り替え可能</returns>
	bool ChangeCharacterTime();

	/// <summary>
	/// 全滅したか
	/// </summary>
	bool IsAnnihilation();

	/// <summary>
	/// 座標の計算
	/// </summary>
	void CalcPosition(Vector3 moveSpeed, float deltaTime);

	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

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
	/// サブのアクターのステータスを返す
	/// </summary>
	/// <returns></returns>
	Status GetSubActorStatus()
	{
		return m_subActor->GetStatus();
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
	/// 現在のアクティブなキャラクターを返す
	/// </summary>
	/// <returns></returns>
	const EnCharacters& GetActiveCharacter() const
	{
		return m_enActiveCharacter;
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
	/// サブのキャラの死んだかのフラグ取得
	/// </summary>
	/// <returns></returns>
	const bool GetSubActorDieFlag() const
	{
		return m_subActor->GetDieFlag();
	}
		
	/// <summary>
	/// 現在のキャラクターの攻撃力を返す
	/// </summary>
	/// <returns></returns>
	int GetAtk();

	/// <summary>
	/// 現在のキャラクターのスキルの攻撃力を返す
	/// </summary>
	/// <returns></returns>
	int GetSkillAtk();
	
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
	/// キャラ切り替えフラグの設定
	/// </summary>
	/// <param name="flag"></param>
	void SetChangCharacterFlag(bool flag)
	{
		m_ChangCharacterFlag = flag;
	}
	/// <summary>
	/// キャラ切り替えフラグの取得
	/// </summary>
	/// <returns></returns>
	const bool& GetChangCharacterFlag() const
	{
		return m_ChangCharacterFlag;
	}
	/// <summary>
	/// キャラ切り替え後の計算中のタイマーの値を返す
	/// </summary>
	/// <returns></returns>
	const float& GetChangeCharacterTimer() const
	{
		return m_ChangeCharacterTimer;
	}

	/// <summary>
	/// GameUI用のキャラ切り替えフラグの設定
	/// </summary>
	/// <param name="flag"></param>
	void SetChangCharacterFlagForGameUI(bool flag)
	{
		m_ChangCharacterForGameUIFlag = flag;
	}
	/// <summary>
	/// GameUI用のキャラ切り替えフラグの取得
	/// </summary>
	/// <returns></returns>
	const bool& GetChangCharacterFlagForGameUI() const
	{
		return m_ChangCharacterForGameUIFlag;
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

private:
	Game*						m_game = nullptr;
	Hero*						m_hero = nullptr;
	Wizard*						m_wizard = nullptr;
	Brave*						m_brave = nullptr;

	FontRender					m_mpFont;
	FontRender					m_hpFont;

	//RigidBody					rigitBody;

	Actor*						actor[enCharacter_num];							//キャラクターの数分のアクター
	Actor*						m_nowActor = nullptr;							//現在のアクター
	Actor*						m_subActor = nullptr;							//サブのアクター


	EnCharacters				m_enActiveCharacter= enHero;

	Vector3						m_position = Vector3::Zero;						//現在のキャラクターの座標

	Vector3						m_moveSpeed = Vector3::Zero;

	const float					m_dieToChangeTime = 1.0f;

	float						m_dieToChangeTimer = 0.0f;

	bool						m_informGameFlag = false;

	bool						m_ChangCharacterFlag = false;					//キャラ切り替えフラグ
	bool						m_ChangCharacterForGameUIFlag = false;			//キャラ切り替えフラグ。gameUI用

	const float					m_ChangeCharacterTime = 3.0f;
	float						m_ChangeCharacterTimer = 3.0f;					//キャラ切り替えのクールタイム
	

	bool						m_gameEndFlag = false;							//勝ち負け関わらずバトルが終わったときにtrueにする
	bool						m_VictoryAnimFlag = false;						//勝利アニメーション再生フラグ
	bool						m_idleAnimFlag = false;							//アイドルアニメーション再生フラグ

	bool						m_dontActionFlag = true;						//行動しないフラグ。trueで行動しない

};

