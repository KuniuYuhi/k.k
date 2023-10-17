#pragma once
class CascadModel:public IGameObject
{
public:
	CascadModel();
	~CascadModel();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void UpDateWeapon();

	void MoveCamera();

	// �A�j���[�V�����N���b�v�̔ԍ���\���񋓌^�B
	enum EnAnimationClip {
		enAnimClip_Idle,
		enAnimClip_Sprint,
		enAnimClip_DashForward,
		enAnimClip_KnockBack,
		enAnimClip_Hit,
		enAnimClip_Defend,
		enAnimClip_DefendHit,
		enAnimClip_Die,
		enAnimClip_ChangeSwordShield,
		enAnimClip_Win_Start,
		enAnimClip_Win_Main,
		enAnimClip_attack1,
		enAnimClip_attack2,
		enAnimClip_attack3,
		enAnimClip_attack4,
		enAnimClip_attack5,

		enAnimClip_Num
	};

private:
	ModelRender tea;
	ModelRender bg;


	Animation	m_animation;				// �A�j���[�V����
	AnimationClip	m_animationClip[enAnimClip_Num];// �A�j���[�V�����N���b�v 
	ModelRender Player;

	ModelRender Sword;
	int m_swordBoonId = -1;

	ModelRender Shield;
	int m_shieldBoonId = -1;

	int m_playAnimCount = 0;
};

