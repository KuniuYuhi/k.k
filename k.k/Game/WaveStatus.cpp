#include "stdafx.h"
#include "WaveStatus.h"
#include <fstream>
#include <sstream>


WaveStatus WaveStatus::StatusDataMAP;

//ステータスを記載したファイルパス
const std::string WaveStatus::waveStatusCsvFilePath = "Assets/CSV/WaveStatus.csv";
bool WaveStatus::IsLoadedWaveStatusCSVFile = false;


//ウェーブのステータス
// [トータルウェーブ数][休憩ウェーブの番号]
// [1ウェーブ時間][ウェーブ間隔時間][初手生成数]
// [追加生成数][生成範囲(半径)][召喚できない範囲(ボス)][召喚できない範囲(プレイヤー)]
// [陣から生成までの時間]

void WaveStatus::LoadWaveStatusCSV()
{
	//読み込んだcsvファイルのデータを格納するstring型
	std::string str_buf;
	//「,」までを読み込んで格納するstring型
	std::string buf;
	// 読み込むcsvファイルを開く(std::ifstreamのコンストラクタで開く)
	std::ifstream csvFile(waveStatusCsvFilePath);

	//最初の1行は飛ばす。
	getline(csvFile, str_buf);
	// getline関数で1行ずつ読み込む(読み込んだ内容はstr_bufに格納)
	while (getline(csvFile, str_buf))
	{
		WaveStatus statusData;
		// 「,」区切りごとにデータを読み込むためにistringstream型にする
		std::istringstream i_stream(str_buf);

		//トータルウェーブ数を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status.totalWaves = stoi(buf);

		//休憩ウェーブの番号を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status.breakTimeWaveNumber = stoi(buf);

		//1ウェーブ時間を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status.waveTimeLimit = stof(buf);

		//ウェーブ間隔時間を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status.shortBreakTime = stof(buf);

		//エネミー最大数を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status.maxMobEnemys = stoi(buf);

		//初手生成数を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status.firstCreateMobEnemyAmount = stoi(buf);

		//追加生成数を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status.addCreateMobEnemyAmonut = stoi(buf);

		//生成範囲(半径)を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status.createRadius = stof(buf);

		//生成座標同士最低限空ける距離を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status.coordinateDistance = stof(buf);

		//召喚できない範囲(ボス)を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status.bossAvoidanceRadius = stof(buf);

		//召喚できない範囲(プレイヤー)を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status.playerAvoidanceRadius = stof(buf);

		//陣から生成までの時間を読み込む
		getline(i_stream, buf, ',');
		statusData.m_status.incantationTimeLimit = stof(buf);

		//マップに取得したデータを代入
		StatusDataMAP = statusData;

	}
	//ロードした
	IsLoadedWaveStatusCSVFile = true;
	return;
}

void WaveStatus::InitWaveStatus()
{
	//CSVファイルをロードしていないなら
	if (IsLoadedWaveStatusCSVFile == false)
	{
		//CSVファイルを読み込む
		LoadWaveStatusCSV();
	}
	
	//引数と同じ名前のデータを代入
	*this = StatusDataMAP;

	return;

}
