#include "k2EngineLowPreCompile.h"
#include "Shadow.h"

namespace ShadowConst
{
	const int RENDER_TARGET_WIDTH = 1024;	//レンダリングターゲットの幅
	const int RENDER_TARGET_HEIGHT = 1024;	//レンダリングターゲットの高さ

	const float LIGHT_CAMERA_WIDTH = 600.0f;
	const float LIGHT_CAMERA_HEIGHT = 600.0f;

	const float LIGHT_CAMERA_ANGLE = 20.0f;	//ライトカメラのアングル
}

namespace nsK2EngineLow {
	void Shadow::Init()
	{
		InitRenderTarget();
		InitLightCamera();
		//InitShadowSprite();
	}

	void Shadow::Render(RenderContext& rc)
	{
		UpDateLightCamera();

		rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
		rc.SetRenderTargetAndViewport(m_shadowMap);
		rc.ClearRenderTargetView(m_shadowMap);
		//影モデルを描画
		g_renderingEngine->ShadowModelRendering(rc, m_lightCamera);
		rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);
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
		//シャドウマップ描画用のレンダリングターゲット
		m_shadowMap.Create(
			ShadowConst::RENDER_TARGET_WIDTH,
			ShadowConst::RENDER_TARGET_HEIGHT,
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			m_clearColor
		);
	}

	void Shadow::InitLightCamera()
	{
		//影描写用のライトカメラの初期化
		m_lightCamera.SetPosition(m_lightCameraPosition);
		//ライトを照らす場所
		m_lightCamera.SetTarget(0, 0, 0);
		//X方向を上にする
		m_lightCamera.SetUp(1, 0, 0);
		//画角
		m_lightCamera.SetViewAngle(Math::DegToRad(ShadowConst::LIGHT_CAMERA_ANGLE));

		//影が動かないようにするためにカメラを平行投影にする
		/*m_lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
		m_lightCamera.SetWidth(ShadowConst::LIGHT_CAMERA_WIDTH);
		m_lightCamera.SetHeight(ShadowConst::LIGHT_CAMERA_HEIGHT);*/

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

