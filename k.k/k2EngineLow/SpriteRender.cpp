#include "k2EngineLowPreCompile.h"
#include "SpriteRender.h"

void nsK2EngineLow::SpriteRender::Init(const char* filePath, const float width, const float height, AlphaBlendMode alphaBlendMode)
{
	SpriteInitData spriteInitData;
	//DDSファイル(画像ファイル)のファイルパスを指定する
	spriteInitData.m_ddsFilePath[0] = filePath;
	//sprite表示用のシェーダーのファイルパスを指定する
	spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	//spriteの幅と高さを設定する
	spriteInitData.m_width= static_cast<UINT>(width);
	spriteInitData.m_height = static_cast<UINT>(height);

	//spriteのブレンドモードを設定
	spriteInitData.m_alphaBlendMode = alphaBlendMode;

	//定数バッファーとして送る
	spriteInitData.m_expandConstantBuffer = &m_spriteCB;
	spriteInitData.m_expandConstantBufferSize = sizeof(m_spriteCB);


	//sprite初期化オブジェクトを使って、spriteを初期化する
	m_sprite.Init(spriteInitData);

	m_isInit = true;
}

void nsK2EngineLow::SpriteRender::Draw(RenderContext& rc)
{
	g_renderingEngine->AddSpriteList(this);
}
