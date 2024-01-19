#pragma once
class Loading:public IGameObject
{
public:
	Loading();
	~Loading();

	/// <summary>
	/// 
	/// </summary>
	enum EnLoadingRoot
	{
		enLoadingRoot_None,
		enLoadingRoot_TitleToGame,
		enLoadingRoot_GameToTitle,
	};

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

	/// <summary>
	/// ローディングルートの設定
	/// </summary>
	/// <param name="loadingRoot"></param>
	void SetLoadingRoot(EnLoadingRoot loadingRoot)
	{
		m_enLoadingRoot = loadingRoot;
	}

	/// <summary>
	/// 次のシーンを生成
	/// </summary>
	/// <param name="loadingRoot"></param>
	void CreateNextSeen(EnLoadingRoot loadingRoot);

private:

	/// <summary>
	/// ゲームクラスを生成
	/// </summary>
	void CreateGame();
	/// <summary>
	/// タイトルクラスを生成
	/// </summary>
	void CreateTitle();

	/// <summary>
	/// α値の処理
	/// </summary>
	void ProcessSpriteAlpha();

private:

	SpriteRender m_backSprite;
	SpriteRender m_loadTextSprite;

	EnLoadingState m_enLoadingState = enLoadingState_None;
	EnMethodLoading m_enMethodLoading = enMethodLoading_None;

	EnLoadingRoot m_enLoadingRoot = enLoadingRoot_None;

	float m_currentAlpha = 0.0f;

	bool m_drawLoadSpriteFlag = false;


	float m_seenChangeTimer = 0.0f;

	float m_fadeSpeed = 2.0f;

};

