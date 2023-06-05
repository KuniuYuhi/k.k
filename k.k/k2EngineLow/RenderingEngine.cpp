#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow {

	//モデルリストに格納されているモデルを描画する
	void RenderingEngine::ModelRendering(RenderContext& rc)
	{
		for (auto& modelObj : m_modelList)
		{
			modelObj->OnRenderModel(rc);
		}
	}

	//スプライトリストに格納されているスプライトを描画する
	void RenderingEngine::SpriteRendering(RenderContext& rc)
	{
		for (auto& spriteObj : m_spriteList)
		{
			spriteObj->OnRenderSprite(rc);
		}
	}

	//フォントリストに格納されているフォントを描画する
	void RenderingEngine::FontRendering(RenderContext& rc)
	{
		for (auto& fontObj : m_fontList)
		{
			fontObj->OnRenderFont(rc);
		}
	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		//モデルを描画
		ModelRendering(rc);
		//スプライトを描画
		SpriteRendering(rc);
		//フォントを描画
		FontRendering(rc);

		m_modelList.clear();
		m_spriteList.clear();
		m_fontList.clear();
	}

}
