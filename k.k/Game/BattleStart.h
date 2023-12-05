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

	/// <summary>
	/// 画像の移動処理
	/// </summary>
	/// <param name="spriteRender"></param>
	/// <param name="startPos"></param>
	/// <param name="endPos"></param>
	void MoveText(SpriteRender& spriteRender,Vector3 startPos,Vector3 endPos);

	/// <summary>
	/// 透明化
	/// </summary>
	void CalcAlpha();
	/// <summary>
	/// 画像の初期化
	/// </summary>
	void InitSprite();

	/// <summary>
	/// ゲームスタート時のカメラ
	/// </summary>
	void GameStartCamera();

	/// <summary>
	/// カメラのズームアウトタイムの計算
	/// </summary>
	/// <returns></returns>
	bool CalcCameraZoomOutTime();

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

private:
	Fade* m_fade = nullptr;

	SpriteRender m_battleTextRender;
	SpriteRender m_startTextRender;

	CameraCollisionSolver	m_cameraCollisionSolver;
	SpringCamera			m_springCamera;

	Vector3				m_target = Vector3::Zero;		//カメラ注視点
	Vector4 m_color = g_vec4White;

	float m_timer = 0.0f;

	float m_alpha = 0.0f;
	bool m_calcAlphaEndFlag = false;

	bool m_fadeStartFlag = false;

	const float					m_cameraZoomOutTime = 2.0f;
	float						m_cameraZoomOutTimer = 0.0f;

	float m_count = 1.0f;
};

