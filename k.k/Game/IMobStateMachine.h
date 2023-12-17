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
	/// インスタンスを生成
	/// </summary>
	/// <param name="bossInstance"></param>
	static void CreateInstance(MobMonster* mobInstance)
	{
		m_stateMachineInstance = new IMobStateMachine(mobInstance);
	}
	/// <summary>
	/// インスタンスの削除
	/// </summary>
	static void DeleteInstance()
	{
		delete m_stateMachineInstance;
		m_stateMachineInstance = nullptr;
	}

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static IMobStateMachine* GetInstance()
	{
		return m_stateMachineInstance;
	}

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
	static IMobStateMachine* m_stateMachineInstance;

	MobMonster* m_mobMonster = nullptr;

};

