#include "stdafx.h"
#include "SummonerSkillStatus.h"
#include <fstream>
#include <sstream>

SummonerSkillStatus SummonerSkillStatus::StatusDataMAP;

//ステータスを記載したファイルパス
const std::string SummonerSkillStatus::skillTypeStatusCsvFilePath = 
"Assets/CSV/EnemyUniqueStatus/BossEnemy/SummonerSkillStatus.csv";

bool SummonerSkillStatus::IsLoadedSkillTypeStatusCSVFile = false;


void SummonerSkillStatus::InitSkillStatus()
{
	//CSVファイルをロードしていないなら
	if (IsLoadedSkillTypeStatusCSVFile == false)
	{
		//CSVファイルを読み込む
		LoadSkillStatusCSV();
	}

	//引数と同じ名前のデータを代入
	*this = StatusDataMAP;

	return;
}

void SummonerSkillStatus::LoadSkillStatusCSV()
{
	//読み込んだcsvファイルのデータを格納するstring型
	std::string str_buf;
	//「,」までを読み込んで格納するstring型
	std::string buf;
	// 読み込むcsvファイルを開く(std::ifstreamのコンストラクタで開く)
	std::ifstream csvFile(skillTypeStatusCsvFilePath);

	//最初の1行は飛ばす。
	getline(csvFile, str_buf);


	//インスタンス
	SummonerSkillStatus statusData;

	//配列の番号
	int num = 0;

	// getline関数で1行ずつ読み込む(読み込んだ内容はstr_bufに格納)
	while (getline(csvFile, str_buf))
	{
		// 「,」区切りごとにデータを読み込むためにistringstream型にする
		std::istringstream i_stream(str_buf);

		//パターンの名前は飛ばす
		getline(i_stream, buf, ',');

		//攻撃力を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_skillStatus[num].power = stoi(buf);

		//ノックバックの進行速度を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_skillStatus[num].knockBackTimeScale = stof(buf);

		//ノックバックのパターンを読み込む。
		getline(i_stream, buf, ',');
		statusData.m_skillStatus[num].m_knockBackPattern = StringToKnockBackPattern(buf);
		
		//次の配列にする
		num++;

	}


	//マップに取得したデータを代入
	StatusDataMAP = statusData;

	//ロードした
	IsLoadedSkillTypeStatusCSVFile = true;
	return;
}

EnKnockBackPattern SummonerSkillStatus::StringToKnockBackPattern(std::string& knockBackPattern)
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
