#pragma once

#include "CameraCollisionSolver.h"
#include "SpringCamera.h"

class Fade;

class BattleStart:public IGameObject
{
public:
	BattleStart();
	~BattleStart();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:

	/// <summary>
	/// 勇者登場の流れステート
	/// </summary>
	enum EnBraveState
	{
		enBraveState_Complete,
		enBraveState_Fall,			//落ちる
		enBraveState_Landing,		//着地
		enBraveState_Idle,			//アイドル状態
		enBraveState_End			//終わり
	};

	
	/// <summary>
	/// ゲームスタート時のカメラ
	/// </summary>
	void GameStartCamera();

	/// <summary>
	/// カメラのズームアウトタイムの計算
	/// </summary>
	/// <returns></returns>
	bool IsFadeStart();

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
	/// モデルの初期化
	/// </summary>
	void InitModel();
	/// <summary>
	/// 勇者モデルの移動処理
	/// </summary>
	void ProcessMoveBrave();

	/// <summary>
	/// 勇者の状態管理
	/// </summary>
	void ManageBraveState();
	/// <summary>
	/// 落ちる状態遷移処理
	/// </summary>
	void ProcessFallStateTransition();
	/// <summary>
	/// 着地状態遷移処理
	/// </summary>
	void ProcessLandingStateTransition();
	/// <summary>
	/// アイドル状態遷移処理
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// 全て終了状態遷移処理
	/// </summary>
	void ProcessEndStateTransition();

	/// <summary>
	/// 着地エフェクト再生
	/// </summary>
	void PlayLandingEffect();

private:
	Fade* m_fade = nullptr;

	enum EnAnimationClip {
		enAnimClip_Idle,
		enAnimClip_Fall,
		enAnimClip_Landing,
		enAnimClip_Num
	};

	ModelRender m_braveModel;
	AnimationClip m_animationClip[enAnimClip_Num];

	EnBraveState m_enBraveState = enBraveState_Fall;

	CameraCollisionSolver	m_cameraCollisionSolver;
	SpringCamera			m_springCamera;

	Vector3				m_target = Vector3::Zero;		//カメラ注視点

	Vector3 m_bravePosition = g_vec3Zero;

	float m_timer = 0.0f;

	bool m_fadeStartFlag = false;

	const float					m_fadeTime = 2.0f;
	float						m_fadeTimer = 0.0f;


	float m_addYPos = 0.0f;
	float m_mulXZPos = 0.0f;

	bool m_isProcessEndFlag = false;

};

