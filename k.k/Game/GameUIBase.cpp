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

    //�����_���Ȓl���擾
    int ramdomX = rand() % max - m_shakeInfo.strength;
    int ramdomY = rand() % max - m_shakeInfo.strength;

    Vector2 shakePos = currentPosition;

    shakePos.x += ramdomX;
    shakePos.y += ramdomY;

    //�h��̋����̍ő�l
    float vibrato = m_shakeInfo.vibrato;
    //���Ԃ̊��������߂�
    float ratio = 1.0f - currentTimer / TimeLimit;
    //�����ɂ���āA�^�C�����~�b�g�ɋ߂Â��Ɨh��̋������������Ȃ��Ă���
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
