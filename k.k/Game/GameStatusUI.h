#pragma once
#include "GameUIBase.h"


/// <summary>
/// ���ԂȂǂ̃Q�[�����̏�Ԃ�\������N���X
/// </summary>
class GameStatusUI : public GameUIBase
{
private:

	enum EnSpriteName
	{
		enSpriteName_TimerFlame,
		
		enSprite_Num
	};


	enum EnFontName
	{
		enFontName_timer,

		enFontName_Num
	};


public:

	GameStatusUI() {}
	~GameStatusUI() {}

	bool Start() override;

	void UIUpdate() override;

	void Draw(RenderContext& rc) override;

	void DeleteSelf() override;

private:
	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �X�v���C�g��������
	/// </summary>
	void InitSpriteRenders();

	/// <summary>
	/// �t�H���g��������
	/// </summary>
	void InitFontRenders();

	/// <summary>
	/// �Q�[�����̃^�C�}�[�̍X�V
	/// </summary>
	void UpdateGameTimer();



private:

	SpriteRender m_spriteList[enSprite_Num];


	FontRender m_fontList[enFontName_Num];


};

