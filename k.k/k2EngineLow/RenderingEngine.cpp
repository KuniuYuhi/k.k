#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow {

	void RenderingEngine::Init()
	{
		InitRenderTargets();
		m_shadow.Init();
		m_postEffect.Init(m_mainRenderTarget);
		InitCopyToFrameBufferSprite();

		m_sceneLight.Init();
	}

	void RenderingEngine::InitRenderTargets()
	{
		//メインレンダリングターゲット
		m_mainRenderTarget.Create(
			FRAME_BUFFER_W,
			FRAME_BUFFER_H,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}

	void RenderingEngine::InitCopyToFrameBufferSprite()
	{
		SpriteInitData spriteInitData;

		//	テクスチャはm_mainRenderTargetのカラーバッファー
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		spriteInitData.m_width = m_mainRenderTarget.GetWidth();
		spriteInitData.m_height = m_mainRenderTarget.GetHeight();

		//2Dのシェーダーを指定する
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";

		//スプライトを初期化
		m_copyToFrameBufferSprite.Init(spriteInitData);
	}

	//モデルリストに格納されているモデルを描画する
	void RenderingEngine::ModelRendering(RenderContext& rc)
	{
		for (auto& modelObj : m_modelList)
		{
			modelObj->OnRenderModel(rc);
		}
	}

	//モデルリストの格納されているモデルのシャドウマップ描画用のモデルを描画する
	void RenderingEngine::ShadowModelRendering(RenderContext& rc, Camera& camera)
	{
		for (auto& modelObj : m_modelList)
		{
			modelObj->OnRenderShadowModel(rc, camera);
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
		//視点の位置を設定する
		SetEyePos(g_camera3D->GetPosition());

		//シャドウマップ描画用のモデルを描画
		m_shadow.Render(rc);

		//レンダリングターゲットをm_mainRenderTargetに変更する
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		//レンダリングターゲットとビューポートを設定する
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);
		//レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_mainRenderTarget);

		//モデルを描画
		ModelRendering(rc);

		//レンダリングターゲット書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
		//輝度抽出とガウシアンブラー実行
		//ボケ画像をメインレンダリングターゲットに加算合成
		m_postEffect.Render(rc, m_mainRenderTarget);

		//メインレンダリングターゲットの絵をフレームバッファーにコピー
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		m_copyToFrameBufferSprite.Draw(rc);

		//スプライトを描画
		SpriteRendering(rc);
		//フォントを描画
		FontRendering(rc);

		//m_shadow.ShadowSpriteRender(rc);

		m_modelList.clear();
		m_spriteList.clear();
		m_fontList.clear();
	}

}
