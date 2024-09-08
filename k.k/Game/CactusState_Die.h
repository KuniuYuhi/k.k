#pragma once
#include "IMobEnemyState.h"
#include "CactusStateContext.h"

class Cactus;
class CactusStateContext;

class CactusState_Die : public IMobEnemyState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="slime">サボテンのインスタンス</param>
	/// <param name="stateContest">ステートコンテキスト</param>
	CactusState_Die(Cactus* cactus, CactusStateContext* stateContest)
	{
		m_cactus = cactus;
		m_stateCotext = stateContest;
	}

	~CactusState_Die();

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
	Cactus* m_cactus = nullptr;
	CactusStateContext* m_stateCotext = nullptr;
};

