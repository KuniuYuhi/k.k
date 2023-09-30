#pragma once

class Game;

class EntryBoss:public IGameObject
{
public:
	EntryBoss();
	~EntryBoss();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	void MamageState();

	void Animation();

	void positionUp();
	/// <summary>
	/// �X�v���C�g�����_�[�̏�����
	/// </summary>
	void InitSprite();
	/// <summary>
	/// ���`��ԂŎg���n�_�ƏI�_�̏�����
	/// </summary>
	void InitLerpPosition();
	/// <summary>
	/// �摜�̈ړ�����
	/// </summary>
	void SpriteMove();

	/// <summary>
	/// ��ʂ����X�ɈÂ�����
	/// </summary>
	void slowlyDarkScreen();
	/// <summary>
	/// ��ʂ����X�ɖ��邭����
	/// </summary>
	void SlowlyBrightScreen();

	/// <summary>
	/// �o�ꃀ�[�r�[���I����Ă���̏���
	/// </summary>
	void CompleteTime();

	/// <summary>
	/// �X�v���C�g�̍��W�A�T�C�Y�A��]��ݒ�
	/// </summary>
	/// <param name="spriterender">�X�v���C�g�����_�[</param>
	/// <param name="position">���W</param>
	/// <param name="scale">�T�C�Y</param>
	/// <param name="rotation">��]</param>
	void SettingSpriteRender(
		SpriteRender& spriterender, Vector3 position, Vector3 scale, Quaternion rotation)
	{
		spriterender.SetPosition(position);
		spriterender.SetScale(scale);
		spriterender.SetRotation(rotation);
		spriterender.Update();
	}

	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

	const Vector3 GetPosition() const
	{
		return m_position;
	}

	void SetGame(Game* game)
	{
		m_game = game;
	}

	void SetSkyCube(SkyCube* skyCube)
	{
		m_skyCube = skyCube;
	}


	// �A�j���[�V�����N���b�v�̔ԍ���\���񋓌^�B
	enum EnAnimationClip {
		enAnimClip_Idle,			// 0 : �ҋ@�A�j���[�V����
		enAnimClip_FogRemoval,
		enAnimClip_Num,				// 7 :�A�j���[�V�����N���b�v�̐�
	};

	//�A�j���[�V�����X�e�[�g
	enum EnAnimationState {
		enAninationState_Idle,
		enAnimationState_FogRemoval
	};

	void SetNextAnimationState(EnAnimationState NextAnimationState)
	{
		m_enAnimationState = NextAnimationState;
	}

	/// <summary>
	/// �J�ڏ��������s
	/// </summary>
	void OnProcessCommonStateTransition();
	/// <summary>
	/// �J�ڏ��������s
	/// </summary>
	void OnProcessFogRemovalStateTransition();

	enum EnLichName
	{
		L,
		I,
		C,
		H,
		END
	};

private:

	struct LichCharInfo
	{
		SpriteRender m_CharRender;
		Vector3 m_startPosition = g_vec3Zero;
		Vector3 m_endPosition = g_vec3Zero;
	};


	Game*						m_game = nullptr;
	SkyCube*					m_skyCube = nullptr;
	EffectEmitter*				m_CircleEffect = nullptr;
	EffectEmitter*				m_FogRemovalEffect = nullptr;

	ModelRender					m_model;
	Vector3						m_position = g_vec3Zero;
	Quaternion					m_rotation = g_quatIdentity;
	Vector3						m_scale = g_vec3One;

	Vector3						m_finalAmbientColor = g_vec3One;

	AnimationClip				m_animationClip[enAnimClip_Num];				// �A�j���[�V�����N���b�v 
	EnAnimationState			m_enAnimationState = enAninationState_Idle;		//�A�j���[�V�����X�e�[�g

	SpriteRender				m_bossTextRender;								//BOSS
	SpriteRender				m_PressAButton;									//�E���̃e�L�X�g�̉摜

	LichCharInfo				m_lichCharInfo[END];							//���`��ԂŎg���n�_�ƏI�_

	bool						m_completeFlag = false;							//�S�Ă̏����������������̃t���O
	bool						m_positionUpEndFlag = false;					//���W���グ�鏈�����I��������̃t���O
	
	float						m_time = 0.0f;				

	bool						m_GoTextFlag = false;							//�����摜�̏������n�߂邩�̃t���O
	int							m_lichTextCount = 0;

	bool						m_drawBOSSTextFlag = false;

	float						m_mulValue = 2.0f;

	const float					m_endTime = 2.0f;
	float						m_endTimer = 0.0f;

	float						m_skyLuminance = 0.9f;
	float                       m_ambientColor = 0.0f;

	float						m_lightTimer = 0.0f;

	bool						m_SlowlyDarkScreenEndFlag = false;				//��ʂ��Â��������̃t���O
	bool						m_SlowlyBrightScreenEndFlag = false;			//��ʂ𖾂邭�������̃t���O
};

