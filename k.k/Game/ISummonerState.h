#pragma once

class Summoner;
class SummonerStateContext;

/// <summary>
/// サモナーのステートの基底クラス
/// </summary>
class ISummonerState
{
public:

	ISummonerState(Summoner* summoner, SummonerStateContext* context)
	{
		m_summoner = summoner;
		m_summonerStateContext = context;
	}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ISummonerState() {}

	/// <summary>
	/// アニメーションを再生
	/// </summary>
	virtual void PlayAnimation() = 0;
	/// <summary>
	/// 入った時の処理
	/// </summary>
	virtual void Entry() = 0;
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Ubdate() = 0;
	/// <summary>
	/// 終了時の処理
	/// </summary>
	virtual void Exit() = 0;

protected:
	//サモナーのインスタンス
	Summoner* m_summoner = nullptr;
	SummonerStateContext* m_summonerStateContext = nullptr;

};

