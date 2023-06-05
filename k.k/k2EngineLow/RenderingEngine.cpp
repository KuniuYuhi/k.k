#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

//���f����`�悷��
void nsK2EngineLow::RenderingEngine::ModelRendering(RenderContext& rc)
{
	for (auto& modelObj : m_modelList)
	{
		modelObj->OnRenderModel(rc);
	}
}

//�X�v���C�g��`�悷��
void nsK2EngineLow::RenderingEngine::SpriteRendering(RenderContext& rc)
{
	for (auto& spriteObj : m_spriteList)
	{
		spriteObj->OnRenderSprite(rc);
	}
}

void nsK2EngineLow::RenderingEngine::Execute(RenderContext& rc)
{
	//���f����`��
	ModelRendering(rc);
	//�X�v���C�g��`��
	SpriteRendering(rc);

	m_modelList.clear();
}


