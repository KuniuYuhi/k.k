#pragma once
#include "GameUIBase.h"


/// <summary>
/// 時間などのゲーム中の状態を表示するクラス
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
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// スプライトを初期化
	/// </summary>
	void InitSpriteRenders();

	/// <summary>
	/// フォントを初期化
	/// </summary>
	void InitFontRenders();

	/// <summary>
	/// ゲーム中のタイマーの更新
	/// </summary>
	void UpdateGameTimer();



private:

	SpriteRender m_spriteList[enSprite_Num];


	FontRender m_fontList[enFontName_Num];


};

