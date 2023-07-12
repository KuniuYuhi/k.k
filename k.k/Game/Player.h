#pragma once

class Hero;
class Wizard;
class Actor;

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
	/// キャラクターを切り替える
	/// </summary>
	/// <param name="nextCharacter">次のキャラクター</param>
	void ChangeCharacter(EnCharacters nextCharacter);

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetPosition() const
	{
		return m_position;
	}

	Vector3 GetMoveSpeed() const
	{
		return m_moveSpeed;
	}

	CharacterController& GetCharacterController()
	{
		return m_charaCon;
	}

private:
	Hero* m_hero = nullptr;
	Wizard* m_wizard = nullptr;

	FontRender m_MpFont;

	//キャラコンこれ使う
	CharacterController m_charaCon;

	Actor* actor[enCharacter_num];
	Actor* m_nowActor = nullptr;	// 現在のアクター
	EnCharacters m_enActiveCharacter= enHero;

	Vector3 m_position = Vector3::Zero;		//現在のキャラクターの座標

	Vector3 m_moveSpeed = Vector3::Zero;
};

