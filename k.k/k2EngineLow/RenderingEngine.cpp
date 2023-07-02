#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow {

	void RenderingEngine::Init()
	{
		
		InitZPrepassRenderTarget();
		InitRenderTargets();
		m_shadow.Init();
		m_postEffect.Init(m_mainRenderTarget);
		InitCopyToFrameBufferSprite();



		m_sceneLight.Init();
		SetToonTextureDDS();
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

	//トゥーンテクスチャを設定する
	void RenderingEngine::SetToonTextureDDS()
	{
		const wchar_t* toontexture = L"Assets/shader/ToonTextrue/toontexture2.DDS";
		m_toontexture.InitFromDDSFile(toontexture);
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

	void RenderingEngine::ZPrepass(RenderContext& rc)
	{
		// まず、レンダリングターゲットとして設定できるようになるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_zprepassRenderTarget);

		// レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_zprepassRenderTarget);

		// レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_zprepassRenderTarget);

		for (auto& renderObj : m_zprepassModelList) {
			renderObj->OnZPrepass(rc);
		}

		rc.WaitUntilFinishDrawingToRenderTarget(m_zprepassRenderTarget);
	}

	void RenderingEngine::InitZPrepassRenderTarget()
	{
		float clearColor[] = { 1.0f,1.0f,1.0f,1.0f };
		m_zprepassRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}

	

	void RenderingEngine::Execute(RenderContext& rc)
	{
		//視点の位置を設定する
		SetEyePos(g_camera3D->GetPosition());

		//シャドウマップ描画用のモデルを描画
		m_shadow.Render(rc);

		SetmLVP(g_renderingEngine->GetLightCamera().GetViewProjectionMatrix());

		ZPrepass(rc);

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
		m_zprepassModelList.clear();
	}

}
