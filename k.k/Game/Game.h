#pragma once
#include "Level3DRender.h"


class Game:public IGameObject
{
public:
	Game();
	~Game();

	bool Start();
	void Update();

	void Move();
	void SpriteTransform();
	void PlayAnim();

	void Render(RenderContext& rc);

private:
	// �A�j���[�V�����N���b�v�̔ԍ���\���񋓌^�B
	enum EnAnimationClip {
		enAnimClip_Idle,	// 0 : �ҋ@�A�j���[�V�����A
		enAnimClip_Run,		// 1 : ����A�j���[�V�����B
		enAnimClip_Num,		// 2 :�A�j���[�V�����N���b�v�̐��B
	};

	FontRender fontTest;
	float m_fontScale = 1.0f;

	SpriteRender spriteTest;

	Level3DRender levelbg;
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

