#pragma once
#include "EnemyBase.h"
#include "KnockBackInfo.h"


class DamageProvider;
class BossEnemyMovement;
class Brave;

using namespace DamageInformaiton;
using namespace KnockBackInfo;

/// <summary>
/// ボスエネミーベースクラス
/// </summary>
class BossEnemyBase : public EnemyBase
{
public:

	void SetArrivedApproachDistance(bool flag)
	{
		m_isArrivedApproachDistance = flag;
	}

	bool IsArrivedApproachDistance()
	{
		return m_isArrivedApproachDistance;
	}

	/// <summary>
	/// 無敵にする
	/// </summary>
	void InvicibleActive()
	{
		m_isInvincible = true;
	}
	/// <summary>
	/// 無敵を解除
	/// </summary>
	void InvicibleDeactive()
	{
		m_isInvincible = false;
	}
	/// <summary>
	/// 無敵か？
	/// </summary>
	/// <returns></returns>
	bool IsInvincible()
	{
		return m_isInvincible;
	}



	/// <summary>
	/// ダメージ情報を設定する
	/// </summary>
	/// <param name="damageId"></param>
	/// <param name="attackPower"></param>
	/// <param name="knockBackTimeScale"></param>
	/// <param name="knockBackPattern"></param>
	/// <param name="weaponAttribute"></param>
	void SetDamageInfo(
		int damageId,int attackPower,float knockBackTimeScale,
		EnKnockBackPattern knockBackPattern,
		EnWeaponAttribute weaponAttribute = enWeaponAttribute_None
	);



protected:

	/// <summary>
	/// 自身をオブジェクトプールに戻す
	/// </summary>
	virtual void ReleaseThis() = 0;

	/// <summary>
	/// 当たり判定の処理をしない条件
	/// </summary>
	/// <returns></returns>
	virtual bool IgnoreCollision() = 0;

	/// <summary>
	/// 基本的なコンポーネントをセッティング
	/// </summary>
	virtual void SettingDefaultComponent() override;

	/// <summary>
	/// さらに追加するコンポーネントをセッティング
	/// </summary>
	virtual void AddMoreComponent() {}

	/// <summary>
	/// ダメージを受けた時の処理
	/// </summary>
	virtual void ProcessHit(DamageInfo damageInfo) {}

	/// <summary>
	/// 自身の当たり判定確認
	/// </summary>
	void CheckSelfCollision();


	/// <summary>
	/// 追いかける移動処理
	/// </summary>
	/// <param name="targetPosition">ターゲットの座標</param>
	void ChaseMovement(Vector3 targetPosition);


	/// <summary>
	/// ターゲットのほうに向く
	/// </summary>
	void TurnToTarget();
	
	/// <summary>
	/// 攻撃時の追いかける移動
	/// </summary>
	/// <param name="targetPosition">ターゲットの座標</param>
	/// <param name="speed">速さ</param>
	void AttackChaseMovement(Vector3 targetPosition,float speed);


protected:


	DamageProvider* m_damageProvider = nullptr;
	BossEnemyMovement* m_movement = nullptr;

	Brave* m_player = nullptr;			//プレイヤーのインスタンス

	std::vector<CurvePoint> m_curvePointList;			//ノックバックカーブポイントリスト

	Vector2 m_knockBackSpeed = g_vec2Zero;

	EnKnockBackPattern m_hitKnockBackPattern;
	float m_knockBackTimeScale = 0.0f;

	bool m_isArrivedApproachDistance = false;		//接近できる距離に到達したか


	int oldAttackId = -1;				//前フレームの攻撃ID


	bool m_isStopMove = false;			//移動するのを止める

	bool m_isInvincible = false;		//無敵か？

	bool m_isCreateCollision = false;	//当たり判定を生成するか？




};

