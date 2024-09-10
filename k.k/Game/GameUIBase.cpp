#include "stdafx.h"
#include "GameUIBase.h"

Vector3 GameUIBase::CalcGaugeRatio(float maxValue, float currentValue)
{
    Vector3 scale = g_vec3One;
    //�����Ă����������v�Z
    scale.x = currentValue / maxValue;
    return scale;
}

bool GameUIBase::IsTimerRunning(float timeLimit, float& timer)
{
    //�^�C�}�[���^�C�����~�b�g�ɒB������
    if (timer >= timeLimit)
    {
        //�^�C�}�[�͓����Ă��Ȃ�
        return false;
    }
    //�^�C�}�[���Z
    timer += g_gameTime->GetFrameDeltaTime();
    //�^�C�}�[�͓����Ă���
    return true;
}

void GameUIBase::InitSpriteRender(
    SpriteRender& spriteRender,
    const char* filePath, 
    const float width, const float height, 
    Vector2 position, Vector3 scale, Quaternion rotation
)
{
    //������
    spriteRender.Init(filePath, width, height);
    //���W�A�T�C�Y�A��]�̐ݒ�ƍX�V
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
