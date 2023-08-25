#pragma once

class Fade;
class Title :public IGameObject
{
public:
	Title();
	~Title();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void InitSprite();

	void StepManage();

	void SelectMode();

	void CalcAlpha();

	void CalcAlphaAButtonText();

	/// <summary>
	/// �������点�鏈��
	/// </summary>
	void ShineStar();

	/// <summary>
	/// �X�v���C�g�̍��W�A�T�C�Y�A��]��ݒ�
	/// </summary>
	/// <param name="spriterender">�X�v���C�g�����_�[</param>
	/// <param name="position">���W</param>
	/// <param name="scale">�T�C�Y</param>
	/// <param name="rotation">��]</param>
	void SettingSpriteRender(
		SpriteRender& spriterender, Vector3 position, Vector3 scale, Quaternion rotation)
	{
		spriterender.SetPosition(position);
		spriterender.SetScale(scale);
		spriterender.SetRotation(rotation);
		spriterender.Update();
	}

	enum EnTitleAnimStep
	{
		enStep1_TitleTextAlpha,
		enStep2_ShineStar,
		enStep_End
	};

	enum EnStarScaleState
	{
		enStarStep1_ScaleUp,
		enStarStep2_ScaleDown,
		enStarStep_End
	};

private:

	SpriteRender m_titleNameRender;
	SpriteRender m_backRender;
	SpriteRender m_starRender;
	SpriteRender m_pushAButtonTextRender;
	SpriteRender m_goToPlayTextRender;
	SpriteRender m_howToPlayTextRender;
	SpriteRender m_actionTextRender;
	SpriteRender m_gameEndTextRender;

	Fade* m_fade = nullptr;

	EnTitleAnimStep m_step = enStep1_TitleTextAlpha;
	EnStarScaleState m_starScaleStep = enStarStep1_ScaleUp;

	float m_wipeSize = 860.0f;

	bool m_pushAButtonFlag = false;

	bool m_calcAlphaEndFlag = false;
	float m_alphaTimer = 0.0f;
	Vector4 m_color = g_vec4Clear;

	Vector4 m_AButtonColor = g_vec4Clear;
	float m_alphaAButtonTimer = 0.0f;
	bool m_Flag = false;
	
	float m_mulScale = 0.0f;
	Quaternion m_starRotation = g_quatIdentity;
	float m_scaleTimer = 0.0f;
	float m_mulTimerValue = 5.0f;
};

