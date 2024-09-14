#include "stdafx.h"
#include "GameUIBase.h"

void GameUIBase::SetShakeInfo(int Strength, float Vibrato, Vector2 DefaultPosition)
{
    m_shakeInfo.SetShakeInfo(Strength, Vibrato, DefaultPosition);
}

Vector2 GameUIBase::GetUpdateShakePosition(
    float currentTimer, float TimeLimit, Vector2 currentPosition
)
{
    int max = m_shakeInfo.strength * 2 + 1;

    //ランダムな値を取得
    int ramdomX = rand() % max - m_shakeInfo.strength;
    int ramdomY = rand() % max - m_shakeInfo.strength;

    Vector2 shakePos = currentPosition;

    shakePos.x += ramdomX;
    shakePos.y += ramdomY;

    //揺れの強さの最大値
    float vibrato = m_shakeInfo.vibrato;
    //時間の割合を求める
    float ratio = 1.0f - currentTimer / TimeLimit;
    //割合によって、タイムリミットに近づくと揺れの強さが小さくなっていく
    vibrato *= ratio;

    shakePos.x =
        Clamp(shakePos.x, m_shakeInfo.defaultPosition.x - vibrato, m_shakeInfo.defaultPosition.x + vibrato);
    shakePos.y =
        Clamp(shakePos.y, m_shakeInfo.defaultPosition.y - vibrato, m_shakeInfo.defaultPosition.y + vibrato);


    return shakePos;
}

float GameUIBase::Clamp(float value, float min, float max)
{
    if (value < min) return min;

    if (value > max) return max;

    return value;
}

Vector3 GameUIBase::CalcGaugeRatio(float maxValue, float currentValue)
{
    Vector3 scale = g_vec3One;
    //減っていく割合を計算
    scale.x = currentValue / maxValue;
    return scale;
}

bool GameUIBase::IsTimerRunning(float timeLimit, float& timer)
{
    //タイマーがタイムリミットに達したら
    if (timer >= timeLimit)
    {
        //タイマーは動いていない
        return false;
    }
    //タイマー加算
    timer += g_gameTime->GetFrameDeltaTime();
    //タイマーは動いている
    return true;
}

void GameUIBase::InitSpriteRender(
    SpriteRender& spriteRender,
    const char* filePath, 
    const float width, const float height, 
    Vector2 position, Vector3 scale, Quaternion rotation
)
{
    //初期化
    spriteRender.Init(filePath, width, height);
    //座標、サイズ、回転の設定と更新
    spriteRender.SetPosition(position);
    spriteRender.SetScale(scale);
    spriteRender.SetRotation(rotation);
    spriteRender.Update();
}

void GameUIBase::InitFontRender(
    FontRender& fontRender, 
    Vector2 position, float scale, Vector4 color,
    const wchar_t* text, 
    bool isShadowParam, float shadowOffset, Vector4 shadowColor
)
{
    fontRender.SetPosition(position);
    fontRender.SetScale(scale);
    fontRender.SetColor(color);
    fontRender.SetText(text);
    if (isShadowParam)
    {
        fontRender.SetShadowParam(
            isShadowParam, shadowOffset, shadowColor
        );
    }
}
