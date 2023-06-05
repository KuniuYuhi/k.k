#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow {

	//���f�����X�g�Ɋi�[����Ă��郂�f����`�悷��
	void RenderingEngine::ModelRendering(RenderContext& rc)
	{
		for (auto& modelObj : m_modelList)
		{
			modelObj->OnRenderModel(rc);
		}
	}

	//�X�v���C�g���X�g�Ɋi�[����Ă���X�v���C�g��`�悷��
	void RenderingEngine::SpriteRendering(RenderContext& rc)
	{
		for (auto& spriteObj : m_spriteList)
		{
			spriteObj->OnRenderSprite(rc);
		}
	}

	//�t�H���g���X�g�Ɋi�[����Ă���t�H���g��`�悷��
	void RenderingEngine::FontRendering(RenderContext& rc)
	{
		for (auto& fontObj : m_fontList)
		{
			fontObj->OnRenderFont(rc);
		}
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		//���f����`��
		ModelRendering(rc);
		//�X�v���C�g��`��
		SpriteRendering(rc);
		//�t�H���g��`��
		FontRendering(rc);

		m_modelList.clear();
		m_spriteList.clear();
		m_fontList.clear();
	}

}
