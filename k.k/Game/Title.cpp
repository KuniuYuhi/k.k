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
    //��ʊE�[�x�̗L����
    g_renderingEngine->EnableDof();

    //�摜�̏�����
    InitSprite();
    //���f���̏�����
    InitModel();
    //�X�J�C�L���[�u�̏�����
    InitSkyCube();
    //�J�����̏�����
    InitTitleCamera();

    m_fade = FindGO<Fade>("fade");

    g_renderingEngine->SetAmbient(DEFAULT_AMBIENT);
    g_renderingEngine->SetDirLightColor(DEFAULT_DIRECTION_LIGHT_COLOR);
    g_renderingEngine->SetDirLightDirection(DEFAULT_DIRECTION_LIGHT_DIRECTION);


    //��ʊE�[�x�̖�����
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
        //�^�C�g���̖��O�����X�ɕs�����ɂ���
        CalcAlpha();
        break;
    case Title::enStep2_ShineStar:
        //�^�C�g���́���\��
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
    //�uA�{�^���������v��B�����Ă��Ȃ��Ȃ珈�����Ȃ�
    if (IsTriggerPushAButton() != true)
    {
        return;
    }

    //�J�[�\���̈ړ�
    MoveCursor();

    //�I�΂�Ă��郂�[�h�ɂ���ăJ�[�\���̈ʒu��ύX
    switch (m_selectCursor)
    {
        //�Q�[�����n�߂�
    case enMode_GoToPlay:
        m_cursorRender.SetPosition(GO_TO_PLAY_POS);
        m_cursorRender.SetScale(CURSOR_GO_TO_PLAY_SCALE);
        GoToPlayMode();
        break;
        //�V�ѕ�
    case enMode_HowToPlay:
        m_cursorRender.SetPosition(HOW_TO_PLAY_POS);
        m_cursorRender.SetScale(CURSOR_HOW_TO_PLAY_SCALE);
        HowToPlayMode();
        break;
        //�Q�[�����I���
    case enMode_GameEnd:
        m_cursorRender.SetPosition(GAME_END_POS);
        m_cursorRender.SetScale(CURSOR_GAME_END_SCALE);
        GameEndMode();
        break;
    default:
        break;
    }
    //�I������Ă���e�L�X�g��傫������
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
    //�`�J�`�J������

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
        g_soundManager->InitAndPlaySoundSource(enSoundName_Select,g_soundManager->GetSEVolume());
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
        g_soundManager->InitAndPlaySoundSource(enSoundName_Select, g_soundManager->GetSEVolume());
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

void Title::CalcMuteBGMVolume()
{
    //�Q�[���ɑJ�ڂ���t���O���Z�b�g����Ă��Ȃ��Ȃ珈�����Ȃ�
    if (m_goToGameFlag != true)
    {
        return;
    }
    //�~���[�g�ł��Ă���Ȃ珈�����Ȃ�
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

    //BGM�����������Ă���
    g_soundManager->GetSoundSource(
        enSoundName_TitleBGM)->SetVolume(m_bgmVolume);
}

void Title::GoToPlayMode()
{
    //
    if (m_goToGameFlag==true)
    {
        //�o�g���ɓ��邱�Ƃ����܂�����BGM�����X�Ƀ~���[�g�ɂ���
        CalcMuteBGMVolume();

        //�t�F�[�h���I�������Q�[�������B���g���폜
        if (m_fade->GetCurrentAlpha() >= 1.0f)
        {
            Game* game = NewGO<Game>(0, "game");
            DeleteGO(this);
        }
        //�E�҃��f���̃A�j���[�V�������I�������
        if (m_braveModel.IsPlayingAnimation() == false&&
            m_fade->IsFade()==false)
        {
            //�t�F�[�h�J�n
            m_fade->StartFadeIn(2.0f);

            m_blackOutRender.SetWipeWithDirection(true);
        }

        return;
    }

    

    //�Q�[���Ɉڂ�̌���
    if (g_pad[0]->IsTrigger(enButtonA))
    {
        m_goToGameFlag = true;
        //BGM�{�����[�����擾
        m_bgmVolume = g_soundManager->GetBGMVolume();
        //���艹�Đ�
        g_soundManager->InitAndPlaySoundSource(enSoundName_Decision);
        //�R�����э~���A�j���[�V�����Đ�
        m_braveModel.PlayAnimation(enAnimClip_DashJamp);
        //�摜��\�����Ȃ��悤�ɂ���
        m_drawSpriteFlag = false;
    }
}

void Title::HowToPlayMode()
{
    if (g_pad[0]->IsTrigger(enButtonA))
    {
        g_soundManager->InitAndPlaySoundSource(enSoundName_Decision, g_soundManager->GetSEVolume());
        //���[�h����
        m_SelectModeFlag = !m_SelectModeFlag;
        //�V�ѕ��̕\��
        m_drawHowToPlayFlag = !m_drawHowToPlayFlag;
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
            g_soundManager->InitAndPlaySoundSource(enSoundName_StarShine,g_soundManager->GetSEVolume());
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
    //���x���Ŕz�u
    m_titleLevel.Init(
        "Assets/level3D/TitleLevel.tkl",
        [&](LevelObjectData& objData)
        {
            if (objData.EqualObjectName(L"mountain") == true) {
                //�R���f���̏�����
                m_mountainModel.Init("Assets/modelData/Title/mountain.tkm",
                    L"Assets/shader/ToonTextrue/lamp_glay.DDS",
                    0,
                    0,
                    enModelUpAxisZ);
                //�g�����X�t�H�[���̐ݒ�ƍX�V
                m_mountainModel.SetTransform(
                    objData.position,
                    objData.rotation,
                    objData.scale
                );
                m_mountainModel.Update();
            }

            if (objData.EqualObjectName(L"Cliff") == true) {
                //�R���f���̏�����
                m_cliffModel.Init("Assets/modelData/Title/Cliff.tkm",
                    L"Assets/shader/ToonTextrue/lamp_glay.DDS",
                    0,
                    0,
                    enModelUpAxisZ);
                //�g�����X�t�H�[���̐ݒ�ƍX�V
                m_cliffModel.SetTransform(
                    objData.position,
                    objData.rotation,
                    objData.scale
                );
                m_cliffModel.Update();

            }

            if (objData.EqualObjectName(L"brave") == true) {
                //�A�j���[�V�����̃��[�h
                m_animationClip[enAnimClip_Idle].Load(
                    "Assets/animData/character/Player/Title/Idle.tka"
                );
                m_animationClip[enAnimClip_Idle].SetLoopFlag(true);
                m_animationClip[enAnimClip_DashJamp].Load(
                    "Assets/animData/character/Player/Title/DashJamp.tka"
                );
                m_animationClip[enAnimClip_DashJamp].SetLoopFlag(false);

                //�E�҃��f���̏�����
                m_braveModel.Init("Assets/modelData/Title/brave.tkm",
                    L"Assets/shader/ToonTextrue/lamp_glay.DDS",
                    m_animationClip,
                    enAnimClip_Num,
                    enModelUpAxisZ);
                //�g�����X�t�H�[���̐ݒ�ƍX�V
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
    //�����_���王�_�܂ł̃x�N�g����ݒ�B300,400
    m_toCameraPos.Set(DEFAULT_TO_CAMERAPOS);

    g_camera3D->SetNear(1.0f);
    g_camera3D->SetFar(5000.0f);

    //�΂˃J�����̏������B
    m_springCamera.Init(
        *g_camera3D,		//�΂˃J�����̏������s���J�������w�肷��B
        1000.0f,			//�J�����̈ړ����x�̍ő�l�B
        false,				//�J�����ƒn�`�Ƃ̂����蔻�����邩�ǂ����̃t���O�Btrue���Ƃ����蔻����s���B
        1.0f				//�J�����ɐݒ肳��鋅�̃R���W�����̔��a�B��R������true�̎��ɗL���ɂȂ�B
    );

    m_target = TARGET_CAMERA_POS;
    Vector3 finalCameraPos = m_toCameraPos + m_target;

    //���_�ƒ����_��ݒ�
    m_springCamera.SetTarget(m_target);
    m_springCamera.SetPosition(finalCameraPos);

    //�J�����̍X�V�B
    m_springCamera.Update();
}

bool Title::IsTriggerPushAButton()
{
    //�uA�{�^���������v��������Ă����珈�����Ȃ�
    if (m_pushAButtonFlag == true)
    {
        //������Ă���̂�true
        return true;
    }

    if (g_pad[0]->IsTrigger(enButtonA))
    {
        //���艹�̍Đ�
        g_soundManager->InitAndPlaySoundSource(enSoundName_Decision);
        m_pushAButtonFlag = true;
    }
    
    //������Ă��Ȃ��̂�false
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

