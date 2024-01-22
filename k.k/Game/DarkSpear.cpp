#include "stdafx.h"
#include "DarkSpear.h"
#include "DarkSpearObj.h"
#include "CharactersInfoManager.h"
#include "InitEffect.h"

#include "GameManager.h"

namespace {
	const float DELETE_TIMER_LIMMIT = 2.0f;
}

DarkSpear::DarkSpear()
{
}

DarkSpear::~DarkSpear()
{
	//�G�t�F�N�g�̍폜
	DeleteRangeEffect();
}

bool DarkSpear::Start()
{
	//�_�[�N�X�s�A�𐶐�������W�����߂�
	DecideCreatePosition();

	//�͈̓G�t�F�N�g�𐶐�
	PlayRangeEffect();

	return true;
}

void DarkSpear::Update()
{
	//���s�����܂�����폜
	if (GameManager::GetInstance()->GetOutComeState() !=
		GameManager::enOutComeState_None)
	{
		DeleteGO(this);
		return;
	}

	//�����J�n�t���O���Z�b�g����Ă��Ȃ��Ȃ珈�����Ȃ�
	if (m_createStartFlag != true)
	{
		return;
	}

	//�����d�؂�܂�
	if (m_rowsCount < SPEAR_ROWS)
	{
		//��������^�C�~���O�Ȃ�
		if (IsCreateTiming() == true)
		{
			CreateDarkSpearObj();

			//�����J�E���g�����Z
			//���ꂪ��̐�
			m_rowsCount++;
		}
	}
	else
	{
		//�������鎞�ԂɒB������
		if (m_timer > DELETE_TIMER_LIMMIT)
		{
			//���g���폜
			DeleteGO(this);
		}
		else
		{
			//�^�C�}�[�����Z
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
	//���
	float en = 360.0f;
	//��������ꏊ�Ŋ����Ċp�x�����߂�
	en /= SPEAR_CREATE_PLACE;
	//�_�[�N�X�s�A�𐶐�������W�ɑ���������ݒ�
	float addLength = FIRST_CREATE_POS_LENGTH;

	//��������ꏊ�̐��J��Ԃ�
	//����������W�����߂�
	for (int i = 1; i <= SPEAR_CREATE_PLACE; i++)
	{
		//�p�x�����W�A���ɕϊ�
		float theta = Math::PI / 180 * en * i;
		//����������W�����߂邽�߂Ɋp�x�����߂�
		Vector3 pos = g_vec3Zero;
		pos.x = center.x + FIRST_CREATE_POS_LENGTH * cos(theta);
		pos.z = center.z + FIRST_CREATE_POS_LENGTH * sin(theta);

		//���߂��p�x�̍��W���璆�S�Ɍ������x�N�g�����v�Z
		Vector3 finalPos = m_position - pos;
		//���K��
		finalPos.Normalize();

		//��̐��J��Ԃ�
		for (int j = 0; j < SPEAR_ROWS; j++)
		{
			//����������W��ݒ�
			m_createPosition[i-1][j] = m_position + finalPos * addLength;
			//���ɐ���������W�̋�����L�΂�
			addLength += ADD_CREATE_POS_LENGTH;
		}
		//���Z���钷�����ŏ��ɖ߂�
		addLength = FIRST_CREATE_POS_LENGTH;
	}

}

void DarkSpear::CreateDarkSpearObj()
{
	//��񂲂Ƃɐ�������
	for (int num2 = m_rowsCount; num2 <= m_rowsCount; num2++)
	{
		//�����ꏊ�̐��J��Ԃ�
		for (int num = 0; num < SPEAR_CREATE_PLACE; num++)
		{
			//�_�[�N�X�s�A�I�u�W�F�N�g�̐���
			m_darkSpearObj[num][num2] = NewGO<DarkSpearObj>(0, "darkspearobj");
			//���W�̐ݒ�
			m_darkSpearObj[num][num2]->SetPosition(
				m_createPosition[num][num2]
			);
			//�_�[�N�X�s�A�̍ő�T�C�Y�����߂�B
			//�񂪑�����ƃT�C�Y���傫���Ȃ�
			m_darkSpearObj[num][num2]->SetMaxScale(m_rowsCount);

			//���������ꏊ�͈̔̓G�t�F�N�g���폜
			m_rangeEffect[num][num2]->Stop();
		}
	}
}

bool DarkSpear::IsCreateTiming()
{
	//��b���Ƃɐ�������
	if (m_createTimer > CREATE_TIMER_LIMMIT)
	{
		m_createTimer = 0.0f;
		return true;
	}
	else
	{
		//�^�C�}�[�����Z
		m_createTimer += g_gameTime->GetFrameDeltaTime();
	}

	return false;
}

void DarkSpear::PlayRangeEffect()
{
	//��񂲂Ƃɐ�������
	for (int num2 = 0; num2 < SPEAR_ROWS; num2++)
	{
		float size = num2 + 1;
		//�����ꏊ�̐��J��Ԃ�
		for (int num = 0; num < SPEAR_CREATE_PLACE; num++)
		{
			Vector3 createPos = m_createPosition[num][num2];
			createPos.y += 3.0f;

			//���������ꏊ�͈̔̓G�t�F�N�g���폜
			m_rangeEffect[num][num2] = NewGO<EffectEmitter>(0);
			m_rangeEffect[num][num2]->Init(InitEffect::enEffect_Meteo_Range);
			m_rangeEffect[num][num2]->Play();
			m_rangeEffect[num][num2]->SetScale(g_vec3One * (size * 7.0f));
			m_rangeEffect[num][num2]->SetPosition(createPos);
			m_rangeEffect[num][num2]->Update();
		}
	}
	//�G�t�F�N�g�������Z�b�g
	m_playRangeEffectFlag = true;
}

void DarkSpear::DeleteRangeEffect()
{
	//�G�t�F�N�g����������Ă��Ȃ��Ȃ珈�����Ȃ�
	if (m_playRangeEffectFlag != true)
	{
		return;
	}

	for (int num2 = 0; num2 < SPEAR_ROWS; num2++)
	{
		//�����ꏊ�̐��J��Ԃ�
		for (int num = 0; num < SPEAR_CREATE_PLACE; num++)
		{
			if (m_rangeEffect[num][num2] != nullptr)
			{
				//�͈̓G�t�F�N�g���폜
				m_rangeEffect[num][num2]->Stop();
			}
		}
	}
}