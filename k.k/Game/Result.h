#pragma once
class ResultSeen:public IGameObject
{
public:
	ResultSeen();
	~ResultSeen();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	bool GetRoundWipeEndFlag()
	{
		return m_roundWipeEndFlag;
	}

private:
	SpriteRender m_resultSprite;

	float m_wipeSize = 960.0f;

	bool m_roundWipeEndFlag = false;
};

