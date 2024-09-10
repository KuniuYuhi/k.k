#pragma once


/// <summary>
/// �Q�[��UI�̃x�[�X�N���X
/// </summary>
class GameUIBase:public IGameObject
{
public:
	virtual ~GameUIBase(){}

	

	virtual void UIUpdate() = 0;

	virtual void Draw(RenderContext& rc) = 0;

	virtual void DeleteSelf() = 0;

protected:


	/// <summary>
	/// �Q�[�W�̊������v�Z
	/// </summary>
	/// <param name="maxValue">�ő�l</param>
	/// <param name="currentValue">���݂̒l</param>
	Vector3 CalcGaugeRatio(float maxValue, float currentValue);



	/// <summary>
	/// �^�C�}�[�������Ă��邩�H
	/// </summary>
	/// <param name="timeLimit">�^�C�����~�b�g</param>
	/// <param name="timer">�^�C�}�[</param>
	/// <returns>�^�C�}�[�����Z����Ă���Ȃ�true</returns>
	bool IsTimerRunning(float timeLimit, float& timer);

	/// <summary>
	/// �X�v���C�g�����_�[�̏������B
	/// </summary>
	/// <param name="spriterender">�������������X�v���C�g�����_�[</param>
	/// <param name="filePath">�摜�̃t�@�C���p�X</param>
	/// <param name="width">�摜�̕�</param>
	/// <param name="height">�摜�̍���</param>
	/// <param name="position">���W</param>
	/// <param name="scale">�T�C�Y</param>
	/// <param name="rotation">��]</param>
	void InitSpriteRender(
		SpriteRender& spriteRender,
		const char* filePath,
		const float width, const float height,
		Vector2 position,
		Vector3 scale = g_vec3One,
		Quaternion rotation = g_quatIdentity
	);

	/// <summary>
	/// �t�H���g�����_�[�̏������B
	/// </summary>
	/// <param name="fontRender">�������������t�H���g�����_�[</param>
	/// <param name="position">���W</param>
	/// <param name="color">�J���[</param>
	/// <param name="scale">�T�C�Y</param>
	/// <param name="isShadowParam">�e(�֊s)�����邩</param>
	/// <param name="shadowOffset">�I�t�Z�b�g��</param>
	/// <param name="shadowColor">�e�̃J���[</param>
	void InitFontRender(
		FontRender& fontRender,
		Vector2 position = g_vec2Zero,
		float scale = 1.0f,
		Vector4 color = g_vec4White,
		const wchar_t* text = L"None",
		bool isShadowParam = true,
		float shadowOffset = 1.8f,
		Vector4 shadowColor = g_vec4Black
	);

protected:




};

