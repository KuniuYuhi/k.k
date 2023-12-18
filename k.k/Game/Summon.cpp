#include "stdafx.h"
#include "Summon.h"
#include "Lich.h"
#include "AIActor.h"

#include "Slime.h"
#include "TurtleShell.h"
#include "Mushroom.h"
#include "Cactus.h"

#include "InitEffect.h"

#include "CharactersInfoManager.h"

namespace {
	const float PI = 3.14f;

	const int SUMMON_POS_MAX_SIZE = 10;
}


Summon::Summon()
{
}

Summon::~Summon()
{
}

bool Summon::Start()
{
	m_lichPosition = m_lich->GetPosition();
	//�i�[�ł���T�C�Y��������߂�
	m_summonPositions.reserve(SUMMON_POS_MAX_SIZE);

	//m_createMonsters���󋵂ɂ���ĕς�����

	//����������W�����߂�
	CalcCirclePoints(
		m_lichPosition,
		300.0f,
		m_createMonsters
	);

	SetCircleEffect();

	return true;
}

void Summon::Update()
{
	//�����t���O�������Ă��Ȃ��Ȃ炱�̐揈�����Ȃ�
	if (m_isSummonStartFlag != true)
	{
		return;
	}

	//��������
	//�i�[���ꂽ���W���烂���X�^�[����������
	SetSummonMonsterPos();
	//�������I������̂ō폜
	DeleteGO(this);
}

void Summon::CalcCirclePoints(Vector3 center, float radius, int numPoints)
{
	//todo �{�X�̌����ɂ���Ă�����
	//�p�x
	float en = 360.0f;
	en /= numPoints;

	for (int i = 1; i <= numPoints; i++)
	{
		//�p�x�����W�A���ɕϊ�
		float theta = Math::PI / 180 * en * i;

		Vector3 pos = g_vec3Zero;
		pos.x = center.x + radius * cos(theta);
		pos.z = center.z + radius * sin(theta);

		//����������W���i�[����
		m_summonPositions.emplace_back(pos);
	}
}

void Summon::SetSummonMonsterPos()
{
	//�@�������������B
	srand((unsigned)time(NULL));

	for (auto summonPos : m_summonPositions)
	{
		//�i�[�������W�Ƀ����X�^�[����������
		SummonMonster(summonPos);
	}

}

void Summon::SummonMonster(Vector3 summonPosition)
{
	//�^�[�g���͈�x�̏�����2�̂܂�
	if (m_summonTurtleShellCount <= 0)
	{
		m_divMonster = 3;
	}
	
	//�ŏ��̏����Ȃ�^�[�g�����������Ȃ�
	if (m_firstSummonFlag == true)
	{
		m_divMonster = 3;
	}
	
	//�����_���ɑI��
	int number = rand() % m_divMonster;
	
	//�ԍ��ɂ���Đ������郂���X�^�[��ς���
	switch (number)
	{
	case enMonster_Slime:
		m_mobMonster = NewGO<Slime>(0, "slime");
		break;
	case enMonster_TurtleShell:
		m_summonTurtleShellCount--;
		m_mobMonster = NewGO<TurtleShell>(0, "turtleshell");
		break;
	case enMonster_Mushroom:
		m_mobMonster = NewGO<Mushroom>(0, "mushroom");
		break;
	case enMonster_Cactus:
		m_mobMonster = NewGO<Cactus>(0, "cactus");
		break;                   

	default:
		break;
	}
	//�������̐ݒ�
	m_mobMonster->SetPosition(summonPosition);
	m_mobMonster->SetRotation(m_lich->GetRotation());
	//�}�l�[�W���[�̃��u�����X�^�[���X�g�ɒǉ�
	CharactersInfoManager::GetInstance()->AddMobMonsterFromList(m_mobMonster);

	//�������ꂽ�Ƃ��̌��̃G�t�F�N�g�̍Đ�
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(InitEffect::enEffect_Mob_Summon_Right);
	effectEmitter->Play();
	effectEmitter->SetPosition(summonPosition);
	effectEmitter->SetScale({ 15.0f,15.0f,15.0f });
	effectEmitter->Update();

}

void Summon::SetCircleEffect()
{
	for (auto pos : m_summonPositions)
	{
		Vector3 Pos = pos;
		Pos.y += 5.0f;
		EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
		effectEmitter->Init(InitEffect::enEffect_Mob_Summon_Circle);
		effectEmitter->Play();
		effectEmitter->SetPosition(Pos);
		effectEmitter->SetScale({ 15.0f,15.0f,15.0f });
		effectEmitter->Update();
	}
}
