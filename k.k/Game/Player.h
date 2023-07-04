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


	//��ɂ�����
	/// <summary>
	/// �q�[���[���疂�@�g���ɐ؂�ւ���
	/// </summary>
	void ChangeFromHeroToWizard();

	/// <summary>
	/// ���@�g������q�[���[�ɐ؂�ւ���
	/// </summary>
	void ChangeFromWizardToHero();

private:
	Hero* m_hero = nullptr;
	Wizard* m_wizard = nullptr;

	Actor* actor[enCharacter_num];

	EnCharacters m_enCharacters= enHero;
};

