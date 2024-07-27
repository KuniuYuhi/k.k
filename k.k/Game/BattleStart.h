#pragma once

#include "CameraCollisionSolver.h"
#include "SpringCamera.h"

class Fade;

class BattleStart:public IGameObject
{
public:
	BattleStart();
	~BattleStart();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:

	/// <summary>
	/// �E�ғo��̗���X�e�[�g
	/// </summary>
	enum EnBraveState
	{
		enBraveState_Complete,
		enBraveState_Fall,			//������
		enBraveState_Landing,		//���n
		enBraveState_Idle,			//�A�C�h�����
		enBraveState_End			//�I���
	};

	
	/// <summary>
	/// �Q�[���X�^�[�g���̃J����
	/// </summary>
	void GameStartCamera();

	/// <summary>
	/// �J�����̃Y�[���A�E�g�^�C���̌v�Z
	/// </summary>
	/// <returns></returns>
	bool IsFadeStart();

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

	/// <summary>
	/// ���f���̏�����
	/// </summary>
	void InitModel();
	/// <summary>
	/// �E�҃��f���̈ړ�����
	/// </summary>
	void ProcessMoveBrave();

	/// <summary>
	/// �E�҂̏�ԊǗ�
	/// </summary>
	void ManageBraveState();
	/// <summary>
	/// �������ԑJ�ڏ���
	/// </summary>
	void ProcessFallStateTransition();
	/// <summary>
	/// ���n��ԑJ�ڏ���
	/// </summary>
	void ProcessLandingStateTransition();
	/// <summary>
	/// �A�C�h����ԑJ�ڏ���
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// �S�ďI����ԑJ�ڏ���
	/// </summary>
	void ProcessEndStateTransition();

	/// <summary>
	/// ���n�G�t�F�N�g�Đ�
	/// </summary>
	void PlayLandingEffect();

private:
	Fade* m_fade = nullptr;

	enum EnAnimationClip {
		enAnimClip_Idle,
		enAnimClip_Fall,
		enAnimClip_Landing,
		enAnimClip_Num
	};

	ModelRender m_braveModel;
	AnimationClip m_animationClip[enAnimClip_Num];

	EnBraveState m_enBraveState = enBraveState_Fall;

	CameraCollisionSolver	m_cameraCollisionSolver;
	SpringCamera			m_springCamera;

	Vector3				m_target = Vector3::Zero;		//�J���������_

	Vector3 m_bravePosition = g_vec3Zero;

	float m_timer = 0.0f;

	bool m_fadeStartFlag = false;

	const float					m_fadeTime = 2.0f;
	float						m_fadeTimer = 0.0f;


	float m_addYPos = 0.0f;
	float m_mulXZPos = 0.0f;

	bool m_isProcessEndFlag = false;

};

