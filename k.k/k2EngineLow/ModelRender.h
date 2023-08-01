#pragma once

namespace nsK2EngineLow {

	/// <summary>
   /// フォワードレンダリング用のモデル初期化構造体。
   /// </summary>
   /// <remark>
   /// ModelInitDataを継承しています。
   /// フォワードレンダリングのために必要なデフォルト設定をコンストラクタで行ってくれます。
   /// </remark>
	struct ModelInitDataFR :public ModelInitData
	{
		ModelInitDataFR()
		{
			m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
	};

	class ModelRender
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
			const char* tkmFilepath, 
			AnimationClip* animationClips = nullptr, 
			int numAnimationClips=0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool shadow=true,
			bool toon=true,
			bool outline=true
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
		/// 影を受けるモデルの初期化
		/// </summary>
		/// <param name="tkmFilepath">tkmファイルパス</param>
		void InitShadow(
			const char* tkmFilepath
		);

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
			g_renderingEngine->AddModelList(this);
			g_renderingEngine->Add3DModelToZPrepass(this);
		}

		/// <summary>
		/// モデルを描画する(RenderingEngineで描画するときに呼び出す)
		/// </summary>
		/// <param name="rc"></param>
		void OnRenderModel(RenderContext& rc)
		{
			m_model.Draw(rc);
		}

		/// <summary>
		/// ZPrepassモデルを描画する
		/// </summary>
		/// <param name="rc"></param>
		void OnZPrepass(RenderContext& rc)
		{
			m_zprepassModel.Draw(rc);
		}

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
		/// モデルを取得する
		/// </summary>
		/// <returns>モデル</returns>
		Model& GetModel()
		{
			return m_model;
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
		/// 更新処理
		/// </summary>
		void Update();

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
		void MakeDirectionData();

	// メンバ変数
	private:
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
		Model m_zprepassModel;                  // ZPrepassで描画されるモデル
		Model						m_frowardRenderModel;					 // フォワードレンダリングの描画パスで描画されるモデル
		ModelInitData				m_modelInitData;						//モデルを初期化するための情報を設定するクラス
		ModelInitDataFR             m_modelInitDataFR;

	};
}

