#include "stdafx.h"
#include "Summon.h"
#include "Lich.h"
#include "AIActor.h"

#include "Slime.h"
#include "TurtleShell.h"
#include "Mushroom.h"
#include "Cactus.h"

#include "InitEffect.h"

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
	//格納できるサイズ上限を決める
	m_summonPositions.reserve(SUMMON_POS_MAX_SIZE);

	//m_createMonsters←状況によって変えたい

	//召喚する座標を決める
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
	//召喚フラグが立っていないならこの先処理しない
	if (m_isSummonStartFlag != true)
	{
		return;
	}

	//召喚処理
	//格納された座標からモンスターを召喚する
	SetSummonMonsterPos();
	//召喚が終わったので削除
	DeleteGO(this);
}

void Summon::CalcCirclePoints(Vector3 center, float radius, int numPoints)
{
	//todo ボスの向きによってかえる
	//角度
	float en = 360.0f;
	en /= numPoints;

	for (int i = 1; i <= numPoints; i++)
	{
		//角度をラジアンに変換
		float theta = Math::PI / 180 * en * i;

		Vector3 pos = g_vec3Zero;
		pos.x = center.x + radius * cos(theta);
		pos.z = center.z + radius * sin(theta);

		//召喚する座標を格納する
		m_summonPositions.emplace_back(pos);
	}
}

void Summon::SetSummonMonsterPos()
{
	//　乱数を初期化。
	srand((unsigned)time(NULL));

	for (auto summonPos : m_summonPositions)
	{
		//格納した座標にモンスターを召喚する
		SummonMonster(summonPos);
	}

}

void Summon::SummonMonster(Vector3 summonPosition)
{
	//タートルは一度の召喚で2体まで
	if (m_summonTurtleShellCount <= 0)
	{
		m_divMonster = 3;
	}
	
	//最初の召喚ならタートルを召喚しない
	if (m_firstSummonFlag == true)
	{
		m_divMonster = 3;
	}
	
	//ランダムに選ぶ
	int number = rand() % m_divMonster;
	

	switch (number)
	{
	case enMonster_Slime:
		m_slime = NewGO<Slime>(0, "slime");
		m_slime->SetPosition(summonPosition);
		m_slime->SetRotation(m_lich->GetRotation());
		m_slime->SetLich(m_lich);
		m_lich->AddAIActorFromList(m_slime);
		break;
	case enMonster_TurtleShell:
		m_summonTurtleShellCount--;
		m_turtleShell = NewGO<TurtleShell>(0, "turtleshell");
		m_turtleShell->SetPosition(summonPosition);
		m_turtleShell->SetRotation(m_lich->GetRotation());
		m_turtleShell->SetLich(m_lich);
		m_lich->AddAIActorFromList(m_turtleShell);
		break;
	case enMonster_Mushroom:
		m_mushroom = NewGO<Mushroom>(0, "mushroom");
		m_mushroom->SetPosition(summonPosition);
		m_mushroom->SetRotation(m_lich->GetRotation());
		m_mushroom->SetLich(m_lich);
		m_lich->AddAIActorFromList(m_mushroom);
		break;
	case enMonster_Cactus:
		m_cactus = NewGO<Cactus>(0, "cactus");
		m_cactus->SetPosition(summonPosition);
		m_cactus->SetRotation(m_lich->GetRotation());
		m_cactus->SetLich(m_lich);
		m_lich->AddAIActorFromList(m_cactus);
		break;

	default:
		break;
	}

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
