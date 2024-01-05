#pragma once
#include "IMobStateMachine.h"

class MobMonsterSM_Chase:public IMobStateMachine
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="mobmonster"></param>
	MobMonsterSM_Chase(MobMonster* mobmonster)
		:IMobStateMachine(mobmonster)
	{
	}

	/// <summary>
	/// デストラクタ
	/// </summary>
	~MobMonsterSM_Chase(){}

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
	/// 次のアクションを決める
	/// </summary>
	void DecideNextAction();


	/// <summary>
	/// 攻撃ステートを決める
	/// </summary>
	void ProcessAttackState();

	/// <summary>
	/// 追いかけるステート
	/// </summary>
	void ProcessChase();

private:

};

