#pragma once



class DamageFont:public IGameObject
{
public:
	DamageFont();
	~DamageFont();

	//ダメージを表示するキャラ
	enum EnDamageActor
	{
		enDamageActor_Player,
		enDamageActor_Monster,
		enDamageActor_Boss
	};

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void InitFont();

	/// <summary>
	/// フォントの移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// フォントの透明化
	/// </summary>
	void TransparencyFont();

	/// <summary>
	/// 生成時に必要な設定
	/// </summary>
	/// <param name="damageActor">ダメージを受けるキャラ</param>
	/// <param name="damage">受けるダメージ</param>
	void Setting(EnDamageActor damageActor, int damage,Vector3 position)
	{
		SetDamageActor(damageActor);
		SetDamage(damage);
		SetPosition(position);
	}

	void SetDamage(int damage)
	{
		m_damage = damage;
	}

	void SetDamageActor(EnDamageActor damageActor)
	{
		m_damageActor = damageActor;
	}

	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

private:

	FontRender m_damageFont;
	SpriteRender m_damageBackRender;

	EnDamageActor m_damageActor;

	Vector3 m_position = g_vec3Zero;
	Vector2 m_damagePosition = g_vec2Zero;
	Vector3 m_damageBackPosition = g_vec3Zero;

	Vector3 m_scale = g_vec3One;
	Vector2 m_ofset;
	Vector4 m_color = g_vec4White;
	Vector4 m_backColor = g_vec4White;

	int m_damage;
	float m_yUp;

	float m_Alpha = 1.0f;

	bool m_positionRandomFlag = false;

};

