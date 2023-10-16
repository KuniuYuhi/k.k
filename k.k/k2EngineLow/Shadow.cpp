#include "k2EngineLowPreCompile.h"
#include "Shadow.h"

namespace ShadowConst
{
	const int RENDER_TARGET_WIDTH = 6000;	//レンダリングターゲットの幅
	const int RENDER_TARGET_HEIGHT = 6000;	//レンダリングターゲットの高さ

	const float LIGHT_CAMERA_WIDTH = 2000.0f;
	const float LIGHT_CAMERA_HEIGHT = 2000.0f;

	const float LIGHT_CAMERA_ANGLE = 20.0f;	//ライトカメラのアングル
}

namespace nsK2EngineLow {
	void Shadow::Init()
	{
		InitRenderTarget();
		InitLightCamera();
	}

	void Shadow::Render(
		RenderContext& rc,
		Vector3& lightDirection,
		std::vector< IRenderer* >& renderObjects
	)
	{
		// ライトの最大の高さをレンダラーのAABBから計算する。

		int shadowMapNo = 0;
		for (auto& shadowMap : m_shadowMaps) {
			rc.WaitUntilToPossibleSetRenderTarget(shadowMap);
			rc.SetRenderTargetAndViewport(shadowMap);
			rc.ClearRenderTargetView(shadowMap);

			for (auto& renderer : renderObjects) {
				renderer->OnRenderShadowMap(
					rc,
					shadowMapNo,
					m_lightCamera
				);
			}

			//描画が終わったらクリア
			//m_renderers.clear();

			// 書き込み完了待ち
			rc.WaitUntilFinishDrawingToRenderTarget(shadowMap);
			shadowMapNo++;
		}

		//ブラーを実行する
		for (auto& blur : m_blur)
		{
			blur.ExecuteOnGPU(rc, 1.0f);
		}

		//レンダリングエンジンの方でやる
		UpDateLightCamera();
	}

	void Shadow::ShadowSpriteRender(RenderContext& rc)
	{
		shadowSprite.Update(
			{ FRAME_BUFFER_W / -2.0f,FRAME_BUFFER_H / 2.0f,0.0f },
			g_quatIdentity,
			g_vec3One,
			{ 0.0f,1.0f }
		);
		shadowSprite.Draw(rc);
	}

	void Shadow::InitRenderTarget()
	{
		float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

		DXGI_FORMAT colorFormat;
		DXGI_FORMAT depthFormat;

		colorFormat = g_hardShadowMapFormat.colorBufferFormat;
		depthFormat = g_hardShadowMapFormat.depthBufferFormat;

		//近景用のシャドウマップ
		m_shadowMaps[0].Create(
			2048,
			2048,
			1,
			1,
			colorFormat,
			depthFormat,
			clearColor
		);

		//シャドウマップをぼかすためのオブジェクトの初期化
		m_blur[0].Init(&m_shadowMaps[0].GetRenderTargetTexture());

	}

	void Shadow::InitLightCamera()
	{
		//影描写用のライトカメラの初期化
		m_lightCamera.SetPosition(m_lightCameraPosition);
		//ライトを照らす場所(影が落とされる場所)
		m_lightCamera.SetTarget(0, 0, 0);
		//X方向を上にする
		m_lightCamera.SetUp(1, 0, 0);
		//画角
		m_lightCamera.SetViewAngle(Math::DegToRad(ShadowConst::LIGHT_CAMERA_ANGLE));

		//影が動かないようにするためにカメラを平行投影にする
		m_lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
		m_lightCamera.SetWidth(ShadowConst::LIGHT_CAMERA_WIDTH);
		m_lightCamera.SetHeight(ShadowConst::LIGHT_CAMERA_HEIGHT);

		//ライトビュープロジェクション行列を計算している
		m_lightCamera.Update();

	}

	void Shadow::InitShadowSprite()
	{
		SpriteInitData spriteInitData;
		spriteInitData.m_textures[0] = &m_shadowMap.GetRenderTargetTexture();
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		spriteInitData.m_width = 512;
		spriteInitData.m_height = 512;

		shadowSprite.Init(spriteInitData);
	}

	void Shadow::UpDateLightCamera()
	{
		//ライトカメラの位置を計算する
		Vector3 lightCamPos = g_camera3D->GetTarget();
		lightCamPos.x += m_lightCameraPosition.x;
		lightCamPos.y = m_lightCameraPosition.y;
		lightCamPos.z += m_lightCameraPosition.z;

		m_lightCamera.SetPosition(lightCamPos);
		m_lightCamera.SetTarget(g_camera3D->GetTarget());
		m_lightCamera.Update();
	}
	


}

