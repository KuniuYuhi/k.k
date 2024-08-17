#include "stdafx.h"
#include "CommonEnemyStatus.h"
#include <fstream>
#include <sstream>

std::unordered_map<std::string, CommonEnemyStatus> CommonEnemyStatus::StatusDataMAP = {};
//ステータスを記載したファイルパス
const std::string CommonEnemyStatus::commonEnemyStatusCsvFilePath = "Assets/CSV/CommonEnemyStatus.csv";

bool CommonEnemyStatus::IsLoadedCommonEnemyStatusCSVFile = false;


//エネミーのステータス
//[体力][攻撃力][基礎スピード][攻撃インターバル]

void CommonEnemyStatus::InitCommonEnemyStatus(const std::string& name)
{
	//CSVファイルをロードしていないなら
	if (IsLoadedCommonEnemyStatusCSVFile == false)
	{
		//CSVファイルを読み込む
		LoadCommonEnemyStatusCSV(name);
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

void CommonEnemyStatus::LoadCommonEnemyStatusCSV(const std::string& name)
{
	//読み込んだcsvファイルのデータを格納するstring型
	std::string str_buf;
	//「,」までを読み込んで格納するstring型
	std::string buf;
	// 読み込むcsvファイルを開く(std::ifstreamのコンストラクタで開く)
	std::ifstream csvFile(commonEnemyStatusCsvFilePath);

	//最初の1行は飛ばす。
	getline(csvFile, str_buf);
	// getline関数で1行ずつ読み込む(読み込んだ内容はstr_bufに格納)
	while (getline(csvFile, str_buf))
	{
		CommonEnemyStatus statusData;
		// 「,」区切りごとにデータを読み込むためにistringstream型にする
		std::istringstream i_stream(str_buf);
		//名前を読み込む。
		getline(i_stream, buf, ',');
		statusData.name = buf;
		//体力を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_commonEnemyStatus.maxHp = stoi(buf);
		//攻撃力を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_commonEnemyStatus.defaultPower = stoi(buf);
		//スピードを読み込む。
		getline(i_stream, buf, ',');
		statusData.m_commonEnemyStatus.defaultSpeed = stof(buf);
		//攻撃インターバルを読み込む。
		getline(i_stream, buf, ',');
		statusData.m_commonEnemyStatus.attackIntarval = stof(buf);
		//接近できる距離を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_commonEnemyStatus.approachDistance = stof(buf);
		//待機する距離を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_commonEnemyStatus.waitingDistance = stof(buf);
		
		//現在のHPを設定
		statusData.SetCurrentHp(statusData.m_commonEnemyStatus.maxHp);
		//現在の攻撃力を設定
		statusData.SetCurrentAtk(statusData.m_commonEnemyStatus.defaultPower);
	

		//マップに取得したデータを代入
		StatusDataMAP[statusData.name] = statusData;

	}
	//ロードした
	IsLoadedCommonEnemyStatusCSVFile = true;
	return;
}
