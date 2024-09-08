#pragma once
#include "IMobEnemyState.h"
#include "MushroomStateContext.h"

class Mushroom;
class MushroomStateContext;

class MushroomState_Attack : public IMobEnemyState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="slime">キノコのインスタンス</param>
	/// <param name="stateContest">ステートコンテキスト</param>
	MushroomState_Attack(Mushroom* mushroom, MushroomStateContext* stateContest)
	{
		m_mushroom = mushroom;
		m_stateCotext = stateContest;
	}

	~MushroomState_Attack();

	/// <summary>
	/// アニメーションを再生
	/// </summary>
	void PlayAnimation() override;
	/// <summary>
	/// 入った時の処理
	/// </summary>
	void Entry() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Ubdate() override;
	/// <summary>
	/// 終了時の処理
	/// </summary>
	void Exit() override;


private:
	Mushroom* m_mushroom = nullptr;
	MushroomStateContext* m_stateCotext = nullptr;
};

