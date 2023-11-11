#pragma once

#include "CascadeShadowMapMatrix.h"

namespace nsK2EngineLow {
	class Shadow
	{
	public:
		void Init();

		void Render(
			RenderContext& rc,
			Camera& lightCamera,
			std::vector< IRenderer* >& renderObjects
		);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rc"></param>
		//void ShadowSpriteRender(RenderContext& rc);

		/// <summary>
		/// カスケードシャドウのエリア率を設定。
		/// </summary>
		/// <remark>
		/// ゲームカメラの近平面から遠平面までのエリアの、何%の範囲を近距離用の高解像度のシャドウマップに、
		/// 何%を中距離用のシャドウマップに、何%を遠距離用のシャドウマップに描画するかを指定したもの。
		/// 例えば、nearArea  = 0.1(10%)、middleArea = 0.3(30%), farArea = 0.8(80%)と指定されていたら、
		/// カメラから10%のエリアは近距離用のシャドウマップに描画、
		/// カメラから10%～30%のエリアは中距離用のシャドウマップに描画、
		/// カメラから30%～80%のエリアは遠距離用のシャドウマップに描画となる。
		/// 影の品質の調整に使ってください。
		/// </remark>
		/// <param name="nearArea">近影エリア率。</param>
		void SetCascadeNearAreaRates(float nearArea, float middleArea, float farArea);

		/// <summary>
		/// ライトビュープロジェクション行列を取得
		/// </summary>
		/// <param name="areaNo"></param>
		/// <returns></returns>
		const Matrix& GetLVPMatrix(int areaNo) const
		{
			return m_cascadeShadowMapMatrix.GetLightViewProjectionCropMatrix(areaNo);
		}

		/// <summary>
		/// シャドウマップを取得
		/// </summary>
		/// <param name="areaNo">エリア番号</param>
		/// <returns></returns>
		Texture& GetShadowMap(int areaNo)
		{
			//return m_blur[areaNo].GetBokeTexture();
			/*if (m_isSoftShadow) {
				return m_blur[areaNo].GetBokeTexture();
			}*/
			return m_shadowMaps[areaNo].GetRenderTargetTexture();
		}


		/// <summary>
		/// ライトカメラの取得
		/// </summary>
		/// <returns></returns>
		Camera& GetLightCamera()
		{
			return m_lightCamera;
		}
		/// <summary>
		/// ライトの座標更新
		/// </summary>
		void UpDateLightCamera();


	private:
		/// <summary>
		/// レンダリングターゲットの初期化
		/// </summary>
		void InitRenderTarget();

		/// <summary>
		/// 影描写用のカメラの初期化
		/// </summary>
		void InitLightCamera();

	private:
		CascadeShadowMapMatrix m_cascadeShadowMapMatrix;		//カスケードシャドウの行列を扱うオブジェクト
		//RenderTarget m_shadowMap;
		RenderTarget m_shadowMaps[NUM_SHADOW_MAP];		//シャドウマップ
		GaussianBlur m_blur[NUM_SHADOW_MAP];			//シャドウマップにブラーをかける処理
		Camera m_lightCamera;
		float m_cascadeAreaRateArray[NUM_SHADOW_MAP] = { 800.0f,2000.0f, g_camera3D->GetFar()};//近影、中影、遠影の範囲。いじっていい
		Vector3 m_lightCameraPosition = Vector3(500.0f, 1200.0f, 0.0f);	//カメラの座標(座標からターゲットに向かって影が落とされる)
		std::vector< IRenderer* > m_renderers;              // シャドウマップへのレンダラーの配列。
		Sprite shadowSprite;

		float m_clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };

	};
}
