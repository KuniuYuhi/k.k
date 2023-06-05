#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

//モデルを描画する
void nsK2EngineLow::RenderingEngine::ModelRendering(RenderContext& rc)
{
	for (auto& modelObj : m_modelList)
	{
		modelObj->OnRenderModel(rc);
	}
}

//スプライトを描画する
void nsK2EngineLow::RenderingEngine::SpriteRendering(RenderContext& rc)
{
	for (auto& spriteObj : m_spriteList)
	{
		spriteObj->OnRenderSprite(rc);
	}
}

void nsK2EngineLow::RenderingEngine::Execute(RenderContext& rc)
{
	//モデルを描画
	ModelRendering(rc);
	//スプライトを描画
	SpriteRendering(rc);

	m_modelList.clear();
}


