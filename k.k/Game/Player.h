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


	//一つにしたい
	/// <summary>
	/// ヒーローから魔法使いに切り替える
	/// </summary>
	void ChangeFromHeroToWizard();

	/// <summary>
	/// 魔法使いからヒーローに切り替える
	/// </summary>
	void ChangeFromWizardToHero();

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

private:
	Hero* m_hero = nullptr;
	Wizard* m_wizard = nullptr;

	//キャラコンこれ使う
	CharacterController m_charaCon;

	Actor* actor[enCharacter_num];

	EnCharacters m_enActiveCharacter= enHero;

	Vector3 m_position = Vector3::Zero;		//現在のキャラクターの座標

	Vector3 m_moveSpeed = Vector3::Zero;
};

