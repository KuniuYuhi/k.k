#pragma once
class Game:public IGameObject
{
public:
	Game();
	~Game();

	bool Start();
	void Update();
	void Move();
	void PlayAnim();
	void Render(RenderContext& rc);

private:
	// �A�j���[�V�����N���b�v�̔ԍ���\���񋓌^�B
	enum EnAnimationClip {
		enAnimClip_Idle,	// 0 : �ҋ@�A�j���[�V�����A
		enAnimClip_Run,		// 1 : ����A�j���[�V�����B
		enAnimClip_Num,		// 2 :�A�j���[�V�����N���b�v�̐��B
	};

	ModelRender backGround;
	PhysicsStaticObject BGPhysicsStaticObject;

	ModelRender model;
	CharacterController m_charaCon;
	Animation m_animation;	// �A�j���[�V����
	AnimationClip m_animationClipArray[enAnimClip_Num];	// �A�j���[�V�����N���b�v 

	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;
};

