#include "stdafx.h"
#include "Status.h"
#include <fstream>
#include <sstream>

//ステータスを記載したファイルパス
const std::string Status::csvFilePath = "Assets/CSV/Status.csv";
const std::string Status::weaponCsvFilePath= "Assets/CSV/WeaponStatus.csv";

std::unordered_map<std::string, Status> Status::StatusDataMAP = {};
bool Status::IsLoadedCSVFile = false;
bool Status::IsLoadedWeaponCSVFile = false;



void Status::InitCharacterStatus(const std::string& name)
{
	//CSVファイルをロードしていないなら
	if (IsLoadedCSVFile == false) 
	{
		//CSVファイルを読み込む
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

void Status::InitWeaponStatus(const std::string& weaponName)
{
	//武器のCSVファイルを読み込んでいないなら
	if (IsLoadedWeaponCSVFile == false) 
	{
		//武器のCSVファイルを読み込む
		LoadWeaponCSV(weaponName);
	}
	//キーがnameである要素を探す
	auto itr = StatusDataMAP.find(weaponName);
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
		statusData.characterStatus.maxHp = stoi(buf);
		//Attackを読み込む。
		getline(i_stream, buf, ',');
		statusData.characterStatus.defaultAtk = stoi(buf);
		//Speedを読み込む。
		getline(i_stream, buf, ',');
		statusData.characterStatus.defaultSpeed = stoi(buf);
		//スーパーアーマーを読み込む。ボス以外は-1
		getline(i_stream, buf, ',');
		statusData.characterStatus.maxSuperArmorPoint = stoi(buf);



		//HPを設定
		statusData.characterStatus.hp = statusData.characterStatus.maxHp;
		//攻撃力を設定
		statusData.characterStatus.atk = statusData.characterStatus.defaultAtk;
		//スーパーアーマーの値を設定
		statusData.characterStatus.superArmorPoint = statusData.characterStatus.maxSuperArmorPoint;

		//マップに取得したデータを代入
		StatusDataMAP[statusData.name] = statusData;

	}
	//ロードした
	IsLoadedCSVFile = true;
	return;
}

void Status::LoadWeaponCSV(const std::string& name)
{
	//読み込んだcsvファイルのデータを格納するstring型
	std::string str_buf;
	//「,」までを読み込んで格納するstring型
	std::string buf;

	// 読み込むcsvファイルを開く(std::ifstreamのコンストラクタで開く)
	std::ifstream csvFile(weaponCsvFilePath);

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
		//攻撃力を読み込む。
		getline(i_stream, buf, ',');
		statusData.weaponStatus.defaultAtk = stoi(buf);
		//耐久力を読み込む。
		getline(i_stream, buf, ',');
		statusData.weaponStatus.maxEndurance = stoi(buf);
		//スキル攻撃力を読み込む。
		getline(i_stream, buf, ',');
		statusData.weaponStatus.skillAtk = stoi(buf);

		for (int i = 0; i < 3; i++)
		{
			//通常攻撃のノックバックパワーを読み込む。
			getline(i_stream, buf, ',');
			statusData.weaponStatus.comboKnockBackPower[i] = stoi(buf);
		}

		//スキルのノックバックパワーを読み込む。
		getline(i_stream, buf, ',');
		statusData.weaponStatus.skillKnockBackPower = stoi(buf);

		//攻撃力を設定
		statusData.weaponStatus.atk = statusData.weaponStatus.defaultAtk;
		//耐久力を設定
		statusData.weaponStatus.endurance = statusData.weaponStatus.maxEndurance;

		//マップに取得したデータを代入
		StatusDataMAP[statusData.name] = statusData;

	}
	//ロードした
	IsLoadedWeaponCSVFile = true;
	return;
}
