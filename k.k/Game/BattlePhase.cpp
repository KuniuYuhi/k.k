#include "stdafx.h"
#include "BattlePhase.h"

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


namespace {
	//const int SUMMON_MONSTERS_CONUT = 10;

	const int SUMMMON_RADIUS = 900.0;

	const float SUMMON_POS_DISTANCE = 320.0f;

	const float SUMMON_POS_DISTANCE_FOR_BOSS = 200.0f;
	const float SUMMON_POS_DISTANCE_FOR_PLAYER = 100.0f;

	const float INCANTATION_TIME_LIMMIT = 2.0f;

	const float CIRCLE_EFFECT_SIZE = 15.0F;

}

BattlePhase::BattlePhase()
{
}

BattlePhase::~BattlePhase()
{
	//todo エフェクトの削除
}

bool BattlePhase::Start()
{
	//乱数を初期化。
	srand((unsigned)time(NULL));

	for (int num = 0; num < SUMMON_MONSTERS_CONUT; num++)
	{
		model[num].Init("Assets/modelData/character/Slime/slime.tkm",
			L"Assets/shader/ToonTextrue/lamp_Slime.DDS");
		model[num].Update();
	}

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
		//召喚する座標の設定
		SetSummonRandomPosition(SUMMMON_RADIUS);
		//召喚陣の設置
		CreateMagicCircleEffect(SUMMON_MONSTERS_CONUT);
		//次のステップに進む
		m_enSummonMonsterStep = enSummonMonsterStep_SummonMonsters;
		break;
	case BattlePhase::enSummonMonsterStep_SummonMonsters:
		//一定時間待つ
		if (IsIncantation() == true)
		{
			//モンスターを生成
			CreateMobMonsters(SUMMON_MONSTERS_CONUT);
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

void BattlePhase::CreateMobMonsters(const int createCount)
{
	//フェーズによって敵の種類を変える
	AdjustDifficultyByPhase(createCount);

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
	//中心
	Vector3 Center = g_vec3Zero;
	//生成する座標
	for (int num = 0; num < SUMMON_MONSTERS_CONUT; num++)
	{
		m_createPos[num] = g_vec3Zero;
	}

	//X座標とZ座標
	float X;
	float Z;
	//配列のカウント
	int count = 0;
	//ランダムな値の最大値の計算
	int max = radius * 2 + 1;
	//生成する座標が決まるまでループ
	while (count< SUMMON_MONSTERS_CONUT)
	{
		//円の範囲内でランダムな値を設定
		X = rand() % max - radius;
		Z = rand() % max - radius;
		//生成する座標
		m_createPos[count] = { X,0.0f,Z };
		//生成する座標が半径より小さいなら、座標を決定
		if (m_createPos[count].Length() < radius)
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
	Vector3 diff = m_createPos[count] -
		CharactersInfoManager::GetInstance()->GetBossInstance()->GetPosition();
	//ボスとの間隔が狭かったら処理を止める(座標設定やり直し)
	if (diff.Length() < SUMMON_POS_DISTANCE_FOR_BOSS)
	{
		return false;
	}
	//プレイヤーとの間隔を調べる
	diff = m_createPos[count] -
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
		float length = (m_createPos[count] - m_createPos[i]).Length();
		//距離が離れていたら、生成する座標に決定
		if (length > distance)
		{
			distanceCount++;
		}
	}
	//全ての座標と距離が空いていたら
	if (distanceCount == count)
	{
		//
		return true;
	}
	else
	{
		return false;
	}
	
}

void BattlePhase::AdjustDifficultyByPhase(const int summonMonstersCount)
{
	//todo メタAiでやってみたい
	switch (GameManager::GetInstance()->GetNowPhaseState())
	{
		//難易度：簡単
	case GameManager::EnPhaseState_Phase1:
		processEasyDifficulty(summonMonstersCount);
		break;
		//難易度：普通
	case GameManager::EnPhaseState_Phase2:
		processNormalDifficulty(summonMonstersCount);
		break;
		//難易度：難しい
	case GameManager::EnPhaseState_Phase3:
		processHardDifficulty(summonMonstersCount);
		break;
	default:
		break;
	}



}

void BattlePhase::processEasyDifficulty(const int summonMonstersCount)
{
	for (int num = 0; num < summonMonstersCount; num++)
	{
		Slime* slime = NewGO<Slime>(0, "slime");
		slime->SetPosition(m_createPos[num]);
		CharactersInfoManager::GetInstance()->AddMobMonsterFromList(slime);
		//召喚時の光のエフェクトの再生
		CreateSummonLightEffect(m_createPos[num]);
	}
}

void BattlePhase::processNormalDifficulty(const int summonMonstersCount)
{
	for (int num = 0; num < summonMonstersCount; num++)
	{
		Cactus* cactus = NewGO<Cactus>(0, "cactus");
		cactus->SetPosition(m_createPos[num]);
		CharactersInfoManager::GetInstance()->AddMobMonsterFromList(cactus);
		//召喚時の光のエフェクトの再生
		CreateSummonLightEffect(m_createPos[num]);
	}
}

void BattlePhase::processHardDifficulty(const int summonMonstersCount)
{
	for (int num = 0; num < summonMonstersCount; num++)
	{
		//Cactus* cactus = NewGO<Cactus>(0, "cactus");
		//cactus->SetPosition(m_createPos[num]);
		//CharactersInfoManager::GetInstance()->AddMobMonsterFromList(cactus);
		////召喚時の光のエフェクトの再生
		//CreateSummonLightEffect(m_createPos[num]);

		Mushroom* mushroom = NewGO<Mushroom>(0, "mushroom");
		mushroom->SetPosition(m_createPos[num]);
		CharactersInfoManager::GetInstance()->AddMobMonsterFromList(mushroom);
		//召喚時の光のエフェクトの再生
		CreateSummonLightEffect(m_createPos[num]);
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
		Vector3 Pos = m_createPos[num];
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


