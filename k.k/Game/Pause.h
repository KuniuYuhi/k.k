#pragma once



class Pause:public IGameObject
{
public:
	Pause();
	~Pause();

	bool Start();
	void Update();
	void Render(RenderContext& rc);


	/// <summary>
	/// ƒQ[ƒ€I—¹ƒtƒ‰ƒO‚ğæ“¾
	/// </summary>
	/// <returns></returns>
	const bool& GetGameExitFlag() const
	{
		return m_gameExitFlag;
	}

private:
	/// <summary>
	/// ‰æ‘œ‚Ì‰Šú‰»
	/// </summary>
	void InitSprite();
	/// <summary>
	/// 
	/// </summary>
	void Select();

private:
	SpriteRender m_bgRender;		//”wŒi
	
	bool m_gameExitFlag = false;
};

