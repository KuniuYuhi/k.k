#include "stdafx.h"
#include "MobEnemyCommonStatus.h"
#include <fstream>
#include <sstream>


std::unordered_map<std::string, MobEnemyCommonStatus> MobEnemyCommonStatus::StatusDataMAP = {};
//ステータスを記載したファイルパス
const std::string MobEnemyCommonStatus::mobEnemyCommonStatusCsvFilePath = "Assets/CSV/EnemyUniqueStatus/MobEnemy/MobEnemyCommonStatus.csv";

bool MobEnemyCommonStatus::IsLoadedmobEnemyCommonStatusCSVFile = false;



void MobEnemyCommonStatus::InitMobEnemyCommonStatus(const std::string& name)
{
	//CSVファイルをロードしていないなら
	if (IsLoadedmobEnemyCommonStatusCSVFile == false)
	{
		//CSVファイルを読み込む
		LoadEnemyCommonStatusCSV(name);
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

void MobEnemyCommonStatus::LoadEnemyCommonStatusCSV(const std::string& name)
{
	//読み込んだcsvファイルのデータを格納するstring型
	std::string str_buf;
	//「,」までを読み込んで格納するstring型
	std::string buf;
	// 読み込むcsvファイルを開く(std::ifstreamのコンストラクタで開く)
	std::ifstream csvFile(mobEnemyCommonStatusCsvFilePath);

	//最初の1行は飛ばす。
	getline(csvFile, str_buf);
	// getline関数で1行ずつ読み込む(読み込んだ内容はstr_bufに格納)
	while (getline(csvFile, str_buf))
	{
		MobEnemyCommonStatus statusData;
		// 「,」区切りごとにデータを読み込むためにistringstream型にする
		std::istringstream i_stream(str_buf);
		//名前を読み込む。
		getline(i_stream, buf, ',');
		statusData.name = buf;

		//通常攻撃のノックバックパターンを読み込む。
		getline(i_stream, buf, ',');
		statusData.m_mobStatus.normalAttackKnockBackPattern = StringToKnockBackPattern(buf);

		//通常攻撃のノックバックの進行速度を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_mobStatus.normalAttackTimeScale = stof(buf);


		//マップに取得したデータを代入
		StatusDataMAP[statusData.name] = statusData;

	}
	//ロードした
	IsLoadedmobEnemyCommonStatusCSVFile = true;
	return;

}

EnKnockBackPattern MobEnemyCommonStatus::StringToKnockBackPattern(std::string& knockBackPattern)
{
	//地面に接したまま少し後ずさるパターン
	if (knockBackPattern == "ShortGroundedRetreat")
	{
		return EnKnockBackPattern::enKBPattern_ShortGroundedRetreat;
	}
	//地面に接したまま長めに後ずさるパターン
	else if (knockBackPattern == "LongGroundedRetreat")
	{
		return EnKnockBackPattern::enKBPattern_LongGroundedRetreat;
	}
	//少し吹っ飛びながら後ずさるパターン
	else if (knockBackPattern == "SlightAirborneRetreat")
	{
		return EnKnockBackPattern::enKBPattern_SlightAirborneRetreat;
	}
	//強めに吹っ飛びながら後ずさるパターン
	else if (knockBackPattern == "FullAirborneRetreat")
	{
		return EnKnockBackPattern::enKBPattern_FullAirborneRetreat;
	}
	//その場から動かずに空中に吹っ飛ぶ(空中に浮く)
	else if (knockBackPattern == "VerticalLaunch")
	{
		return EnKnockBackPattern::enKBPattern_VerticalLaunch;
	}

	//ここまで来たらいけない
	std::abort();
	return EnKnockBackPattern::enKnockBackPattern_Num;
}
