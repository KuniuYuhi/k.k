#pragma once

class MobMonster;

class IMobStateMachine
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="mobMonsterInstance"></param>
	IMobStateMachine(MobMonster* mobMonsterInstance);
	/// <summary>
	/// デストラクタ
	/// </summary>
	~IMobStateMachine();

	/// <summary>
	/// 毎フレーム行う処理
	/// </summary>
	void Execute();

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
	/// 巡回ステートを決める
	/// </summary>
	void ProcessPatrol();

	/// <summary>
	/// 追いかけるステート
	/// </summary>
	void ProcessChase();



private:

	MobMonster* m_mobMonster = nullptr;

};

