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

	void positionUp();
	/// <summary>
	/// �X�v���C�g�����_�[�̏�����
	/// </summary>
	void InitSprite();
	/// <summary>
	/// ���`��ԂŎg���n�_�ƏI�_�̏�����
	/// </summary>
	void InitLerpPosition();

	void SpriteMove();


	void FireMove();

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


	// �A�j���[�V�����N���b�v�̔ԍ���\���񋓌^�B
	enum EnAnimationClip {
		enAnimClip_Idle,			// 0 : �ҋ@�A�j���[�V����
		enAnimClip_Victory,
		enAnimClip_Num,				// 7 :�A�j���[�V�����N���b�v�̐�
	};

	//�A�j���[�V�����X�e�[�g
	enum EnAnimationState {
		enAninationState_Idle,
		enAnimationState_Victory
	};

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


	Game* m_game = nullptr;

	ModelRender m_model;
	Vector3 m_position = g_vec3Zero;
	Quaternion m_rotation = g_quatIdentity;
	Vector3 m_scale = g_vec3One;

	AnimationClip m_animationClip[enAnimClip_Num];	// �A�j���[�V�����N���b�v 
	EnAnimationState m_enAnimationState = enAninationState_Idle;	//�A�j���[�V�����X�e�[�g


	SpriteRender m_bossTextRender;
	SpriteRender m_FireRender;
	SpriteRender m_PressAButton;

	
	LichCharInfo m_lichCharInfo[END];				//���`��ԂŎg���n�_�ƏI�_

	//std::vector<SpriteRender> m_lichSprite;		//LICH�̕����̉摜���ꕶ�����i�[

	bool m_completeFlag = false;
	
	float m_time = 0.0f;

	bool m_gOTextFlag = false;
	bool m_FireDrawFlag = false;

	int m_lichTextCount = 0;

	float m_mulValue = 2.0f;

	const float m_endTime = 2.0f;
	float m_endTimer = 0.0f;
};
