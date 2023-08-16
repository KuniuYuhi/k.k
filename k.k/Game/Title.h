#pragma once

class Fade;
class Title :public IGameObject
{
public:
	Title();
	~Title();

	bool Start();
	void Update();
	void Render(RenderContext& rc);



private:
	SpriteRender m_titleSprite;
	Fade* m_fade = nullptr;

	float m_wipeSize = 860.0f;
};

