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

	bool GetRoundWipeEndFlag()
	{
		return m_roundWipeEndFlag;
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


private:

	SpriteRender m_winTextRender;
	SpriteRender m_rightRender;

	SpriteRender m_loseTextRender;

	SpriteRender m_goToTitleRender;




	SpriteRender m_resultSprite;


	EnOutcomeState m_enOutComeState;

	float m_wipeSize = 1200.0f;
	bool m_roundWipeEndFlag = false;


	Quaternion m_rightRotation = g_quatIdentity;

	bool m_resultEndFlag = false;




};

