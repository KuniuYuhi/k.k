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

private:

	Game* m_game = nullptr;

	ModelRender m_model;
	Vector3 m_position = g_vec3Zero;
	Quaternion m_rotation = g_quatIdentity;
	Vector3 m_scale = g_vec3One;

	AnimationClip m_animationClip[enAnimClip_Num];	// �A�j���[�V�����N���b�v 

	EnAnimationState m_enAnimationState = enAninationState_Idle;	//�A�j���[�V�����X�e�[�g

};

