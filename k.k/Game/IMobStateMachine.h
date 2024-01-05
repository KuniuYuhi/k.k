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
	virtual void Execute(){}

protected:

	MobMonster* m_mobMonster = nullptr;

};

