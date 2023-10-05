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

	void MoveCursor();

	void CalcMuteBGMVolume();


	void GoToPlayMode();
	void HowToPlayMode();
	void ActionMode();
	void GameEndMode();


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

	/// <summary>
	/// �I�΂�Ă��郂�[�h�̃e�L�X�g�摜��傫������B����ȊO�͏���������
	/// </summary>
	void SetScaleModeText();

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

	enum EnMode
	{
		enMode_GoToPlay,
		enMode_HowToPlay,
		enMode_Action,
		enMode_GameEnd,
		enMode_Num
	};

private:

	SpriteRender			m_titleNameRender;
	SpriteRender			m_backRender;
	SpriteRender			m_starRender;
	SpriteRender			m_pushAButtonTextRender;
	SpriteRender			m_goToPlayTextRender;
	SpriteRender			m_howToPlayTextRender;
	SpriteRender			m_actionTextRender;
	SpriteRender			m_gameEndTextRender;
	SpriteRender			m_cursorRender;
	SpriteRender			m_howToPlayRender;
	SpriteRender			m_actionRender;

	Fade*					m_fade = nullptr;

	EnTitleAnimStep			m_step = enStep1_TitleTextAlpha;
	EnStarScaleState		m_starScaleStep = enStarStep1_ScaleUp;


	Vector3					m_gTPScale = g_vec3One;
	Vector3					m_hTPScale = g_vec3One;
	Vector3					m_actionScale = g_vec3One;
	Vector3					m_gameEndScale = g_vec3One;


	float					m_wipeSize = 860.0f;

	bool					m_pushAButtonFlag = false;

	bool					m_calcAlphaEndFlag = false;
	float					m_alphaTimer = 0.0f;
	Vector4					m_color = g_vec4Clear;

	Vector4					m_AButtonColor = g_vec4Clear;
	float					m_alphaAButtonTimer = 0.0f;
	bool					m_Flag = false;
	
	float					m_mulScale = 0.0f;
	Quaternion				m_starRotation = g_quatIdentity;
	float					m_scaleTimer = 0.0f;
	float					m_mulTimerValue = 5.0f;

	int						m_selectCursor = 0;

	bool					m_SelectModeFlag = false;				//���[�h�����߂Ă��邩�̃t���O

	bool					m_drawHowToPlayFlag = false;
	bool					m_drawActionFlag = false;

	bool					m_goToGameFlag = false;				//�Q�[�����n�܂邩�̃t���O

	bool					m_muteBGMFlag = false;

	float					m_bgmVolume = 0.0f;



};

