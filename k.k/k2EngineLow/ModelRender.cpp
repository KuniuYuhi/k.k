#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow {

	void ModelRender::Init(const char* tkmFilePath, const wchar_t* lampTextureFIlePath, AnimationClip* animationClips,
		int numAnimationClips, EnModelUpAxis enModelUpAxis, bool isShadowCaster, bool isToon,bool isOutline)
	{
		//スケルトンを初期化
		InitSkeleton(tkmFilePath);
		//アニメーションの初期化
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);

		//フォワードレンダリング用のモデルを初期化
		InitModelOnfrowardRender(
			*g_renderingEngine, tkmFilePath, lampTextureFIlePath, enModelUpAxis, isShadowCaster,isToon,isOutline);

		//GBuffer描画用のモデルを初期化
		InitModelOnRenderGBuffer(*g_renderingEngine, tkmFilePath, enModelUpAxis, isShadowCaster);
		//ZPrepass描画用のモデルを初期化。
		InitModelOnZprepass(tkmFilePath, enModelUpAxis);
		//シャドウマップ描画用のモデルの初期化
		InitModelOnShadowMap(
			*g_renderingEngine, tkmFilePath, enModelUpAxis
		);
	}

	//ZPrepassモデルの初期化
	void ModelRender::InitModelOnZprepass(const char* tkmFilePath, EnModelUpAxis modelUpAxis)
	{
		ModelInitData modelInitData;
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_fxFilePath = "Assets/shader/ZPrepass.fx";
		modelInitData.m_modelUpAxis = modelUpAxis;

		SetupVertexShaderEntryPointFunc(modelInitData);

		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		m_zprepassModel.Init(modelInitData);
	}

	void ModelRender::InitModelOnfrowardRender(
		RenderingEngine& renderingEngine, 
		const char* tkmFilePath, 
		const wchar_t* lampTextureFIlePath,
		EnModelUpAxis enModelUpAxis, 
		bool isShadowCaster, bool isToon, bool isOutline)
	{
		ModelInitData modelInitData;
		//tkmファイルパスを設定
		modelInitData.m_tkmFilePath = tkmFilePath;
		//シェーダーのファイルパスを設定
		modelInitData.m_fxFilePath = "Assets/shader/model.fx";
		//モデルの上方向を設定
		modelInitData.m_modelUpAxis = enModelUpAxis;

		SetupVertexShaderEntryPointFunc(modelInitData);
		
		int expandSRVNo = 0;
		//トゥーンシェーダーを使用するなら
		if (isToon == true)
		{
			//拡張SRVにランプテクスチャを設定する
			m_lampTextrue.InitFromDDSFile(lampTextureFIlePath);
			modelInitData.m_expandShaderResoruceView[expandSRVNo] =
				&m_lampTextrue;
			//UVサンプラをクランプにする
			//クランプ＝UV座標が１を超えたときに手前の色をサンプリングするようにする
			modelInitData.addressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			modelInitData.addressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		}
		expandSRVNo++;
		//輪郭線を描画するなら
		if (isOutline == true)
		{
			//輪郭線
			//拡張SRVにZPrepassで作成された深度テクスチャを設定する
			modelInitData.m_expandShaderResoruceView[expandSRVNo] =
				&g_renderingEngine->GetZPrepassDepthTexture();
		}
		expandSRVNo++;
		g_renderingEngine->QueryShadowMapTexture([&](Texture& shadowMap) {
			//シャドウマップを拡張SRVに設定する
			modelInitData.m_expandShaderResoruceView[expandSRVNo] = &shadowMap;
			expandSRVNo++;
			});
		
		SetupPixelShaderEntryPointFuncToFrowardModel(modelInitData, isShadowCaster, isToon);

		//ライトの情報を作成
		MakeDirectionData(modelInitData);

		//作成した初期化データを元にモデルを初期化する
		m_frowardRenderModel.Init(modelInitData);
	}

	void ModelRender::InitModelOnRenderGBuffer(
		RenderingEngine& renderingEngine, 
		const char* tkmFilePath, 
		EnModelUpAxis enModelUpAxis, 
		bool isShadowReciever)
	{
		ModelInitData modelInitData;
		modelInitData.m_fxFilePath = "Assets/shader/RenderToGBufferFor3DModel.fx";

		// 頂点シェーダーのエントリーポイントをセットアップ。
		//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する。
		SetupVertexShaderEntryPointFunc(modelInitData);
		//シャドウレシーバーなら
		if (isShadowReciever) {
			modelInitData.m_psEntryPointFunc = "PSMainShadowReciever";
		}
		//モデルの上方向を指定。
		modelInitData.m_modelUpAxis = enModelUpAxis;

		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
		modelInitData.m_colorBufferFormat[1] = DXGI_FORMAT_R8G8B8A8_SNORM;
		modelInitData.m_colorBufferFormat[2] = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_renderToGBufferModel.Init(modelInitData);
	}

	void ModelRender::InitModelOnShadowMap(
		RenderingEngine& renderingEngine, 
		const char* tkmFilePath, 
		EnModelUpAxis modelUpAxis, 
		bool isFrontCullingOnDrawShadowMap)
	{
		ModelInitData modelInitData;
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_modelUpAxis = modelUpAxis;
		modelInitData.m_cullMode = 
			isFrontCullingOnDrawShadowMap ? D3D12_CULL_MODE_FRONT : D3D12_CULL_MODE_BACK;
		// 頂点シェーダーのエントリーポイントをセットアップ。
		SetupVertexShaderEntryPointFunc(modelInitData);
		modelInitData.m_fxFilePath = "Assets/shader/DrawShadowMap.fx";
		//ハードシャドウ用のカラーバッファーフォーマット
		modelInitData.m_colorBufferFormat[0] = g_hardShadowMapFormat.colorBufferFormat;

		//shadowModelArrayが変わるとm_shadowModelsも変わる
		ConstantBuffer* cameraParamCBArray = m_drawShadowMapCameraParamCB;
		Model* shadowModelArray = m_shadowModels;
		for (int shadowMapNo = 0; shadowMapNo < NUM_SHADOW_MAP; shadowMapNo++)
		{
			shadowModelArray[shadowMapNo].Init(modelInitData);
		}
	}

	void ModelRender::SetupVertexShaderEntryPointFunc(ModelInitData& modelInitData)
	{
		//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する。
		modelInitData.m_vsEntryPointFunc = "VSMain";
		//アニメーションが設定されているなら
		if (m_animationClips != nullptr)
		{
			//スケルトンを指定する
			modelInitData.m_skeleton = &m_skeleton;
			//スキンがある用の頂点シェーダーを設定する。
			modelInitData.m_vsSkinEntryPointFunc = "VSMainSkin";
		}
	}

	void ModelRender::SetupPixelShaderEntryPointFuncToFrowardModel(
		ModelInitData& modelInitData, bool isShadowCaster, bool isToon)
	{
		//シャドウキャスターかつトゥーンシェーディングするなら
		if (isShadowCaster == true&& isToon == true)
		{
			modelInitData.m_psEntryPointFunc = "PSShadowCasterToonMain";
		}
		else if(isShadowCaster)
		{
			modelInitData.m_psEntryPointFunc = "PSShadowCasterMain";
		}
		else if(isToon)
		{
			modelInitData.m_psEntryPointFunc = "PSToonMain";
		}
		else
		{
			modelInitData.m_psEntryPointFunc = "PSMain";
		}
		
	}

	//使ってない
	//引数のモデルをフォワードレンダリングの描画パスで描画
	void ModelRender::InitForwardRendering(RenderingEngine& renderingEngine, ModelInitData& modelInitData)
	{
		m_frowardRenderModel.Init(modelInitData);

		InitCommon(renderingEngine, modelInitData.m_fxFilePath);
	}

	void ModelRender::InitSkyCube(ModelInitData& initData)
	{
		initData.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		m_frowardRenderModel.Init(initData);

		m_frowardRenderModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	}

	//モデルの情報を更新
	void ModelRender::Update()
	{
		//各種モデルのワールド座標を更新
		UpdateWorldMatrixInModes();

		//スケルトンが初期化されていたら
		if (m_skeleton.IsInited())
		{
			//スケルトンを更新する
			m_skeleton.Update(m_frowardRenderModel.GetWorldMatrix());
		}

		//アニメーションを進める
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}

	void ModelRender::UpdateWorldMatrixInModes()
	{
		//モデルのワールド行列の更新(座標、回転、大きさ)
		m_frowardRenderModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		//ZPrepassモデルのワールド行列の更新(座標、回転、大きさ)
		m_zprepassModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);

		//Gbuffer用のモデルが初期化されていたら
		if (m_renderToGBufferModel.IsInited())
		{
			m_renderToGBufferModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
		//シャドウマップ描画モデル
		//ファイル情報入ってない
		for (auto& model : m_shadowModels) {
			model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
	}

	void ModelRender::OnRenderShadowMap(
		RenderContext& rc, 
		int shadowMapNo, Camera& lightCamera, const Matrix& lvpMatrix)
	{
		Vector4 cameraParam;
		cameraParam.x = g_camera3D->GetNear();
		cameraParam.y = g_camera3D->GetFar();
		m_drawShadowMapCameraParamCB[shadowMapNo].CopyToVRAM(cameraParam);
		m_shadowModels[shadowMapNo].Draw(
			rc,
			g_matIdentity,
			lvpMatrix,
			1
		);

		/*m_shadowModels[shadowMapNo].Draw(
			rc,
			lightCamera
		);*/

		//if (m_isShadowCaster) {
			
		//}
	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		//スケルトンのデータを読み込み。
		std::string skeletonFilePath = filePath;
		int pos = (int)skeletonFilePath.find(".tkm");
		skeletonFilePath.replace(pos, 4, ".tks");
		m_skeleton.Init(skeletonFilePath.c_str());
	}

	void ModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips, EnModelUpAxis enModelUpAxis)
	{
		m_animationClips = animationClips;
		m_numAnimationClips = numAnimationClips;

		//アニメーションが設定されているなら
		if (m_animationClips != nullptr) {
			//アニメーションの初期化。
			m_animation.Init(
				m_skeleton,				//アニメーションを流し込むスケルトン。
				m_animationClips,		//アニメーションクリップ。
				numAnimationClips		//アニメーションクリップの数。
			);
		}
	}

	void ModelRender::InitCommon(RenderingEngine& renderingEngine, const char* tkmFilePath)
	{
		// ZPrepass描画用のモデルを初期化
		{
			ModelInitData modelInitData;
			modelInitData.m_tkmFilePath = tkmFilePath;
			modelInitData.m_fxFilePath = "Assets/shader/preset/ZPrepass.fx";
			modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32_FLOAT;

			m_zprepassModel.Init(modelInitData);
		}
	}

	//ライトの情報をディスクリプタヒープに定数バッファーとして
	//登録するためにモデルの初期化情報として渡す
	void ModelRender::MakeDirectionData(ModelInitData& modelInitData)
	{
		modelInitData.m_expandConstantBuffer = &g_renderingEngine->GetSceneLight();
		modelInitData.m_expandConstantBufferSize = sizeof(g_renderingEngine->GetSceneLight());
	}



	void ModelRender::OnZPrepass(RenderContext& rc)
	{
		m_zprepassModel.Draw(rc, 1);
	}

	void ModelRender::OnRenderToGBuffer(RenderContext& rc)
	{
		if (m_renderToGBufferModel.IsInited()) {
			m_renderToGBufferModel.Draw(rc, 1);
		}
	}

	void ModelRender::OnForwardRender(RenderContext& rc)
	{
		if (m_frowardRenderModel.IsInited()) {
			m_frowardRenderModel.Draw(rc, 1);
		}
	}

	void ModelRender::OnTlanslucentRender(RenderContext& rc)
	{
		if (m_translucentModel.IsInited()) {
			m_translucentModel.Draw(rc, 1);
		}
	}

}