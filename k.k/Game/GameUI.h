#pragma once
#include "MyWeapon.h"

class Game;

class PlayerGameUI;


class GameUI:public IGameObject
{
public:
	GameUI();
	~GameUI();


	bool Start();
	void Update();
	void Render(RenderContext& rc);

	
private:
	
	/// <summary>
	/// スプライトレンダーの初期化。
	/// </summary>
	/// <param name="spriterender">初期化したいスプライトレンダー</param>
	/// <param name="filePath">画像のファイルパス</param>
	/// <param name="width">画像の幅</param>
	/// <param name="height">画像の高さ</param>
	/// <param name="position">座標</param>
	/// <param name="scale">サイズ</param>
	/// <param name="rotation">回転</param>
	void InitSpriteRender(
		SpriteRender& spriterender,
		const char* filePath,
		const float width, const float height,
		Vector3 position,
		Vector3 scale = g_vec3One,
		Quaternion rotation = g_quatIdentity
	);
	
	/// <summary>
	/// フォントレンダーの初期化。
	/// </summary>
	/// <param name="fontRender">初期化したいフォントレンダー</param>
	/// <param name="position">座標</param>
	/// <param name="color">カラー</param>
	/// <param name="scale">サイズ</param>
	/// <param name="isShadowParam">影(輪郭)をつけるか</param>
	/// <param name="shadowOffset">オフセット量</param>
	/// <param name="shadowColor">影のカラー</param>
	void InitFontRender(
		FontRender& fontRender,
		Vector2 position,
		float scale = 1.0f,
		Vector4 color = g_vec4White,
		bool isShadowParam = true,
		float shadowOffset = 1.8f,
		Vector4 shadowColor = g_vec4Black
	);
	
private:


	PlayerGameUI* m_playerGameUI = nullptr;







};

