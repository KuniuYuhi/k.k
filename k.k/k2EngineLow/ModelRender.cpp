#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow {

	void ModelRender::Init(const char* tkmFilepath, const wchar_t* lampTextureFIlePath, AnimationClip* animationClips,
		int numAnimationClips, EnModelUpAxis enModelUpAxis, bool shadow, bool toon,bool outline)
	{
		//tkm�t�@�C���p�X��ݒ�
		m_modelInitData.m_tkmFilePath = tkmFilepath;
		//�V�F�[�_�[�̃t�@�C���p�X��ݒ�
		m_modelInitData.m_fxFilePath = "Assets/shader/model.fx";
		//���f���̏������ݒ�
		m_modelInitData.m_modelUpAxis = enModelUpAxis;
		
		//�X�P���g����������
		InitSkeleton(tkmFilepath);
		//�A�j���[�V�����̏�����
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);

		SetupVertexShaderEntryPointFunc(m_modelInitData);

		//GBuffer�`��p�̃��f����������
		InitModelOnRenderGBuffer(*g_renderingEngine, tkmFilepath, enModelUpAxis, shadow);
		//ZPrepass�`��p�̃��f�����������B
		InitModelOnZprepass(tkmFilepath, enModelUpAxis);

		//�g�D�[���V�F�[�_�[���g�p����Ȃ�
		if (toon == true)
		{
			//�g��SRV�Ƀ����v�e�N�X�`����ݒ肷��
			m_lampTextrue.InitFromDDSFile(lampTextureFIlePath);
			m_modelInitData.m_expandShaderResoruceView[0] = &m_lampTextrue;
			m_modelInitData.m_psEntryPointFunc = "PSToonMain";

			//UV�T���v�����N�����v�ɂ���
			//�N�����v��UV���W���P�𒴂����Ƃ��Ɏ�O�̐F���T���v�����O����悤�ɂ���
			m_modelInitData.addressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			m_modelInitData.addressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		}
		//�֊s����`�悷��Ȃ�
		if (outline == true)
		{
			//�֊s��
			//�g��SRV��ZPrepass�ō쐬���ꂽ�[�x�e�N�X�`����ݒ肷��
			m_modelInitData.m_expandShaderResoruceView[2] =
				&g_renderingEngine->GetZPrepassDepthTexture();
		}
		
		//�f�B���N�V�������C�g�̏����쐬
		MakeDirectionData();

		//�쐬�����������f�[�^�����Ƀ��f��������������
		m_model.Init(m_modelInitData);

		//�V���h�E�}�b�v�`��p�̃��f���̏�����
		if (shadow == true)
		{
			//�V���h�E�}�b�v�`��p�̒��_�V�F�[�_�[��ݒ肷��
			m_modelInitData.m_psEntryPointFunc = "PSShadowMapMain";
			m_modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT;
			//�V���h�E���f����������
			m_shadowModel.Init(m_modelInitData);
		}
	}

	//ZPrepass���f���̏�����
	void ModelRender::InitModelOnZprepass(const char* tkmFilePath, EnModelUpAxis modelUpAxis)
	{
		ModelInitData modelInitData;
		modelInitData.m_tkmFilePath = tkmFilePath;
		modelInitData.m_fxFilePath = "Assets/shader/ZPrepass.fx";
		modelInitData.m_modelUpAxis = modelUpAxis;

		//�X�L���Ȃ�
		modelInitData.m_vsEntryPointFunc = "VSMain";

		if (m_animationClips != nullptr) {
			// �A�j���[�V��������B
			modelInitData.m_vsSkinEntryPointFunc = "VSMainSkin";
		}

		// ���_�̎��O�v�Z�������g���B
		//modelInitData.m_computedAnimationVertexBuffer = &m_computeAnimationVertexBuffer;

		if (m_animationClips != nullptr) {
			//�X�P���g�����w�肷��B
			modelInitData.m_skeleton = &m_skeleton;
		}
		modelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		m_zprepassModel.Init(modelInitData);
	}

	void ModelRender::InitModelOnTranslucent(RenderingEngine& renderingEngine, const char* tkmFilePath, EnModelUpAxis enModelUpAxis, bool isShadowReciever)
	{
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
		modelInitData.m_vsEntryPointFunc = "VSMain";
		//�A�j���[�V�������ݒ肳��Ă���Ȃ�
		if (m_animationClips != nullptr)
		{
			//�X�P���g�����w�肷��
			modelInitData.m_skeleton = &m_skeleton;
			//�X�L��������p�̒��_�V�F�[�_�[��ݒ肷��B
			modelInitData.m_vsSkinEntryPointFunc = "VSMainSkin";
		}

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

	void ModelRender::InitModelOnShadowMap(RenderingEngine& renderingEngine, const char* tkmFilePath, EnModelUpAxis modelUpAxis, bool isFrontCullingOnDrawShadowMap)
	{
	}

	void ModelRender::SetupVertexShaderEntryPointFunc(ModelInitData& modelInitData)
	{
		//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
		m_modelInitData.m_vsEntryPointFunc = "VSMain";
		//�A�j���[�V�������ݒ肳��Ă���Ȃ�
		if (m_animationClips != nullptr)
		{
			//�X�P���g�����w�肷��
			m_modelInitData.m_skeleton = &m_skeleton;
			//�X�L��������p�̒��_�V�F�[�_�[��ݒ肷��B
			m_modelInitData.m_vsSkinEntryPointFunc = "VSMainSkin";
		}
	}

	//�g���ĂȂ�
	//�����̃��f�����t�H���[�h�����_�����O�̕`��p�X�ŕ`��
	void ModelRender::InitForwardRendering(RenderingEngine& renderingEngine, ModelInitData& modelInitData)
	{
		m_frowardRenderModel.Init(modelInitData);

		InitCommon(renderingEngine, modelInitData.m_fxFilePath);
	}

	//�e���󂯂郂�f���̏�����
	void ModelRender::InitShadow(const char* tkmFilepath)
	{
		m_modelInitData.m_tkmFilePath = tkmFilepath;
		m_modelInitData.m_fxFilePath = "Assets/shader/ShadowReciever.fx";

		m_modelInitData.m_expandShaderResoruceView[0] = &g_renderingEngine->GatShadowMapTexture();

		//���C�g�J�����̃r���[�v���W�F�N�V�����s���ݒ肷��
		g_renderingEngine->SetmLVP(g_renderingEngine->GetLightCamera().GetViewProjectionMatrix());

		InitModelOnRenderGBuffer(*g_renderingEngine, tkmFilepath, enModelUpAxisY, true);

		/*m_modelInitData.m_expandConstantBuffer = (void*)&g_renderingEngine->GetLightCamera().GetProjectionMatrix();
		m_modelInitData.m_expandConstantBufferSize = sizeof(g_renderingEngine->GetLightCamera().GetProjectionMatrix());*/

		MakeDirectionData();

		m_model.Init(m_modelInitData);
	}

	void ModelRender::InitSkyCube(ModelInitData& initData)
	{
		initData.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		m_model.Init(initData);

		m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	}

	//���f���̏����X�V
	void ModelRender::Update()
	{
		//���f���̃��[���h�s��̍X�V(���W�A��]�A�傫��)
		m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		//ZPrepass���f���̃��[���h�s��̍X�V(���W�A��]�A�傫��)
		m_zprepassModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);

		//Gbuffer�p�̃��f��������������Ă�����
		if (m_renderToGBufferModel.IsInited())
		{
			m_renderToGBufferModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}
		//�X�P���g��������������Ă�����
		if (m_skeleton.IsInited())
		{
			//�X�P���g�����X�V����
			m_skeleton.Update(m_model.GetWorldMatrix());
		}
		//�V���h�E���f��������������Ă�����
		if (m_shadowModel.IsInited())
		{
			m_shadowModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		}

		//�A�j���[�V������i�߂�
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
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

	//�f�B���N�V�������C�g�̏����f�B�X�N���v�^�q�[�v�ɒ萔�o�b�t�@�[�Ƃ���
	//�o�^���邽�߂Ƀ��f���̏��������Ƃ��ēn��
	void ModelRender::MakeDirectionData()
	{
		m_modelInitData.m_expandConstantBuffer = &g_renderingEngine->GetSceneLight();
		m_modelInitData.m_expandConstantBufferSize = sizeof(g_renderingEngine->GetSceneLight());
	}



	void ModelRender::OnRenderToGBuffer(RenderContext& rc)
	{
		if (m_renderToGBufferModel.IsInited()) {
			m_renderToGBufferModel.Draw(rc, 1);
		}
	}

}