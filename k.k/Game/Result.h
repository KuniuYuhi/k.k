#pragma once
class ResultSeen:public IGameObject
{
public:
	ResultSeen();
	~ResultSeen();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void InitSprite();

	/// <summary>
	/// ステート管理
	/// </summary>
	void ManageState();
	/// <summary>
	/// 勝利時の処理
	/// </summary>
	void WinState();
	/// <summary>
	/// 敗北時の処理
	/// </summary>
	void LoseState();

	void ResultEnd();

	/// <summary>
	/// スプライトの座標、サイズ、回転を設定
	/// </summary>
	/// <param name="spriterender">スプライトレンダー</param>
	/// <param name="position">座標</param>
	/// <param name="scale">サイズ</param>
	/// <param name="rotation">回転</param>
	void SettingSpriteRender(
		SpriteRender& spriterender, Vector3 position, Vector3 scale, Quaternion rotation)
	{
		spriterender.SetPosition(position);
		spriterender.SetScale(scale);
		spriterender.SetRotation(rotation);
		spriterender.Update();
	}

	/// <summary>
	/// 円形ワイプ終了フラグを取得
	/// </summary>
	/// <returns></returns>
	bool GetRoundWipeEndFlag()
	{
		return m_roundWipeEndFlag;
	}

	/// <summary>
	/// リザルトの処理が終わったかのフラグを取得
	/// </summary>
	/// <param name="flag"></param>
	/// <returns></returns>
	const bool& GetResultEndFlag() const
	{
		return m_resultEndFlag;
	}

	//勝敗
	enum EnOutcomeState
	{
		enOutcomeNone,
		enOutcome_Win,
		enOutcome_Lose
	};
	/// <summary>
	/// 勝敗を設定する
	/// </summary>
	/// <param name="outcom"></param>
	void SetOutcome(EnOutcomeState outcom)
	{
		m_enOutComeState = outcom;
	}

	/// <summary>
	/// リザルトでの処理ステップ
	/// </summary>
	enum EnResultStep
	{
		enResultStep_main,
		enResultStep_end,
	};

	/// <summary>
	/// ゲームクリアの処理
	/// </summary>
	void OnProcessGameClearMainTranstion();
	/// <summary>
	/// ゲームオーバーの処理
	/// </summary>
	void OnProcessGameOverMainTranstion();
	/// <summary>
	/// 終了時の処理
	/// </summary>
	void OnProcessEndTranstion();

private:

	SpriteRender m_winTextRender;
	SpriteRender m_rightRender;

	SpriteRender m_loseTextRender;

	SpriteRender m_goToTitleRender;




	SpriteRender m_resultSprite;


	EnOutcomeState m_enOutComeState = enOutcomeNone;
	EnResultStep m_enResultStep = enResultStep_main;

	float m_wipeSize = 1200.0f;
	bool m_roundWipeEndFlag = false;


	Quaternion m_rightRotation = g_quatIdentity;

	bool m_resultEndFlag = false;
	bool m_roundWipeStartFlag = false;


	float m_goToTitleTime = 1.0f;

	bool m_possibleGoToTitleFlag = false;

};

