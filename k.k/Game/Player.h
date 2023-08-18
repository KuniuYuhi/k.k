#pragma once
#include "Actor.h"

class Hero;
class Wizard;
class Actor;
class Game;

class Player:public IGameObject
{
public:

	enum EnCharacters {
		enHero,
		enWizard,
		enCharacter_num
	};

	Player();
	~Player();

	bool Start();
	void Update();

	void Render(RenderContext& rc);

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
	/// 全滅したか
	/// </summary>
	bool IsAnnihilation();

	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	const Vector3& GetMoveSpeed() const
	{
		return m_moveSpeed;
	}

	CharacterController& GetCharacterController()
	{
		return m_charaCon;
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

private:
	Game* m_game = nullptr;
	Hero* m_hero = nullptr;
	Wizard* m_wizard = nullptr;

	FontRender m_mpFont;
	FontRender m_hpFont;

	//キャラコンこれ使う
	CharacterController m_charaCon;

	RigidBody rigitBody;

	Actor* actor[enCharacter_num];	//キャラクターの数分のアクター
	Actor* m_nowActor = nullptr;	// 現在のアクター
	Actor* m_subActor = nullptr;    // サブのアクター


	EnCharacters m_enActiveCharacter= enHero;

	Vector3 m_position = Vector3::Zero;		//現在のキャラクターの座標

	Vector3 m_moveSpeed = Vector3::Zero;

	const float m_dieToChangeTime = 1.0f;

	float m_dieToChangeTimer = 0.0f;

	bool m_informGameFlag = false;



	bool m_gameEndFlag = false;		//勝ち負け関わらずバトルが終わったときにtrueにする

};

