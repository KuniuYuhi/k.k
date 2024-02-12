#pragma once

#include "MyRenderer.h"
#include "PostEffect.h"
#include "Light.h"
#include "Shadow.h"

namespace nsK2EngineLow {
	class ModelRender;
	class SpriteRender;
	class FontRender;
	class RenderContext;

	class RenderingEngine
	{
	public:
		// ディファードライティング用の定数バッファ
		struct SDeferredLightingCB
		{
			Light m_light;              // ライト
			Matrix mlvp[NUM_SHADOW_MAP]; // ライトビュープロジェクション行列。
			float m_iblLuminance;       // IBLの明るさ。
			int m_isIBL;                // IBLを行う。
			int m_isEnableRaytracing;   // レイトレが行われている。
		};



		  /// <summary>
		/// イベント。
		/// </summary>
		enum EnEvent {
			enEventReInitIBLTexture,    // IBLテクスチャが再初期化された。
		};

		/// <summary>
		/// レンダーターゲットやライトの初期化
		/// </summary>
		void Init();

		/// <summary>
		/// レンダリングターゲットの初期化
		/// </summary>
		void InitRenderTargets();

		/// <summary>
		/// テクスチャを貼り付けるためのスプライトの初期化
		/// </summary>
		void InitCopyToFrameBufferSprite();

		/// <summary>
		/// 描画オブジェクトを追加。
		/// </summary>
		/// <param name="renderObject"></param>
		void AddRenderObject(IRenderer* renderObject)
		{
			m_renderObjects.push_back(renderObject);
		}

		/// <summary>
		/// モデルレンダークラスをリストに追加する
		/// </summary>
		/// <param name="modelRender"></param>
		void AddModelList(ModelRender* modelRender)
		{
			m_modelList.emplace_back(modelRender);
		}

		/// <summary>
		/// Gbufferモデル
		/// </summary>
		/// <param name="modelRender"></param>
		void AddGBufferModelList(ModelRender* modelRender)
		{
			m_gBufferModelList.emplace_back(modelRender);
		}

		/// <summary>
		/// スプライトレンダークラスをリストに追加する
		/// </summary>
		/// <param name="spriteRender"></param>
		void AddSpriteList(SpriteRender* spriteRender)
		{
			m_spriteList.emplace_back(spriteRender);
		}

		/// <summary>
		/// フォントレンダークラスをリストに追加する
		/// </summary>
		/// <param name="fontRender"></param>
		void AddFontList(FontRender* fontRender)
		{
			m_fontList.emplace_back(fontRender);
		}

		/// <summary>
		/// シャドウマップ描画用のモデルを描画する
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		/// <param name="camera">カメラ</param>
		void ShadowModelRendering(RenderContext& rc, Camera& camera);

		/// <summary>
		/// イベントリスナーを追加。
		/// </summary>
		void AddEventListener(
			void* pListenerObj,
			std::function<void(EnEvent enEvent)> listenerFunc
		)
		{
			m_eventListeners.push_back({ pListenerObj, listenerFunc });
		}
		/// <summary>
		/// イベントリスナーを削除。
		/// </summary>
		void RemoveEventListener(void* pListenerObj)
		{

			auto it = std::find_if(
				m_eventListeners.begin(),
				m_eventListeners.end(),
				[&](const SEventListenerData& listenerData) {return listenerData.pListenerObj == pListenerObj; }
			);
			if (it != m_eventListeners.end()) {
				m_eventListeners.erase(it);
			}
		}

		/// <summary>
		/// 描画処理を実行
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void Execute(RenderContext& rc);

		void SetMainRenderTargetAndDepthStencilBuffer(RenderContext& rc)
		{
			rc.SetRenderTarget(m_mainRenderTarget.GetRTVCpuDescriptorHandle(), m_zprepassRenderTarget.GetDSVCpuDescriptorHandle());
		}

		/// <summary>
		/// シーンライトを取得
		/// </summary>
		/// <returns>シーンライト</returns>
		SceneLight& GetSceneLight()
		{
			return m_sceneLight;
		}

		/// <summary>
		/// ライトビュープロジェクション行列を設定する
		/// </summary>
		/// <param name="LVP">ライトビュープロジェクション行列</param>
		void SetmLVP()
		{
			for (int shadowMapNo = 0; shadowMapNo < NUM_SHADOW_MAP; shadowMapNo++)
			{
				m_sceneLight.SetmLVP(GetlvpcMatrix(shadowMapNo),shadowMapNo);
			}
		}

		/// <summary>
		/// トゥーンマップテクスチャを取得
		/// </summary>
		/// <returns></returns>
		Texture& GetToonTextrue()
		{
			return m_toontexture;
		}

		/// <summary>
	   /// ZPrepassの描画パスにモデルを追加
	   /// </summary>
	   /// <param name="model"></param>
		void Add3DModelToZPrepass(ModelRender* model)
		{
			m_zprepassModelList.emplace_back(model);
		}

		/// <summary>
		/// ZPrepassで作成された深度テクスチャを取得
		/// </summary>
		/// <returns></returns>
		Texture& GetZPrepassDepthTexture()
		{
			return m_zprepassRenderTarget.GetRenderTargetTexture();
		}

		/// <summary>
		/// 被写界深度の有効化
		/// </summary>
		void EnableDof()
		{
			m_postEffect.DofEnable();
		}
		/// <summary>
		/// 被写界深度の無効化
		/// </summary>
		void DisableDof()
		{
			m_postEffect.DofDisable();
		}

		/////////////////////////////////////////////////////
		///シャドウマップの関数
		/////////////////////////////////////////////////////

		/// <summary>
		/// シャドウマップテクスチャにクエリを行う。
		/// </summary>
		/// <param name="queryFunc">クエリ関数</param>
		void QueryShadowMapTexture(std::function< void(Texture& shadowMap) > queryFunc)
		{
			for (int areaNo = 0; areaNo < NUM_SHADOW_MAP; areaNo++)
			{
				queryFunc(m_shadowMapRenders.GetShadowMap(areaNo));
			}
		}

		/// <summary>
		/// ライトカメラを取得
		/// </summary>
		/// <returns>ライトカメラ</returns>
		Camera& GetLightCamera()
		{
			return m_shadowMapRenders.GetLightCamera();
		}

		/// <summary>
		/// ライトビュープロジェクションクロップ行列を取得
		/// </summary>
		/// <param name="shadowNo"></param>
		/// <returns></returns>
		Matrix GetlvpcMatrix(int shadowNo)
		{
			return m_shadowMapRenders.GetLVPMatrix(shadowNo);
		}

		/// <summary>
		/// ライトカメラの更新
		/// </summary>
		void LightCameraUpDate()
		{
			m_shadowMapRenders.UpDateLightCamera();
		}

		/////////////////////////////////////////////////////
		///ディレクションライトの関数
		/////////////////////////////////////////////////////
		
		/// <summary>
		/// ディレクションライトの設定
		/// </summary>
		/// <param name="lightNo">ライト番号</param>
		/// <param name="direction">ライトの方向</param>
		/// <param name="color">ライトの色</param>
		void SetDerectionLight(int lightNo, Vector3 direction, Vector3 color)
		{
			m_sceneLight.SetDirectionLight(lightNo, direction, color);
		}

		/// <summary>
		/// ディレクションライトの光の方向を設定する
		/// </summary>
		/// <param name="direction">方向</param>
		void SetDirLightDirection(Vector3 direction)
		{
			m_sceneLight.SetDirLightDirection(direction);
		}
		/// <summary>
		/// ディレクションライトの光の色を設定する
		/// </summary>
		/// <param name="color">色</param>
		void SetDirLightColor(Vector3 color)
		{
			m_sceneLight.SetDirLightColor(color);
		}

		/// <summary>
		/// ディレクションライトの光の方向を取得する
		/// </summary>
		/// <returns>光の方向</returns>
		const Vector3& GetDirLigDirection() const
		{
			return m_sceneLight.GetDirLigDirection();
		}
		/// <summary>
		/// ディレクションライトの光の色を取得する
		/// </summary>
		/// <returns>色</returns>
		const Vector3& GetDirLigColor() const
		{
			return m_sceneLight.GetDirLigColor();
		}

		/////////////////////////////////////////////////////////////
		///環境光の関数
		/////////////////////////////////////////////////////////////

		/// <summary>
		/// 環境光の設定
		/// </summary>
		/// <param name="ambient">環境光の強さ</param>
		void SetAmbient(Vector3 ambient)
		{
			m_sceneLight.SetAmbient(ambient);
		}
		/// <summary>
		/// 環境光を取得
		/// </summary>
		/// <returns></returns>
		const Vector3& GetAmbient() const
		{
			return m_sceneLight.GetAmbient();
		}

		/////////////////////////////////////////////////////////////
		///ポイントライトの関数
		/////////////////////////////////////////////////////////////

		/// <summary>
		/// ポイントライトを設定する
		/// </summary>
		/// <param name="pos">ライトの位置</param>
		/// <param name="color">ライトの色</param>
		/// <param name="range">xにライトの影響範囲,yに影響範囲に累乗するパラメータ</param>
		void SetPointLight(Vector3 pos, Vector3 color, Vector3 range)
		{
			m_sceneLight.SetPointLight(pos, color, range);
		}

		/// <summary>
		/// ポイントライトの座標を設定する
		/// </summary>
		/// <param name="pos"></param>
		void SetPointLightPosition(Vector3 pos)
		{
			m_sceneLight.SetPointLightPosition(pos);
		}

		/// <summary>
		/// ポイントライトの色を設定する
		/// </summary>
		/// <param name="color">色</param>
		void SetPointLightColor(Vector3 color)
		{
			m_sceneLight.SetPointLightColor(color);
		}

		/// <summary>
		/// 影響範囲と累乗するパラメータを設定
		/// </summary>
		/// <param name="attn">Xに影響範囲,Yに累乗するパラメータ</param>
		void SetPointLightAttn(Vector3 attn)
		{
			m_sceneLight.SetPointLightAttn(attn);
		}

		/// <summary>
		/// ポイントライトを使用する
		/// </summary>
		void UsePointLight()
		{
			m_sceneLight.UsePointLight();
		}

		/// <summary>
		/// ポイントライトを使用しない
		/// </summary>
		void UnUsePointLight()
		{
			m_sceneLight.UnUsePointLight();
		}

		/// <summary>
		/// ポイントライトの位置を取得する
		/// </summary>
		/// <returns>座標</returns>
		const Vector3& GetPointLightPosition() const
		{
			return m_sceneLight.GetPointLightPosition();
		}

		/// <summary>
		/// ポイントライトの光の色を取得
		/// </summary>
		/// <returns>色</returns>
		const Vector3& GetPointLightColor() const
		{
			return m_sceneLight.GetPointLightColor();
		}

		/// <summary>
		/// スポットライトの影響範囲と累乗するパラメータを取得
		/// </summary>
		/// <returns>Xに影響範囲,Yに累乗するパラメータ</returns>
		const Vector3& GetPointLightAttn() const
		{
			return m_sceneLight.GetPointLightAttn();
		}

		/// <summary>
		/// ポイントライトは使用中?
		/// </summary>
		/// <returns>使用中ならtrue</returns>
		const int PointLightIsUse() const
		{
			return m_sceneLight.PointLightIsUse();
		}

		/////////////////////////////////////////////////////////////
		///スポットライトの関数
		/////////////////////////////////////////////////////////////

		/// <summary>
		/// スポットライトを設定する
		/// </summary>
		/// <param name="pos">位置</param>
		/// <param name="color">色</param>
		/// <param name="range">xに影響範囲,yに影響範囲に累乗するパラメータ</param>
		/// <param name="direction">照射方向</param>
		/// <param name="angle">xは照射角度,ｙは影響に累乗するパラメータ</param>
		void SetSpotLight(Vector3 pos, Vector3 color, Vector3 attn, Vector3 direction, Vector3 angle)
		{
			m_sceneLight.SetSpotLight(pos, color, attn, direction, angle);
		}

		/// <summary>
		/// スポットライトの位置を設定する
		/// </summary>
		/// <param name="pos">座標</param>
		void SetSpotLightPosition(Vector3 pos)
		{
			m_sceneLight.SetSpotLightPosition(pos);
		}

		/// <summary>
		/// スポットライトのライト色の設定
		/// </summary>
		/// <param name="color">色</param>
		void SetSpotLightColor(Vector3 color)
		{
			m_sceneLight.SetSpotLightColor(color);
		}

		/// <summary>
		/// 影響範囲と累乗するパラメータを設定
		/// </summary>
		/// <param name="attn">Xに影響範囲,Yに累乗するパラメータ</param>
		void SetSpotLightAttn(Vector3 attn)
		{
			m_sceneLight.SetSpotLightAttn(attn);
		}

		/// <summary>
		/// スポットライトのライトの方向を設定
		/// </summary>
		/// <param name="direction">方向</param>
		void SetSpotLightDirection(Vector3 direction)
		{
			m_sceneLight.SetSpotLightDirection(direction);
		}

		/// <summary>
		/// スポットライトのライトの角度を設定
		/// </summary>
		/// <param name="angle">角度</param>
		void SetSpotLightAngle(Vector3 angle)
		{
			m_sceneLight.SetSpotLightAngle(angle);
		}

		/// <summary>
		/// スポットライトを使用する
		/// </summary>
		void UseSpotLight()
		{
			m_sceneLight.UseSpotLight();
		}

		/// <summary>
		/// スポットライトを使用しない
		/// </summary>
		void UnUseSpotLight()
		{
			m_sceneLight.UnUseSpotLight();
		}

		/// <summary>
		/// スポットライトの位置を取得
		/// </summary>
		/// <returns>座標</returns>
		const Vector3& GetSpotLightPosition() const
		{
			return m_sceneLight.GetSpotLightPosition();
		}

		/// <summary>
		/// スポットライトの光の色を取得
		/// </summary>
		/// <returns>色</returns>
		const Vector3& GetSpotLightColor() const
		{
			return m_sceneLight.GetSpotLightColor();
		}

		/// <summary>
		/// スポットライトの影響範囲と累乗するパラメータを取得
		/// </summary>
		/// <returns></returns>
		const Vector3& GetSpotLightAttn()const
		{
			return m_sceneLight.GetSpotLightAttn();
		}

		/// <summary>
		/// スポットライトの光の方向を取得する
		/// </summary>
		/// <returns>方向</returns>
		const Vector3& GetSpotLightDirection() const
		{
			return m_sceneLight.GetSpotLightDirection();
		}

		/// <summary>
		/// スポットライトの角度を取得する
		/// </summary>
		/// <returns>角度</returns>
		const Vector3& GetSpotLightAngle() const
		{
			return m_sceneLight.GetSpotLightAngle();
		}

		/// <summary>
		/// スポットライトは使用中？
		/// </summary>
		/// <returns>使用中の場合true</returns>
		const int SpotLightIsUse() const
		{
			return m_sceneLight.SpotLightIsUse();
		}

		/////////////////////////////////////////////////////////////
		///半球ライトの関数
		/////////////////////////////////////////////////////////////

		/// <summary>
		/// 半球ライトを設定する
		/// </summary>
		/// <param name="groundColor">地面色</param>
		/// <param name="skyColor">天球色</param>
		/// <param name="groundNormal">地面の法線</param>
		void SetHemiLight(Vector3 groundColor, Vector3 skyColor, Vector3 groundNormal)
		{
			SetHemiLightGroundColor(groundColor);
			SetHemiLightSkyColor(skyColor);
			SetHemiLightGroundNormal(groundNormal);
			UseHemiLight();
		}

		/// <summary>
		/// 半球ライトの地面色を設定
		/// </summary>
		/// <param name="groundColor">地面色</param>
		void SetHemiLightGroundColor(Vector3 groundColor)
		{
			m_sceneLight.SetHemiLightGroundColor(groundColor);
		}

		/// <summary>
		/// 半球ライトの天球色を設定
		/// </summary>
		/// <param name="skyColor">天球色</param>
		void SetHemiLightSkyColor(Vector3 skyColor)
		{
			m_sceneLight.SetHemiLightSkyColor(skyColor);
		}

		/// <summary>
		/// 半球ライトの地面の法線
		/// </summary>
		/// <param name="normal">地面の法線</param>
		void SetHemiLightGroundNormal(Vector3 normal)
		{
			m_sceneLight.SetHemiLightGroundNormal(normal);
		}

		/// <summary>
		/// 半球ライトを使用する
		/// </summary>
		void UseHemiLight()
		{
			m_sceneLight.UseHemiLight();
		}

		/// <summary>
		/// 半球ライトを使用しない
		/// </summary>
		void UnUseHemiLight()
		{
			m_sceneLight.UnUseHemiLight();
		}

		/// <summary>
		/// 半球ライトの地面色を取得する
		/// </summary>
		/// <returns>地面色</returns>
		const Vector3& GetHemiLightGroundColor() const
		{
			return m_sceneLight.GetHemiLightGroundColor();
		}

		/// <summary>
		/// 半球ライトの天球色を取得する
		/// </summary>
		/// <returns></returns>
		const Vector3& GetHemiLightSkyColor() const
		{
			return m_sceneLight.GetHemiLightSkyColor();
		}

		/// <summary>
		/// 半球ライトの地面の法線を取得する
		/// </summary>
		/// <returns></returns>
		const Vector3& GetHemiLightGroundNormal() const
		{
			return m_sceneLight.GetHemiLightGroundNormal();
		}

		/// <summary>
		/// 半球ライトは使用中?
		/// </summary>
		/// <returns>使用中のときtrue</returns>
		const int HemiLightIsUse() const
		{
			return m_sceneLight.HemiLightIsUse();
		}

		//////////////////////////////////////////////////////////////
		///リムライトの関数
		//////////////////////////////////////////////////////////////

			/// <summary>
		/// リムライトの強さを設定
		/// </summary>
		/// <param name="power"></param>
		void SetLimLIghtPower(float power)
		{
			m_sceneLight.SetLimLIghtPower(power);
		}
		/// <summary>
		/// リムライトの強さを取得
		/// </summary>
		/// <returns></returns>
		const float& GetLimLightPower() const
		{
			return m_sceneLight.GetLimLightPower();
		}

		/// <summary>
		/// リムライトを使用する
		/// </summary>
		void UseLimLight()
		{
			m_sceneLight.UseLimLight();
		}
		/// <summary>
		/// リムライトを使用しない
		/// </summary>
		void UnUseLimLight()
		{
			m_sceneLight.UnUseLimLight();
		}

		/// <summary>
		/// リムライトは使用中?
		/// </summary>
		/// <returns>使用中のときtrue</returns>
		const int LimLightIsUse() const
		{
			return m_sceneLight.LimLightIsUse();
		}


		/////////////////////////////////////////////////////////////
		///カメラの位置の関数
		/////////////////////////////////////////////////////////////

		/// <summary>
		/// カメラの位置を設定する
		/// </summary>
		/// <param name="eyePos">カメラの座標</param>
		void SetEyePos(Vector3 eyePos)
		{
			m_sceneLight.SetEyePos(eyePos);
		}

		RenderTarget& GetZprepassRenderTarget()
		{
			return m_zprepassRenderTarget;
		}

		/// <summary>
		/// モデルを描画する
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void ModelRendering(RenderContext& rc);

	private:
		// GBufferの定義
		enum EnGBuffer
		{
			enGBufferAlbedoDepth,           // アルベドと深度値。αに深度値が記憶されています。
			enGBufferNormal,                // 法線
			enGBufferNum,                   // G-Bufferの数
		};

		/// <summary>
		/// G-Bufferを初期化
		/// </summary>
		void InitGBuffer();
		/// <summary>
		/// ディファードライティングで使用するスプライトを初期化。
		/// </summary>
		void InitDefferedLighting_Sprite();

		/// <summary>
		/// スプライトを描画する
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void SpriteRendering(RenderContext& rc);

		/// <summary>
		/// フォントを描画する
		/// </summary>
		/// <param name="rc">レンダーコンテキスト</param>
		void FontRendering(RenderContext& rc);

		/// <summary>
		/// シャドウマップに描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void RenderToShadowMap(RenderContext& rc);

		/// <summary>
		/// ZPrepassへの描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void ZPrepass(RenderContext& rc);

		/// <summary>
		/// G-Bufferへの描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト。</param>
		void RenderToGBuffer(RenderContext& rc);

		/// <summary>
	   /// フォワードレンダリング
	   /// </summary>
	   /// <param name="rc">レンダリングコンテキスト</param>
		void ForwardRendering(RenderContext& rc);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="mainRenderTarget"></param>
		void PostEffecting(RenderContext& rc);

		/// <summary>
		/// 2D描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void Render2D(RenderContext& rc);

		/// <summary>
		/// メインレンダリングターゲットの内容をフレームバッファにコピーする
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);

		/// <summary>
		/// ZPrepass用のレンダリングターゲットを初期化
		/// </summary>
		void InitZPrepassRenderTarget();

		/// <summary>
		/// シャドウマップへの描画処理を初期化
		/// </summary>
		void InitShadowMapRender();


		///////////////////
//////////////////////////////////////////7

		


		std::vector<ModelRender*>		m_modelList;	//モデルリスト
		std::vector<SpriteRender*>		m_spriteList;	//スプライトリスト
		std::vector<FontRender*>		m_fontList;		//スプライトリスト
		std::vector< ModelRender* >		m_zprepassModelList;  // ZPrepassの描画パスで描画されるモデルのリスト 
		std::vector<ModelRender*>		m_gBufferModelList;	//モデルリスト

		RenderTarget m_zprepassRenderTarget;			//ZPrepass描画用のレンダリングターゲット

		Sprite						m_copyToFrameBufferSprite;	//テクスチャを貼り付けるためのスプライトを初期化
		SceneLight						m_sceneLight;	//シーンライト
		SDeferredLightingCB m_deferredLightingCB;                       // ディファードライティング用の定数バッファ
		Sprite m_diferredLightingSprite;                                // ディファードライティングを行うためのスプライト
		RenderTarget					m_mainRenderTarget;	//レンダリングターゲット
		PostEffect						m_postEffect;		//ポストエフェクト
		Shadow							m_shadowMapRenders;
		Texture							m_toontexture;
		RenderTarget					m_gBuffer[enGBufferNum];
		std::vector< IRenderer* > m_renderObjects;                      // 描画オブジェクトのリスト。



		/// <summary>
	   /// イベントリスナーのデータ。
	   /// </summary>
		struct SEventListenerData {
			void* pListenerObj;     // リスナーオブジェクト
			std::function<void(EnEvent enEvent)> listenerFunc;
		};

		std::list< SEventListenerData > m_eventListeners;
	};
}


