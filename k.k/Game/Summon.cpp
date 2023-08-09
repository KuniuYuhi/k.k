#include "stdafx.h"
#include "Summon.h"
#include "Lich.h"
#include "AIActor.h"

#include "Slime.h"
#include "TurtleShell.h"
#include "Mushroom.h"
#include "Cactus.h"

namespace {
	const float PI = 3.14f;
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

	m_monsters.emplace_back(m_slime);
	m_monsters.emplace_back(m_turtleShell);
	m_monsters.emplace_back(m_mushroom);
	m_monsters.emplace_back(m_cactus);


	return true;
}

void Summon::Update()
{
	//m_createMonsters���󋵂ɂ���ĕς�����

	//����������W�����߂�
	CalcCirclePoints(
		m_lichPosition,
		500.0f,
		m_createMonsters
	);

	//�i�[���ꂽ���W���烂���X�^�[����������
	SetSummonMonsterPos();

	DeleteGO(this);
}

void Summon::CalcCirclePoints(Vector3 center, float radius, int numPoints)
{
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
	//�����_���ɑI��
	int number = rand() % 4;

	switch (number)
	{
	case enMonster_Slime:
		m_slime = NewGO<Slime>(0, "slime");
		m_slime->SetPosition(summonPosition);
		m_slime->SetRotation(m_lich->GetRotation());
		break;
	case enMonster_TurtleShell:
		m_turtleShell = NewGO<TurtleShell>(0, "turtleshell");
		m_turtleShell->SetPosition(summonPosition);
		m_turtleShell->SetRotation(m_lich->GetRotation());
		break;
	case enMonster_Mushroom:
		m_mushroom = NewGO<Mushroom>(0, "mushroom");
		m_mushroom->SetPosition(summonPosition);
		m_mushroom->SetRotation(m_lich->GetRotation());
		break;
	case enMonster_Cactus:
		m_cactus = NewGO<Cactus>(0, "cactus");
		m_cactus->SetPosition(summonPosition);
		m_cactus->SetRotation(m_lich->GetRotation());
		break;

	default:
		break;
	}


}
