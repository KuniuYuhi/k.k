#pragma once
#include "EnemyBase.h"
#include "MobEnemyCommonStatus.h"

#include "KnockBackInfo.h"

class MobEnemyMovement;
class DamageProvider;

class Brave;
class MobEnemyCommonStatus;

using namespace DamageInformaiton;
using namespace KnockBackInfo;


/// <summary>
/// モブエネミー用のベースクラス
/// モブエネミーで共通する処理を実装する
/// </summary>
class MobEnemyBase : public EnemyBase
{
public:

	

	/// <summary>
	/// ターゲットとの距離の長さを計算
	/// </summary>
	/// <param name="target">ターゲットの座標</param>
	/// <returns>距離の長さ</returns>
	float CalcDistanceToTargetPosition(Vector3 target);

	/// <summary>
	/// 待機するかのフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetWaitingFlag(bool flag)
	{
		m_isWaitingFlag = flag;
	}



protected:

	/// <summary>
	/// 自身をオブジェクトプールに戻す
	/// </summary>
	virtual void ReleaseThis() = 0;

	/// <summary>
	/// 基本的なコンポーネントをセッティング
	/// </summary>
	virtual void SettingDefaultComponent() override;

	/// <summary>
	/// さらに追加するコンポーネントをセッティング
	/// </summary>
	virtual void AddMoreComponent() {}

	/// <summary>
	/// コリジョンを作成する
	/// </summary>
	virtual void CreateCollisionObject(){}

	/// <summary>
	/// ダメージを受けた時の処理
	/// </summary>
	virtual void ProcessHit(DamageInfo damageInfo){}

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
	/// 回転処理
	/// </summary>
	void Rotation();

	/// <summary>
	/// ノックバックする前の準備
	/// </summary>
	void SettingKnockBackProcess();

	void KnockBackMove(int listNum);

	void KnockBackGravityFall();


	/// <summary>
	/// ダメージフォントを生成
	/// </summary>
	/// <param name="hitDamage">ヒットダメージ</param>
	//void CreateDamageFont(int hitDamage);



protected:

	MobEnemyCommonStatus m_commonStatus;

	Vector3 m_deactivatePosition = { 0.0f, 100.0f, 0.0f };

	//移動コンポーネント
	MobEnemyMovement* m_movement = nullptr;
	//
	DamageProvider* m_damageProvider = nullptr;

	bool m_isWaitingFlag = false;		//プレイヤーに近づき過ぎず遠くで待つかのフラグ

	Brave* m_player = nullptr;			//プレイヤーのインスタンス

	std::vector<CurvePoint> m_curvePointList;			//ノックバックカーブポイントリスト

	Vector2 m_knockBackSpeed = g_vec2Zero;

	EnKnockBackPattern m_hitKnockBackPattern;
	float m_knockBackTimeScale = 0.0f;

	int oldAttackId = -1;				//前フレームの攻撃ID

	float m_starkTimer = 0.0f;			//硬直タイマー//全員使うやろ




	bool adadadadada = false;


};

