#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "Fade.h"
#include "system/system.h"

#include "SelectWeapon.h"

namespace {
    const float SPRITE_OFFSET = -400.0f;

    const Vector3 TITLE_NAME_POS = { SPRITE_OFFSET,230.0f,0.0f };

    const Vector3 STAR_POS = { SPRITE_OFFSET + 170.0f ,134.0f,0.0f};

    const Vector3 PUSH_A_POS = { SPRITE_OFFSET,-340.0f,0.0f };

    const Vector3 GO_TO_PLAY_POS = { SPRITE_OFFSET,-140.0f,0.0f };
    const Vector3 HOW_TO_PLAY_POS = { SPRITE_OFFSET,-230.0f,0.0f };
    const Vector3 GAME_END_POS = { SPRITE_OFFSET,-320.0f,0.0f };


    const Vector3 CURSOR_GO_TO_PLAY_SCALE = { 1.2f,1.0f,1.0f };
    const Vector3 CURSOR_HOW_TO_PLAY_SCALE = { 1.0f,1.0f,1.0f };
    const Vector3 CURSOR_GAME_END_SCALE = { 2.0f,1.0f,1.0f };

    const Vector3 DICIDE_SCALE = { 1.2f,1.2f,1.0f };

    const Vector3 DEFEULT_SPRITE_SCALE = { 0.7f,0.7f,1.0f };

    const float STAR_MAX_SCALE = 2.0f;

    const float DEFAULT_TITLE_BGM = 2.5f;



    const Vector3 DEFAULT_TO_CAMERAPOS = { 0.0f, -210.0f, -900.0f };
    const Vector3 TARGET_CAMERA_POS = { 0.0f,190.0f,0.0f };


    const Vector3 DEFAULT_AMBIENT = { 3.0f,3.0f,3.0f };
    const Vector3 DEFAULT_DIRECTION_LIGHT_COLOR = { 0.5f,0.5f,0.5f };
    const Vector3 DEFAULT_DIRECTION_LIGHT_DIRECTION = { 0.57f,-0.57f,-0.57f };


}

Title::Title()
{
}

Title::~Title()
{
    DeleteGO(m_skyCube);

    g_soundManager->StopSound(enSoundName_TitleBGM);
}

bool Title::Start()
{
    //被写界深度の有効化
    g_renderingEngine->EnableDof();

    //画像の初期化
    InitSprite();
    //モデルの初期化
    InitModel();
    //スカイキューブの初期化
    InitSkyCube();
    //カメラの初期化
    InitTitleCamera();

    m_fade = FindGO<Fade>("fade");

    g_renderingEngine->SetAmbient(DEFAULT_AMBIENT);
    g_renderingEngine->SetDirLightColor(DEFAULT_DIRECTION_LIGHT_COLOR);
    g_renderingEngine->SetDirLightDirection(DEFAULT_DIRECTION_LIGHT_DIRECTION);


    //被写界深度の無効化
    //g_renderingEngine->DisableDof();

    return true;
}

void Title::Update()
{
    StepManage();

    CalcAlphaAButtonText();

    
    m_braveModel.Update();
}

void Title::StepManage()
{
    switch (m_step)
    {
    case Title::enStep1_TitleTextAlpha:
        //タイトルの名前を徐々に不透明にする
        CalcAlpha();
        break;
    case Title::enStep2_ShineStar:
        //タイトルの☆を表示
        ShineStar();
        break;
    case Title::enStep_End:
        //ゲームをはじめることができる
        SelectMode();
        break;
    default:
        break;
    }
}

void Title::SelectMode()
{
    //「Aボタンを押す」を達成していないなら処理しない
    if (IsTriggerPushAButton() != true)
    {
        return;
    }

    //カーソルの移動
    MoveCursor();

    //選ばれているモードによってカーソルの位置を変更
    switch (m_selectCursor)
    {
        //ゲームを始める
    case enMode_GoToPlay:
        m_cursorRender.SetPosition(GO_TO_PLAY_POS);
        m_cursorRender.SetScale(CURSOR_GO_TO_PLAY_SCALE);
        GoToPlayMode();
        break;
        //遊び方
    case enMode_HowToPlay:
        m_cursorRender.SetPosition(HOW_TO_PLAY_POS);
        m_cursorRender.SetScale(CURSOR_HOW_TO_PLAY_SCALE);
        HowToPlayMode();
        break;
        //ゲームを終わる
    case enMode_GameEnd:
        m_cursorRender.SetPosition(GAME_END_POS);
        m_cursorRender.SetScale(CURSOR_GAME_END_SCALE);
        GameEndMode();
        break;
    default:
        break;
    }
    //選択されているテキストを大きくする
    SetScaleModeText();

    m_goToPlayTextRender.SetScale(m_gTPScale);
    m_howToPlayTextRender.SetScale(m_hTPScale);
    m_gameEndTextRender.SetScale(m_gameEndScale);

    m_goToPlayTextRender.Update();
    m_howToPlayTextRender.Update();
    m_gameEndTextRender.Update();

    m_cursorRender.Update();
}

void Title::CalcAlpha()
{
    if (m_calcAlphaEndFlag == true)
    {
        return;
    }

    if (m_alphaTimer > 1.0f)
    {
        //次のステップに進む
        m_step = enStep2_ShineStar;
        m_calcAlphaEndFlag = true;
    }
    else
    {
        m_alphaTimer += g_gameTime->GetFrameDeltaTime() * 0.8f;
    }

    m_color.w = m_alphaTimer;

    m_titleNameRender.SetMulColor(m_color);
    m_titleNameRender.Update();
}

void Title::CalcAlphaAButtonText()
{
    //チカチカさせる

    if (m_Flag == false)
    {
        m_alphaAButtonTimer += g_gameTime->GetFrameDeltaTime();
    }
    if (m_Flag == true)
    {
        m_alphaAButtonTimer -= g_gameTime->GetFrameDeltaTime();
    }

    if (m_alphaAButtonTimer > 1.0f)
    {
        m_Flag = !m_Flag;
    }
    if (m_alphaAButtonTimer < 0.0f)
    {
        m_Flag = !m_Flag;;
    }

    m_AButtonColor.w = m_alphaAButtonTimer;
    m_pushAButtonTextRender.SetMulColor(m_AButtonColor);
    m_pushAButtonTextRender.Update();
}

void Title::MoveCursor()
{
    //何かしらモードが決まっていたら処理しない
    if (m_SelectModeFlag == true)
    {
        return;
    }

    if (g_pad[0]->IsTrigger(enButtonDown))
    {
        g_soundManager->InitAndPlaySoundSource(enSoundName_Select,g_soundManager->GetSEVolume());
        m_selectCursor++;
        //一番下に移動したら
        if (m_selectCursor > enMode_GameEnd)
        {
            //一番上に戻る
            m_selectCursor = enMode_GoToPlay;
        }
        return;
    }
    else if (g_pad[0]->IsTrigger(enButtonUp))
    {
        g_soundManager->InitAndPlaySoundSource(enSoundName_Select, g_soundManager->GetSEVolume());
        m_selectCursor--;
        //一番上に移動したら
        if (m_selectCursor < enMode_GoToPlay)
        {
            //一番下に戻る
            m_selectCursor = enMode_GameEnd;
        }
        return;
    }
}

void Title::CalcMuteBGMVolume()
{
    //ゲームに遷移するフラグがセットされていないなら処理しない
    if (m_goToGameFlag != true)
    {
        return;
    }
    //ミュートできているなら処理しない
    if (m_muteBGMFlag == true)
    {
        return;
    }

    if (m_bgmVolume > 0.0f)
    {
        m_bgmVolume = Math::Lerp(
            g_gameTime->GetFrameDeltaTime() * 5.0f, m_bgmVolume, -0.1f);
    }
    else
    {
        m_muteBGMFlag = true;
        return;
    }

    //BGMを小さくしていく
    g_soundManager->GetSoundSource(
        enSoundName_TitleBGM)->SetVolume(m_bgmVolume);
}

void Title::GoToPlayMode()
{
    //
    if (m_goToGameFlag==true)
    {
        //バトルに入ることが決まったらBGMを徐々にミュートにする
        CalcMuteBGMVolume();

        //フェードが終わったらゲーム生成。自身を削除
        if (m_fade->GetCurrentAlpha() >= 1.0f)
        {
            Game* game = NewGO<Game>(0, "game");
            DeleteGO(this);
        }
        //勇者モデルのアニメーションが終わったら
        if (m_braveModel.IsPlayingAnimation() == false&&
            m_fade->IsFade()==false)
        {
            //フェード開始
            m_fade->StartFadeIn(2.0f);

            m_blackOutRender.SetWipeWithDirection(true);
        }

        return;
    }

    

    //ゲームに移るの決定
    if (g_pad[0]->IsTrigger(enButtonA))
    {
        m_goToGameFlag = true;
        //BGMボリュームを取得
        m_bgmVolume = g_soundManager->GetBGMVolume();
        //決定音再生
        g_soundManager->InitAndPlaySoundSource(enSoundName_Decision);
        //崖から飛び降りるアニメーション再生
        m_braveModel.PlayAnimation(enAnimClip_DashJamp);
        //画像を表示しないようにする
        m_drawSpriteFlag = false;
    }
}

void Title::HowToPlayMode()
{
    if (g_pad[0]->IsTrigger(enButtonA))
    {
        g_soundManager->InitAndPlaySoundSource(enSoundName_Decision, g_soundManager->GetSEVolume());
        //モード決定
        m_SelectModeFlag = !m_SelectModeFlag;
        //遊び方の表示
        m_drawHowToPlayFlag = !m_drawHowToPlayFlag;
    }
}

void Title::GameEndMode()
{
    if (g_pad[0]->IsTrigger(enButtonA))
    {
        //モード決定
        m_SelectModeFlag = true;
        //ゲームの終了
        g_gameLoop.m_isLoop = false;
    }
}

void Title::ShineStar()
{
    //☆を大きくしながら回転させる
    switch (m_starScaleStep)
    {
    case Title::enStarStep1_ScaleUp:
        m_mulScale = Math::Lerp(m_scaleTimer, 0.0f, STAR_MAX_SCALE);

        if (m_scaleTimer > 1.0f)
        {
            //次のステップに移る
            m_starScaleStep = enStarStep2_ScaleDown;
            m_scaleTimer = 0.0f;
            //タイマーに掛ける値を変える
            m_mulTimerValue = 16.0f;
            //キラーン音再生
            g_soundManager->InitAndPlaySoundSource(enSoundName_StarShine,g_soundManager->GetSEVolume());
        }

        break;
    case Title::enStarStep2_ScaleDown:
        //最大まで大きくなったら元のサイズにする
        m_mulScale = Math::Lerp(m_scaleTimer, STAR_MAX_SCALE, 1.0f);

        if (m_scaleTimer > 1.0f)
        {
            //次のステップに移る
            m_starScaleStep = enStarStep_End;
        }
        break;
    case Title::enStarStep_End:
        //全体のステップ終わり
        m_step = enStep_End;
        //BGM再生
        g_soundManager->InitAndPlaySoundSource(enSoundName_TitleBGM, g_soundManager->GetBGMVolume(), false, true, true);
        break;
    default:
        break;
    }
    m_scaleTimer += g_gameTime->GetFrameDeltaTime()* m_mulTimerValue;

    Vector3 starScale = g_vec3One;
    starScale *= m_mulScale;

    m_starRotation.AddRotationDegZ(-20.0f);

    m_starRender.SetRotation(m_starRotation);
    m_starRender.SetScale(starScale);
    m_starRender.Update();
}

void Title::SetScaleModeText()
{
    if (m_selectCursor == enMode_GoToPlay)
    {
        m_gTPScale = DICIDE_SCALE;
    }
    else
    {
        m_gTPScale = g_vec3One;
    }
    if (m_selectCursor == enMode_HowToPlay)
    {
        m_hTPScale = DICIDE_SCALE;
    }
    else
    {
        m_hTPScale = g_vec3One;
    }
    if (m_selectCursor == enMode_GameEnd)
    {
        m_gameEndScale = DICIDE_SCALE;
    }
    else
    {
        m_gameEndScale = g_vec3One;
    }
}

void Title::InitModel()
{
    //レベルで配置
    m_titleLevel.Init(
        "Assets/level3D/TitleLevel.tkl",
        [&](LevelObjectData& objData)
        {
            if (objData.EqualObjectName(L"mountain") == true) {
                //山モデルの初期化
                m_mountainModel.Init("Assets/modelData/Title/mountain.tkm",
                    L"Assets/shader/ToonTextrue/lamp_glay.DDS",
                    0,
                    0,
                    enModelUpAxisZ);
                //トランスフォームの設定と更新
                m_mountainModel.SetTransform(
                    objData.position,
                    objData.rotation,
                    objData.scale
                );
                m_mountainModel.Update();
            }

            if (objData.EqualObjectName(L"Cliff") == true) {
                //崖モデルの初期化
                m_cliffModel.Init("Assets/modelData/Title/Cliff.tkm",
                    L"Assets/shader/ToonTextrue/lamp_glay.DDS",
                    0,
                    0,
                    enModelUpAxisZ);
                //トランスフォームの設定と更新
                m_cliffModel.SetTransform(
                    objData.position,
                    objData.rotation,
                    objData.scale
                );
                m_cliffModel.Update();

            }

            if (objData.EqualObjectName(L"brave") == true) {
                //アニメーションのロード
                m_animationClip[enAnimClip_Idle].Load(
                    "Assets/animData/character/Player/Title/Idle.tka"
                );
                m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
                m_animationClip[enAnimClip_DashJamp].Load(
                    "Assets/animData/character/Player/Title/DashJamp.tka"
                );
                m_animationClip[enAnimClip_DashJamp].SetLoopFlag(false);

                //勇者モデルの初期化
                m_braveModel.Init("Assets/modelData/Title/brave.tkm",
                    L"Assets/shader/ToonTextrue/lamp_glay.DDS",
                    m_animationClip,
                    enAnimClip_Num,
                    enModelUpAxisZ);
                //トランスフォームの設定と更新
                m_braveModel.SetTransform(
                    objData.position,
                    objData.rotation,
                    g_vec3One*2.0f
                );
                m_braveModel.Update();
            }

            return false;
        });
}

void Title::InitSkyCube()
{
    m_skyCube = NewGO<SkyCube>(0, "skycube");
    m_skyCube->SetScale(100.0f);
    m_skyCube->SetPosition(g_vec3Zero);
    m_skyCube->SetLuminance(1.01f);
    m_skyCube->SetType(enSkyCubeType_DayToon_3);
    m_skyCube->Update();
}

void Title::InitTitleCamera()
{
    //注視点から視点までのベクトルを設定。300,400
    m_toCameraPos.Set(DEFAULT_TO_CAMERAPOS);

    g_camera3D->SetNear(1.0f);
    g_camera3D->SetFar(5000.0f);

    //ばねカメラの初期化。
    m_springCamera.Init(
        *g_camera3D,		//ばねカメラの処理を行うカメラを指定する。
        1000.0f,			//カメラの移動速度の最大値。
        false,				//カメラと地形とのあたり判定を取るかどうかのフラグ。trueだとあたり判定を行う。
        1.0f				//カメラに設定される球体コリジョンの半径。第３引数がtrueの時に有効になる。
    );

    m_target = TARGET_CAMERA_POS;
    Vector3 finalCameraPos = m_toCameraPos + m_target;

    //視点と注視点を設定
    m_springCamera.SetTarget(m_target);
    m_springCamera.SetPosition(finalCameraPos);

    //カメラの更新。
    m_springCamera.Update();
}

bool Title::IsTriggerPushAButton()
{
    //「Aボタンを押す」が押されていたら処理しない
    if (m_pushAButtonFlag == true)
    {
        //押されているのでtrue
        return true;
    }

    if (g_pad[0]->IsTrigger(enButtonA))
    {
        //決定音の再生
        g_soundManager->InitAndPlaySoundSource(enSoundName_Decision);
        m_pushAButtonFlag = true;
    }
    
    //押されていないのでfalse
    return false;
}

bool Title::IsDirectionWipeEnd()
{


    return false;
}

void Title::ProcessDirectionWipe()
{
}

void Title::InitSprite()
{
    m_titleNameRender.Init("Assets/sprite/InGame/Title/TitleName.DDS", 1490, 420);
    SettingSpriteRender(m_titleNameRender, TITLE_NAME_POS, DEFEULT_SPRITE_SCALE, g_quatIdentity);

    m_backRender.Init("Assets/sprite/InGame/Title/Title_BackGround2.DDS", 1920, 1080);
    SettingSpriteRender(m_backRender, g_vec3Zero, g_vec3One, g_quatIdentity);

    m_starRender.Init("Assets/sprite/InGame/Title/Star.DDS", 55, 55);
    SettingSpriteRender(m_starRender, STAR_POS, g_vec3Zero, g_quatIdentity);

    m_pushAButtonTextRender.Init("Assets/sprite/InGame/Title/PushAButtonText.DDS", 540, 78);
    SettingSpriteRender(m_pushAButtonTextRender, PUSH_A_POS, g_vec3One, g_quatIdentity);

    m_goToPlayTextRender.Init("Assets/sprite/InGame/Title/GoToGameText.DDS", 428, 64);
    SettingSpriteRender(m_goToPlayTextRender, GO_TO_PLAY_POS, m_gTPScale, g_quatIdentity);

    m_howToPlayTextRender.Init("Assets/sprite/InGame/Title/HowToPlayText.DDS", 428, 64);
    SettingSpriteRender(m_howToPlayTextRender, HOW_TO_PLAY_POS, m_hTPScale, g_quatIdentity);

    m_gameEndTextRender.Init("Assets/sprite/InGame/Title/GameEndText.DDS", 428, 64);
    SettingSpriteRender(m_gameEndTextRender, GAME_END_POS, m_gameEndScale, g_quatIdentity);

    m_cursorRender.Init("Assets/sprite/InGame/Title/Cursor2.DDS", 310, 80);
    SettingSpriteRender(m_cursorRender, GO_TO_PLAY_POS, g_vec3One, g_quatIdentity);

    m_howToPlayRender.Init("Assets/sprite/InGame/Title/action.DDS", 1920, 1080);
    SettingSpriteRender(m_howToPlayRender, g_vec3Zero, g_vec3One, g_quatIdentity);

 
    m_blackOutRender.Init("Assets/sprite/Fade/Fade_Black.DDS", 1920.0f, 1080.0f);
    SettingSpriteRender(m_blackOutRender, g_vec3Zero, g_vec3One, g_quatIdentity);
    m_blackOutRender.SetDirection(0.0f, -1.0f);
    m_blackOutRender.SetWipeSize(1080.0f);
    m_blackOutRender.SetWipeWithDirection(true);
    m_blackOutRender.SetRoundWipe(true);

}

void Title::Render(RenderContext& rc)
{
   // m_blackOutRender.Draw(rc);

    if (m_drawHowToPlayFlag == true)
    {
        m_howToPlayRender.Draw(rc);
        return;
    }

    m_mountainModel.Draw(rc);
    m_cliffModel.Draw(rc);
    m_braveModel.Draw(rc);


    if (m_drawSpriteFlag == false)
    {
        return;
    }

    m_titleNameRender.Draw(rc);
    m_starRender.Draw(rc);

    if (m_pushAButtonFlag != true)
    {
        m_pushAButtonTextRender.Draw(rc);
        return;
    }

    m_cursorRender.Draw(rc);
    m_goToPlayTextRender.Draw(rc);
    m_howToPlayTextRender.Draw(rc);

    m_gameEndTextRender.Draw(rc);
}

