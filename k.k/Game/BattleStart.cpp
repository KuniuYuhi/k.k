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
    //�t�F�[�h�̃C���X�^���X��T��
    m_fade = FindGO<Fade>("fade");

    g_camera3D->SetNear(1.0f);
    g_camera3D->SetFar(10000.0f);

    //�΂˃J�����̏������B
    m_springCamera.Init(
        *g_camera3D,		//�΂˃J�����̏������s���J�������w�肷��B
        180.0f,			//�J�����̈ړ����x�̍ő�l�B
        false,				//�J�����ƒn�`�Ƃ̂����蔻�����邩�ǂ����̃t���O�Btrue���Ƃ����蔻����s���B
        1.0f				//�J�����ɐݒ肳��鋅�̃R���W�����̔��a�B��R������true�̎��ɗL���ɂȂ�B
    );

    m_addYPos = STARTCAMERA_YUP;
    m_mulXZPos = START_MULXZ;

    //�������C�g�̖�����
   // g_renderingEngine->Use

    //���f���̏�����
    InitModel();

    //�W�����v���Đ�
    g_soundManager->InitAndPlaySoundSource(
        enSoundName_Fall, g_soundManager->GetSEVolume()
    );

    return true;
}

void BattleStart::Update()
{
    //�Q�[���X�^�[�g���̃J��������
    GameStartCamera();

    //�E�҂̏�ԑJ�ڊǗ�
    ManageBraveState();
}

void BattleStart::Render(RenderContext& rc)
{
    m_braveModel.Draw(rc);

}

void BattleStart::GameStartCamera()
{
    //�����_�̌v�Z
    m_target = m_braveModel.GetPosition();
    m_target.y += m_addYPos;
    //�O�����̎擾
    Vector3 CameraPosXZ = g_vec3AxisZ;
    CameraPosXZ *= m_mulXZPos;
    //Y����
    Vector3 CameraPosY = Vector3::AxisY;
    CameraPosY *= STARTCAMERA_YUP;

    //�J�����̍��W
    Vector3 newCameraPos = CameraPosXZ + CameraPosY;

    Vector3 finalCameraPos = newCameraPos + m_target;
    finalCameraPos.y = STARTCAMERA_YUP;

    //���_�ƒ����_��ݒ�
    m_springCamera.SetTarget(m_target);
    m_springCamera.SetPosition(finalCameraPos);

    //�J�����̍X�V�B
    m_springCamera.Update();
}

bool BattleStart::IsFadeStart()
{
    if (m_fadeTimer > m_fadeTime)
    {
        //�t�F�[�h�C���J�n
        //�t�F�[�h�C���d�؂�����Q�[�������Q�[���X�^�[�g�N���X���폜����
        m_fade->StartFadeIn(3.0f);
        m_fadeStartFlag = true;
        return true;
    }
    else
    {
        //�^�C�}�[�����Z
        m_fadeTimer += g_gameTime->GetFrameDeltaTime();
    }

    return false;
}

void BattleStart::InitModel()
{
    m_bravePosition = BRAVE_START_POSITION;

    //�A�j���[�V�����̃��[�h
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

    //�E�҃��f���̏�����
    m_braveModel.Init("Assets/modelData/Title/brave.tkm",
        L"Assets/shader/ToonTextrue/lamp_glay.DDS",
        m_animationClip,
        enAnimClip_Num,
        enModelUpAxisZ);
    //�g�����X�t�H�[���̐ݒ�ƍX�V
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
    //�����Ă�������
    m_bravePosition.y -= g_gameTime->GetFrameDeltaTime() * FALL_SPEED;

    m_addYPos -= g_gameTime->GetFrameDeltaTime() * ADD_YPOS_SPEED;

    //�n�ʂɒ�������
    if (m_bravePosition.y < 0.0f)
    {
        m_bravePosition.y = 0.0f;
        //�J������Y���W����C�ɉ�����
        m_addYPos = LANDING_CAMERA_YUP;
        //�E�҂���J��������������
        m_mulXZPos = IDLE_MULXZ;

        //���n�G�t�F�N�g�Đ�
        PlayLandingEffect();
        //���n�̉��Đ�
        g_soundManager->InitAndPlaySoundSource(
            enSoundName_Landing,
            g_soundManager->GetBGMVolume()
        );

        //���̃X�e�[�g�ɐi��
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
    //�J�����̃^�[�Q�b�g��Y���W���ŏ��Ɠ����ɖ߂�
    m_addYPos = STARTCAMERA_YUP;

    //���n�A�j���[�V�����Đ�
    m_braveModel.PlayAnimation(enAnimClip_Landing,0.1f);

    if (m_braveModel.IsPlayingAnimation() == false)
    {
        //���̃X�e�[�g�ɐi��
        m_enBraveState = enBraveState_Idle;
    }
}

void BattleStart::ProcessIdleStateTransition()
{
    //�A�C�h���A�j���[�V�����Đ�
    m_braveModel.PlayAnimation(enAnimClip_Idle,0.3f);

    //���̃X�e�[�g�ɐi��
    m_enBraveState = enBraveState_End;
}

void BattleStart::ProcessEndStateTransition()
{
    //�������I������Ȃ牽�����Ȃ�
    if (m_isProcessEndFlag == true)
    {
        return;
    }

    //�t�F�[�h�X�^�[�g�t���O��true�łȂ��Ȃ珈������
    if (m_fadeStartFlag != true)
    {
        IsFadeStart();
    }
    else
    {
        if (!m_fade->IsFade())
        {
            //�t�F�[�h�C���d�؂�����Q�[���V�[���}�l�[�W���[�ɃV�[��
            //��؂�ւ��Ă��ǂ����Ƃ�`����
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
