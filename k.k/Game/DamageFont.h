#pragma once



class DamageFont:public IGameObject
{
public:
	DamageFont();
	~DamageFont();

	//�_���[�W��\������L����
	enum EnDamageActor
	{
		enDamageActor_Player,
		enDamageActor_Monster,
		enDamageActor_Boss,
		enDamageActor_None
	};

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	/// <summary>
	/// �������ɕK�v�Ȑݒ�
	/// </summary>
	/// <param name="damageActor">�_���[�W���󂯂�L����</param>
	/// <param name="damage">�󂯂�_���[�W</param>
	void Setting(EnDamageActor damageActor, int damage,Vector3 position)
	{
		SetDamageActor(damageActor);
		SetDamage(damage);
		SetPosition(position);
	}

	

private:

	/// <summary>
	/// �\������t�H���g�̏�����
	/// </summary>
	void InitFont();

	/// <summary>
	/// �t�H���g�̈ړ�����
	/// </summary>
	void Move();

	/// <summary>
	/// �t�H���g�̓�����
	/// </summary>
	void TransparencyFont();

	/// <summary>
	/// �_���[�W�̐ݒ�
	/// </summary>
	/// <param name="damage"></param>
	void SetDamage(int damage)
	{
		m_damage = damage;
	}
	/// <summary>
	/// �_���[�W���󂯂�A�N�^�[��ݒ�
	/// </summary>
	/// <param name="damageActor"></param>
	void SetDamageActor(EnDamageActor damageActor)
	{
		m_damageActor = damageActor;
	}
	/// <summary>
	/// ����������W��ݒ�
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

	/// <summary>
	/// �_���[�W�t�H���g�Ɣw�i�̐ݒ�
	/// </summary>
	/// <param name="fontColor">�t�H���g�̃J���[</param>
	/// <param name="fontScale">�t�H���g�̊g�嗦</param>
	/// <param name="fontOffset">�t�H���g�̃I�t�Z�b�g</param>
	/// <param name="yUp">Y�A�b�v</param>
	/// <param name="backSpriteScale">�w�i�̊g�嗦</param>
	void SetDamageSpriteAndFont(
		Vector4 fontColor,
		float fontScale,
		Vector2 fontOffset,
		float yUp,
		float backSpriteScale
	);

	/// <summary>
	/// �_���[�W�t�H���g�̏���ݒ�
	/// </summary>
	/// <param name="color">�F</param>
	/// <param name="scale">�g�嗦</param>
	/// <param name="offset">�I�t�Z�b�g</param>
	void SetDamageFontInfo(Vector4 color, float scale, Vector2 offset);

	/// <summary>
	/// �_���[�W�t�H���g��\��������W��ݒ�
	/// </summary>
	/// <returns></returns>
	Vector2 CalcDamagePosition();

	/// <summary>
	/// �_���[�W�t�H���g�̃I�t�Z�b�g���v�Z
	/// </summary>
	/// <param name="fontScale">�t�H���g�̃X�P�[��</param>
	/// <returns></returns>
	Vector2 CalcDamageFontOffset(float fontScale = 1.0f);

private:

	FontRender m_damageFont;
	SpriteRender m_damageBackRender;

	EnDamageActor m_damageActor = enDamageActor_None;

	Vector3 m_position = g_vec3Zero;
	Vector2 m_damagePosition = g_vec2Zero;
	Vector3 m_damageBackPosition = g_vec3Zero;

	Vector3 m_scale = g_vec3One;
	Vector2 m_ofset;
	Vector4 m_color = g_vec4White;
	Vector4 m_backColor = g_vec4White;

	int m_damage = 0;
	float m_yUp = 0.0f;

	float m_Alpha = 1.0f;

	bool m_largeRandomPositionFlag = false;

};

