#include "stdafx.h"
#include "Actor.h"


Actor::Actor()
{
	//m_player = FindGO<Player>("player");
	//m_charaCon;
}

Actor::~Actor()
{
}

void Actor::SetTransFormModel(ModelRender& modelRender)
{
	modelRender.SetTransform(m_position, m_rotation, m_scale);
}

void Actor::SetTransForm(Vector3 position, Quaternion rotation,Vector3 scale)
{
	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
}

void Actor::RecoveryMP()
{
	//MP�񕜏�ԂȂ�
	if (m_recoveryMpFlag == true)
	{
		if (m_status.mp < m_status.maxMp)
		{
			m_status.mp += g_gameTime->GetFrameDeltaTime();

			if (m_status.mp > m_status.maxMp)
			{
				m_status.mp = m_status.maxMp;
				//MP�񕜏�Ԃ��Ȃ��ɂ���
				m_recoveryMpFlag = false;
			}
		}
	}

	
}

Vector3 Actor::calcVelocity(Status status)
{
	Vector3 moveSpeed = Vector3::Zero;

	//�J�����̑O�����ƉE�����̃x�N�g���������Ă���B
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y�����ɂ͈ړ������Ȃ��B
	forward.y = 0.0f;
	right.y = 0.0f;
	forward.Normalize();

	//�ړ��̓���
	Vector3 stickL = Vector3::Zero;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//�_�b�V���t���O��false�Ȃ�
	if (m_dashFlag == false)
	{
		//�ړ��̓��͗ʂ�status�̃X�s�[�h����Z�B
		right *= stickL.x * status.defaultSpeed;
		forward *= stickL.y * status.defaultSpeed;
	}
	else
	{
		//�ړ��̓��͗ʂ�status�̃X�s�[�h�ƃ_�b�V���̒l����Z�B
		right *= stickL.x * (status.defaultSpeed + status.dashSpeed);
		forward *= stickL.y * (status.defaultSpeed + status.dashSpeed);
	}

	moveSpeed += right + forward;

	//�l���Z�[�u���Ă���
	m_SaveMoveSpeed = moveSpeed;

	//����̃A�j���[�V�������Đ����Ȃ�ړ��Ȃ�
	if (isAnimationEntable() != true)
	{
		return moveSpeed = Vector3::Zero;
	}
	else
	{
		return moveSpeed;
	}
	
	
}

bool Actor::IsComboStateSame()
{
	//���݂̃R���{�X�e�[�g�ƃ_���[�W���󂯂����̃R���{�X�e�[�g���Ⴄ�Ȃ�
	if (GetNowComboState() != GetDamagedComboState())
	{
		//�Ⴄ
		return true;
	}
	else
		//����
		return false;
}

Quaternion Actor::Rotation()
{
	if (RotationOnly() == true)
	{
		return m_rotation;
	}

	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		m_rotation.SetRotationYFromDirectionXZ(m_moveSpeed);
	}

	return m_rotation;
}
