#include "stdafx.h"
#include "DarkSpear.h"

#include "CharactersInfoManager.h"

DarkSpear::DarkSpear()
{
}

DarkSpear::~DarkSpear()
{
}

bool DarkSpear::Start()
{
	//����������W�����߂�
	DecideCreatePosition();

	for (int num = 0; num < SPEAR_CREATE_PLACE; num++)
	{
		for (int num2 = 0; num2 < SPEAR_ROWS; num2++)
		{
			m_model[num][num2].Init("Assets/modelData/character/Slime/slime.tkm",
				L"Assets/shader/ToonTextrue/lamp_Slime.DDS");

			m_model[num][num2].SetPosition(m_createPosition[num][num2]);
			m_model[num][num2].Update();
		}

	
	}

	return true;
}

void DarkSpear::Update()
{
	//�����J�n�t���O���Z�b�g����Ă�����
	if (m_createStartFlag != true)
	{
		return;
	}

	//�����d�؂�܂�
	if (m_createCount < SPEAR_ROWS)
	{
		//��������^�C�~���O�Ȃ�
		if (IsCreateTiming() == true)
		{
			//�����J�E���g�����Z
			//���ꂪ��̐�
			m_createCount++;

			
			for (int num = 0; num < SPEAR_CREATE_PLACE; num++)
			{
				for (int num2 = 0; num2 < m_createCount; num2++)
				{
					m_model[num][num2].SetScale(g_vec3One * 3.0f);
					m_model[num][num2].Update();
				}
			}
		}
	}
	else
	{
		if (m_timer > 2.0f)
		{
			DeleteGO(this);
		}
		else
		{
			m_timer += g_gameTime->GetFrameDeltaTime();
		}
	}

	
}

void DarkSpear::DecideCreatePosition()
{
	//�v���C���[�̍��W
	Vector3 playerPos= CharactersInfoManager::GetInstance()->GetPlayerInstance()->GetPosition();
	//���g(�T���i�[)����v���C���[�Ɍ������x�N�g�����v�Z
	Vector3 diff = playerPos - m_position;
	diff.y = 0.0f;
	diff.Normalize();

	//�S���ʂɑ��𐶐�����Ƃ�
	Vector3 center = m_position;
	
	float en = 360.0f;
	en /= SPEAR_CREATE_PLACE;

	float addLength = FIRST_CREATE_POS_LENGTH;

	for (int i = 1; i <= SPEAR_CREATE_PLACE; i++)
	{
		//�p�x�����W�A���ɕϊ�
		float theta = Math::PI / 180 * en * i;

		

		Vector3 pos = g_vec3Zero;
		pos.x = center.x + FIRST_CREATE_POS_LENGTH * cos(theta);
		pos.z = center.z + FIRST_CREATE_POS_LENGTH * sin(theta);

		Vector3 finalPos = m_position - pos;

		m_rotation.Apply(finalPos);

		m_createPosition[i-1][0] = finalPos;

		finalPos.Normalize();

		for (int j = 0; j < SPEAR_ROWS; j++)
		{
			m_createPosition[i-1][j] = m_position + finalPos * addLength;
			addLength += ADD_CREATE_POS_LENGTH;
		}
		addLength = FIRST_CREATE_POS_LENGTH;
	}

}

bool DarkSpear::IsCreateTiming()
{
	if (m_createTimer > CREATE_TIMER_LIMMIT)
	{
		m_createTimer = 0.0f;
		return true;
	}
	else
	{
		m_createTimer += g_gameTime->GetFrameDeltaTime();
	}

	return false;
}

void DarkSpear::Render(RenderContext& rc)
{
	for (int num = 0; num < SPEAR_CREATE_PLACE; num++)
	{
		for (int num2 = 0; num2 < SPEAR_ROWS; num2++)
		{
			m_model[num][num2].Draw(rc);
		}
	}
}