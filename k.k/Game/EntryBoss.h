#pragma once

class Game;

class EntryBoss:public IGameObject
{
public:
	EntryBoss();
	~EntryBoss();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	void MamageState();

	void Animation();

	void positionUp();
	/// <summary>
	/// スプライトレンダーの初期化
	/// </summary>
	void InitSprite();
	/// <summary>
	/// 線形補間で使う始点と終点の初期化
	/// </summary>
	void InitLerpPosition();

	void SpriteMove();


	void FireMove();

	/// <summary>
	/// 登場ムービーが終わってからの処理
	/// </summary>
	void CompleteTime();

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

	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

	const Vector3 GetPosition() const
	{
		return m_position;
	}

	void SetGame(Game* game)
	{
		m_game = game;
	}


	// アニメーションクリップの番号を表す列挙型。
	enum EnAnimationClip {
		enAnimClip_Idle,			// 0 : 待機アニメーション
		enAnimClip_FogRemoval,
		enAnimClip_Num,				// 7 :アニメーションクリップの数
	};

	//アニメーションステート
	enum EnAnimationState {
		enAninationState_Idle,
		enAnimationState_FogRemoval
	};

	void SetNextAnimationState(EnAnimationState NextAnimationState)
	{
		m_enAnimationState = NextAnimationState;
	}

	/// <summary>
	/// 遷移処理を実行
	/// </summary>
	void OnProcessCommonStateTransition();
	/// <summary>
	/// 遷移処理を実行
	/// </summary>
	void OnProcessFogRemovalStateTransition();

	enum EnLichName
	{
		L,
		I,
		C,
		H,
		END
	};

private:

	struct LichCharInfo
	{
		SpriteRender m_CharRender;
		Vector3 m_startPosition = g_vec3Zero;
		Vector3 m_endPosition = g_vec3Zero;
	};


	Game* m_game = nullptr;
	EffectEmitter* m_CircleEffect = nullptr;
	EffectEmitter* m_FogRemovalEffect = nullptr;

	ModelRender m_model;
	Vector3 m_position = g_vec3Zero;
	Quaternion m_rotation = g_quatIdentity;
	Vector3 m_scale = g_vec3One;

	AnimationClip m_animationClip[enAnimClip_Num];	// アニメーションクリップ 
	EnAnimationState m_enAnimationState = enAninationState_Idle;	//アニメーションステート


	SpriteRender m_bossTextRender;
	SpriteRender m_FireRender;
	SpriteRender m_PressAButton;

	
	LichCharInfo m_lichCharInfo[END];				//線形補間で使う始点と終点

	bool m_completeFlag = false;
	bool m_positionUpFlag = false;
	
	float m_time = 0.0f;

	bool m_gOTextFlag = false;
	bool m_FireDrawFlag = false;

	int m_lichTextCount = 0;

	float m_mulValue = 2.0f;

	const float m_endTime = 2.0f;
	float m_endTimer = 0.0f;
};

