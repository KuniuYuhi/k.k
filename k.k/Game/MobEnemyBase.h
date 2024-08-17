#pragma once
#include "EnemyBase.h"

class MobEnemyMovement;

class Brave;


/// <summary>
/// モブエネミー用のベースクラス
/// モブエネミーで共通する処理を実装する
/// </summary>
class MobEnemyBase : public EnemyBase
{
public:

	//virtual ~MobEnemyBase() = default;


	
	/// <summary>
	/// 被ダメージ処理
	/// </summary>
	virtual void Damage() override {}

	/// <summary>
	/// 死亡時処理
	/// </summary>
	virtual void Dead() override {}

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
	/// 追いかける移動処理
	/// </summary>
	/// <param name="targetPosition">ターゲットの座標</param>
	void ChaseMovement(Vector3 targetPosition);

	/// <summary>
	/// 回転処理
	/// </summary>
	void Rotation();



protected:

	Vector3 m_deactivatePosition = { 0.0f, 100.0f, 0.0f };

	//移動コンポーネント
	MobEnemyMovement* m_movement = nullptr;
	//


	bool m_isWaitingFlag = false;		//プレイヤーに近づき過ぎず遠くで待つかのフラグ

	Brave* m_player = nullptr;			//プレイヤーのインスタンス

};

