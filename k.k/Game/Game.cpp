#include "stdafx.h"
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

bool Game::Start()
{
	model.Init("Assets/modelData/unityChan.tkm");

	return true;
}

void Game::Update()
{
}

void Game::Render(RenderContext& rc)
{
	model.Draw(rc);
}
