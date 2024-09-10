#pragma once


/// <summary>
/// ゲームUIのベースクラス
/// </summary>
class GameUIBase:public IGameObject
{
public:
	virtual ~GameUIBase(){}

	

	virtual void UIUpdate() = 0;

	virtual void Draw(RenderContext& rc) = 0;

	virtual void DeleteSelf() = 0;

protected:


	/// <summary>
	/// ゲージの割合を計算
	/// </summary>
	/// <param name="maxValue">最大値</param>
	/// <param name="currentValue">現在の値</param>
	Vector3 CalcGaugeRatio(float maxValue, float currentValue);



	/// <summary>
	/// タイマーが動いているか？
	/// </summary>
	/// <param name="timeLimit">タイムリミット</param>
	/// <param name="timer">タイマー</param>
	/// <returns>タイマーが加算されているならtrue</returns>
	bool IsTimerRunning(float timeLimit, float& timer);

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
		SpriteRender& spriteRender,
		const char* filePath,
		const float width, const float height,
		Vector2 position,
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
		Vector2 position = g_vec2Zero,
		float scale = 1.0f,
		Vector4 color = g_vec4White,
		const wchar_t* text = L"None",
		bool isShadowParam = true,
		float shadowOffset = 1.8f,
		Vector4 shadowColor = g_vec4Black
	);

protected:




};

