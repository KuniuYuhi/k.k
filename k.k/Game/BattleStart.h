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

private:
	SpriteRender m_battleTextRender;
	SpriteRender m_startTextRender;

	Vector4 m_color = g_vec4White;

	float m_timer = 0.0f;

	float m_alpha = 0.0f;
	bool m_calcAlphaEndFlag = false;
};

