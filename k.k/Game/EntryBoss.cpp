#include "stdafx.h"
#include "EntryBoss.h"
#include "Game.h"
#include "InitEffect.h"
#include "Fade.h"

namespace {
	//スカイキューブの初期の明るさ
	const float START_SKY_CUBE_LMINANCE = 1.008f;
	const float END_SKY_CUBE_LMINANCE = 0.2f;

	//モデルのサイズ
	const float SCALE_UP = 4.0f;
	//モデルのY座標と回転
	const float DEFAULT_MODEL_POS_Y = 380.0f;
	const float DEFAULT__ROT_Y = 180.0f;
	//Y座標の加算量
	const float ADD_MODEL_Y_UP = 45.0f;

	//L、I、C、H、の表示タイミング
	const float LICH_SPRITE_DRAW_TIMING = -20.0f;

	//L、I、C、H、の初期座標と最終的な座標
	const Vector3 L_START_POS = { -707.0f,430.0f,0.0f };
	const Vector3 L_END_POS = { -407.0f,-268.0f,0.0f };
	const Vector3 I_START_POS = { -488.0f,430.0f,0.0f };
	const Vector3 I_END_POS = { -188.0f,-268.0f,0.0f };
	const Vector3 C_START_POS = { 361.0f,430.0f,0.0f };
	const Vector3 C_END_POS = { 61.0f,-268.0f,0.0f };
	const Vector3 H_START_POS = { 676.0f,430.0f,0.0f };
	const Vector3 H_END_POS = { 376.0f,-268.0f,0.0f };
	//LICH画像のサイズ
	const Vector3 START_SCALE = { 4.0f,4.0f,4.0f };
	const Vector3 END_SCALE = g_vec3One;

	//BOSSテキスト画像の座標
	const Vector3 BOSS_TEXT_POS = { 0.0f,-98.0f,0.0f };

	//Aボタンを押すとスキップの画像の座標とサイズ
	const Vector3 PRESS_A_POS = { 723.0f,-501.0f ,0.0f };
	const Vector3 PRESS_A_SCALE = { 0.6f,0.6f ,0.6f };

	//魔法陣のY座標とサイズ
	const float ADD_CIRCLE_POS_Y = 5.0f;
	const float CIRICLE_EFFECT_SIZE = 15.0f;

	//環境光の初期カラーと最終的なカラー
	//0.6
	const float START_AMBIENT_COLOR = 1.0f;
	const float END_AMBIENT_COLOR = 0.01f;

	//ポイントライト
	const Vector3 ADD_POINT_LIGHT_POS = { 0.0f,50.0f,-140.0f };
	const Vector3 POINT_LIGHT_COLOR = { 14.0f, 7.0f, 12.0f };
	const Vector3 POINT_LIGHT_RANGE = { 1500.0f, 4.0f, 0.0f };

	const Vector3 DIRECTION_RIGHT_COLOR = Vector3(0.1f, 0.1f, 0.1f);

	const float MUTE_SPEED = 15.0f;

	const Vector3 START_POS = { 0.0f,670.0f,800.0f };
	const Vector3 CENTER_POS = { 700.0f,470.0f,0.0f };
	const Vector3 END_POS = { 160.0f,230.0f,-550.0f };

	const float BOSS_MOVIE_SKIP_TIME = 2.0f;
}

EntryBoss::EntryBoss()
{
}

EntryBoss::~EntryBoss()
{
	DeleteTask();
}

bool EntryBoss::Start()
{
	m_fade = FindGO<Fade>("fade");

	m_animationClip[enAnimClip_Idle].Load("Assets/animData/character/Lich/Idle.tka");
	m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClip[enAnimClip_FogRemoval].Load("Assets/animData/character/Lich/FogRemoval.tka");
	m_animationClip[enAnimClip_FogRemoval].SetLoopFlag(false);

	m_model.Init(
		"Assets/modelData/character/Lich/Lich_real.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		m_animationClip,
		enAnimClip_Num,
		enModelUpAxisZ
	);

	Vector3 pos = m_position;
	pos.y += ADD_CIRCLE_POS_Y;
	m_CircleEffect = NewGO<EffectEmitter>(0);
	m_CircleEffect->Init(InitEffect::enEffect_Boss_Summon_Circle);
	m_CircleEffect->Play();
	m_CircleEffect->SetPosition(pos);
	m_CircleEffect->SetScale(g_vec3One * CIRICLE_EFFECT_SIZE);
	m_CircleEffect->Update();

	//モデルの初期設定
	m_position.y -= DEFAULT_MODEL_POS_Y;
	m_scale *= SCALE_UP;
	m_rotation.SetRotationDegY(DEFAULT__ROT_Y);
	m_model.SetTransform(m_position, m_rotation, m_scale);
	m_model.Update();

	//ポイントライトの座標
	Vector3 pointLightPos = m_position;
	pointLightPos.Add(ADD_POINT_LIGHT_POS);
	//ポイントライト
	g_renderingEngine->SetPointLight(
		pointLightPos,
		POINT_LIGHT_COLOR,
		POINT_LIGHT_RANGE
	);

	//スカイキューブも明るさを取得
	m_skyLuminance = m_skyCube->GetLuminance();
	
	/*g_renderingEngine->SetSpotLight(
		pointLightPos,
		Vector3(100.0f,0.0f,100.0f),
		Vector3(500.0f, 10.0f, 0.0f),
		Vector3(1.0f,-1.0f,0.0f),
		Vector3(90.0f, 40.0f, 0.0f)
	);*/

	
	//アニメーションイベント用の関数を設定する。
	m_model.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});
	//画像の読み込み
	InitSprite();
	//線形補間で使う始点と終点の初期化
	InitLerpPosition();


	//環境光
	g_renderingEngine->SetAmbient({0.1f,0.1f,0.1f});
	//初期の環境光のカラーを設定
	m_ambientColor = END_AMBIENT_COLOR;

	//ゴゴゴゴゴゴゴの再生
	g_soundManager->InitAndPlaySoundSource(enSoundName_Gogogo, g_soundManager->GetSEVolume());
	//被写界深度の無効化
	g_renderingEngine->DisableDof();

	//リムライトの無効化
	//g_renderingEngine->UnUseLimLight();
	g_renderingEngine->SetLimLIghtPower(5.0f);

	//ばねカメラの初期化。
	m_springCamera.Init(
		*g_camera3D,		//ばねカメラの処理を行うカメラを指定する。
		500.0f,			//カメラの移動速度の最大値。
		false,				//カメラと地形とのあたり判定を取るかどうかのフラグ。trueだとあたり判定を行う。
		1.0f				//カメラに設定される球体コリジョンの半径。第３引数がtrueの時に有効になる。
	);

	m_fade->StartFadeOut(3.0f);

	return true;
}

void EntryBoss::Update()
{
	//全ての処理が完了して、フェードインが終わったら
	if (m_completeFlag == true && m_fade->IsFade() == false)
	{
		//登場ムービーが終わったことゲームに伝える
		m_game->SetBossMovieFlag(true);
		return;
	}

	//ムービースキップ処理
	BossMovieSkip();
	//画面を徐々に暗くする。暗くなったら処理が止まる
	slowlyDarkScreen();
	//ステート管理
	ManageState();
	//カメラ処理
	ChaseBossCamera();

	MamageActionBossState();
	Animation();
	m_model.Update();
}

void EntryBoss::positionUp()
{
	//座標を上げきったら処理をしない
	if (m_positionUpEndFlag == true)
	{
		return;
	}

	if (m_position.y >= 0.0f)
	{
		m_position.y = 0.0f;
		SetNextAnimationState(enAnimationState_FogRemoval);
		m_positionUpEndFlag = true;
	}
	else
	{
		//画像の表示を始める
		if (m_position.y > LICH_SPRITE_DRAW_TIMING)
		{
			m_GoTextFlag = true;
		}
		//少しずつ浮き上がる
		m_position.y += g_gameTime->GetFrameDeltaTime() * ADD_MODEL_Y_UP;
	}

	m_model.SetPosition(m_position);
	
}

void EntryBoss::InitSprite()
{
	m_lichCharInfo[L].m_CharRender.Init("Assets/sprite/InGame/BossAppearance/L.DDS", 243, 290);
	SettingSpriteRender(m_lichCharInfo[L].m_CharRender, L_START_POS, START_SCALE, g_quatIdentity);
	m_lichCharInfo[I].m_CharRender.Init("Assets/sprite/InGame/BossAppearance/I.DDS", 167, 290);
	SettingSpriteRender(m_lichCharInfo[I].m_CharRender, I_START_POS, START_SCALE, g_quatIdentity);
	m_lichCharInfo[C].m_CharRender.Init("Assets/sprite/InGame/BossAppearance/C.DDS", 280, 290);
	SettingSpriteRender(m_lichCharInfo[C].m_CharRender, C_START_POS, START_SCALE, g_quatIdentity);
	m_lichCharInfo[H].m_CharRender.Init("Assets/sprite/InGame/BossAppearance/H.DDS", 307, 290);
	SettingSpriteRender(m_lichCharInfo[H].m_CharRender, H_START_POS, START_SCALE, g_quatIdentity);
	
	m_bossTextRender.Init("Assets/sprite/InGame/BossAppearance/Boss.DDS", 284, 84);
	SettingSpriteRender(m_bossTextRender, BOSS_TEXT_POS, g_vec3One, g_quatIdentity);

	m_PressAButton.Init("Assets/sprite/InGame/BossAppearance/SkipText.DDS", 720, 76);
	SettingSpriteRender(m_PressAButton, PRESS_A_POS, PRESS_A_SCALE, g_quatIdentity);
}

void EntryBoss::InitLerpPosition()
{
	//「L」の始点と終点
	m_lichCharInfo[L].m_startPosition = L_START_POS;
	m_lichCharInfo[L].m_endPosition = L_END_POS;
	//「I」の始点と終点
	m_lichCharInfo[I].m_startPosition = I_START_POS;
	m_lichCharInfo[I].m_endPosition = I_END_POS;
	//「C」の始点と終点
	m_lichCharInfo[C].m_startPosition = C_START_POS;
	m_lichCharInfo[C].m_endPosition = C_END_POS;
	//「I」の始点と終点
	m_lichCharInfo[H].m_startPosition = H_START_POS;
	m_lichCharInfo[H].m_endPosition = H_END_POS;
}

void EntryBoss::SpriteMove()
{
	if (m_lichTextCount == END)
	{
		m_drawBOSSTextFlag = true;
		return;
	}
	//補間率の計算
	m_time += g_gameTime->GetFrameDeltaTime() * 3.8f;

	//文字の座標の線形補間
	Vector3 LerpPos;
	LerpPos.Lerp(m_time, m_lichCharInfo[m_lichTextCount].m_startPosition, m_lichCharInfo[m_lichTextCount].m_endPosition);
	//文字のサイズの線形補間
	Vector3 size;
	size.Lerp(m_time, START_SCALE, END_SCALE);
	//回転
	Quaternion rot = g_quatIdentity;

	//更新
	m_lichCharInfo[m_lichTextCount].m_CharRender.SetTransForm(LerpPos, size, rot);
	m_lichCharInfo[m_lichTextCount].m_CharRender.Update();


	//一秒超えたら次の文字に切り替える
	if (m_time >= 1.0f)
	{
		m_lichTextCount++;
		m_time = 0.0f;
	}
}

void EntryBoss::slowlyDarkScreen()
{
	//暗くなったら処理しない
	if (m_SlowlyDarkScreenEndFlag == true)
	{
		return;
	}

	m_lightTimer += g_gameTime->GetFrameDeltaTime() * 2.0f;

	//画面を暗くする
	m_ambientColor = Math::Lerp(m_lightTimer, m_ambientColor, END_AMBIENT_COLOR);
	m_finalAmbientColor = g_vec3One * m_ambientColor;
	g_renderingEngine->SetAmbient(m_finalAmbientColor);

	//空を暗くする
	m_skyLuminance = Math::Lerp(m_lightTimer, m_skyLuminance, END_SKY_CUBE_LMINANCE);
	m_skyCube->SetLuminance(m_skyLuminance);
	m_skyCube->Update();

	//環境光が最大まで小さくなったら
	if (m_ambientColor <= END_AMBIENT_COLOR)
	{
		m_SlowlyDarkScreenEndFlag = true;
		//ライトタイマーリセット
		m_lightTimer = 0.0f;
	}
}

void EntryBoss::SlowlyBrightScreen()
{
	//暗くなったら処理しない
	if (m_SlowlyBrightScreenEndFlag == true)
	{
		return;
	}

	m_lightTimer += g_gameTime->GetFrameDeltaTime() * m_mulBrightTimer;

	m_mulBrightTimer += m_mulBrightTimer*0.1f;

	//画面を明るくする
	m_ambientColor = Math::Lerp(m_lightTimer, m_ambientColor, START_AMBIENT_COLOR);
	m_finalAmbientColor = g_vec3One * m_ambientColor;
	g_renderingEngine->SetAmbient(m_finalAmbientColor);

	//空を明るくする
	m_skyLuminance = Math::Lerp(m_lightTimer, m_skyLuminance, START_SKY_CUBE_LMINANCE);
	m_skyCube->SetLuminance(m_skyLuminance);
	m_skyCube->Update();

	//環境光が最大までおおきくなったら
	if (m_ambientColor >= START_AMBIENT_COLOR)
	{
		m_SlowlyBrightScreenEndFlag = true;
		g_renderingEngine->SetLimLIghtPower(1.5f);
	}
}

void EntryBoss::CompleteTime()
{
	//処理がおわったらしばらく停止してから
	if (m_endTime < m_endTimer)
	{
		//やることが終わったのでフェードインする
		m_fade->StartFadeIn(3.0f);
		//処理完了フラグを立てる
		m_completeFlag = true;
	}
	else
	{
		m_endTimer += g_gameTime->GetFrameDeltaTime();
	}
}

void EntryBoss::CalcMuteGogogoSE()
{
	if (m_muteFlag != true)
	{
		return;
	}
	//音を線形補間で小さくしていく
	m_gogogoVolume = Math::Lerp(
		g_gameTime->GetFrameDeltaTime() * MUTE_SPEED, m_gogogoVolume, 0.1f);

	if (m_gogogoVolume <= 0.1f)
	{
		g_soundManager->StopSound(enSoundName_Gogogo);
		m_muteFlag = false;
	}
	else
	{
		//ボリュームを設定
		g_soundManager->GetSoundSource(enSoundName_Gogogo)->SetVolume(m_gogogoVolume);
	}
}

void EntryBoss::ManageState()
{
	switch (m_enWholeState)
	{
	case EntryBoss::enWholeState_RiseBoss:
		OnProcessRiseBossTransition();
		break;
	case EntryBoss::enWholeState_FogRemoval:
		OnProcessFogRemovalTransition();
		break;
	case EntryBoss::enWholeState_Complete:
		OnProcessCompleteTransition();
		break;
	default:
		break;
	}
}

void EntryBoss::OnProcessRiseBossTransition()
{
	//文字が全て出て、BOSSのテキスト画像フラグがtrue
	// になったら次のステートに遷移
	if (m_drawBOSSTextFlag == true)
	{
		m_enWholeState = enWholeState_FogRemoval;
		return;
	}

	//ボスの座標を上げる
	positionUp();
	//LICHのテキスト画像の表示タイミング
	if (m_GoTextFlag == true)
	{
		SpriteMove();
	}
}

void EntryBoss::OnProcessFogRemovalTransition()
{
	//霧払いエフェクトがnullptyでないなら
	if (m_FogRemovalEffect != nullptr)
	{
		//ゴゴゴゴゴゴ音を徐々に小さくする
		CalcMuteGogogoSE();
		//画面を徐々に明るくする
		SlowlyBrightScreen();
		//霧払いエフェクトがおわったら
		if (m_FogRemovalEffect->IsPlay() == false)
		{
			//やること終わり
			//最後のステートに遷移
			m_enWholeState = enWholeState_Complete;
			return;

		}
	}
}

void EntryBoss::OnProcessCompleteTransition()
{
	//しばらく静止してから終わる
	CompleteTime();
}

void EntryBoss::MamageActionBossState()
{
	switch (m_enAnimationState)
	{
	case EntryBoss::enAninationState_Idle:
		OnProcessCommonStateTransition();
		break;
	case EntryBoss::enAnimationState_FogRemoval:
		OnProcessFogRemovalStateTransition();
		break;
	default:
		break;
	}
}

void EntryBoss::Animation()
{
	switch (m_enAnimationState)
	{
	case EntryBoss::enAninationState_Idle:
		m_model.PlayAnimation(enAnimClip_Idle, 0.4f);
		break;
	case EntryBoss::enAnimationState_FogRemoval:
		m_model.PlayAnimation(enAnimClip_FogRemoval);
		break;
	default:
		break;
	}
}

void EntryBoss::OnProcessCommonStateTransition()
{
	SetNextAnimationState(enAninationState_Idle);
}

void EntryBoss::OnProcessFogRemovalStateTransition()
{
	if (m_model.IsPlayingAnimation() == false)
	{
		OnProcessCommonStateTransition();
	}
}

void EntryBoss::ChaseBossCamera()
{
	if (m_chaseBossTime > 1.0f)
	{
		return;
	}

	//注視点の計算
	m_target = GetPosition();
	m_target.y = 0.0f;

	//線形補間
	m_pos1.Lerp(m_chaseBossTime, START_POS, CENTER_POS);
	m_pos2.Lerp(m_chaseBossTime, CENTER_POS, END_POS);
	m_toCameraPosForBoss.Lerp(m_chaseBossTime, m_pos1, m_pos2);

	m_chaseBossTime += g_gameTime->GetFrameDeltaTime() * 0.12f;

	Vector3 finalCameraPos = m_toCameraPosForBoss + m_target;

	//視点と注視点を設定
	m_springCamera.SetTarget(m_target);
	m_springCamera.SetPosition(finalCameraPos);
	//カメラの更新。
	m_springCamera.Update();
}

void EntryBoss::BossMovieSkip()
{
	//スキップが確定したら処理しない
	if (m_movieSkipFlag != false)
	{
		return;
	}

	if (g_pad[0]->IsPress(enButtonA))
	{
		//3秒たったらスキップ
		if (m_bossMovieSkipTimer > BOSS_MOVIE_SKIP_TIME)
		{
			//フェードインする
			m_fade->StartFadeIn(3.0f);
			m_completeFlag = true;
			m_movieSkipFlag = true;
		}
		else
		{
			//タイマーを加算
			m_bossMovieSkipTimer += g_gameTime->GetFrameDeltaTime();
		}
	}
	else
	{
		//タイマーをリセット
		m_bossMovieSkipTimer = 0.0f;
	}
}

void EntryBoss::DeleteTask()
{
	//音の停止
	g_soundManager->StopSound(enSoundName_FogRemoval);
	g_soundManager->StopSound(enSoundName_Gogogo);

	//エフェクトの停止
	if (m_CircleEffect != nullptr)
	{
		m_CircleEffect->Stop();
	}

	if (m_FogRemovalEffect != nullptr)
	{
		m_FogRemovalEffect->Stop();
	}

	//ライトを元に戻す
	g_renderingEngine->SetDirLightColor(DIRECTION_RIGHT_COLOR);
	//環境光
	g_renderingEngine->SetAmbient(g_vec3One * START_AMBIENT_COLOR);
	//ポイントライトを消す
	g_renderingEngine->UnUsePointLight();
	//スカイキューブの明るさを戻す
	m_skyCube->SetLuminance(START_SKY_CUBE_LMINANCE);
	//リムライトの有効化
	g_renderingEngine->SetLimLIghtPower(1.5f);
}

void EntryBoss::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//霧払いエフェクトの再生
	if (wcscmp(eventName, L"FogRemoval") == 0)
	{
		m_FogRemovalEffect = NewGO<EffectEmitter>(0);
		m_FogRemovalEffect->Init(InitEffect::enEffect_Boss_Summon_FogRemoval);
		m_FogRemovalEffect->Play();
		m_FogRemovalEffect->SetPosition(m_position);
		m_FogRemovalEffect->SetScale({ 15.0f,40.0f,15.0f });
		m_FogRemovalEffect->Update();

		//霧払い音再生
		g_soundManager->InitAndPlaySoundSource(enSoundName_FogRemoval, g_soundManager->GetSEVolume());
		//gogogo音を小さくしていく
		m_muteFlag = true;
		m_gogogoVolume = g_soundManager->GetDefaultSEVolume();
		//ポイントライトを消す
		g_renderingEngine->UnUsePointLight();

		//リムライトの有効化
		g_renderingEngine->UseLimLight();

		
	}
}

void EntryBoss::Render(RenderContext& rc)
{
	m_model.Draw(rc);

	if (m_drawBOSSTextFlag == true)
	{
		m_bossTextRender.Draw(rc);
	}
	else
	{
		m_PressAButton.Draw(rc);
	}

	//表示開始フラグが立っていないなら
	if (m_GoTextFlag != true)
	{
		return;
	}

	if (m_lichTextCount >= L)
	{
		m_lichCharInfo[L].m_CharRender.Draw(rc);
	}
	if (m_lichTextCount >= I)
	{
		m_lichCharInfo[I].m_CharRender.Draw(rc);
	}
	if (m_lichTextCount >= C)
	{
		m_lichCharInfo[C].m_CharRender.Draw(rc);
	}
	if (m_lichTextCount >= H)
	{
		m_lichCharInfo[H].m_CharRender.Draw(rc);
	}
}
