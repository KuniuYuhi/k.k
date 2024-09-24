#include "stdafx.h"
#include "WeaponStatus.h"
#include <fstream>
#include <sstream>


std::unordered_map<std::string, WeaponStatus> WeaponStatus::StatusDataMAP = {};
//ステータスを記載したファイルパス
const std::string WeaponStatus::weaponCommonStatusCsvFilePath = "Assets/CSV/WeaponCommonStatus.csv";

bool WeaponStatus::IsLoadedWeaponCommonStatusCSVFile = false;



// [基礎攻撃力][現在の攻撃力][スキル攻撃力]
// [スキルのスタミナ消費量][回避、防御のスタミナ消費量][武器の属性]
// [1コンボ目ノックバックパターン][2コンボ目ノックバックパターン][3コンボ目ノックバックパターン]
// [スキルのノックバックパターン]


void WeaponStatus::InitWeaponCommonStatus(const std::string& name)
{
	//CSVファイルをロードしていないなら
	if (IsLoadedWeaponCommonStatusCSVFile == false)
	{
		//CSVファイルを読み込む
		LoadWeaponCommonStatusCSV(name);
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

void WeaponStatus::LoadWeaponCommonStatusCSV(const std::string& name)
{
	//読み込んだcsvファイルのデータを格納するstring型
	std::string str_buf;
	//「,」までを読み込んで格納するstring型
	std::string buf;
	// 読み込むcsvファイルを開く(std::ifstreamのコンストラクタで開く)
	std::ifstream csvFile(weaponCommonStatusCsvFilePath);

	//最初の1行は飛ばす。
	getline(csvFile, str_buf);
	// getline関数で1行ずつ読み込む(読み込んだ内容はstr_bufに格納)
	while (getline(csvFile, str_buf))
	{
		WeaponStatus statusData;
		// 「,」区切りごとにデータを読み込むためにistringstream型にする
		std::istringstream i_stream(str_buf);
		//名前を読み込む。
		getline(i_stream, buf, ',');
		statusData.name = buf;
		
		//基礎攻撃力を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status.defaultPower = stoi(buf);

		//スキル攻撃力を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status.skillPower = stoi(buf);

		//スキル攻撃のスタミナ消費量を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status.skillStaminaCost = stof(buf);

		//回避、防御のスタミナ消費量を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status.defensiveStaminaCost = stof(buf);

		//武器の属性を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status.weaponAttribute = StringToWeaponAttribute(buf);

		//1コンボ目のノックバックパターンを読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status.firstComboPattern = StringToKnockBackPattern(buf);
		
		//2コンボ目のノックバックパターンを読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status.secondComboPattern = StringToKnockBackPattern(buf);

		//3コンボ目のノックバックパターンを読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status.thirdComboPattern = StringToKnockBackPattern(buf);

		//スキルのノックバックパターンを読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status.m_SkillPattern = StringToKnockBackPattern(buf);


		//現在の攻撃力を設定
		statusData.SetCurrentPower(statusData.m_status.defaultPower);

		//マップに取得したデータを代入
		StatusDataMAP[statusData.name] = statusData;

	}
	//ロードした
	IsLoadedWeaponCommonStatusCSVFile = true;
	return;
}

EnWeaponAttribute WeaponStatus::StringToWeaponAttribute(std::string& attributeName)
{
	//斬タイプなら
	if (attributeName == "Slash")
	{
		return EnWeaponAttribute::enWeaponAttribute_Slash;
	}
	//突タイプなら
	else if(attributeName == "Pierce")
	{
		return EnWeaponAttribute::enWeaponAttribute_Pierce;
	}
	//ここまで来たらいけない
	std::abort();
	return EnWeaponAttribute::enWeaponAttribute_None;
}

EnKnockBackPattern WeaponStatus::StringToKnockBackPattern(std::string& knockBackPattern)
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
