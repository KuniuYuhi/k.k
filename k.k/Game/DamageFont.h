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
		enDamageActor_Boss,
		enDamageActor_None
	};

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

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

	

private:

	/// <summary>
	/// 表示するフォントの初期化
	/// </summary>
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
	/// ダメージの設定
	/// </summary>
	/// <param name="damage"></param>
	void SetDamage(int damage)
	{
		m_damage = damage;
	}
	/// <summary>
	/// ダメージを受けるアクターを設定
	/// </summary>
	/// <param name="damageActor"></param>
	void SetDamageActor(EnDamageActor damageActor)
	{
		m_damageActor = damageActor;
	}
	/// <summary>
	/// 生成する座標を設定
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

	/// <summary>
	/// ダメージフォントと背景の設定
	/// </summary>
	/// <param name="fontColor">フォントのカラー</param>
	/// <param name="fontScale">フォントの拡大率</param>
	/// <param name="fontOffset">フォントのオフセット</param>
	/// <param name="yUp">Yアップ</param>
	/// <param name="backSpriteScale">背景の拡大率</param>
	void SetDamageSpriteAndFont(
		Vector4 fontColor,
		float fontScale,
		Vector2 fontOffset,
		float yUp,
		float backSpriteScale
	);

	/// <summary>
	/// ダメージフォントの情報を設定
	/// </summary>
	/// <param name="color">色</param>
	/// <param name="scale">拡大率</param>
	/// <param name="offset">オフセット</param>
	void SetDamageFontInfo(Vector4 color, float scale, Vector2 offset);

	/// <summary>
	/// ダメージフォントを表示する座標を設定
	/// </summary>
	/// <returns></returns>
	Vector2 CalcDamagePosition();

	/// <summary>
	/// ダメージフォントのオフセットを計算
	/// </summary>
	/// <param name="fontScale">フォントのスケール</param>
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

