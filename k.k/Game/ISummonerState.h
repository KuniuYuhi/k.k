#pragma once

#include "IBossStateMachine.h"

class Summoner;

using namespace SummonerActions;

/// <summary>
/// ボスのステートの基底クラス
/// </summary>
class ISummonerState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="bossBase">ボスのインスタンス</param>
	/// <param name="actionName">アクションの名前</param>
	ISummonerState(Summoner* bossBase, EnActionName actionName)
	{
		m_summoner = bossBase;
		m_actionName = actionName;
	}

	virtual ~ISummonerState(){}

	/// <summary>
	/// 状態を管理する
	/// </summary>
	virtual void ManageState() = 0;
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	virtual void PlayAnimation() = 0;

	/// <summary>
	/// アクションの優先度の取得方法変更
	/// </summary>
	/// <returns></returns>
	const int GetPriority() const
	{
		return m_priority;
	}

	/// <summary>
	/// 行動の名前を取得
	/// </summary>
	/// <returns></returns>
	const EnActionName GetActionName()
	{
		return m_actionName;
	}

protected:
	/// <summary>
	/// アクションの優先度の設定
	/// </summary>
	/// <param name="priority"></param>
	void SetPriority(const int priority)
	{
		m_priority = priority;
	}
	
	/// <summary>
	/// 自身のアクションの名前を設定
	/// </summary>
	/// <param name="actionName"></param>
	void SetActionName(EnActionName actionName)
	{
		m_actionName = actionName;
	}


protected:
	Summoner* m_summoner = nullptr;

	int m_priority = 0;		//アクションの優先度
	EnActionName m_actionName;

};

