#pragma once

class Player;

class CascadModel:public IGameObject
{
public:
	CascadModel();
	~CascadModel();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void MoveCamera();

private:
	ModelRender tea;
	ModelRender bg;

	ModelRender ball;
};

