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
	
	//番号によって生成するモンスターを変える
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
	//召喚時の設定
	m_mobMonster->SetPosition(summonPosition);
	m_mobMonster->SetRotation(m_lich->GetRotation());
	//マネージャーのモブモンスターリストに追加
	CharactersInfoManager::GetInstance()->AddMobMonsterFromList(m_mobMonster);

	//召喚されたときの光のエフェクトの再生
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
