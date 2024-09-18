#include "stdafx.h"
#include "EffectItemStatus.h"
#include <fstream>
#include <sstream>


std::unordered_map<std::string, EffectItemStatus> EffectItemStatus::StatusDataMAP = {};
//ステータスを記載したファイルパス
const std::string EffectItemStatus::effectItemStatusCsvFilePath = "Assets/CSV/EffectItemStatus.csv";

bool EffectItemStatus::IsLoadedEffectItemStatusCSVFile = false;


void EffectItemStatus::InitEffectItemStatus(const std::string& name)
{
	//CSVファイルをロードしていないなら
	if (IsLoadedEffectItemStatusCSVFile == false)
	{
		//CSVファイルを読み込む
		LoadEffectItemStatusCSV(name);
	}
	//キーがnameである要素を探す
	auto itr = StatusDataMAP.find(name);
	//名前がない
	if (itr == StatusDataMAP.end())
	{
		std::abort();
		return;
	}
	//引数と同じ名前のデータを代入
	*this = itr->second;

	return;
}

void EffectItemStatus::LoadEffectItemStatusCSV(const std::string& name)
{
	//読み込んだcsvファイルのデータを格納するstring型
	std::string str_buf;
	//「,」までを読み込んで格納するstring型
	std::string buf;
	// 読み込むcsvファイルを開く(std::ifstreamのコンストラクタで開く)
	std::ifstream csvFile(effectItemStatusCsvFilePath);

	//最初の1行は飛ばす。
	getline(csvFile, str_buf);
	// getline関数で1行ずつ読み込む(読み込んだ内容はstr_bufに格納)
	while (getline(csvFile, str_buf))
	{
		EffectItemStatus statusData;
		// 「,」区切りごとにデータを読み込むためにistringstream型にする
		std::istringstream i_stream(str_buf);
		//名前を読み込む。
		getline(i_stream, buf, ',');
		statusData.name = buf;

		//効果量を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status.effectValue = stoi(buf);

		//持続時間を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status.duration = stof(buf);
		
		//マップに取得したデータを代入
		StatusDataMAP[statusData.name] = statusData;

	}
	//ロードした
	IsLoadedEffectItemStatusCSVFile = true;
	return;
}
