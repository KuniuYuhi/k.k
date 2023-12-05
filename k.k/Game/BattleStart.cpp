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
    //フェードのインスタンスを探す
    m_fade = FindGO<Fade>("fade");

    //ばねカメラの初期化。
    m_springCamera.Init(
        *g_camera3D,		//ばねカメラの処理を行うカメラを指定する。
        1000.0f,			//カメラの移動速度の最大値。
        false,				//カメラと地形とのあたり判定を取るかどうかのフラグ。trueだとあたり判定を行う。
        1.0f				//カメラに設定される球体コリジョンの半径。第３引数がtrueの時に有効になる。
    );

    InitSprite();
    //バトルスタートの音再生
    g_soundManager->InitAndPlaySoundSource(enSoundName_BattleStart, g_soundManager->GetBGMVolume());
    return true;
}

void BattleStart::Update()
{
    if (m_fadeStartFlag != true)
    {
        CalcCameraZoomOutTime();
    }
    
    //ゲームスタート時のカメラ処理
    GameStartCamera();

    //見えるようになったらもうやらない
    if (m_calcAlphaEndFlag != true)
    {
        CalcAlpha();
    }
    else
    {
        //BATTLEの移動
        MoveText(m_battleTextRender, BATTLE_TEXT_START_POS, BATTLE_TEXT_END_POS);
        //STARTの移動
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
        //不透明になった
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
    //注視点の計算
    m_target =
        CharactersInfoManager::GetInstance()->GetPlayerInstance()->GetPosition();
    m_target.y += TARGETPOS_GAMESTART_YUP;
    //前方向の取得
    Vector3 CameraPosXZ = 
        CharactersInfoManager::GetInstance()->GetPlayerInstance()->GetForward();
    CameraPosXZ.y = 0.0f;

    //XZ方向の
    CameraPosXZ *= 100.0f + m_count;
    //Y方向の
    Vector3 CameraPosY = Vector3::AxisY;
    CameraPosY *= STARTCAMERA_YUP + (m_count * 0.5f);

    //カメラの座標
    Vector3 newCameraPos = CameraPosXZ + CameraPosY;

    Vector3 finalCameraPos = newCameraPos + m_target;

    //視点と注視点を設定
    m_springCamera.SetTarget(m_target);
    m_springCamera.SetPosition(finalCameraPos);

    //カメラの更新。
    m_springCamera.Update();

    m_count++;
}

bool BattleStart::CalcCameraZoomOutTime()
{
    if (m_cameraZoomOutTimer > m_cameraZoomOutTime)
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
        m_cameraZoomOutTimer += g_gameTime->GetFrameDeltaTime();
    }

   

    return false;
}
