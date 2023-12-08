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

	/// <summary>
	/// �摜�̈ړ�����
	/// </summary>
	/// <param name="spriteRender"></param>
	/// <param name="startPos"></param>
	/// <param name="endPos"></param>
	void MoveText(SpriteRender& spriteRender,Vector3 startPos,Vector3 endPos);

	/// <summary>
	/// ������
	/// </summary>
	void CalcAlpha();
	/// <summary>
	/// �摜�̏�����
	/// </summary>
	void InitSprite();

	/// <summary>
	/// �Q�[���X�^�[�g���̃J����
	/// </summary>
	void GameStartCamera();

	/// <summary>
	/// �J�����̃Y�[���A�E�g�^�C���̌v�Z
	/// </summary>
	/// <returns></returns>
	bool CalcCameraZoomOutTime();

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

private:
	Fade* m_fade = nullptr;

	SpriteRender m_battleTextRender;
	SpriteRender m_startTextRender;

	CameraCollisionSolver	m_cameraCollisionSolver;
	SpringCamera			m_springCamera;

	Vector3				m_target = Vector3::Zero;		//�J���������_
	Vector4 m_color = g_vec4White;

	float m_timer = 0.0f;

	float m_alpha = 0.0f;
	bool m_calcAlphaEndFlag = false;

	bool m_fadeStartFlag = false;

	const float					m_cameraZoomOutTime = 2.0f;
	float						m_cameraZoomOutTimer = 0.0f;

	float m_count = 1.0f;
};

