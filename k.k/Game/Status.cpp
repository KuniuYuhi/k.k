#include "stdafx.h"
#include "Status.h"
#include <fstream>
#include <sstream>

//ステータスを記載したファイルパス
const std::string Status::csvFilePath = "Assets/CSV/Status.csv";
std::unordered_map<std::string, Status> Status::StatusDataMAP = {};
bool Status::IsLoadedCSVFile = false;


void Status::Init(const std::string& name)
{
	if (IsLoadedCSVFile == false) {

		LoadCSV(name);
	}
	//キーがnameである要素を探す
	auto itr = StatusDataMAP.find(name);
	//名前がない
	if (itr == StatusDataMAP.end())
	{
		return;
	}
	//引数と同じ名前のデータを代入
	*this = itr->second;

	return;
}

void Status::LoadCSV(const std::string& name) {

	//読み込んだcsvファイルのデータを格納するstring型
	std::string str_buf;
	//「,」までを読み込んで格納するstring型
	std::string buf;

	// 読み込むcsvファイルを開く(std::ifstreamのコンストラクタで開く)
	std::ifstream csvFile(csvFilePath);

	//最初の1行は飛ばす。
	getline(csvFile, str_buf);
	// getline関数で1行ずつ読み込む(読み込んだ内容はstr_bufに格納)
	while (getline(csvFile, str_buf))
	{
		Status statusData;
		// 「,」区切りごとにデータを読み込むためにistringstream型にする
		std::istringstream i_stream(str_buf);

		//名前を読み込む。
		getline(i_stream, buf, ',');
		statusData.name = buf;
		//HPを読み込む。
		getline(i_stream, buf, ',');
		statusData.maxHp = stoi(buf);
		//Attackを読み込む。
		getline(i_stream, buf, ',');
		statusData.defaultAtk = stoi(buf);
		//Speedを読み込む。
		getline(i_stream, buf, ',');
		statusData.defaultSpeed = stoi(buf);

		//名前を設定
		const char* di = statusData.name.c_str();
		statusData.characterName = di;
		//HPを設定
		statusData.hp = statusData.maxHp;
		//攻撃力を設定
		statusData.atk = statusData.defaultAtk;


		//マップに取得したデータを代入
		StatusDataMAP[statusData.name] = statusData;

	}
	//ロードした
	IsLoadedCSVFile = true;
	return;

}
