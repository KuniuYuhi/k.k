#pragma once
#include "Level3DRender.h"

#include "SpringCamera.h"

class Fade;
class Title :public IGameObject
{
public:
	Title();
	~Title();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void InitSprite();

	void StepManage();

	void SelectMode();

	void CalcAlpha();

	void CalcAlphaAButtonText();

	void MoveCursor();

	void CalcMuteBGMVolume();


	void GoToPlayMode();
	void HowToPlayMode();
	void GameEndMode();


	/// <summary>
	/// ☆を光らせる処理
	/// </summary>
	void ShineStar();

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
	/// 選ばれているモードのテキスト画像を大きくする。それ以外は小さくする
	/// </summary>
	void SetScaleModeText();

	enum EnTitleAnimStep
	{
		enStep1_TitleTextAlpha,
		enStep2_ShineStar,
		enStep_End
	};

	enum EnStarScaleState
	{
		enStarStep1_ScaleUp,
		enStarStep2_ScaleDown,
		enStarStep_End
	};

	enum EnMode
	{
		enMode_GoToPlay,
		enMode_HowToPlay,
		enMode_GameEnd,
		enMode_Num
	};


private:
	/// <summary>
	/// 各種モデルの初期化
	/// </summary>
	void InitModel();
	/// <summary>
	/// スカイキューブの初期化
	/// </summary>
	void InitSkyCube();
	/// <summary>
	/// タイトルのカメラの初期化
	/// </summary>
	void InitTitleCamera();


	/// <summary>
	/// 「Aボタンを押す」の指示に従ったか
	/// </summary>
	/// <returns>従ったらtrue</returns>
	bool IsTriggerPushAButton();



	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool IsDirectionWipeEnd();
	/// <summary>
	/// 
	/// </summary>
	void ProcessDirectionWipe();

private:

	enum EnAnimationClip {
		enAnimClip_Idle,
		enAnimClip_DashJamp,
		enAnimClip_Num
	};

	ModelRender m_braveModel;
	AnimationClip				m_animationClip[enAnimClip_Num];


	ModelRender m_cliffModel;
	ModelRender m_mountainModel;

	SkyCube* m_skyCube;

	Level3DRender m_titleLevel;

	SpringCamera			m_springCamera;



	SpriteRender			m_titleNameRender;
	SpriteRender			m_backRender;
	SpriteRender			m_starRender;
	SpriteRender			m_pushAButtonTextRender;
	SpriteRender			m_goToPlayTextRender;
	SpriteRender			m_howToPlayTextRender;
	SpriteRender			m_gameEndTextRender;
	SpriteRender			m_cursorRender;
	SpriteRender			m_howToPlayRender;
	SpriteRender			m_blackOutRender;

	Fade*					m_fade = nullptr;

	EnTitleAnimStep			m_step = enStep1_TitleTextAlpha;
	EnStarScaleState		m_starScaleStep = enStarStep1_ScaleUp;


	Vector3					m_gTPScale = g_vec3One;
	Vector3					m_hTPScale = g_vec3One;
	Vector3					m_gameEndScale = g_vec3One;


	Vector3				m_toCameraPos = Vector3::Zero;		//カメラ位置から注視点に向かうベクトル
	Vector3				m_target = Vector3::Zero;		//カメラ注視点


	float					m_wipeSize = 860.0f;

	bool					m_pushAButtonFlag = false;

	bool					m_calcAlphaEndFlag = false;
	float					m_alphaTimer = 0.0f;
	Vector4					m_color = g_vec4Clear;

	Vector4					m_AButtonColor = g_vec4Clear;
	float					m_alphaAButtonTimer = 0.0f;
	bool					m_Flag = false;
	
	float					m_mulScale = 0.0f;
	Quaternion				m_starRotation = g_quatIdentity;
	float					m_scaleTimer = 0.0f;
	float					m_mulTimerValue = 5.0f;

	int						m_selectCursor = 0;

	bool					m_SelectModeFlag = false;				//モードを決めているかのフラグ

	bool					m_drawHowToPlayFlag = false;

	bool					m_goToGameFlag = false;				//ゲームが始まるかのフラグ

	bool					m_muteBGMFlag = false;

	bool					m_drawSpriteFlag = true;


	float					m_bgmVolume = 0.0f;



};

