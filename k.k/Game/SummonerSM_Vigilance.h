#pragma once
#include "IBossStateMachine.h"

class SummonerSM_Vigilance :public IBossStateMachine
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="summoner"></param>
	SummonerSM_Vigilance(Summoner* summoner,bool saveTimerlesetFlag = false)
		:IBossStateMachine(summoner)
	{
		Init(saveTimerlesetFlag);
	}
	/// <summary>
	/// デストラクタ
	/// </summary>
	~SummonerSM_Vigilance();

	/// <summary>
	/// 毎フレーム行う処理
	/// </summary>
	virtual void Execute() override;



private:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="saveTimerlesetFlag">ステイプレイヤータイマーをリセットするか</param>
	void Init(bool saveTimerlesetFlag);

	/// <summary>
	/// アクションを決める処理
	/// </summary>
	void ProcessDecideAction();

	/// <summary>
	///	待機ステートを決める
	/// </summary>
	void ProcessWaitState();

	/// <summary>
	/// ボスがアクション中か
	/// </summary>
	/// <returns></returns>
	bool IsInActionBoss()
	{
		return false;
	}

	/// <summary>
	/// ノックバックするか決める処理
	/// </summary>
	/// <returns>ノックバックするならtrue</returns>
	bool IsKnockBack();
	/// <summary>
	/// ノックバックタイマーを加速させるか
	/// </summary>
	void IsKnockBackTimerAccelerate();

	/// <summary>
	/// プレイヤーが近くにとどまっているタイマーを加算する
	/// </summary>
	void AddStayPlayerTimer();

	/// <summary>
	/// プレイヤーを追いかけるか
	/// </summary>
	/// <returns>追いかけるならtrue,待機ならfalse</returns>
	bool IsChasePlayer();

	


private:
	Vector3 m_toPlayer = g_vec3Zero;

	bool m_idleStateFlag = true;

	static float m_decideActionTimer;


	float m_waitTimer = 0.0f;		//待機時間を計算するタイマー

	
	float m_accelerateStayPlayerTimer = 1.0f;	//プレイヤーが近くにいる間を計るタイマーを加速させる


};

