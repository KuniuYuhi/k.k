#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow {

	void RenderingEngine::Init()
	{
		InitZPrepassRenderTarget();
		InitRenderTargets();
		InitGBuffer();
		m_sceneLight.Init();
		InitShadowMapRender();
		

		m_postEffect.Init(m_mainRenderTarget);

		InitCopyToFrameBufferSprite();
		
		InitDefferedLighting_Sprite();
		
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
		const wchar_t* toontexture = L"Assets/shader/ToonTextrue/lamp_glay.DDS";
		m_toontexture.InitFromDDSFile(toontexture);
	}

	void RenderingEngine::InitGBuffer()
	{
		int frameBuffer_w = g_graphicsEngine->GetFrameBufferWidth();
		int frameBuffer_h = g_graphicsEngine->GetFrameBufferHeight();

		// アルベドカラーを出力用のレンダリングターゲットを初期化する
		float clearColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		m_gBuffer[enGBufferAlbedoDepth].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
		// 法線出力用のレンダリングターゲットを初期化する
		m_gBuffer[enGBufferNormal].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_SNORM,
			DXGI_FORMAT_UNKNOWN
		);
		//// メタリック、影パラメータ、スムース出力用のレンダリングターゲットを初期化する    
		//m_gBuffer[enGBufferMetaricShadowSmooth].Create(
		//	frameBuffer_w,
		//	frameBuffer_h,
		//	1,
		//	1,
		//	DXGI_FORMAT_R8G8B8A8_UNORM,
		//	DXGI_FORMAT_UNKNOWN
		//);
		// ワールド座標のレンダリングターゲット
		/*m_gBuffer[enGBufferWorldPos].Create(
			frameBuffer_w,
			frameBuffer_h,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_UNKNOWN
		);*/

	}

	void RenderingEngine::InitDefferedLighting_Sprite()
	{
		//ディファードライティングを行うためのスプライトを初期化
		SpriteInitData spriteInitData;

		// 画面全体にレンダリングするので幅と高さはフレームバッファーの幅と高さと同じ
		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();

		//ディファードライティングで使用するテクスチャを設定
		int texNo = 0;
		for (auto& gBuffer : m_gBuffer)
		{
			spriteInitData.m_textures[texNo++] = &gBuffer.GetRenderTargetTexture();
		}

		spriteInitData.m_fxFilePath= "Assets/shader/DeferredLighting.fx";
		spriteInitData.m_expandConstantBuffer = &GetSceneLight();
		spriteInitData.m_expandConstantBufferSize = sizeof(GetSceneLight());

		//初期化データを使ってスプライトを作成
		m_diferredLightingSprite.Init(spriteInitData);
	}

	//モデルリストに格納されているモデルを描画する
	void RenderingEngine::ModelRendering(RenderContext& rc)
	{
		for (auto& modelObj : m_renderObjects)
		{
			modelObj->OnForwardRender(rc);
		}
	}

	//モデルリストの格納されているモデルのシャドウマップ描画用のモデルを描画する
	void RenderingEngine::ShadowModelRendering(RenderContext& rc, Camera& camera)
	{
		for (auto& modelObj : m_renderObjects)
		{
			modelObj->OnRenderShadowModel(rc, camera);
		}
	}

	//スプライトリストに格納されているスプライトを描画する
	void RenderingEngine::SpriteRendering(RenderContext& rc)
	{
		for (auto& spriteObj : m_renderObjects)
		{
			spriteObj->OnRender2D(rc);
		}
	}

	//フォントリストに格納されているフォントを描画する
	void RenderingEngine::FontRendering(RenderContext& rc)
	{
		for (auto& fontObj : m_renderObjects)
		{
			fontObj->OnRender2D(rc);
		}
	}

	void RenderingEngine::RenderToShadowMap(RenderContext& rc)
	{
		BeginGPUEvent("RenderToShadowMap");
		
		m_shadowMapRenders.Render(
			rc,
			GetLightCamera(),
			m_renderObjects
		);

		EndGPUEvent();
	}

	void RenderingEngine::ZPrepass(RenderContext& rc)
	{
		BeginGPUEvent("ZPrepass");
		// まず、レンダリングターゲットとして設定できるようになるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(m_zprepassRenderTarget);

		// レンダリングターゲットを設定
		rc.SetRenderTargetAndViewport(m_zprepassRenderTarget);

		// レンダリングターゲットをクリア
		rc.ClearRenderTargetView(m_zprepassRenderTarget);

		for (auto& renderObj : m_renderObjects) {
			renderObj->OnZPrepass(rc);
		}

		rc.WaitUntilFinishDrawingToRenderTarget(m_zprepassRenderTarget);
		EndGPUEvent();
	}

	void RenderingEngine::RenderToGBuffer(RenderContext& rc)
	{
		BeginGPUEvent("RenderToGBuffer");
		// レンダリングターゲットをG-Bufferに変更
		RenderTarget* rts[enGBufferNum] = {
			&m_gBuffer[enGBufferAlbedoDepth],         // 0番目のレンダリングターゲット
			&m_gBuffer[enGBufferNormal]             // 1番目のレンダリングターゲット
			//& m_gBuffer[enGBufferWorldPos]              // 1番目のレンダリングターゲット
		};

		//まず、レンダリングターゲットとして設定できるようになるまで待つ
		rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);
		//レンダーターゲットを設定
		rc.SetRenderTargets(ARRAYSIZE(rts), rts);
		//レンダーターゲットをクリア
		rc.ClearRenderTargetViews(ARRAYSIZE(rts), rts);
		//
		for (auto& modelObj : m_renderObjects)
		{
			modelObj->OnRenderToGBuffer(rc);
		}

		//レンダリングターゲットへの書き込み待ち
		rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);

		m_diferredLightingSprite.Draw(rc);

		EndGPUEvent();
	}

	void RenderingEngine::ForwardRendering(RenderContext& rc)
	{
		BeginGPUEvent("ForwardRendering");
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
		EndGPUEvent();
	}

	void RenderingEngine::Render2D(RenderContext& rc)
	{
		BeginGPUEvent("Render2D");
		//スプライトを描画
		SpriteRendering(rc);
		//フォントを描画
		//FontRendering(rc);
		EndGPUEvent();
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

	void RenderingEngine::InitShadowMapRender()
	{
		//シャドウマップ描画用のレンダリングターゲット初期化
		m_shadowMapRenders.Init();

	}

	void RenderingEngine::Execute(RenderContext& rc)
	{
		//シャドウマップへの描画
		//ZPrepass
		//GBufferへの描画
		//フォワードレンダリング←ライティングも行う
		//ポストエフェクト
		//メインレンダリングターゲットの絵をフレームバッファーにコピー

		// シーンライトのデータをコピー。
		m_deferredLightingCB.m_light = m_sceneLight.GetSceneLight();

		//視点の位置を設定する	
		SetEyePos(g_camera3D->GetPosition());

		//シャドウマップ描画用のモデルを描画
		RenderToShadowMap(rc);

		//ライトカメラの設定
		//LightCameraUpDate();
		
		//ライトビュープロジェクション行列を設定
		SetmLVP();

		//ZPrepassモデルを描画
		ZPrepass(rc);
		//Gbuffer
		RenderToGBuffer(rc);
		//フォワードレンダリング
		ForwardRendering(rc);

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

		// 登録されている描画オブジェクトをクリア
		m_renderObjects.clear();
	}

}
