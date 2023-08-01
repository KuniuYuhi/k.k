#pragma once

namespace nsK2EngineLow {

	/// <summary>
   /// �t�H���[�h�����_�����O�p�̃��f���������\���́B
   /// </summary>
   /// <remark>
   /// ModelInitData���p�����Ă��܂��B
   /// �t�H���[�h�����_�����O�̂��߂ɕK�v�ȃf�t�H���g�ݒ���R���X�g���N�^�ōs���Ă���܂��B
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
		/// �ʏ�̏�����
		/// </summary>
		/// <param name="tkmFilepath">tkm�t�@�C���p�X</param>
		/// <param name="animationClips">�A�j���[�V�����N���b�v</param>
		/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̐�</param>
		/// <param name="enModelUpAxis">���f���̏����</param>
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
		/// ZPrepass�`��p�̃��f�����������B
		/// </summary>
		/// <param name="renderingEngine"></param>
		/// <param name="tkmFilePath"></param>
		void InitModelOnZprepass(
			/*RenderingEngine& renderingEngine,*/
			const char* tkmFilePath,
			EnModelUpAxis modelUpAxis
		);

		/// <summary>
	   /// ������
	   /// </summary>
	   /// <remark>
	   /// �{�֐��𗘗p���ď��������ꂽ���f����
	   /// �t�H���[�h�����_�����O�̕`��p�X�ŕ`�悳��܂�
	   /// </remark>
	   /// <param name="renderingEngine">�����_�����O�G���W��</param>
	   /// <param name="modelInitData"></param>
		void InitForwardRendering(RenderingEngine& renderingEngine, ModelInitData& modelInitData);

		/// <summary>
		/// �e���󂯂郂�f���̏�����
		/// </summary>
		/// <param name="tkmFilepath">tkm�t�@�C���p�X</param>
		void InitShadow(
			const char* tkmFilepath
		);

		/// <summary>
		/// �X�J�C�L���[�u������������
		/// </summary>
		/// <param name="initData"></param>
		void InitSkyCube(ModelInitData& initData);

		/// <summary>
		/// ���f�������_�[�N���X�����X�g�ɒǉ�
		/// </summary>
		/// <param name="rc"></param>
		void Draw(RenderContext& rc)
		{
			g_renderingEngine->AddModelList(this);
			g_renderingEngine->Add3DModelToZPrepass(this);
		}

		/// <summary>
		/// ���f����`�悷��(RenderingEngine�ŕ`�悷��Ƃ��ɌĂяo��)
		/// </summary>
		/// <param name="rc"></param>
		void OnRenderModel(RenderContext& rc)
		{
			m_model.Draw(rc);
		}

		/// <summary>
		/// ZPrepass���f����`�悷��
		/// </summary>
		/// <param name="rc"></param>
		void OnZPrepass(RenderContext& rc)
		{
			m_zprepassModel.Draw(rc);
		}

		/// <summary>
		/// �V���h�E�}�b�v�`��p�̃��f����`�悷��
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="lightCamera"></param>
		void OnRenderShadowModel(RenderContext& rc, Camera& lightCamera)
		{
			m_shadowModel.Draw(rc, lightCamera);
		}

		/// <summary>
		/// ���f�����擾����
		/// </summary>
		/// <returns>���f��</returns>
		Model& GetModel()
		{
			return m_model;
		}

		/// <summary>
		/// ���W�A��]�A�g������ׂĐݒ�
		/// </summary>
		/// <param name="pos">���W�̐ݒ�</param>
		/// <param name="rot">��]�̐ݒ�</param>
		/// <param name="scale">�g��̐ݒ�</param>
		void SetTransform(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
		{
			SetPosition(pos);
			SetRotation(rot);
			SetScale(scale);
		}

		/// <summary>
		/// ���W�̐ݒ�
		/// </summary>
		/// <param name="position">Vector3�̍��W</param>
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}

		/// <summary>
		/// ���W�̐ݒ�
		/// </summary>
		/// <param name="x">X��</param>
		/// <param name="y">Y��</param>
		/// <param name="z">Z��</param>
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x,y,z });
		}

		/// <summary>
		/// ��]�̐ݒ�
		/// </summary>
		/// <param name="rotation">��]</param>
		void SetRotation(const Quaternion rotation)
		{
			m_rotation = rotation;
		}

		/// <summary>
		/// �傫���̐ݒ�
		/// </summary>
		/// <param name="scale">�傫��</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}

		/// <summary>
		/// �傫���̐ݒ�
		/// </summary>
		/// <param name="x">X��</param>
		/// <param name="y">Y��</param>
		/// <param name="z">Z��</param>
		void SetScale(float x, float y, float z)
		{
			SetScale({ x,y,z });
		};

		/// <summary>
		/// �X�V����
		/// </summary>
		void Update();

		/// <summary>
		/// �{�[���̖��O����{�[���ԍ�������
		/// </summary>
		/// <param name="boneName">�{�[���̖��O</param>
		/// <returns>�{�[���ԍ��B������Ȃ������ꍇ��-1���Ԃ�</returns>
		int FindBoneID(const wchar_t* boneName)const
		{
			return m_skeleton.FindBoneID(boneName);
		}

		/// <summary>
		/// �{�[���ԍ�����{�[�����擾����
		/// </summary>
		/// <param name="boneNo">�{�[���ԍ�</param>
		/// <returns>�{�[��</returns>
		Bone* GetBone(int boneNo) const
		{
			return m_skeleton.GetBone(boneNo);
		}

		/// <summary>
		/// �A�j���[�V�����C�x���g��ǉ�����
		/// </summary>
		/// <param name="eventListener"></param>
		void AddAnimationEvent(AnimationEventListener eventListener)
		{
			m_animation.AddAnimationEventListener(eventListener);
		}

		/// <summary>
		/// �A�j���[�V�����Đ��B
		/// </summary>
		/// <param name="animNo">�A�j���[�V�����N���b�v�̔ԍ��B</param>
		/// <param name="interpolateTime">�⊮����(�P�ʁF�b�B)</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}

		/// <summary>
		/// �A�j���[�V�����̍Đ����H
		/// </summary>
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}

		/// <summary>
		/// �A�j���[�V�����Đ��̑��x��ݒ肷��B
		/// </summary>
		/// <param name="animationSpeed">���l�̕������{�ɂ���B</param>
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}

		/// <summary>
		/// �f�B���N�V�������C�g�̏����쐬
		/// </summary>
		void MakeDirectionData();

	// �����o�ϐ�
	private:
		/// <summary>
		/// �X�P���g���̏������B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		void InitSkeleton(const char* filePath);

		/// <summary>
		/// �A�j���[�V�����̏������B
		/// </summary>
		/// <param name="animationClips">�A�j���[�V�����N���b�v�B</param>
		/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̐��B</param>
		/// <param name="enModelUpAxis">���f���̏�����B</param>
		void InitAnimation(
			AnimationClip* animationClips,
			int numAnimationClips,
			EnModelUpAxis enModelUpAxis
		);

		/// <summary>
	   /// ���ʂ̏���������(���̓f�t�@�[�h�����_�����O�̂�)
	   /// </summary>
	   /// <param name="renderingEngine">�����_�����O�G���W��</param>
	   /// <param name="tkmFilePath">tkm�t�@�C���p�X</param>
		void InitCommon(RenderingEngine& renderingEngine, const char* tkmFilePath);

		Skeleton					m_skeleton;									//�X�P���g��
		AnimationClip*				m_animationClips = nullptr;					//�A�j���[�V�����N���b�v�B
		int							m_numAnimationClips = 0;					//�A�j���[�V�����N���b�v�̐��B
		Animation					m_animation;								//�A�j���[�V�����B
		float						m_animationSpeed = 1.0f;					//�A�j���[�V�����̑��x�B
		//ComputeAnimationVertexBuffer m_computeAnimationVertexBuffer;	// �A�j���[�V�����ςݒ��_�o�b�t�@�̌v�Z�����B

		Vector3						m_position = Vector3::Zero;				//���W
		Vector3						m_scale = Vector3::One;					//�傫��
		Quaternion					m_rotation = Quaternion::Identity;		//��]

		Model						m_model;								//���f���N���X
		Model						m_shadowModel;
		Model m_zprepassModel;                  // ZPrepass�ŕ`�悳��郂�f��
		Model						m_frowardRenderModel;					 // �t�H���[�h�����_�����O�̕`��p�X�ŕ`�悳��郂�f��
		ModelInitData				m_modelInitData;						//���f�������������邽�߂̏���ݒ肷��N���X
		ModelInitDataFR             m_modelInitDataFR;

	};
}

