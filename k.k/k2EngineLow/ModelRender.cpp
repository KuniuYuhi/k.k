#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow {

	void ModelRender::Init(const char* tkmFilepath,AnimationClip* animationClips, int numAnimationClips, EnModelUpAxis enModelUpAxis, bool shadow)
	{
		//tkm�t�@�C���p�X��ݒ�
		m_modelInitData.m_tkmFilePath = tkmFilepath;
		//�V�F�[�_�[�̃t�@�C���p�X��ݒ�
		m_modelInitData.m_fxFilePath = "Assets/shader/model.fx";
		//���f���̏������ݒ�
		m_modelInitData.m_modelUpAxis = enModelUpAxis;
		//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
		m_modelInitData.m_vsEntryPointFunc = "VSMain";
		//�X�P���g����������
		InitSkeleton(tkmFilepath);
		//�A�j���[�V�����̏�����
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);

		//�A�j���[�V�������ݒ肳��Ă���Ȃ�
		if (animationClips != nullptr)
		{
			//�X�P���g�����w�肷��
			m_modelInitData.m_skeleton = &m_skeleton;
			//�X�L��������p�̒��_�V�F�[�_�[��ݒ肷��B
			m_modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
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

	//�e���󂯂郂�f���̏�����
	void ModelRender::InitShadow(const char* tkmFilepath)
	{
		m_modelInitData.m_tkmFilePath = tkmFilepath;
		m_modelInitData.m_fxFilePath = "Assets/shader/ShadowReciever.fx";

		m_modelInitData.m_expandShaderResoruceView[0] = &g_renderingEngine->GatShadowMapTexture();

		//���C�g�J�����̃r���[�v���W�F�N�V�����s���ݒ肷��
		g_renderingEngine->SetmLVP(g_renderingEngine->GetLightCamera().GetViewProjectionMatrix());

		/*m_modelInitData.m_expandConstantBuffer = (void*)&g_renderingEngine->GetLightCamera().GetProjectionMatrix();
		m_modelInitData.m_expandConstantBufferSize = sizeof(g_renderingEngine->GetLightCamera().GetProjectionMatrix());*/

		MakeDirectionData();

		m_model.Init(m_modelInitData);
	}

	//���f���̏����X�V
	void ModelRender::Update()
	{
		//���[���h�s��̍X�V(���W�A��]�A�傫��)
		m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

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
			g_renderingEngine->SetmLVP(g_renderingEngine->GetLightCamera().GetViewProjectionMatrix());
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

	//�f�B���N�V�������C�g�̏����f�B�X�N���v�^�q�[�v�ɒ萔�o�b�t�@�[�Ƃ���
	//�o�^���邽�߂Ƀ��f���̏��������Ƃ��ēn��
	void ModelRender::MakeDirectionData()
	{
		m_modelInitData.m_expandConstantBuffer = &g_renderingEngine->GetSceneLight();
		m_modelInitData.m_expandConstantBufferSize = sizeof(g_renderingEngine->GetSceneLight());
	}


}