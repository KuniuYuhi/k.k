#pragma once

//todo たまに.objのエラー
#include "MyRenderer.h"

namespace nsK2EngineLow {

	class RenderingEngine;

	/// <summary>
	/// スキンモデルレンダー。
	/// </summary>
	class ModelRender:public IRenderer
	{
	public:
		
		/// <summary>
		/// 通常の初期化
		/// </summary>
		/// <param name="tkmFilepath">tkmファイルパス</param>
		/// <param name="animationClips">アニメーションクリップ</param>
		/// <param name="numAnimationClips">アニメーションクリップの数</param>
		/// <param name="enModelUpAxis">モデルの上方向</param>
		void Init(
			const char* tkmFilePath,
			const wchar_t* lampTextureFIlePath,
			AnimationClip* animationClips = nullptr, 
			int numAnimationClips=0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool isShadowCaster = true,
			bool isToon = true,
			bool isOutline = true
		);

		/// <summary>
	   /// 初期化
	   /// </summary>
	   /// <remark>
	   /// 本関数を利用して初期化されたモデルは
	   /// フォワードレンダリングの描画パスで描画されます
	   /// </remark>
	   /// <param name="renderingEngine">レンダリングエンジン</param>
	   /// <param name="modelInitData"></param>
		void InitForwardRendering(RenderingEngine& renderingEngine, ModelInitData& modelInitData);

		/// <summary>
		/// スカイキューブを初期化する
		/// </summary>
		/// <param name="initData"></param>
		void InitSkyCube(ModelInitData& initData);

		/// <summary>
		/// モデルレンダークラスをリストに追加
		/// </summary>
		/// <param name="rc"></param>
		void Draw(RenderContext& rc)
		{
			g_renderingEngine->AddRenderObject(this);
			/*g_renderingEngine->AddModelList(this);
			g_renderingEngine->AddGBufferModelList(this);
			g_renderingEngine->Add3DModelToZPrepass(this);*/
		}

		/// <summary>
		/// モデルを描画する(RenderingEngineで描画するときに呼び出す)
		/// </summary>
		/// <param name="rc"></param>
		void OnRenderModel(RenderContext& rc)
		{
			m_frowardRenderModel.Draw(rc);
		}

		/// <summary>
		/// モデルを取得する
		/// </summary>
		/// <returns>モデル</returns>
		Model& GetModel()
		{
			return m_frowardRenderModel;
		}

		/// <summary>
		/// 座標、回転、拡大をすべて設定
		/// </summary>
		/// <param name="pos">座標の設定</param>
		/// <param name="rot">回転の設定</param>
		/// <param name="scale">拡大の設定</param>
		void SetTransform(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
		{
			SetPosition(pos);
			SetRotation(rot);
			SetScale(scale);
		}

		/// <summary>
		/// 座標の設定
		/// </summary>
		/// <param name="position">Vector3の座標</param>
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}

		/// <summary>
		/// 座標の設定
		/// </summary>
		/// <param name="x">X軸</param>
		/// <param name="y">Y軸</param>
		/// <param name="z">Z軸</param>
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x,y,z });
		}

		/// <summary>
		/// 回転の設定
		/// </summary>
		/// <param name="rotation">回転</param>
		void SetRotation(const Quaternion rotation)
		{
			m_rotation = rotation;
		}

		/// <summary>
		/// 大きさの設定
		/// </summary>
		/// <param name="scale">大きさ</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}

		/// <summary>
		/// 大きさの設定
		/// </summary>
		/// <param name="x">X軸</param>
		/// <param name="y">Y軸</param>
		/// <param name="z">Z軸</param>
		void SetScale(float x, float y, float z)
		{
			SetScale({ x,y,z });
		};

		/// <summary>
		/// 大きさの設定(全ての軸同じ)
		/// </summary>
		/// <param name="scale"></param>
		void SetScale(float scale)
		{
			SetScale({ scale,scale,scale });
		}

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();
		/// <summary>
		/// 各種モデルのワールド行列を更新する。
		/// </summary>
		void UpdateWorldMatrixInModes();


		/// <summary>
		/// ボーンの名前からボーン番号を検索
		/// </summary>
		/// <param name="boneName">ボーンの名前</param>
		/// <returns>ボーン番号。見つからなかった場合は-1が返る</returns>
		int FindBoneID(const wchar_t* boneName)const
		{
			return m_skeleton.FindBoneID(boneName);
		}

		/// <summary>
		/// ボーン番号からボーンを取得する
		/// </summary>
		/// <param name="boneNo">ボーン番号</param>
		/// <returns>ボーン</returns>
		Bone* GetBone(int boneNo) const
		{
			return m_skeleton.GetBone(boneNo);
		}

		/// <summary>
		/// アニメーションイベントを追加する
		/// </summary>
		/// <param name="eventListener"></param>
		void AddAnimationEvent(AnimationEventListener eventListener)
		{
			m_animation.AddAnimationEventListener(eventListener);
		}

		/// <summary>
		/// アニメーション再生。
		/// </summary>
		/// <param name="animNo">アニメーションクリップの番号。</param>
		/// <param name="interpolateTime">補完時間(単位：秒。)</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}

		/// <summary>
		/// アニメーションの再生中？
		/// </summary>
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}

		/// <summary>
		/// アニメーション再生の速度を設定する。
		/// </summary>
		/// <param name="animationSpeed">数値の分だけ倍にする。</param>
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}

		/// <summary>
		/// ディレクションライトの情報を作成
		/// </summary>
		void MakeDirectionData(ModelInitData& modelInitData);





	// メンバ変数
	private:
		/// <summary>
		/// ZPrepassモデルを描画する
		/// </summary>
		/// <param name="rc"></param>
		void OnZPrepass(RenderContext& rc) override;
		/// <summary>
		/// G-Buffer描画パスから呼ばれる処理。
		/// </summary>
		void OnRenderToGBuffer(RenderContext& rc) override;
		/// <summary>
		/// フォワードレンダーパスから呼ばれる処理。
		/// </summary>
		void OnForwardRender(RenderContext& rc) override;
		/// <summary>
		/// 半透明オブジェクト描画パスから呼ばれる処理。
		/// </summary>
		/// <param name="rc"></param>
		void OnTlanslucentRender(RenderContext& rc) override;
		/// <summary>
		/// シャドウマップ描画用のモデルを描画する
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="lightCamera"></param>
		void OnRenderShadowModel(RenderContext& rc, Camera& lightCamera)
		{
			m_shadowModel.Draw(rc, lightCamera);
		}
		/// <summary>
		/// シャドウマップへの描画パスから呼ばれる処理。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="ligNo">ライト番号</param>
		/// <param name="shadowMapNo">シャドウマップ番号</param>
		/// <param name="lvpMatrix">ライトビュープロジェクション行列</param>
		void OnRenderShadowMap(
			RenderContext& rc,
			int shadowMapNo,
			Camera& lightCamera
		) override;
		

		/// <summary>
		/// スケルトンの初期化。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		void InitSkeleton(const char* filePath);

		/// <summary>
		/// アニメーションの初期化。
		/// </summary>
		/// <param name="animationClips">アニメーションクリップ。</param>
		/// <param name="numAnimationClips">アニメーションクリップの数。</param>
		/// <param name="enModelUpAxis">モデルの上向き。</param>
		void InitAnimation(
			AnimationClip* animationClips,
			int numAnimationClips,
			EnModelUpAxis enModelUpAxis
		);

		/// <summary>
		/// ZPrepass描画用のモデルを初期化。
		/// </summary>
		/// <param name="renderingEngine"></param>
		/// <param name="tkmFilePath"></param>
		void InitModelOnZprepass(
			/*RenderingEngine& renderingEngine,*/
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis
		);
		/// <summary>
		/// 半透明オブジェクト描画パスで使用されるモデルを初期化。
		/// </summary>
		/// <param name="renderingEngine"></param>
		/// <param name="tkmFilePath"></param>
		/// <param name="enModelUpAxis"></param>
		/// <param name="isShadowReciever"></param>
		void InitModelOnfrowardRender(
			RenderingEngine& renderingEngine,
			const char* tkmFilePath,
			const wchar_t* lampTextureFIlePath,
			EnModelUpAxis enModelUpAxis,
			bool isShadowCaster, bool isToon, bool isOutline
		);
		/// <summary>
		/// GBuffer描画用のモデルを初期化。
		/// </summary>
		/// <param name="renderingEngine">レンダリングエンジン</param>
		/// <param name="tkmFilePath">tkmファイルパス</param>
		void InitModelOnRenderGBuffer(
			RenderingEngine& renderingEngine,
			const char* tkmFilePath,
			EnModelUpAxis enModelUpAxis,
			bool isShadowReciever);
		/// <summary>
		/// シャドウマップ描画用のモデルを初期化。
		/// </summary>
		/// <param name="renderingEngine">レンダリングエンジン</param>
		/// <param name="tkmFilePath">tkmファイルパス</param>
		/// <param name="modelUpAxis">モデルの上軸</param>
		void InitModelOnShadowMap(
			RenderingEngine& renderingEngine,
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis,
			bool isFrontCullingOnDrawShadowMap=false
		);



		/// <summary>
		/// 各種モデルの頂点シェーダーのエントリーポイントを設定。
		/// </summary>
		void SetupVertexShaderEntryPointFunc(
			ModelInitData& modelInitData);

		/// <summary>
		/// フォワードレンダリング用モデルのピクセルシェーダーのエントリーポイントを設定。
		/// </summary>
		void SetupPixelShaderEntryPointFuncToFrowardModel(
			ModelInitData& modelInitData,
			bool isShadowCaster,
			bool isToon
		);

		/// <summary>
	   /// 共通の初期化処理(今はデファードレンダリングのみ)
	   /// </summary>
	   /// <param name="renderingEngine">レンダリングエンジン</param>
	   /// <param name="tkmFilePath">tkmファイルパス</param>
		void InitCommon(RenderingEngine& renderingEngine, const char* tkmFilePath);

		Skeleton					m_skeleton;									//スケルトン
		AnimationClip*				m_animationClips = nullptr;					//アニメーションクリップ。
		int							m_numAnimationClips = 0;					//アニメーションクリップの数。
		Animation					m_animation;								//アニメーション。
		float						m_animationSpeed = 1.0f;					//アニメーションの速度。
		//ComputeAnimationVertexBuffer m_computeAnimationVertexBuffer;	// アニメーション済み頂点バッファの計算処理。

		Vector3						m_position = Vector3::Zero;				//座標
		Vector3						m_scale = Vector3::One;					//大きさ
		Quaternion					m_rotation = Quaternion::Identity;		//回転

		Model						m_model;								//モデルクラス
		Model						m_shadowModel;
		Model						m_zprepassModel;                  // ZPrepassで描画されるモデル
		Model						m_renderToGBufferModel;				// RenderToGBufferで描画されるモデル
		Model						m_frowardRenderModel;					 // フォワードレンダリングの描画パスで描画されるモデル
		Model						m_translucentModel;					// 半透明モデル。
		ModelInitData				m_modelInitData;						//モデルを初期化するための情報を設定するクラス
		Texture						m_lampTextrue;							//ランプテクスチャ


		Model						m_shadowModels[NUM_SHADOW_MAP];	// シャドウマップに描画するモデル
		ConstantBuffer				m_drawShadowMapCameraParamCB[NUM_SHADOW_MAP];// シャドウマップ作成時に必要なカメラパラメータ用の定数バッファ。
		bool						m_isShadowCaster = true;			// シャドウキャスターフラグ


	};
}

