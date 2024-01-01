#pragma once
#include "IMobStateMachine.h"


class MobMonsterSM_Patrol:public IMobStateMachine
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="mobmonster"></param>
	MobMonsterSM_Patrol(MobMonster* mobmonster)
		:IMobStateMachine(mobmonster)
	{
	}

	/// <summary>
	/// デストラクタ
	/// </summary>
	~MobMonsterSM_Patrol() {}

	/// <summary>
	/// 毎フレーム行う処理
	/// </summary>
	void Execute() override;


private:

	/// <summary>
	/// アクションを決める処理
	/// </summary>
	void ProcessDecideAction();

	/// <summary>
	/// 巡回ステートを決める
	/// </summary>
	void ProcessPatrol();

private:


};

