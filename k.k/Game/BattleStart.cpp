#include "stdafx.h"
#include "BattleStart.h"
#include "Fade.h"

#include "GameSceneManager.h"
#include "AllGameSceneState.h"


#include "EffectNumbers.h"

using namespace EffectNumbers;

namespace {
    const Vector3 BATTLE_TEXT_START_POS = { 0.0f,336.0f,0.0f };
    const Vector3 BATTLE_TEXT_END_POS = { 1650.0f,336.0f,0.0f };
 
    const Vector3 START_TEXT_START_POS = { 0.0f,-336.0f,0.0f };
    const Vector3 START_TEXT_END_POS = { -1650.0f,-336.0f,0.0f };


    const float TARGETPOS_GAMESTART_YUP = 38.0f;
    const float STARTCAMERA_YUP = 60.0f;
    const float START_MULXZ = 120.0f;

    const float LANDING_CAMERA_YUP = -200.0f;
    const float IDLE_MULXZ = 200.0f;

    const float FALL_SPEED = 140.0f;
    const float ADD_YPOS_SPEED = 30.0f;

    const Vector3 BRAVE_START_POSITION = { 0.0f,200.0f,-400.0f };

    const float LANDING_EFFECT_SIZE = 5.0f;
}

BattleStart::BattleStart()
{
}

BattleStart::~BattleStart()
{
}

bool BattleStart::Start()
{
    //フェードのインスタンスを探す
    m_fade = FindGO<Fade>("fade");

    g_camera3D->SetNear(1.0f);
    g_camera3D->SetFar(10000.0f);

    //ばねカメラの初期化。
    m_springCamera.Init(
        *g_camera3D,		//ばねカメラの処理を行うカメラを指定する。
        180.0f,			//カメラの移動速度の最大値。
        false,				//カメラと地形とのあたり判定を取るかどうかのフラグ。trueだとあたり判定を行う。
        1.0f				//カメラに設定される球体コリジョンの半径。第３引数がtrueの時に有効になる。
    );

    m_addYPos = STARTCAMERA_YUP;
    m_mulXZPos = START_MULXZ;

    //リムライトの無効化
   // g_renderingEngine->Use

    //モデルの初期化
    InitModel();

    //ジャンプ音再生
    g_soundManager->InitAndPlaySoundSource(
        enSoundName_Fall, g_soundManager->GetSEVolume()
    );

    return true;
}

void BattleStart::Update()
{
    //ゲームスタート時のカメラ処理
    GameStartCamera();

    //勇者の状態遷移管理
    ManageBraveState();
}

void BattleStart::Render(RenderContext& rc)
{
    m_braveModel.Draw(rc);

}

void BattleStart::GameStartCamera()
{
    //注視点の計算
    m_target = m_braveModel.GetPosition();
    m_target.y += m_addYPos;
    //前方向の取得
    Vector3 CameraPosXZ = g_vec3AxisZ;
    CameraPosXZ *= m_mulXZPos;
    //Y方向
    Vector3 CameraPosY = Vector3::AxisY;
    CameraPosY *= STARTCAMERA_YUP;

    //カメラの座標
    Vector3 newCameraPos = CameraPosXZ + CameraPosY;

    Vector3 finalCameraPos = newCameraPos + m_target;
    finalCameraPos.y = STARTCAMERA_YUP;

    //視点と注視点を設定
    m_springCamera.SetTarget(m_target);
    m_springCamera.SetPosition(finalCameraPos);

    //カメラの更新。
    m_springCamera.Update();
}

bool BattleStart::IsFadeStart()
{
    if (m_fadeTimer > m_fadeTime)
    {
        //フェードイン開始
        //フェードイン仕切ったらゲーム側がゲームスタートクラスを削除する
        m_fade->StartFadeIn(3.0f);
        m_fadeStartFlag = true;
        return true;
    }
    else
    {
        //タイマーを加算
        m_fadeTimer += g_gameTime->GetFrameDeltaTime();
    }

    return false;
}

void BattleStart::InitModel()
{
    m_bravePosition = BRAVE_START_POSITION;

    //アニメーションのロード
    m_animationClip[enAnimClip_Idle].Load(
        "Assets/animData/character/Player/GameStart/Idle.tka"
    );
    m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
    m_animationClip[enAnimClip_Fall].Load(
        "Assets/animData/character/Player/GameStart/Fall.tka"
    );
    m_animationClip[enAnimClip_Fall].SetLoopFlag(true);
    m_animationClip[enAnimClip_Landing].Load(
        "Assets/animData/character/Player/GameStart/Landing.tka"
    );
    m_animationClip[enAnimClip_Landing].SetLoopFlag(false);

    //勇者モデルの初期化
    m_braveModel.Init("Assets/modelData/Title/brave.tkm",
        L"Assets/shader/ToonTextrue/lamp_glay.DDS",
        m_animationClip,
        enAnimClip_Num,
        enModelUpAxisZ);
    //トランスフォームの設定と更新
    m_braveModel.SetTransform(
        m_bravePosition,
        g_quatIdentity,
        g_vec3One
    );
    m_braveModel.PlayAnimation(enAnimClip_Fall);
    m_braveModel.Update();
}

void BattleStart::ProcessMoveBrave()
{
    //落ちていく処理
    m_bravePosition.y -= g_gameTime->GetFrameDeltaTime() * FALL_SPEED;

    m_addYPos -= g_gameTime->GetFrameDeltaTime() * ADD_YPOS_SPEED;

    //地面に着いたら
    if (m_bravePosition.y < 0.0f)
    {
        m_bravePosition.y = 0.0f;
        //カメラのY座標を一気に下げる
        m_addYPos = LANDING_CAMERA_YUP;
        //勇者からカメラを少し離す
        m_mulXZPos = IDLE_MULXZ;

        //着地エフェクト再生
        PlayLandingEffect();
        //着地の音再生
        g_soundManager->InitAndPlaySoundSource(
            enSoundName_Landing,
            g_soundManager->GetBGMVolume()
        );

        //次のステートに進む
        m_enBraveState = enBraveState_Landing;
    }

    m_braveModel.SetPosition(m_bravePosition);

}

void BattleStart::ManageBraveState()
{
    switch (m_enBraveState)
    {
    case BattleStart::enBraveState_Fall:
        ProcessFallStateTransition();
        break;
    case BattleStart::enBraveState_Landing:
        ProcessLandingStateTransition();
        break;
    case BattleStart::enBraveState_Idle:
        ProcessIdleStateTransition();
        break;
    case BattleStart::enBraveState_End:
        ProcessEndStateTransition();
        break;
    default:
        break;
    }


    m_braveModel.Update();
}

void BattleStart::ProcessFallStateTransition()
{
    ProcessMoveBrave();
}

void BattleStart::ProcessLandingStateTransition()
{
    //カメラのターゲットのY座標を最初と同じに戻す
    m_addYPos = STARTCAMERA_YUP;

    //着地アニメーション再生
    m_braveModel.PlayAnimation(enAnimClip_Landing,0.1f);

    if (m_braveModel.IsPlayingAnimation() == false)
    {
        //次のステートに進む
        m_enBraveState = enBraveState_Idle;
    }
}

void BattleStart::ProcessIdleStateTransition()
{
    //アイドルアニメーション再生
    m_braveModel.PlayAnimation(enAnimClip_Idle,0.3f);

    //次のステートに進む
    m_enBraveState = enBraveState_End;
}

void BattleStart::ProcessEndStateTransition()
{
    //処理が終わったなら何もしない
    if (m_isProcessEndFlag == true)
    {
        return;
    }

    //フェードスタートフラグがtrueでないなら処理する
    if (m_fadeStartFlag != true)
    {
        IsFadeStart();
    }
    else
    {
        if (!m_fade->IsFade())
        {
            //フェードイン仕切ったらゲームシーンマネージャーにシーン
            //を切り替えても良いことを伝える
            GameSceneManager::GetInstance()->SetIsSceneChangeableFlag(true);
            m_isProcessEndFlag = true;
        }
       
    }
}

void BattleStart::PlayLandingEffect()
{
    EffectEmitter* landingEffect = NewGO<EffectEmitter>(0);
    landingEffect->Init(enEffect_BraveLanding);
    landingEffect->Play();
    landingEffect->SetPosition(m_bravePosition);
    landingEffect->SetScale(g_vec3One * LANDING_EFFECT_SIZE);
    landingEffect->Update();
}
