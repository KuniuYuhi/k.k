#pragma once



class Pause:public IGameObject
{
public:
	Pause();
	~Pause();

	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;


	/// <summary>
	/// ゲーム終了フラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetGameExitFlag() const
	{
		return m_gameExitFlag;
	}

private:
	/// <summary>
	/// 画像の初期化
	/// </summary>
	void InitSprite();
	/// <summary>
	/// 選択
	/// </summary>
	void Select();

private:
	SpriteRender m_bgRender;		//背景
	
	bool m_gameExitFlag = false;
};

