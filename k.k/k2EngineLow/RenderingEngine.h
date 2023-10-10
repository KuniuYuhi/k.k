#pragma once

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
		//// �f�B�t�@�[�h���C�e�B���O�p�̒萔�o�b�t�@
		//struct SDeferredLightingCB
		//{
		//	Light m_light;              // ���C�g
		//	Matrix mlvp[MAX_DIRECTIONAL_LIGHT][NUM_SHADOW_MAP]; // ���C�g�r���[�v���W�F�N�V�����s��B
		//	float m_iblLuminance;       // IBL�̖��邳�B
		//	int m_isIBL;                // IBL���s���B
		//	int m_isEnableRaytracing;   // ���C�g�����s���Ă���B
		//};



		  /// <summary>
		/// �C�x���g�B
		/// </summary>
		enum EnEvent {
			enEventReInitIBLTexture,    // IBL�e�N�X�`�����ď��������ꂽ�B
		};

		/// <summary>
		/// �����_�[�^�[�Q�b�g�⃉�C�g�̏�����
		/// </summary>
		void Init();

		/// <summary>
		/// �����_�����O�^�[�Q�b�g�̏�����
		/// </summary>
		void InitRenderTargets();

		/// <summary>
		/// �e�N�X�`����\��t���邽�߂̃X�v���C�g�̏�����
		/// </summary>
		void InitCopyToFrameBufferSprite();

		/// <summary>
		/// �`��I�u�W�F�N�g��ǉ��B
		/// </summary>
		/// <param name="renderObject"></param>
		void AddRenderObject(IRenderer* renderObject)
		{
			m_renderObjects.push_back(renderObject);
		}

		/// <summary>
		/// ���f�������_�[�N���X�����X�g�ɒǉ�����
		/// </summary>
		/// <param name="modelRender"></param>
		void AddModelList(ModelRender* modelRender)
		{
			m_modelList.emplace_back(modelRender);
		}

		/// <summary>
		/// Gbuffer���f��
		/// </summary>
		/// <param name="modelRender"></param>
		void AddGBufferModelList(ModelRender* modelRender)
		{
			m_gBufferModelList.emplace_back(modelRender);
		}

		/// <summary>
		/// �X�v���C�g�����_�[�N���X�����X�g�ɒǉ�����
		/// </summary>
		/// <param name="spriteRender"></param>
		void AddSpriteList(SpriteRender* spriteRender)
		{
			m_spriteList.emplace_back(spriteRender);
		}

		/// <summary>
		/// �t�H���g�����_�[�N���X�����X�g�ɒǉ�����
		/// </summary>
		/// <param name="fontRender"></param>
		void AddFontList(FontRender* fontRender)
		{
			m_fontList.emplace_back(fontRender);
		}

		/// <summary>
		/// �V���h�E�}�b�v�`��p�̃��f����`�悷��
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		/// <param name="camera">�J����</param>
		void ShadowModelRendering(RenderContext& rc, Camera& camera);

		/// <summary>
		/// �C�x���g���X�i�[��ǉ��B
		/// </summary>
		void AddEventListener(
			void* pListenerObj,
			std::function<void(EnEvent enEvent)> listenerFunc
		)
		{
			m_eventListeners.push_back({ pListenerObj, listenerFunc });
		}
		/// <summary>
		/// �C�x���g���X�i�[���폜�B
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
		/// �`�揈�������s
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void Execute(RenderContext& rc);

		/// <summary>
		/// �V�[�����C�g���擾
		/// </summary>
		/// <returns>�V�[�����C�g</returns>
		SceneLight& GetSceneLight()
		{
			return m_sceneLight;
		}

		/// <summary>
		/// ���C�g�r���[�v���W�F�N�V�����s���ݒ肷��
		/// </summary>
		/// <param name="LVP">���C�g�r���[�v���W�F�N�V�����s��</param>
		void SetmLVP(Matrix LVP)
		{
			m_sceneLight.SetmLVP(LVP);
		}

		/// <summary>
		/// �g�D�[���}�b�v�e�N�X�`�����擾
		/// </summary>
		/// <returns></returns>
		Texture& GetToonTextrue()
		{
			return m_toontexture;
		}

		/// <summary>
		/// �g�D�[���}�b�v�̃e�N�X�`����ݒ肷��
		/// </summary>
		void SetToonTextureDDS();

		/// <summary>
	   /// ZPrepass�̕`��p�X�Ƀ��f����ǉ�
	   /// </summary>
	   /// <param name="model"></param>
		void Add3DModelToZPrepass(ModelRender* model)
		{
			m_zprepassModelList.emplace_back(model);
		}

		/// <summary>
		/// ZPrepass�ō쐬���ꂽ�[�x�e�N�X�`�����擾
		/// </summary>
		/// <returns></returns>
		Texture& GetZPrepassDepthTexture()
		{
			return m_zprepassRenderTarget.GetRenderTargetTexture();
		}

		/////////////////////////////////////////////////////
		///�V���h�E�}�b�v�̊֐�
		/////////////////////////////////////////////////////

		/// <summary>
		/// �V���h�E�}�b�v�̃e�N�X�`�����擾
		/// </summary>
		/// <returns>�V���h�E�}�b�v�̃e�N�X�`��</returns>
		Texture& GatShadowMapTexture()
		{
			return m_shadow.GetShadowMapTextrue();
		}

		/// <summary>
		/// ���C�g�J�������擾
		/// </summary>
		/// <returns>���C�g�J����</returns>
		Camera& GetLightCamera()
		{
			return m_shadow.GetLightCamera();
		}

		/////////////////////////////////////////////////////
		///�f�B���N�V�������C�g�̊֐�
		/////////////////////////////////////////////////////
		
		/// <summary>
		/// �f�B���N�V�������C�g�̐ݒ�
		/// </summary>
		/// <param name="lightNo">���C�g�ԍ�</param>
		/// <param name="direction">���C�g�̕���</param>
		/// <param name="color">���C�g�̐F</param>
		void SetDerectionLight(int lightNo, Vector3 direction, Vector3 color)
		{
			m_sceneLight.SetDirectionLight(lightNo, direction, color);
		}

		/// <summary>
		/// �f�B���N�V�������C�g�̌��̕�����ݒ肷��
		/// </summary>
		/// <param name="direction">����</param>
		void SetDirLightDirection(Vector3 direction)
		{
			m_sceneLight.SetDirLightDirection(direction);
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̐F��ݒ肷��
		/// </summary>
		/// <param name="color">�F</param>
		void SetDirLightColor(Vector3 color)
		{
			m_sceneLight.SetDirLightColor(color);
		}

		/// <summary>
		/// �f�B���N�V�������C�g�̌��̕������擾����
		/// </summary>
		/// <returns>���̕���</returns>
		const Vector3& GetDirLigDirection() const
		{
			return m_sceneLight.GetDirLigDirection();
		}
		/// <summary>
		/// �f�B���N�V�������C�g�̌��̐F���擾����
		/// </summary>
		/// <returns>�F</returns>
		const Vector3& GetDirLigColor() const
		{
			return m_sceneLight.GetDirLigColor();
		}

		/////////////////////////////////////////////////////////////
		///�����̊֐�
		/////////////////////////////////////////////////////////////

		/// <summary>
		/// �����̐ݒ�
		/// </summary>
		/// <param name="ambient">�����̋���</param>
		void SetAmbient(Vector3 ambient)
		{
			m_sceneLight.SetAmbient(ambient);
		}

		/////////////////////////////////////////////////////////////
		///�|�C���g���C�g�̊֐�
		/////////////////////////////////////////////////////////////

		/// <summary>
		/// �|�C���g���C�g��ݒ肷��
		/// </summary>
		/// <param name="pos">���C�g�̈ʒu</param>
		/// <param name="color">���C�g�̐F</param>
		/// <param name="range">x�Ƀ��C�g�̉e���͈�,y�ɉe���͈͂ɗݏ悷��p�����[�^</param>
		void SetPointLight(Vector3 pos, Vector3 color, Vector3 range)
		{
			m_sceneLight.SetPointLight(pos, color, range);
		}

		/// <summary>
		/// �|�C���g���C�g�̍��W��ݒ肷��
		/// </summary>
		/// <param name="pos"></param>
		void SetPointLightPosition(Vector3 pos)
		{
			m_sceneLight.SetPointLightPosition(pos);
		}

		/// <summary>
		/// �|�C���g���C�g�̐F��ݒ肷��
		/// </summary>
		/// <param name="color">�F</param>
		void SetPointLightColor(Vector3 color)
		{
			m_sceneLight.SetPointLightColor(color);
		}

		/// <summary>
		/// �e���͈͂Ɨݏ悷��p�����[�^��ݒ�
		/// </summary>
		/// <param name="attn">X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</param>
		void SetPointLightAttn(Vector3 attn)
		{
			m_sceneLight.SetPointLightAttn(attn);
		}

		/// <summary>
		/// �|�C���g���C�g���g�p����
		/// </summary>
		void UsePointLight()
		{
			m_sceneLight.UsePointLight();
		}

		/// <summary>
		/// �|�C���g���C�g���g�p���Ȃ�
		/// </summary>
		void UnUsePointLight()
		{
			m_sceneLight.UnUsePointLight();
		}

		/// <summary>
		/// �|�C���g���C�g�̈ʒu���擾����
		/// </summary>
		/// <returns>���W</returns>
		const Vector3& GetPointLightPosition() const
		{
			return m_sceneLight.GetPointLightPosition();
		}

		/// <summary>
		/// �|�C���g���C�g�̌��̐F���擾
		/// </summary>
		/// <returns>�F</returns>
		const Vector3& GetPointLightColor() const
		{
			return m_sceneLight.GetPointLightColor();
		}

		/// <summary>
		/// �X�|�b�g���C�g�̉e���͈͂Ɨݏ悷��p�����[�^���擾
		/// </summary>
		/// <returns>X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</returns>
		const Vector3& GetPointLightAttn() const
		{
			return m_sceneLight.GetPointLightAttn();
		}

		/// <summary>
		/// �|�C���g���C�g�͎g�p��?
		/// </summary>
		/// <returns>�g�p���Ȃ�true</returns>
		const int PointLightIsUse() const
		{
			return m_sceneLight.PointLightIsUse();
		}

		/////////////////////////////////////////////////////////////
		///�X�|�b�g���C�g�̊֐�
		/////////////////////////////////////////////////////////////

		/// <summary>
		/// �X�|�b�g���C�g��ݒ肷��
		/// </summary>
		/// <param name="pos">�ʒu</param>
		/// <param name="color">�F</param>
		/// <param name="range">x�ɉe���͈�,y�ɉe���͈͂ɗݏ悷��p�����[�^</param>
		/// <param name="direction">�Ǝ˕���</param>
		/// <param name="angle">x�͏Ǝˊp�x,���͉e���ɗݏ悷��p�����[�^</param>
		void SetSpotLight(Vector3 pos, Vector3 color, Vector3 attn, Vector3 direction, Vector3 angle)
		{
			m_sceneLight.SetSpotLight(pos, color, attn, direction, angle);
		}

		/// <summary>
		/// �X�|�b�g���C�g�̈ʒu��ݒ肷��
		/// </summary>
		/// <param name="pos">���W</param>
		void SetSpotLightPosition(Vector3 pos)
		{
			m_sceneLight.SetSpotLightPosition(pos);
		}

		/// <summary>
		/// �X�|�b�g���C�g�̃��C�g�F�̐ݒ�
		/// </summary>
		/// <param name="color">�F</param>
		void SetSpotLightColor(Vector3 color)
		{
			m_sceneLight.SetSpotLightColor(color);
		}

		/// <summary>
		/// �e���͈͂Ɨݏ悷��p�����[�^��ݒ�
		/// </summary>
		/// <param name="attn">X�ɉe���͈�,Y�ɗݏ悷��p�����[�^</param>
		void SetSpotLightAttn(Vector3 attn)
		{
			m_sceneLight.SetSpotLightAttn(attn);
		}

		/// <summary>
		/// �X�|�b�g���C�g�̃��C�g�̕�����ݒ�
		/// </summary>
		/// <param name="direction">����</param>
		void SetSpotLightDirection(Vector3 direction)
		{
			m_sceneLight.SetSpotLightDirection(direction);
		}

		/// <summary>
		/// �X�|�b�g���C�g�̃��C�g�̊p�x��ݒ�
		/// </summary>
		/// <param name="angle">�p�x</param>
		void SetSpotLightAngle(Vector3 angle)
		{
			m_sceneLight.SetSpotLightAngle(angle);
		}

		/// <summary>
		/// �X�|�b�g���C�g���g�p����
		/// </summary>
		void UseSpotLight()
		{
			m_sceneLight.UseSpotLight();
		}

		/// <summary>
		/// �X�|�b�g���C�g���g�p���Ȃ�
		/// </summary>
		void UnUseSpotLight()
		{
			m_sceneLight.UnUseSpotLight();
		}

		/// <summary>
		/// �X�|�b�g���C�g�̈ʒu���擾
		/// </summary>
		/// <returns>���W</returns>
		const Vector3& GetSpotLightPosition() const
		{
			return m_sceneLight.GetSpotLightPosition();
		}

		/// <summary>
		/// �X�|�b�g���C�g�̌��̐F���擾
		/// </summary>
		/// <returns>�F</returns>
		const Vector3& GetSpotLightColor() const
		{
			return m_sceneLight.GetSpotLightColor();
		}

		/// <summary>
		/// �X�|�b�g���C�g�̉e���͈͂Ɨݏ悷��p�����[�^���擾
		/// </summary>
		/// <returns></returns>
		const Vector3& GetSpotLightAttn()const
		{
			return m_sceneLight.GetSpotLightAttn();
		}

		/// <summary>
		/// �X�|�b�g���C�g�̌��̕������擾����
		/// </summary>
		/// <returns>����</returns>
		const Vector3& GetSpotLightDirection() const
		{
			return m_sceneLight.GetSpotLightDirection();
		}

		/// <summary>
		/// �X�|�b�g���C�g�̊p�x���擾����
		/// </summary>
		/// <returns>�p�x</returns>
		const Vector3& GetSpotLightAngle() const
		{
			return m_sceneLight.GetSpotLightAngle();
		}

		/// <summary>
		/// �X�|�b�g���C�g�͎g�p���H
		/// </summary>
		/// <returns>�g�p���̏ꍇtrue</returns>
		const int SpotLightIsUse() const
		{
			return m_sceneLight.SpotLightIsUse();
		}

		/////////////////////////////////////////////////////////////
		///�������C�g�̊֐�
		/////////////////////////////////////////////////////////////

		/// <summary>
		/// �������C�g��ݒ肷��
		/// </summary>
		/// <param name="groundColor">�n�ʐF</param>
		/// <param name="skyColor">�V���F</param>
		/// <param name="groundNormal">�n�ʂ̖@��</param>
		void SetHemiLight(Vector3 groundColor, Vector3 skyColor, Vector3 groundNormal)
		{
			SetHemiLightGroundColor(groundColor);
			SetHemiLightSkyColor(skyColor);
			SetHemiLightGroundNormal(groundNormal);
			UseHemiLight();
		}

		/// <summary>
		/// �������C�g�̒n�ʐF��ݒ�
		/// </summary>
		/// <param name="groundColor">�n�ʐF</param>
		void SetHemiLightGroundColor(Vector3 groundColor)
		{
			m_sceneLight.SetHemiLightGroundColor(groundColor);
		}

		/// <summary>
		/// �������C�g�̓V���F��ݒ�
		/// </summary>
		/// <param name="skyColor">�V���F</param>
		void SetHemiLightSkyColor(Vector3 skyColor)
		{
			m_sceneLight.SetHemiLightSkyColor(skyColor);
		}

		/// <summary>
		/// �������C�g�̒n�ʂ̖@��
		/// </summary>
		/// <param name="normal">�n�ʂ̖@��</param>
		void SetHemiLightGroundNormal(Vector3 normal)
		{
			m_sceneLight.SetHemiLightGroundNormal(normal);
		}

		/// <summary>
		/// �������C�g���g�p����
		/// </summary>
		void UseHemiLight()
		{
			m_sceneLight.UseHemiLight();
		}

		/// <summary>
		/// �������C�g���g�p���Ȃ�
		/// </summary>
		void UnUseHemiLight()
		{
			m_sceneLight.UnUseHemiLight();
		}

		/// <summary>
		/// �������C�g�̒n�ʐF���擾����
		/// </summary>
		/// <returns>�n�ʐF</returns>
		const Vector3& GetHemiLightGroundColor() const
		{
			return m_sceneLight.GetHemiLightGroundColor();
		}

		/// <summary>
		/// �������C�g�̓V���F���擾����
		/// </summary>
		/// <returns></returns>
		const Vector3& GetHemiLightSkyColor() const
		{
			return m_sceneLight.GetHemiLightSkyColor();
		}

		/// <summary>
		/// �������C�g�̒n�ʂ̖@�����擾����
		/// </summary>
		/// <returns></returns>
		const Vector3& GetHemiLightGroundNormal() const
		{
			return m_sceneLight.GetHemiLightGroundNormal();
		}

		/// <summary>
		/// �������C�g�͎g�p��?
		/// </summary>
		/// <returns>�g�p���̂Ƃ�true</returns>
		const int HemiLightIsUse() const
		{
			return m_sceneLight.HemiLightIsUse();
		}

		/////////////////////////////////////////////////////////////
		///�J�����̈ʒu�̊֐�
		/////////////////////////////////////////////////////////////

		/// <summary>
		/// �J�����̈ʒu��ݒ肷��
		/// </summary>
		/// <param name="eyePos">�J�����̍��W</param>
		void SetEyePos(Vector3 eyePos)
		{
			m_sceneLight.SetEyePos(eyePos);
		}

		

	private:
		// GBuffer�̒�`
		enum EnGBuffer
		{
			enGBufferAlbedoDepth,           // �A���x�h�Ɛ[�x�l�B���ɐ[�x�l���L������Ă��܂��B
			enGBufferNormal,                // �@��
			//enGBufferMetaricShadowSmooth,   // ���^���b�N�A�e�p�����[�^�A�X���[�X�B
			//enGBufferWorldPos,				// ���[���h���W
			// ���^���b�N��r�A�e�p�����[�^��g�A�X���[�X��a�Bg�͖��g�p�B
			enGBufferNum,                   // G-Buffer�̐�
		};

		/// <summary>
		/// G-Buffer��������
		/// </summary>
		void InitGBuffer();
		/// <summary>
		/// �f�B�t�@�[�h���C�e�B���O�Ŏg�p����X�v���C�g���������B
		/// </summary>
		void InitDefferedLighting_Sprite();

		/// <summary>
		/// ���f����`�悷��
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void ModelRendering(RenderContext& rc);

		/// <summary>
		/// �X�v���C�g��`�悷��
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void SpriteRendering(RenderContext& rc);

		/// <summary>
		/// �t�H���g��`�悷��
		/// </summary>
		/// <param name="rc">�����_�[�R���e�L�X�g</param>
		void FontRendering(RenderContext& rc);

		/// <summary>
		/// �V���h�E�}�b�v�ɕ`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void RenderToShadowMap(RenderContext& rc);

		/// <summary>
		/// ZPrepass
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void ZPrepass(RenderContext& rc);

		/// <summary>
		/// G-Buffer�ւ̕`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g�B</param>
		void RenderToGBuffer(RenderContext& rc);

		/// <summary>
	   /// �t�H���[�h�����_�����O
	   /// </summary>
	   /// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void ForwardRendering(RenderContext& rc);

		/// <summary>
		/// 2D�`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void Render2D(RenderContext& rc);

		/// <summary>
	   /// ZPrepass�p�̃����_�����O�^�[�Q�b�g��������
	   /// </summary>
		void InitZPrepassRenderTarget();

		std::vector<ModelRender*>		m_modelList;	//���f�����X�g
		std::vector<SpriteRender*>		m_spriteList;	//�X�v���C�g���X�g
		std::vector<FontRender*>		m_fontList;		//�X�v���C�g���X�g
		std::vector< ModelRender* >		m_zprepassModelList;  // ZPrepass�̕`��p�X�ŕ`�悳��郂�f���̃��X�g 
		std::vector<ModelRender*>		m_gBufferModelList;	//���f�����X�g

		RenderTarget m_zprepassRenderTarget;			//ZPrepass�`��p�̃����_�����O�^�[�Q�b�g

		Sprite						m_copyToFrameBufferSprite;	//�e�N�X�`����\��t���邽�߂̃X�v���C�g��������
		SceneLight						m_sceneLight;	//�V�[�����C�g
		//SDeferredLightingCB m_deferredLightingCB;                       // �f�B�t�@�[�h���C�e�B���O�p�̒萔�o�b�t�@
		Sprite m_diferredLightingSprite;                                // �f�B�t�@�[�h���C�e�B���O���s�����߂̃X�v���C�g
		RenderTarget					m_mainRenderTarget;	//�����_�����O�^�[�Q�b�g
		PostEffect						m_postEffect;		//�|�X�g�G�t�F�N�g
		Shadow							m_shadow;
		Texture							m_toontexture;
		RenderTarget					m_gBuffer[enGBufferNum];
		std::vector< IRenderer* > m_renderObjects;                      // �`��I�u�W�F�N�g�̃��X�g�B



		/// <summary>
	   /// �C�x���g���X�i�[�̃f�[�^�B
	   /// </summary>
		struct SEventListenerData {
			void* pListenerObj;     // ���X�i�[�I�u�W�F�N�g
			std::function<void(EnEvent enEvent)> listenerFunc;
		};

		std::list< SEventListenerData > m_eventListeners;
	};
}


