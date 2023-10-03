#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "Fade.h"
#include "system/system.h"

namespace {
    const Vector3 TITLE_NAME_POS = { 0.0f,230.0f,0.0f };

    const Vector3 STAR_POS = { 249.0f,134.0f,0.0f };

    const Vector3 PUSH_A_POS = { 0.0f,-340.0f,0.0f };
    const Vector3 GO_TO_PLAY_POS = { 0.0f,-140.0f,0.0f };
    const Vector3 HOW_TO_PLAY_POS = { 0.0f,-230.0f,0.0f };
    const Vector3 ACTION_POS = { 0.0f,-320.0f,0.0f };
    const Vector3 GAME_END_POS = { 0.0f,-410.0f,0.0f };


    const Vector3 CURSOR_GO_TO_PLAY_POS = { -200.0f,-140.0f,0.0f };
    const Vector3 CURSOR_HOW_TO_PLAY_POS = { -200.0f,-230.0f,0.0f };
    const Vector3 CURSOR_ACTION_POS = { -200.0f,-320.0f,0.0f };
    const Vector3 CURSOR_GAME_END_POS = { -300.0f,-410.0f,0.0f };

    const Vector3 DICIDE_SCALE = { 1.2f,1.2f,1.0f };


    const float STAR_MAX_SCALE = 2.0f;

}

Title::Title()
{
}

Title::~Title()
{
    g_soundManager->StopSound(enSoundName_TitleBGM);
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

    CalcAlphaAButtonText();
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
        //�Q�[�����͂��߂邱�Ƃ��ł���
        SelectMode();
        break;
    default:
        break;
    }
}

void Title::SelectMode()
{
    //A�{�^������������e�L�X�g���ς��
    if (m_pushAButtonFlag == false)
    {
        if (g_pad[0]->IsTrigger(enButtonA))
        {
            g_soundManager->InitAndPlaySoundSource(enSoundName_Decision);
            m_pushAButtonFlag = true;
            return;
        }

    }
    //A�{�^����������Ă��Ȃ��������̏��������Ȃ�
    if (m_pushAButtonFlag != true)
    {
        return;
    }

    //�J�[�\���̈ړ�
    MoveCursor();

    switch (m_selectCursor)
    {
    case enMode_GoToPlay:
        m_cursorRender.SetPosition(CURSOR_GO_TO_PLAY_POS);
        GoToPlayMode();
        break;
    case enMode_HowToPlay:
        m_cursorRender.SetPosition(CURSOR_HOW_TO_PLAY_POS);
        HowToPlayMode();
        break;
    case enMode_Action:
        m_cursorRender.SetPosition(CURSOR_ACTION_POS);
        ActionMode();
        break;
    case enMode_GameEnd:
        m_cursorRender.SetPosition(CURSOR_GAME_END_POS);
        GameEndMode();
        break;
    default:
        break;
    }

    SetScaleModeText();

    m_goToPlayTextRender.SetScale(m_gTPScale);
    m_howToPlayTextRender.SetScale(m_hTPScale);
    m_actionTextRender.SetScale(m_actionScale);
    m_gameEndTextRender.SetScale(m_gameEndScale);

    m_goToPlayTextRender.Update();
    m_howToPlayTextRender.Update();
    m_actionTextRender.Update();
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
        //���̃X�e�b�v�ɐi��
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

void Title::MoveCursor()
{
    //�������烂�[�h�����܂��Ă����珈�����Ȃ�
    if (m_SelectModeFlag == true)
    {
        return;
    }

    if (g_pad[0]->IsTrigger(enButtonDown))
    {
        g_soundManager->InitAndPlaySoundSource(enSoundName_Select);
        m_selectCursor++;
        //��ԉ��Ɉړ�������
        if (m_selectCursor > enMode_GameEnd)
        {
            //��ԏ�ɖ߂�
            m_selectCursor = enMode_GoToPlay;
        }
        return;
    }
    else if (g_pad[0]->IsTrigger(enButtonUp))
    {
        g_soundManager->InitAndPlaySoundSource(enSoundName_Select);
        m_selectCursor--;
        //��ԏ�Ɉړ�������
        if (m_selectCursor < enMode_GoToPlay)
        {
            //��ԉ��ɖ߂�
            m_selectCursor = enMode_GameEnd;
        }
        return;
    }
}

void Title::GoToPlayMode()
{
    //�t�F�[�h���I����������
    if (m_fade->GetCurrentAlpha() >= 1.0f)
    {
        Game* game = NewGO<Game>(0, "game");
        DeleteGO(this);
    }

    if (g_pad[0]->IsTrigger(enButtonA))
    {
        //g_soundManager->InitAndPlaySoundSource(enSoundName_Decision);
        //�t�F�[�h�J�n
        m_fade->StartFadeIn(2.0f);
    }
}

void Title::HowToPlayMode()
{
    if (g_pad[0]->IsTrigger(enButtonA))
    {
        g_soundManager->InitAndPlaySoundSource(enSoundName_Decision);
        //���[�h����
        m_SelectModeFlag = !m_SelectModeFlag;
        //�V�ѕ��̕\��
        m_drawHowToPlayFlag = !m_drawHowToPlayFlag;
    }
}

void Title::ActionMode()
{
    if (g_pad[0]->IsTrigger(enButtonA))
    {
        g_soundManager->InitAndPlaySoundSource(enSoundName_Decision);
        //���[�h����
        m_SelectModeFlag = !m_SelectModeFlag;
        //��������̕\��
        m_drawActionFlag = !m_drawActionFlag;
    }
}

void Title::GameEndMode()
{
    if (g_pad[0]->IsTrigger(enButtonA))
    {
        //���[�h����
        m_SelectModeFlag = true;
        //�Q�[���̏I��
        g_gameLoop.m_isLoop = false;
    }
}

void Title::ShineStar()
{
    //����傫�����Ȃ����]������
    switch (m_starScaleStep)
    {
    case Title::enStarStep1_ScaleUp:
        m_mulScale = Math::Lerp(m_scaleTimer, 0.0f, STAR_MAX_SCALE);

        if (m_scaleTimer > 1.0f)
        {
            //���̃X�e�b�v�Ɉڂ�
            m_starScaleStep = enStarStep2_ScaleDown;
            m_scaleTimer = 0.0f;
            //�^�C�}�[�Ɋ|����l��ς���
            m_mulTimerValue = 16.0f;
            //�L���[�����Đ�
            g_soundManager->InitAndPlaySoundSource(enSoundName_StarShine);
        }

        break;
    case Title::enStarStep2_ScaleDown:
        //�ő�܂ő傫���Ȃ����猳�̃T�C�Y�ɂ���
        m_mulScale = Math::Lerp(m_scaleTimer, STAR_MAX_SCALE, 1.0f);

        if (m_scaleTimer > 1.0f)
        {
            //���̃X�e�b�v�Ɉڂ�
            m_starScaleStep = enStarStep_End;
        }
        break;
    case Title::enStarStep_End:
        //�S�̂̃X�e�b�v�I���
        m_step = enStep_End;
        //BGM�Đ�
        g_soundManager->InitAndPlaySoundSource(enSoundName_TitleBGM,false,true);
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
    if (m_selectCursor == enMode_Action)
    {
        m_actionScale = DICIDE_SCALE;
    }
    else
    {
        m_actionScale = g_vec3One;
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
    SettingSpriteRender(m_goToPlayTextRender, GO_TO_PLAY_POS, m_gTPScale, g_quatIdentity);

    m_howToPlayTextRender.Init("Assets/sprite/InGame/Title/HowToPlayText.DDS", 428, 64);
    SettingSpriteRender(m_howToPlayTextRender, HOW_TO_PLAY_POS, m_hTPScale, g_quatIdentity);

    m_actionTextRender.Init("Assets/sprite/InGame/Title/actionText.DDS", 428, 64);
    SettingSpriteRender(m_actionTextRender, ACTION_POS, m_actionScale, g_quatIdentity);

    m_gameEndTextRender.Init("Assets/sprite/InGame/Title/GameEndText.DDS", 428, 64);
    SettingSpriteRender(m_gameEndTextRender, GAME_END_POS, m_gameEndScale, g_quatIdentity);

    m_cursorRender.Init("Assets/sprite/InGame/Title/Cursor.DDS", 76, 76);
    SettingSpriteRender(m_cursorRender, CURSOR_GO_TO_PLAY_POS, g_vec3One, g_quatIdentity);

    m_howToPlayRender.Init("Assets/sprite/InGame/Title/HowToPlay.DDS", 1920, 1080);
    SettingSpriteRender(m_howToPlayRender, g_vec3Zero, g_vec3One, g_quatIdentity);

    m_actionRender.Init("Assets/sprite/InGame/Title/action.DDS", 1920, 1080);
    SettingSpriteRender(m_actionRender, g_vec3Zero, g_vec3One, g_quatIdentity);
}

void Title::Render(RenderContext& rc)
{
    if (m_drawHowToPlayFlag == true)
    {
        m_howToPlayRender.Draw(rc);
        return;
    }
    if (m_drawActionFlag == true)
    {
        m_actionRender.Draw(rc);
        return;
    }

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

    m_cursorRender.Draw(rc);

}

