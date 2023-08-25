#pragma once
class BattleStart:public IGameObject
{
public:
	BattleStart();
	~BattleStart();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void MoveText(SpriteRender& spriteRender,Vector3 startPos,Vector3 endPos);

	void CalcAlpha();

	void InitSprite();

	/// <summary>
	/// スプライトの座標、サイズ、回転を設定
	/// </summary>
	/// <param name="spriterender">スプライトレンダー</param>
	/// <param name="position">座標</param>
	/// <param name="scale">サイズ</param>
	/// <param name="rotation">回転</param>
	void SettingSpriteRender(
		SpriteRender& spriterender, Vector3 position, Vector3 scale, Quaternion rotation)
	{
		spriterender.SetPosition(position);
		spriterender.SetScale(scale);
		spriterender.SetRotation(rotation);
		spriterender.Update();
	}

private:
	SpriteRender m_battleTextRender;
	SpriteRender m_startTextRender;

	Vector4 m_color = g_vec4White;

	float m_timer = 0.0f;

	float m_alpha = 0.0f;
	bool m_calcAlphaEndFlag = false;
};

