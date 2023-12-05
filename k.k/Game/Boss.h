#pragma once

#include "Summoner.h"


class BossBase;
class Summoner;
class Lich;

/// <summary>
/// ボスを生成、制御するクラス
/// </summary>
class Boss:public IGameObject
{
public:
	Boss();
	~Boss();

	bool Start();
	void Update();
	
	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position"></param>
	void SetBossPosition(Vector3 position)
	{
		m_bossBase->SetPosition(position);
	}

	/// <summary>
	/// ボスのステータスを取得
	/// </summary>
	/// <returns></returns>
	Status GetStatus()
	{
		return m_bossBase->GetStatus();
	}

private:
	/// <summary>
	/// ボスを生成
	/// </summary>
	void CreatBossCharacter();


private:
	BossBase* m_bossBase = nullptr;

	Summoner* m_sumoner = nullptr;
	Lich* m_lich = nullptr;

	Vector3 m_position = g_vec3Zero;


};

