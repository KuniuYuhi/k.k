#include "stdafx.h"
#include "CharactersInfoManager.h"

#include "Boss.h"

namespace {
	const float MONSTER_NEAR_PLAYER_DISTANCE = 350.0f;

	const int NUM_MONSTERS = 15;

	const int NUM_ENABLE_CHASE_PLAYER = 3;		//プレイヤーを追いかけられるモンスターの数

}

CharactersInfoManager* CharactersInfoManager::m_charactersInfoInstance = nullptr;

CharactersInfoManager::CharactersInfoManager()
{
	if (m_charactersInfoInstance != nullptr) {
		//インスタンスがすでに作られている。
		std::abort();
	}
	m_charactersInfoInstance = this;
}

CharactersInfoManager::~CharactersInfoManager()
{
	m_charactersInfoInstance = nullptr;
}

void CharactersInfoManager::Execute()
{
	SearchMonstersNearPlayer();
}

void CharactersInfoManager::SearchMonstersNearPlayer()
{
	//カウントを0にする
	m_monstersNearPlayerCount = 0;

	//プレイヤーのインスタンスがないなら処理しない
	if (m_playerInstance == nullptr)
	{
		return;
	}

	//モンスターの数が0なら処理しない
	if (m_mobMonsters.size() == 0)
	{
		return;
	}

	//モンスターの番号のカウント
	int monsterNumberCount = 0;

	//追いかけるモンスターの配列。モンスターの総数とm_mobMonstersの配列の番号
	//列行
	std::vector<std::vector<float>> array =
	{ {-1.0f,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f },
	{ -1.0f,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f } };
	

	for (auto monster : m_mobMonsters)
	{
		//プレイヤーからモンスターに向かうベクトルを計算
		Vector3 diff = monster->GetPosition() - m_playerInstance->GetPosition();

		//距離が一定以下なら
		if (diff.Length() < MONSTER_NEAR_PLAYER_DISTANCE)
		{
			//ベクトルとモンスターの配列の番号を設定
			array[0][m_monstersNearPlayerCount] = diff.Length();
			array[1][m_monstersNearPlayerCount] = monsterNumberCount;

			//カウントを加算
			m_monstersNearPlayerCount++;
		}
		//カウント加算
		monsterNumberCount++;
	}

	//todo終わりを示すために配列に-1を格納


	//プレイヤーに近いモンスターがいなかったら処理をしない
	if (m_monstersNearPlayerCount == 0)
	{
		return;
	}

	//リストをモンスターが近い順に入れ替える
	for (int i = 0; i < m_monstersNearPlayerCount; i++)
	{
		for (int j = i + 1; j < m_monstersNearPlayerCount; j++)
		{
			if (array[0][i] > array[0][j])
			{
				//長さの入れ替え
				int temp = array[0][i];
				array[0][i] = array[0][j];
				array[0][j] = temp;
				//番号の入れ替え
				temp = array[1][i];
				array[1][i] = array[1][j];
				array[1][j] = temp;
			}
		}
	}
	//プレイヤーに近づけるモンスターの数繰り返す
	for (int i = 0; i < m_monstersNearPlayerCount; i++)
	{
		//プレイヤーを追いかけることができる数
		if (i < NUM_ENABLE_CHASE_PLAYER)
		{
			//このモンスターは追いかけられる
			m_mobMonsters[array[1][i]]->SetEnableChasePlayerFag(true);
		}
		else
		{
			//追いかけられる数を超えたら終了
			return;
		}
	}
}

void CharactersInfoManager::SortMonsterToPlayerLength(float array[][2], int maxValue)
{
	for (int i = 0; i < maxValue; i++)
	{
		for (int j = i + 1; j < maxValue; j++)
		{
			//
			if (array[i][0] > array[j][0])
			{
				//長さの入れ替え
				int temp = array[i][0];
				array[i][0] = array[j][0];
				array[j][0] = temp;
				//番号の入れ替え
				temp = array[i][1];
				array[i][1] = array[j][1];
				array[j][1] = temp;

			}
		}
	}
}

void CharactersInfoManager::SetAllMonsterDamgeHitFlag(bool flag)
{
	//ボスのフラグを設定
	m_bossInstance->SetDamageHitEnableFlag(flag);

	//モンスターの数が0なら処理しない
	if (m_mobMonsters.size() == 0)
	{
		return;
	}

	for (auto monster : m_mobMonsters)
	{
		//モンスターのフラグを設定
		monster->SetDamageHitEnableFlag(flag);
	}

}

bool CharactersInfoManager::SearchMonsterNearPlayer(
	MobMonster* mobMonster)
{
	Vector3 diff= mobMonster->GetPosition() - m_playerInstance->GetPosition();

	//距離が一定以下なら
	if (diff.Length() < MONSTER_NEAR_PLAYER_DISTANCE)
	{
		//カウントを加算
		m_monstersNearPlayerCount++;
		//プレイヤーに近い
		return true;
	}
	//プレイヤーに近くない
	return false;
}
