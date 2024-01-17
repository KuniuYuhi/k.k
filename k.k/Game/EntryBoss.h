#pragma once
#include "CameraCollisionSolver.h"
#include "SpringCamera.h"

class Game;
class Fade;

class EntryBoss:public IGameObject
{
public:
	EntryBoss();
	~EntryBoss();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

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
	/// 座標の設定
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}
	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns></returns>
	const Vector3 GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// ゲームのインスタンスの設定
	/// </summary>
	/// <param name="game"></param>
	void SetGame(Game* game)
	{
		m_game = game;
	}
	/// <summary>
	/// スカイキューブのインスタンスの設定
	/// </summary>
	/// <param name="skyCube"></param>
	void SetSkyCube(SkyCube* skyCube)
	{
		m_skyCube = skyCube;
	}

private:
	/// <summary>
	/// 全体の流れのステート
	/// </summary>
	enum EnWholeState
	{
		enWholeState_RiseBoss,		//ボスが地面から上昇
		enWholeState_FogRemoval,	//霧払い
		enWholeState_Complete		//完了

	};

	/// <summary>
	/// 全体のステートの処理
	/// </summary>
	void ManageState();
	/// <summary>
	/// ボス上昇ステートの状態遷移処理
	/// </summary>
	void OnProcessRiseBossTransition();
	/// <summary>
	/// 霧払いステートの状態遷移処理
	/// </summary>
	void OnProcessFogRemovalTransition();
	/// <summary>
	/// 完了ステートの状態遷移処理
	/// </summary>
	void OnProcessCompleteTransition();


	/// <summary>
	/// ボスの行動のステート
	/// </summary>
	void MamageActionBossState();
	/// <summary>
	/// アニメーション
	/// </summary>
	void Animation();
	/// <summary>
	/// 座標を上に上げる処理
	/// </summary>
	void positionUp();
	/// <summary>
	/// スプライトレンダーの初期化
	/// </summary>
	void InitSprite();
	/// <summary>
	/// 線形補間で使う始点と終点の初期化
	/// </summary>
	void InitLerpPosition();
	/// <summary>
	/// 画像の移動処理
	/// </summary>
	void SpriteMove();

	/// <summary>
	/// 画面を徐々に暗くする
	/// </summary>
	void slowlyDarkScreen();
	/// <summary>
	/// 画面を徐々に明るくする
	/// </summary>
	void SlowlyBrightScreen();

	/// <summary>
	/// 登場ムービーが終わってからの処理
	/// </summary>
	void CompleteTime();

	/// <summary>
	/// 効果音を徐々に小さくする
	/// </summary>
	void CalcMuteGogogoSE();

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

	/// <summary>
	/// 次のアニメーションステートを設定
	/// </summary>
	/// <param name="NextAnimationState"></param>
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

	/// <summary>
	/// ボスを追うカメラ
	/// </summary>
	void ChaseBossCamera();

	/// <summary>
	/// ボス登場ムービースキップ処理
	/// </summary>
	void BossMovieSkip();

	/// <summary>
	/// 消すときの作業
	/// </summary>
	void DeleteTask();

private:


	/// <summary>
	/// リッチの文字列ステート
	/// </summary>
	enum EnLichName
	{
		L,
		I,
		C,
		H,
		END
	};

	struct LichCharInfo
	{
		SpriteRender m_CharRender;
		Vector3 m_startPosition = g_vec3Zero;
		Vector3 m_endPosition = g_vec3Zero;
	};

	CameraCollisionSolver	m_cameraCollisionSolver;
	SpringCamera			m_springCamera;

	Fade*						m_fade = nullptr;
	Game*						m_game = nullptr;
	SkyCube*					m_skyCube = nullptr;
	EffectEmitter*				m_CircleEffect = nullptr;
	EffectEmitter*				m_FogRemovalEffect = nullptr;

	ModelRender					m_model;
	Vector3						m_position = g_vec3Zero;
	Quaternion					m_rotation = g_quatIdentity;
	Vector3						m_scale = g_vec3One;

	Vector3				m_toCameraPosForBoss = Vector3::Zero;	//カメラ位置から注視点(ボス)に向かうベクトル
	Vector3				m_target = Vector3::Zero;		//カメラ注視点
	Vector3				m_pos1 = Vector3::Zero;
	Vector3				m_pos2 = Vector3::Zero;

	Vector3						m_finalAmbientColor = g_vec3One;

	AnimationClip				m_animationClip[enAnimClip_Num];				// アニメーションクリップ 
	EnAnimationState			m_enAnimationState = enAninationState_Idle;		//アニメーションステート
	EnWholeState				m_enWholeState = enWholeState_RiseBoss;			//全体の流れステート

	SpriteRender				m_bossTextRender;								//BOSS
	SpriteRender				m_PressAButton;									//右下のテキストの画像

	LichCharInfo				m_lichCharInfo[END];							//線形補間で使う始点と終点

	float m_chaseBossTime = 0.0f;

	bool						m_completeFlag = false;							//全ての処理が完了したかのフラグ
	bool						m_positionUpEndFlag = false;					//座標を上げる処理が終わったかのフラグ
	
	float						m_time = 0.0f;				

	bool						m_GoTextFlag = false;							//文字画像の処理を始めるかのフラグ
	int							m_lichTextCount = 0;

	bool						m_drawBOSSTextFlag = false;

	float						m_mulValue = 2.0f;

	const float					m_endTime = 2.0f;
	float						m_endTimer = 0.0f;

	float						m_skyLuminance = 0.9f;
	float                       m_ambientColor = 0.0f;

	float						m_lightTimer = 0.0f;

	float						m_gogogoVolume = 0.0f;

	bool						m_muteFlag = false;

	bool						m_SlowlyDarkScreenEndFlag = false;				//画面を暗くしたかのフラグ
	bool						m_SlowlyBrightScreenEndFlag = false;			//画面を明るくしたかのフラグ

	float                       m_bossMovieSkipTimer = 0.0f;
	bool						m_movieSkipFlag = false;

};

