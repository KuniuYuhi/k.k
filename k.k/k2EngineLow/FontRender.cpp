#include "k2EngineLowPreCompile.h"
#include "FontRender.h"

void nsK2EngineLow::FontRender::Draw(RenderContext& rc)
{
	if (m_text == nullptr)
	{
		return;
	}
	g_renderingEngine->AddRenderObject(this);
	//g_renderingEngine->AddFontList(this);
}
