#pragma once


class WaveTimer
{
public:
	WaveTimer(){}
	~WaveTimer(){}




	/// <summary>
	/// フェーズ内での処理のステップ
	/// </summary>
	enum EnWaveStep
	{
		enPhaseStep_WaveProgress,	//ウェーブ進行
		enPhaseStep_ShortBreak,		//小休憩
	};

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();




private:



private:

	float m_waveTimer = 0.0f;		//ウェーブ管理するタイマー

	int m_nowWaveCounter = 0;			//現在のウェーブの段階

	int m_brakeTimeWaveCount = 3;		//いつのカウントで休憩タイムにするか

};

