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
	/// ボス座標の取得
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_bossBase->GetPosition();
	}
	/// <summary>
	/// 前方向の取得
	/// </summary>
	/// <returns></returns>
	const Vector3& GetForward() const
	{
		return m_bossBase->GetForward();
	}

	/// <summary>
	/// 回転の取得
	/// </summary>
	/// <returns></returns>
	const Quaternion& GetRotation() const
	{
		return m_bossBase->GetRotation();
	}

	/// <summary>
	/// ボスのステータスを取得
	/// </summary>
	/// <returns></returns>
	Status GetStatus()
	{
		return m_bossBase->GetStatus();
	}

	/// <summary>
	/// キャラクターコントローラーの取得
	/// </summary>
	/// <returns></returns>
	CharacterController& GetCharacterController()
	{
		return m_bossBase->GetCharacterController();
	}


	/// <summary>
	/// ダメージを受けられるかのフラグを設定
	/// </summary>
	/// <returns></returns>
	void SetDamageHitEnableFlag(bool flag)
	{
		m_bossBase->SetDamageHitEnableFlag(flag);
	}
	/// <summary>
	/// ダメージを受けられるかのフラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetDamageHitEnableFlag() const
	{
		return m_bossBase->GetDamageHitEnableFlag();
	}



private:
	/// <summary>
	/// ボスを生成
	/// </summary>
	void CreatBossCharacter();

	/// <summary>
	/// ボスを削除してもよいか
	/// </summary>
	void IsDeleteBoss();

private:
	BossBase* m_bossBase = nullptr;

	Summoner* m_sumoner = nullptr;
	Lich* m_lich = nullptr;

	Vector3 m_position = g_vec3Zero;


};

