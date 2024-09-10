#include "stdafx.h"
#include "GameUIBase.h"

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
