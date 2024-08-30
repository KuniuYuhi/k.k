#include "stdafx.h"
#include "WaveManager.h"

#include "InitEffect.h"

#include "EnemyObjectPool.h"

#include "EnemyBase.h"

#include "MobEnemyheaderFiles.h"

#include "EnemyManager.h"


WaveManager::WaveManager()
{
}

WaveManager::~WaveManager()
{
}

bool WaveManager::Start()
{
	//乱数を初期化。
	srand((unsigned)time(NULL));

	//ステータスを初期化
	m_waveStatus.InitWaveStatus();


	m_currentWaveNumber = 0;

	return true;
}

void WaveManager::Update()
{
	//進行管理
	ManageWaveProgression();

}

void WaveManager::CalcWaveTimer(EnWaveManageState nextWaveState)
{
	//タイムリミットの達したら
	if (m_currentWaveTimer >= m_currentTimeLimit)
	{
		//次のステートに切り替える
		m_currentWaveManageState = nextWaveState;
		//タイマーをリセット
		m_currentWaveTimer = 0.0f;
	}
	else
	{
		//タイマーを加算
		m_currentWaveTimer += g_gameTime->GetFrameDeltaTime();
	}

}

void WaveManager::EvaluateWaveResults()
{
}

void WaveManager::SummonMobEnemys()
{
	//生成座標の数だけ繰り返す
	for (int i = 0; i < m_createPositions.size(); i++)
	{
		//todo エネミー数が上限を超えないようにする
		if (EnemyManager::GetInstance()->GetMobEnemyList().size() >=
			m_waveStatus.GetMaxMobEnemys())
		{
			return;
		}


		EnemyBase* enemy = nullptr;
		
		////モンスターの種類から一つランダムに選ぶ
		//int num = rand() % (int)enEnemy_Num;

		//switch (num)
		//{
		//case enEnemy_Slime:

		//	enemy = EnemyObjectPool::GetInstance()->OnGetEnemy<Slime>("Slime");
		//	break;
		//case enEnemy_Cactus:

		//	enemy = EnemyObjectPool::GetInstance()->OnGetEnemy<Cactus>("Cactus");
		//	break;

		//default:
		//	break;
		//}

		//enemy = EnemyObjectPool::GetInstance()->OnGetEnemy<Slime>("Slime");

		//enemy = EnemyObjectPool::GetInstance()->OnGetEnemy<Cactus>("Cactus");

		//enemy = EnemyObjectPool::GetInstance()->OnGetEnemy<Mushroom>("Mushroom");


		enemy = EnemyObjectPool::GetInstance()->OnGetEnemy<BeholderEye>("BeholderEye");

		//enemy = EnemyObjectPool::GetInstance()->OnGetEnemy<Mimic>("Mimic");


		//エネミーの座標を生成座標にする
		enemy->SetPosition(m_createPositions[i]);


	}


}

void WaveManager::CastSummmonCircle(int castAmount)
{
	//モンスターを生成する座標に魔法陣エフェクトを再生
	for (int num = 0; num < castAmount; num++)
	{
		Vector3 Pos = m_createPositions[num];
		Pos.y += 5.0f;
		EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
		effectEmitter->Init(InitEffect::enEffect_Mob_Summon_Circle);
		effectEmitter->Play();
		effectEmitter->SetPosition(Pos);
		effectEmitter->SetScale(g_vec3One * 15.0f);
		effectEmitter->Update();
	}
}

void WaveManager::SetSummonRandomPosition(const int radius, int amount)
{
	//前回の召喚で格納した座標リストをクリア
	m_createPositions.clear();
	//中心
	Vector3 Center = g_vec3Zero;

	//生成するモンスターの数繰り返す
	for (int num = 0; num < amount; num++)
	{
		//座標をリストに格納
		m_createPositions.emplace_back(g_vec3Zero);
	}

	int count = 0;

	Vector3 setPos = g_vec3Zero;

	while (count < amount)
	{
		//ランダムな値の最大値の計算
		int max = radius * 2 + 1;

		//円の範囲内でランダムな値を設定
		setPos.x = rand() % max - radius;
		setPos.z = rand() % max - radius;
		//生成する座標
		m_createPositions[count] = setPos;
		//生成する座標が半径より小さいなら
		if (m_createPositions[count].Length() < radius)
		{
			//他の召喚する座標、ボス、プレイヤーと一定の距離が空いているか判定
			if (IsWithInDistances(count, m_waveStatus.GetCoordinateDistance()) == true)
			{
				//次の座標を決めて調べる
				count++;
			}
		}
	}

}

bool WaveManager::IsWithInDistances(int count, float distance)
{
	if (count <= 1) return true;

	int distanceCount = 0;
	//他の生成する座標と一定の間隔を開ける
	for (int i = 0; i < count; i++)
	{
		Vector3 diff = m_createPositions[count] - m_createPositions[i];
		//生成する座標同士の距離を計算
		float length = diff.Length();
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
	//空いていない
	return false;
}

bool WaveManager::IsChangeSummonState()
{
	if (m_changeSummonTimer >= m_waveStatus.GetIncantationTimeLimit())
	{
		m_changeSummonTimer = 0.0f;
		return true;
	}
	else
	{
		m_changeSummonTimer += g_gameTime->GetFrameDeltaTime();
	}

	return false;
}

void WaveManager::ManageWaveProgression()
{
	switch (m_currentWaveManageState)
	{
	case WaveManager::enWaveState_FirstSummonMobEnemys://最初のエネミー生成
		ProcessFirstSummonMobEnemysStateTransition();
		break;
	case WaveManager::enWaveState_ProgressWaveTime://ウェーブ進行中
		ProcessProgressWaveTimeStateTransition();
		break;
	case WaveManager::enWaveState_EvaluateWaveResults://ウェーブの評価
		ProcessEvaluateWaveResultsStateTransition();
		break;
	case WaveManager::enWaveState_ShortBreakTime://小休憩
		ProcessShortBreakTimeStateTransition();
		break;
	case WaveManager::enWaveState_SettingCreatePos://召喚座標設定
		ProcessSettingCreatePosStateTransition();
		break;
	case WaveManager::enWaveState_SummonMobEnemys://モブエネミーの召喚
		ProcessSummonMobEnemysStateTransition();
		break;
	case WaveManager::enWaveState_BreakTimeWave:
		ProcessBreakTimeWaveStateTransition();
		break;

	default:
		break;
	}

}


void WaveManager::ProcessFirstSummonMobEnemysStateTransition()
{
	//最初のエネミーを召喚


	//ウェーブ１にする
	m_currentWaveNumber = 1;

	m_currentTimeLimit = m_waveStatus.GetCurrentWaveTimeLimit();

	//ウェーブ進行中ステートに遷移
	m_currentWaveManageState = enWaveState_ProgressWaveTime;
}

void WaveManager::ProcessProgressWaveTimeStateTransition()
{
	//ウェーブを進行する。
	//ウェーブが終わったら評価ステートに遷移
	CalcWaveTimer(enWaveState_EvaluateWaveResults);
}

void WaveManager::ProcessEvaluateWaveResultsStateTransition()
{
	//現在のウェーブが休憩ウェーブだったら評価しない
	//小休憩を飛ばす
	if (m_currentWaveNumber == m_waveStatus.GetBreakTimeWaveNumber())
	{
		//小休憩を飛ばして召喚座標設定ステートに遷移
		m_currentWaveManageState = enWaveState_SettingCreatePos;

	}
	else
	{
		//評価する
		EvaluateWaveResults();
		//小休憩ステートに遷移
		m_currentWaveManageState = enWaveState_ShortBreakTime;
		//タイマーを小休憩用のリミットに変更
		m_currentTimeLimit = m_waveStatus.GetShortBreakTime();
	}
	


	//現在のウェーブが最後のウェーブなら
	if (m_currentWaveNumber == m_waveStatus.GetTotalWaves())
	{
		//ウェーブを1にリセット
		m_currentWaveNumber = 1;
	}
	else
	{
		//ウェーブ番号を1進行
		m_currentWaveNumber++;
	}
	//次のウェーブ番号がブレイクタイムウェーブ番号と同じなら
	if (m_currentWaveNumber == m_waveStatus.GetBreakTimeWaveNumber())
	{
		//小休憩を飛ばして休憩ウェーブに遷移
		m_currentWaveManageState = enWaveState_BreakTimeWave;
		//タイマーをウェーブ進行中のリミットに変更
		m_currentTimeLimit = m_waveStatus.GetCurrentWaveTimeLimit();
	}

}

void WaveManager::ProcessShortBreakTimeStateTransition()
{
	//小休憩タイマーを計算
	//次のステートは召喚座標設定ステート
	CalcWaveTimer(enWaveState_SettingCreatePos);
}

void WaveManager::ProcessSettingCreatePosStateTransition()
{
	//座標設定
	SetSummonRandomPosition(
		m_waveStatus.GetCreateRadius(), 
		m_waveStatus.GetAddCreateMobEnemyAmonut()
	);
	//召喚魔法陣の設置
	CastSummmonCircle(m_waveStatus.GetAddCreateMobEnemyAmonut());
	//モブエネミーを召喚するステートに遷移
	m_currentWaveManageState = enWaveState_SummonMobEnemys;
}

void WaveManager::ProcessSummonMobEnemysStateTransition()
{
	//召喚するじかんになるまで処理をしない
	if (!IsChangeSummonState())
	{
		return;
	}

	//モブエネミーを召喚
	SummonMobEnemys();

	//タイマーをウェーブ進行中のリミットに変更
	m_currentTimeLimit = m_waveStatus.GetCurrentWaveTimeLimit();

	//ウェーブ進行中ステートに遷移
	m_currentWaveManageState = enWaveState_ProgressWaveTime;
}

void WaveManager::ProcessBreakTimeWaveStateTransition()
{
	//ウェーブを進行する。
	//ウェーブが終わったら評価ステートに遷移
	CalcWaveTimer(enWaveState_EvaluateWaveResults);
}
