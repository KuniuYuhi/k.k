#pragma once
#include "ISummonerState.h"

class Summoner;
class SummonerStateContext;

class SummonerState_DarkMeteorite : public ISummonerState
{
public:

	SummonerState_DarkMeteorite(Summoner* summoner, SummonerStateContext* context)
		:ISummonerState(summoner, context)
	{
	}

	~SummonerState_DarkMeteorite() {}

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
};

