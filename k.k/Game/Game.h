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
	// アニメーションクリップの番号を表す列挙型。
	enum EnAnimationClip {
		enAnimClip_Idle,	// 0 : 待機アニメーション、
		enAnimClip_Run,		// 1 : 走りアニメーション。
		enAnimClip_Num,		// 2 :アニメーションクリップの数。
	};

	FontRender fontTest;
	float m_fontScale = 1.0f;

	SpriteRender spriteTest;

	Level3DRender levelbg;
	ModelRender backGround;
	PhysicsStaticObject BGPhysicsStaticObject;

	ModelRender model;
	CharacterController m_charaCon;
	Animation m_animation;	// アニメーション
	AnimationClip m_animationClipArray[enAnimClip_Num];	// アニメーションクリップ 

	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;
};

