#include "stdafx.h"
#include "BattleStart.h"
#include "Fade.h"
#include "CharactersInfoManager.h"

namespace {
    const Vector3 BATTLE_TEXT_START_POS = { 0.0f,336.0f,0.0f };
    const Vector3 BATTLE_TEXT_END_POS = { 1650.0f,336.0f,0.0f };
 
    const Vector3 START_TEXT_START_POS = { 0.0f,-336.0f,0.0f };
    const Vector3 START_TEXT_END_POS = { -1650.0f,-336.0f,0.0f };


    const float TARGETPOS_GAMESTART_YUP = 38.0f;
    const float STARTCAMERA_YUP = 60.0f;
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

    //�΂˃J�����̏������B
    m_springCamera.Init(
        *g_camera3D,		//�΂˃J�����̏������s���J�������w�肷��B
        1000.0f,			//�J�����̈ړ����x�̍ő�l�B
        false,				//�J�����ƒn�`�Ƃ̂����蔻�����邩�ǂ����̃t���O�Btrue���Ƃ����蔻����s���B
        1.0f				//�J�����ɐݒ肳��鋅�̃R���W�����̔��a�B��R������true�̎��ɗL���ɂȂ�B
    );

    InitSprite();
    //�o�g���X�^�[�g�̉��Đ�
    g_soundManager->InitAndPlaySoundSource(enSoundName_BattleStart, g_soundManager->GetBGMVolume());
    return true;
}

void BattleStart::Update()
{
    if (m_fadeStartFlag != true)
    {
        CalcCameraZoomOutTime();
    }
    
    //�Q�[���X�^�[�g���̃J��������
    GameStartCamera();

    //������悤�ɂȂ�����������Ȃ�
    if (m_calcAlphaEndFlag != true)
    {
        CalcAlpha();
    }
    else
    {
        //BATTLE�̈ړ�
        MoveText(m_battleTextRender, BATTLE_TEXT_START_POS, BATTLE_TEXT_END_POS);
        //START�̈ړ�
        MoveText(m_startTextRender, START_TEXT_START_POS, START_TEXT_END_POS);

        m_timer += g_gameTime->GetFrameDeltaTime() * 0.4f;
    }
}

void BattleStart::Render(RenderContext& rc)
{
    m_battleTextRender.Draw(rc);
    m_startTextRender.Draw(rc);
}

void BattleStart::MoveText(SpriteRender& spriteRender, Vector3 startPos, Vector3 endPos)
{
    Vector3 pos;
    pos.Lerp(m_timer, startPos, endPos);
    spriteRender.SetPosition(pos);
    spriteRender.Update();
}

void BattleStart::CalcAlpha()
{
    m_alpha += g_gameTime->GetFrameDeltaTime() * 0.7f;

    if (m_alpha > 1.0f)
    {
        //�s�����ɂȂ���
        m_calcAlphaEndFlag = true;
        m_alpha = 1.0f;
    }

    m_color.w = m_alpha;

    m_battleTextRender.SetMulColor(m_color);
    m_startTextRender.SetMulColor(m_color);

    m_battleTextRender.Update();
    m_startTextRender.Update();
}

void BattleStart::InitSprite()
{
    m_battleTextRender.Init("Assets/sprite/InGame/BattleStart/Battle.DDS", 1416, 265);
    m_battleTextRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.0f));
    SettingSpriteRender(m_battleTextRender, BATTLE_TEXT_START_POS, g_vec3One, g_quatIdentity);

    m_startTextRender.Init("Assets/sprite/InGame/BattleStart/Start.DDS", 1236, 265);
    SettingSpriteRender(m_startTextRender, START_TEXT_START_POS, g_vec3One, g_quatIdentity);

}

void BattleStart::GameStartCamera()
{
    //�����_�̌v�Z
    m_target =
        CharactersInfoManager::GetInstance()->GetPlayerInstance()->GetPosition();
    m_target.y += TARGETPOS_GAMESTART_YUP;
    //�O�����̎擾
    Vector3 CameraPosXZ = 
        CharactersInfoManager::GetInstance()->GetPlayerInstance()->GetForward();
    CameraPosXZ.y = 0.0f;

    //XZ������
    CameraPosXZ *= 100.0f + m_count;
    //Y������
    Vector3 CameraPosY = Vector3::AxisY;
    CameraPosY *= STARTCAMERA_YUP + (m_count * 0.5f);

    //�J�����̍��W
    Vector3 newCameraPos = CameraPosXZ + CameraPosY;

    Vector3 finalCameraPos = newCameraPos + m_target;

    //���_�ƒ����_��ݒ�
    m_springCamera.SetTarget(m_target);
    m_springCamera.SetPosition(finalCameraPos);

    //�J�����̍X�V�B
    m_springCamera.Update();

    m_count++;
}

bool BattleStart::CalcCameraZoomOutTime()
{
    if (m_cameraZoomOutTimer > m_cameraZoomOutTime)
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
        m_cameraZoomOutTimer += g_gameTime->GetFrameDeltaTime();
    }

   

    return false;
}
