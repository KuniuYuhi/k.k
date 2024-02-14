#include "stdafx.h"
#include "BattlePhase.h"

#include "MobMonstersList.h"

#include "GameManager.h"
#include "CharactersInfoManager.h"

#include "Boss.h"
#include "Player.h"

#include "InitEffect.h"

//召喚するモンスターのヘッダーファイル
#include "Slime.h"
#include "TurtleShell.h"
#include "Mushroom.h"
#include "Cactus.h"

#include "MobMonster.h"

using namespace MobMonstersList;

namespace {
	const int DEFAULT_SUMMON_MOB_MONSTERS_CONUT = 7;
	const int MAX_SUMMON_MOB_MONSTERS_CONUT = 15;

	const int ADD_SUMMON_MONSTER_COUNT = 2;
	const int SUB_SUMMON_MONSTER_COUNT = 5;

	const int SUMMMON_RADIUS = 900.0;

	const float SUMMON_POS_DISTANCE = 320.0f;

	const float SUMMON_POS_DISTANCE_FOR_BOSS = 200.0f;
	const float SUMMON_POS_DISTANCE_FOR_PLAYER = 100.0f;

	const float INCANTATION_TIME_LIMMIT = 2.0f;

	const float CIRCLE_EFFECT_SIZE = 15.0f;

	const int NONE_PRIORITY = -1;

}

BattlePhase::BattlePhase()
{
}

BattlePhase::~BattlePhase()
{
}

bool BattlePhase::Start()
{
	//乱数を初期化。
	srand((unsigned)time(NULL));

	//最初に召喚生成するモンスターの数を設定
	m_mobMonsterSpawnCount = DEFAULT_SUMMON_MOB_MONSTERS_CONUT;

	return true;
}

void BattlePhase::Update()
{
	//勝敗が決まったら処理を止める
	if (GameManager::GetInstance()->GetOutComeState() !=
		GameManager::enOutComeState_None)
	{
		return;
	}

	//フェーズが変わったときにモブモンスターを生成
	//モンスターを生成ステートなら
	if (GameManager::GetInstance()->GetDAndCMonstersState() == 
		GameManager::enMonsters_Create)
	{
		//モンスターを召喚する
		ProcessSummonMonsters();
	}

	//モンスターを削除ステートなら
	if (GameManager::GetInstance()->GetDAndCMonstersState() ==
		GameManager::enMonsters_Delete)
	{
		//現在のフェーズのモブモンスターを削除
		DeleteMobMonsters();
		//削除したのでゲームマネージャーのステートをなしにする
		GameManager::GetInstance()->
			SetDAndCMonstersState(GameManager::enMonsters_None);
	}
}

void BattlePhase::ProcessSummonMonsters()
{
	switch (m_enSummonMonsterStep)
	{
	case BattlePhase::enSummonMonsterStep_SettingMagicCircle:
		//召喚するモンスターの数を決める
		 SetMobMonsterSpawnCount();
		//召喚する座標の設定
		SetSummonRandomPosition(SUMMMON_RADIUS);
		//召喚陣の設置
		CreateMagicCircleEffect(m_mobMonsterSpawnCount);
		//次のステップに進む
		m_enSummonMonsterStep = enSummonMonsterStep_SummonMonsters;
		break;
	case BattlePhase::enSummonMonsterStep_SummonMonsters:
		//一定時間待つ
		if (IsIncantation() == true)
		{
			//モンスターを生成
			CreateMobMonsters();
			//召喚したのでゲームマネージャーのステートをなしにする
			GameManager::GetInstance()->
				SetDAndCMonstersState(GameManager::enMonsters_None);
			//ステップをリセット
			m_enSummonMonsterStep = enSummonMonsterStep_SettingMagicCircle;
		}
		break;
	default:
		break;
	}

	


}

void BattlePhase::SetMobMonsterSpawnCount()
{
	//現在のフェーズが1かつ一回以上フェーズがループしているなら
	//生成するモンスターの数を少し減らす
	if (GameManager::GetInstance()->GetPhaseLoopCount() != 0 &&
		GameManager::GetInstance()->GetNowPhaseState() ==
		GameManager::EnPhaseState_Phase1)
	{
		m_mobMonsterSpawnCount -= SUB_SUMMON_MONSTER_COUNT;
	}

	//モンスターを倒した数で増やしてもよいかも
	m_mobMonsterSpawnCount += ADD_SUMMON_MONSTER_COUNT;


	//モンスターの数が制限をこえないようにする
	if (m_mobMonsterSpawnCount > MAX_SUMMON_MOB_MONSTERS_CONUT)
	{
		m_mobMonsterSpawnCount = MAX_SUMMON_MOB_MONSTERS_CONUT;
	}

}

void BattlePhase::CreateMobMonsters()
{
	//フェーズによって敵の種類を変える
	AdjustDifficultyByPhase(m_mobMonsterSpawnCount);

}

void BattlePhase::DeleteMobMonsters()
{
	//生成する前に要素を全て削除
	for (auto monster : CharactersInfoManager::GetInstance()->GetMobMonsters())
	{
		monster->ProcessDead(false);
		DeleteGO(monster);
	}
	//リストをクリア
	CharactersInfoManager::GetInstance()->GetMobMonsters().clear();
}

void BattlePhase::SetSummonRandomPosition(const int radius)
{
	//前回の召喚で格納した座標リストをクリア
	m_createPositions.clear();
	//中心
	Vector3 Center = g_vec3Zero;
	//生成するモンスターの数繰り返す
	for (int num = 0; num < m_mobMonsterSpawnCount; num++)
	{
		//座標をリストに格納
		m_createPositions.emplace_back(g_vec3Zero);
	}

	//X座標とZ座標
	float X;
	float Z;
	//配列のカウント
	int count = 0;
	//ランダムな値の最大値の計算
	int max = radius * 2 + 1;
	//生成する座標が決まるまでループ
	while (count< m_mobMonsterSpawnCount)
	{
		//円の範囲内でランダムな値を設定
		X = rand() % max - radius;
		Z = rand() % max - radius;
		//生成する座標
		m_createPositions[count] = { X,0.0f,Z };
		//生成する座標が半径より小さいなら、座標を決定
		if (m_createPositions[count].Length() < radius)
		{
			//他の召喚する座標と一定の距離が空いているか判定
			if (IsWithInDistances(count, SUMMON_POS_DISTANCE) == true)
			{
				//次の配列に進める
				count++;
			}
		}
	}
}

bool BattlePhase::IsWithInDistances(int count, float distance)
{
	//ボスとの間隔を調べる
	Vector3 diff = m_createPositions[count] -
		CharactersInfoManager::GetInstance()->GetBossInstance()->GetPosition();
	//ボスとの間隔が狭かったら処理を止める(座標設定やり直し)
	if (diff.Length() < SUMMON_POS_DISTANCE_FOR_BOSS)
	{
		return false;
	}
	//プレイヤーとの間隔を調べる
	diff = m_createPositions[count] -
		CharactersInfoManager::GetInstance()->GetPlayerInstance()->GetPosition();
	//プレイヤーとの間隔が狭かったら処理を止める(座標設定やり直し)
	if (diff.Length() < SUMMON_POS_DISTANCE_FOR_PLAYER)
	{
		return false;
	}

	int distanceCount = 0;
	//他の生成する座標と一定の間隔を開ける
	for (int i = 0; i < count; i++)
	{
		//生成する座標同士の距離を計算
		float length = (m_createPositions[count] - m_createPositions[i]).Length();
		//距離が離れていたら、生成する座標に決定
		if (length > distance)
		{
			distanceCount++;
		}
	}
	//全ての座標と距離が空いていたら
	if (distanceCount == count)
	{
		//空いている
		return true;
	}
	else
	{
		//空いていない
		return false;
	}
	
}

void BattlePhase::AdjustDifficultyByPhase(const int summonMonstersCount)
{
	//難易度によって生成されるモンスターの種類を制限

	//優先するモブモンスターを設定するか決める
	//優先するモブモンスターの番号を取得
	int priority = GetMobMonsterPriority();
	
	for (int num = 0; num < summonMonstersCount; num++)
	{
		//モンスターを生成する
		MobMonster* monster = GenerateMobMonster(priority);
		//座標を設定
		monster->SetPosition(m_createPositions[num]);
		//モブモンスターリストに格納
		CharactersInfoManager::GetInstance()->AddMobMonsterFromList(monster);
		//召喚時の光のエフェクトの再生
		CreateSummonLightEffect(m_createPositions[num]);
	}

}

bool BattlePhase::IsIncantation()
{
	if (m_incantationTimer > INCANTATION_TIME_LIMMIT)
	{
		m_incantationTimer = 0.0f;
		return true;
	}
	else
	{
		m_incantationTimer += g_gameTime->GetFrameDeltaTime();
		return false;
	}
	
}

void BattlePhase::CreateMagicCircleEffect(const int monsterCount)
{
	//モンスターを生成する座標に魔法陣エフェクトを再生
	for (int num = 0; num < monsterCount; num++)
	{
		Vector3 Pos = m_createPositions[num];
		Pos.y += 5.0f;
		EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
		effectEmitter->Init(InitEffect::enEffect_Mob_Summon_Circle);
		effectEmitter->Play();
		effectEmitter->SetPosition(Pos);
		effectEmitter->SetScale(g_vec3One * CIRCLE_EFFECT_SIZE);
		effectEmitter->Update();
	}
}

void BattlePhase::CreateSummonLightEffect(Vector3 createPos)
{
	//召喚されたときの光のエフェクトの再生
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(InitEffect::enEffect_Mob_Summon_Right);
	effectEmitter->Play();
	effectEmitter->SetPosition(createPos);
	effectEmitter->SetScale(g_vec3One * CIRCLE_EFFECT_SIZE);
	effectEmitter->Update();
}

const int BattlePhase::GetMobMonsterPriority()
{
	//初期値は優先するモブモンスターなし
	int num = NONE_PRIORITY;

	//フェーズが一度もループしていないなら優先するモブモンスターを決めない
	if (GameManager::GetInstance()->GetPhaseLoopCount() == 0)
	{
		return num;
	}

	//優先度を決めるかの判断
	int judgeValue = rand() % 21-10;
	//judgeValueが一定値より下なら優先度を決めない
	if (judgeValue < 5)
	{
		return num;
	}

	//優先するモブモンスターをランダムに決める
	num = rand()% (int)enMobMonsterList_Num;

	return num;
}

MobMonster* BattlePhase::GenerateMobMonster(int priorityMobMonsterNumber)
{
	//フェーズによって生成しないランクのモンスターを決める

	//モブモンスターのインスタンス
	MobMonster* monster = nullptr;
	//生成するモブモンスターの番号を代入する
	int num = 0;

	//優先したいモブモンスターの番号があるならそっちの番号を優先する
	if (priorityMobMonsterNumber >= 0)
	{
		num = priorityMobMonsterNumber;
	}
	else
	{
		//モンスターの種類から一つランダムに選ぶ
		num = rand() % (int)enMobMonsterList_Num;
	}
	
	//番号に対応したモンスターを生成
	switch (num)
	{
	case enMobMonsterList_Slime:
		monster = NewGO<Slime>(0,"slime");
		break;
	case enMobMonsterList_Cactus:
		monster = NewGO<Cactus>(0, "cactus");
		break;
	case enMobMonsterList_Mushroom:
		monster = NewGO<Mushroom>(0, "mushroom");
		break;
	case enMobMonsterList_Turtleshell:
		monster = NewGO<TurtleShell>(0, "turtleshell");
		break;
	default:
		std::abort();
		break;
	}


	return monster;
}


