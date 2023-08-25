#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "Fade.h"

namespace {
    const Vector3 TITLE_NAME_POS = { 0.0f,230.0f,0.0f };

    const Vector3 STAR_POS = { 249.0f,134.0f,0.0f };

    const Vector3 PUSH_A_POS = { 0.0f,-340.0f,0.0f };
    const Vector3 GO_TO_PLAY_POS = { 0.0f,-140.0f,0.0f };
    const Vector3 HOW_TO_PLAY_POS = { 0.0f,-215.0f,0.0f };
    const Vector3 ACTION_POS = { 0.0f,-290.0f,0.0f };
    const Vector3 GAME_END_POS = { 0.0f,-365.0f,0.0f };



    const float STAR_MAX_SCALE = 2.0f;

}

Title::Title()
{
}

Title::~Title()
{
}

bool Title::Start()
{
    InitSprite();
  
    m_fade = FindGO<Fade>("fade");

    return true;
}

void Title::Update()
{
    StepManage();

    //Aボタンを押したらテキストが変わる
    if (m_pushAButtonFlag == false)
    {
        CalcAlphaAButtonText();

        if (g_pad[0]->IsTrigger(enButtonA))
        {
            m_pushAButtonFlag = true;
        }

    }
}

void Title::StepManage()
{
    switch (m_step)
    {
    case Title::enStep1_TitleTextAlpha:
        CalcAlpha();
        break;
    case Title::enStep2_ShineStar:
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
    if (m_pushAButtonFlag != true)
    {
        return;
    }

    //フェードが終わったら消す
    if (m_fade->GetCurrentAlpha() >= 1.0f)
    {
        Game* game = NewGO<Game>(0, "game");
        DeleteGO(this);
    }


    if (g_pad[0]->IsTrigger(enButtonA))
    {
        //フェード開始
        m_fade->StartFadeIn(2.0f);
    }
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

void Title::InitSprite()
{
    m_titleNameRender.Init("Assets/sprite/InGame/Title/TitleName.DDS", 1490, 420);
    SettingSpriteRender(m_titleNameRender, TITLE_NAME_POS, g_vec3One, g_quatIdentity);

    m_backRender.Init("Assets/sprite/InGame/Title/Title_BackGround2.DDS", 1920, 1080);
    SettingSpriteRender(m_backRender, g_vec3Zero, g_vec3One, g_quatIdentity);

    m_starRender.Init("Assets/sprite/InGame/Title/Star.DDS", 55, 55);
    SettingSpriteRender(m_starRender, STAR_POS, g_vec3Zero, g_quatIdentity);

    m_pushAButtonTextRender.Init("Assets/sprite/InGame/Title/PushAButtonText.DDS", 540, 78);
    SettingSpriteRender(m_pushAButtonTextRender, PUSH_A_POS, g_vec3One, g_quatIdentity);

    m_goToPlayTextRender.Init("Assets/sprite/InGame/Title/GoToGameText.DDS", 428, 64);
    SettingSpriteRender(m_goToPlayTextRender, GO_TO_PLAY_POS, g_vec3One, g_quatIdentity);

    m_howToPlayTextRender.Init("Assets/sprite/InGame/Title/HowToPlayText.DDS", 428, 64);
    SettingSpriteRender(m_howToPlayTextRender, HOW_TO_PLAY_POS, g_vec3One, g_quatIdentity);

    m_actionTextRender.Init("Assets/sprite/InGame/Title/actionText.DDS", 428, 64);
    SettingSpriteRender(m_actionTextRender, ACTION_POS, g_vec3One, g_quatIdentity);

    m_gameEndTextRender.Init("Assets/sprite/InGame/Title/GameEndText.DDS", 428, 64);
    SettingSpriteRender(m_gameEndTextRender, GAME_END_POS, g_vec3One, g_quatIdentity);


}

void Title::Render(RenderContext& rc)
{
    m_backRender.Draw(rc);
    m_titleNameRender.Draw(rc);
    m_starRender.Draw(rc);

    if (m_pushAButtonFlag != true)
    {
        m_pushAButtonTextRender.Draw(rc);
        return;
    }


    m_goToPlayTextRender.Draw(rc);
    m_howToPlayTextRender.Draw(rc);
    m_actionTextRender.Draw(rc);
    m_gameEndTextRender.Draw(rc);

    

}

