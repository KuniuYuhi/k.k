#pragma once
#include "Level3DRender.h"
//#include "Level2DRender.h"


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

	void Spotmove();

	const Vector3 GetPosition() const
	{
		return m_position;
	}

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

	//Level2DRender level2DSp;
	SpriteRender spriteTest;
	float wipSize = 5.0f;

	Level3DRender levelbg;
	ModelRender backGround;
	ModelRender Castle;

	PhysicsStaticObject BGPhysicsStaticObject;

	ModelRender model;
	CharacterController m_charaCon;
	Animation m_animation;	// �A�j���[�V����
	AnimationClip m_animationClipArray[enAnimClip_Num];	// �A�j���[�V�����N���b�v 

	//spotLight
	Vector3 spPosition = Vector3::Zero;
	Vector3 spDirection = Vector3::Zero;

	Vector3 m_position = Vector3(0.0f,0.0f,0.0f);
	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;
};

