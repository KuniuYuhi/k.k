#pragma once
class Loading:public IGameObject
{
public:
	Loading();
	~Loading();

	/// <summary>
	/// ローディングの方法
	/// </summary>
	enum EnMethodLoading
	{
		enMethodLoading_None,
		enMethodLoading_Fade,
		enMethodLoading_DirectionWipe,
		enMethodLoading_RoundWipe
	};

	/// <summary>
	/// ローディングステート
	/// </summary>
	enum EnLoadingState
	{
		enLoadingState_None,
		enLoadingState_In,
		enLoadingState_Out
	};

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// ローディングを始める
	/// </summary>
	/// <param name="methodLoading"></param>
	void StartLoading(EnMethodLoading methodLoading);

	/// <summary>
	/// ローディングを終わる
	/// </summary>
	/// <param name="methodLoading"></param>
	void EndLoading(EnMethodLoading methodLoading);

	/// <summary>
	/// 現在のローディングの方法を取得
	/// </summary>
	/// <returns></returns>
	EnMethodLoading& GetCurrentMethodLoading()
	{
		return m_enMethodLoading;
	}

	/// <summary>
	/// 状態管理
	/// </summary>
	void ManageState();
	/// <summary>
	/// 
	/// </summary>
	void ProcessInLoadingStateTransition();
	/// <summary>
	/// 
	/// </summary>
	void ProcessOutLoadingStateTransition();

	/// <summary>
	/// フェード処理
	/// </summary>
	/// <param name="loadingState">ステート</param>
	void ProcessFade(EnLoadingState loadingState);

	/// <summary>
	/// 方向ワイプ処理
	/// </summary>
	/// <param name="loadingState">ステート</param>
	void ProcessDirectionWipe(EnLoadingState loadingState);

	/// <summary>
	/// 円形ワイプ処理
	/// </summary>
	/// <param name="loadingState">ステート</param>
	void ProcessRoundWipe(EnLoadingState loadingState);


private:
	SpriteRender m_backSprite;
	SpriteRender m_loadTextSprite;

	EnLoadingState m_enLoadingState = enLoadingState_None;
	EnMethodLoading m_enMethodLoading = enMethodLoading_None;

	float m_currentAlpha = 0.0f;

	bool m_drawLoadSpriteFlag = false;

};

