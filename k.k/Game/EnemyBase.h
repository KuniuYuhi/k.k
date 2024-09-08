#pragma once
#include "CharacterBase.h"
#include "CommonEnemyStatus.h"			//エネミーのステータス


class CommonEnemyStatus;

/// <summary>
/// エネミー用のベースクラス
/// エネミーで共通する処理を実装する
/// </summary>
class EnemyBase : public CharacterBase
{
public:

	//virtual ~EnemyBase() = default;


	/// <summary>
	/// 基本的ステータスを取得
	/// </summary>
	/// <returns></returns>
	CommonEnemyStatus& GetCommonStatus()
	{
		return m_status;
	}

	/// <summary>
	/// アクション中か
	/// </summary>
	/// <returns></returns>
	const bool IsAction()
	{
		return isAction;
	}
	/// <summary>
	/// アクション中にする
	/// </summary>
	void ActionActive()
	{
		isAction = true;
	}
	/// <summary>
	/// アクションを終わる
	/// </summary>
	void ActionDeactive()
	{
		isAction = false;
	}



protected:


	/// <summary>
	/// 基本的なコンポーネントをセッティング
	/// </summary>
	virtual void SettingDefaultComponent() override {}


	/// <summary>
	/// ダメージを受ける
	/// </summary>
	/// <param name="damage">受けるダメージ量</param>
	virtual void TakeDamage(int damage);


	/// <summary>
	/// ターゲットとの距離の長さを計算
	/// </summary>
	/// <param name="target">ターゲットの座標</param>
	/// <returns>距離の長さ</returns>
	float CalcDistanceToTargetPosition(Vector3 target);


protected:

	//基本的なステータス
	CommonEnemyStatus m_status;

	


	bool isAction = false;		//アクション中かのフラグ

};

