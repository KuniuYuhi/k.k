#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

void nsK2EngineLow::RenderingEngine::ModelRendering(RenderContext& rc)
{
	for (auto& modelObj : m_modelList)
	{
		modelObj->OnRenderModel(rc);
	}
}

void nsK2EngineLow::RenderingEngine::Execute(RenderContext& rc)
{
	//ƒ‚ƒfƒ‹‚ð•`‰æ
	ModelRendering(rc);

	m_modelList.clear();
}
