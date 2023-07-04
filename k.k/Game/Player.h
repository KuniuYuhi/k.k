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

private:
	Hero* m_hero = nullptr;
	Wizard* m_wizard = nullptr;

	Actor* actor[enCharacter_num];

	EnCharacters m_enCharacters= enHero;
};

