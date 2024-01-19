#include "stdafx.h"
#include "CharactersInfoManager.h"

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


	//追いかけるモンスターの配列。モンスターの総数とm_mobMonstersの配列の番号
	//行列
	//float chaseMonsters[NUM_MONSTERS][2];
	//モンスターの番号のカウント
	int monsterNumberCount = 0;

	//列行
	std::vector<std::vector<float>> array =
	{ {-1.0f,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f },
	{ -1.0f,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f ,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f } };
	

	for (auto monster : m_mobMonsters)
	{
		//一定の範囲外なら値を-１にする
		//chaseMonsters[m_monstersNearPlayerCount][0] = -1.0f;
		//chaseMonsters[m_monstersNearPlayerCount][1] = -1;

		//プレイヤーからモンスターに向かうベクトルを計算
		Vector3 diff = monster->GetPosition() - m_playerInstance->GetPosition();

		//距離が一定以下なら
		if (diff.Length() < MONSTER_NEAR_PLAYER_DISTANCE)
		{
			//座標とモンスターの番号を保存
			//chaseMonsters[m_monstersNearPlayerCount][0] = diff.Length();
			//chaseMonsters[m_monstersNearPlayerCount][1] = monsterNumberCount;
		
			array[0][m_monstersNearPlayerCount] = diff.Length();
			array[1][m_monstersNearPlayerCount] = monsterNumberCount;

			//カウントを加算
			m_monstersNearPlayerCount++;
		}
		

		//カウント加算
		monsterNumberCount++;
	}

	//プレイヤーに近いモンスターがいなかったら処理をしない
	if (m_monstersNearPlayerCount == 0)
	{
		return;
	}


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

			//if (chaseMonsters[i][0] > chaseMonsters[j][0])
			//{
			//	//長さの入れ替え
			//	int temp = chaseMonsters[i][0];
			//	chaseMonsters[i][0] = chaseMonsters[j][0];
			//	chaseMonsters[j][0] = temp;
			//	//番号の入れ替え
			//	temp = chaseMonsters[i][1];
			//	chaseMonsters[i][1] = chaseMonsters[j][1];
			//	chaseMonsters[j][1] = temp;

			//}
		}
	}


	



	//for (int i = 0; i < NUM_ENABLE_CHASE_PLAYER; i++)
	//{		
	//	//-１の時はもうリストがないので処理を終わる
	//	/*if (chaseMonsters[i][1] == -1)
	//	{
	//		break;
	//	}*/

	//	//このモンスターはプレイヤーを追いかけられる！
	//	//m_mobMonsters[chaseMonsters[i][1]]->SetEnableChasePlayerFag(true);
	//}
	

	for (int i = 0; i < m_monstersNearPlayerCount; i++)
	{
		if (i < NUM_ENABLE_CHASE_PLAYER)
		{
			//int a = 0;
			m_mobMonsters[array[1][i]]->SetEnableChasePlayerFag(true);
		}
		else
		{
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
