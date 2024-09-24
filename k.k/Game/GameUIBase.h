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
	/// �h��Ɏg�����
	/// </summary>
	struct ShakeInfo
	{
	public:

		void SetShakeInfo(int Strength, float Vibrato, Vector2 DefaultPosition)
		{
			strength = Strength;
			vibrato = Vibrato;
			defaultPosition = DefaultPosition;
		}


		int strength = 0.0f;		//�h��̋���
		float vibrato = 0.0f;		//�ǂ̂��炢�U�����邩
		Vector2 defaultPosition = g_vec2Zero;	//�����ʒu

	};

protected:

	/// <summary>
	/// �h�����ݒ�
	/// </summary>
	/// <param name="Strength">�h��̋����B(�����_���ɐݒ肷��Ƃ��Ɏg��)</param>
	/// <param name="Vibrato">�ǂ̂��炢�U�����邩�B(�h��̋����̍ő�l)</param>
	/// <param name="DefaultPosition">�����ʒu</param>
	void SetShakeInfo(int Strength, float Vibrato, Vector2 DefaultPosition);


	Vector2 GetUpdateShakePosition(
		float currentTimer, float TimeLimit, Vector2 currentPosition
	);


	/// <summary>
	/// �l��͈͓��Ɏ��߂�
	/// </summary>
	/// <param name="value"></param>
	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <returns></returns>
	float Clamp(float value, float min, float max);


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

	ShakeInfo m_shakeInfo;


};

