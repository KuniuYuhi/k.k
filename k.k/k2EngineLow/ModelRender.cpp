#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow {

	void ModelRender::Init(const char* tkmFilePath, const wchar_t* lampTextureFIlePath, AnimationClip* animationClips,
		int numAnimationClips, EnModelUpAxis enModelUpAxis, bool isShadowCaster, bool isToon,bool isOutline)
	{
		//�X�P���g����������
		InitSkeleton(tkmFilePath);
		//�A�j���[�V�����̏�����
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);

		//�t�H���[�h�����_�����O�p�̃��f����������
		InitModelOnfrowardRender(
			*g_renderingEngine, tkmFilePath, lampTextureFIlePath, enModelUpAxis, isShadowCaster,isToon,isOutline);

		//GBuffer�`��p�̃��f����������
		InitModelOnRenderGBuffer(*g_renderingEngine, tkmFilePath, enModelUpAxis, isShadowCaster);
		//ZPrepass�`��p�̃��f�����������B
		InitModelOnZprepass(tkmFilePath, enModelUpAxis);
		//�V���h�E�}�b�v�`��p�̃��f���̏�����
		InitModelOnShadowMap(
			*g_renderingEngine, tkmFilePath, enModelUpAxis
		);
	}

	//ZPrepass���f���̏�����
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
		//tkm�t�@�C���p�X��ݒ�
		modelInitData.m_tkmFilePath = tkmFilePath;
		//�V�F�[�_�[�̃t�@�C���p�X��ݒ�
		modelInitData.m_fxFilePath = "Assets/shader/model.fx";
		//���f���̏������ݒ�
		modelInitData.m_modelUpAxis = enModelUpAxis;

		SetupVertexShaderEntryPointFunc(modelInitData);
		
		int expandSRVNo = 0;
		//�g�D�[���V�F�[�_�[���g�p����Ȃ�
		if (isToon == true)
		{
			//�g��SRV�Ƀ����v�e�N�X�`����ݒ肷��
			m_lampTextrue.InitFromDDSFile(lampTextureFIlePath);
			modelInitData.m_expandShaderResoruceView[expandSRVNo] =
				&m_lampTextrue;
			//UV�T���v�����N�����v�ɂ���
			//�N�����v��UV���W���P�𒴂����Ƃ��Ɏ�O�̐F���T���v�����O����悤�ɂ���
			modelInitData.addressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			modelInitData.addressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		}
		expandSRVNo++;
		//�֊s����`�悷��Ȃ�
		if (isOutline == true)
		{
			//�֊s��
			//�g��SRV��ZPrepass�ō쐬���ꂽ�[�x�e�N�X�`����ݒ肷��
			modelInitData.m_expandShaderResoruceView[expandSRVNo] =
				&g_renderingEngine->GetZPrepassDepthTexture();
		}
		expandSRVNo++;
		g_renderingEngine->QueryShadowMapTexture([&](Texture& shadowMap) {
			//�V���h�E�}�b�v���g��SRV�ɐݒ肷��
			modelInitData.m_expandShaderResoruceView[expandSRVNo] = &shadowMap;
			expandSRVNo++;
			});
		
		SetupPixelShaderEntryPointFuncToFrowardModel(modelInitData, isShadowCaster, isToon);

		//���C�g�̏����쐬
		MakeDirectionData(modelInitData);

		//�쐬�����������f�[�^�����Ƀ��f��������������
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

		// ���_�V�F�[�_�[�̃G���g���[�|�C���g���Z�b�g�A�b�v�B
		//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
		SetupVertexShaderEntryPointFunc(modelInitData);
		//�V���h�E���V�[�o�[�Ȃ�
		if (isShadowReciever) {
			modelInitData.m_psEntryPointFunc = "PSMainShadowReciever";
		}
		//���f���̏�������w��B
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
		// ���_�V�F�[�_�[�̃G���g���[�|�C���g���Z�b�g�A�b�v�B
		SetupVertexShaderEntryPointFunc(modelInitData);
		modelInitData.m_fxFilePath = "Assets/shader/DrawShadowMap.fx";
		//�n�[�h�V���h�E�p�̃J���[�o�b�t�@�[�t�H�[�}�b�g
		modelInitData.m_colorBufferFormat[0] = g_hardShadowMapFormat.colorBufferFormat;

		//shadowModelArray���ς���m_shadowModels���ς��
		ConstantBuffer* cameraParamCBArray = m_drawShadowMapCameraParamCB;
		Model* shadowModelArray = m_shadowModels;
		for (int shadowMapNo = 0; shadowMapNo < NUM_SHADOW_MAP; shadowMapNo++)
		{
			shadowModelArray[shadowMapNo].Init(modelInitData);
		}
	}

	void ModelRender::SetupVertexShaderEntryPointFunc(ModelInitData& modelInitData)
	{
		//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
		modelInitData.m_vsEntryPointFunc = "VSMain";
		//�A�j���[�V�������ݒ肳��Ă���Ȃ�
		if (m_animationClips != nullptr)
		{
			//�X�P���g�����w�肷��
			modelInitData.m_skeleton = &m_skeleton;
			//�X�L��������p�̒��_�V�F�[�_�[��ݒ肷��B
			modelInitData.m_vsSkinEntryPointFunc = "VSMainSkin";
		}
	}

	void ModelRender::SetupPixelShaderEntryPointFuncToFrowardModel(
		ModelInitData& modelInitData, bool isShadowCaster, bool isToon)
	{
		//�V���h�E�L���X�^�[���g�D�[���V�F�[�f�B���O����Ȃ�
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

	//�g���ĂȂ�
	//�����̃��f�����t�H���[�h�����_�����O�̕`��p�X�ŕ`��
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

	//���f���̏����X�V
	void ModelRender::Update()
	{
		//�e�탂�f���̃��[���h���W���X�V
		UpdateWorldMatrixInModes();

		//�X�P���g��������������Ă�����
		if (m_skeleton.IsInited())
		{
			//�X�P���g�����X�V����
			m_skeleton.Update(m_frowardRenderModel.GetWorldMatrix());
		}

		//�A�j���[�V������i�߂�
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}

	void ModelRender::UpdateWorldMatrixInModes()
	{
		//���f���̃��[���h�s��̍X�V(���W�A��]�A�傫��)
		m_frowardRenderModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		//ZPrepass���f���̃��[���h�s��̍X�V(���W�A��]�A�傫��)
		m_zprepassModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);

		//Gbuffer�p�̃��f��������������Ă�����
		if (m_renderToGBufferModel.IsInited())
		{
			m_renderToGBufferModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
		//�V���h�E�}�b�v�`�惂�f��
		//�t�@�C���������ĂȂ�
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
		//�X�P���g���̃f�[�^��ǂݍ��݁B
		std::string skeletonFilePath = filePath;
		int pos = (int)skeletonFilePath.find(".tkm");
		skeletonFilePath.replace(pos, 4, ".tks");
		m_skeleton.Init(skeletonFilePath.c_str());
	}

	void ModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips, EnModelUpAxis enModelUpAxis)
	{
		m_animationClips = animationClips;
		m_numAnimationClips = numAnimationClips;

		//�A�j���[�V�������ݒ肳��Ă���Ȃ�
		if (m_animationClips != nullptr) {
			//�A�j���[�V�����̏������B
			m_animation.Init(
				m_skeleton,				//�A�j���[�V�����𗬂����ރX�P���g���B
				m_animationClips,		//�A�j���[�V�����N���b�v�B
				numAnimationClips		//�A�j���[�V�����N���b�v�̐��B
			);
		}
	}

	void ModelRender::InitCommon(RenderingEngine& renderingEngine, const char* tkmFilePath)
	{
		// ZPrepass�`��p�̃��f����������
		{
			ModelInitData modelInitData;
			modelInitData.m_tkmFilePath = tkmFilePath;
			modelInitData.m_fxFilePath = "Assets/shader/preset/ZPrepass.fx";
			modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32_FLOAT;

			m_zprepassModel.Init(modelInitData);
		}
	}

	//���C�g�̏����f�B�X�N���v�^�q�[�v�ɒ萔�o�b�t�@�[�Ƃ���
	//�o�^���邽�߂Ƀ��f���̏��������Ƃ��ēn��
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