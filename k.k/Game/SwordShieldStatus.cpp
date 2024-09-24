#include "stdafx.h"
#include "SwordShieldStatus.h"
#include <fstream>
#include <sstream>

SwordShieldStatus SwordShieldStatus::StatusDataMAP;

//ステータスを記載したファイルパス
const std::string SwordShieldStatus::uniqueStatusCsvFilePath = "Assets/CSV/WeaponUniqueStatus/SwordShieldStatus.csv";
bool SwordShieldStatus::IsLoadedUniqueStatusCSVFile = false;


void SwordShieldStatus::LoadUniqueStatusCSV()
{
	//読み込んだcsvファイルのデータを格納するstring型
	std::string str_buf;
	//「,」までを読み込んで格納するstring型
	std::string buf;
	// 読み込むcsvファイルを開く(std::ifstreamのコンストラクタで開く)
	std::ifstream csvFile(uniqueStatusCsvFilePath);

	//最初の1行は飛ばす。
	getline(csvFile, str_buf);
	// getline関数で1行ずつ読み込む(読み込んだ内容はstr_bufに格納)
	while (getline(csvFile, str_buf))
	{
		SwordShieldStatus statusData;
		// 「,」区切りごとにデータを読み込むためにistringstream型にする
		std::istringstream i_stream(str_buf);

		//一コンボ目ノックバックの進行速度を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_uniqueStatus.firstAttackTimeScale = stof(buf);

		//二コンボ目ノックバックの進行速度を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_uniqueStatus.secondAttackTimeScale = stof(buf);

		//三コンボ目ノックバックの進行速度を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_uniqueStatus.thirdAttackTimeScale = stof(buf);

		//スキル攻撃のノックバックの進行速度を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_uniqueStatus.skillAttackTimeScale = stof(buf);

		for (int i = 0; i < 3; i++)
		{
			//通常攻撃のスピードを読み込む。
			getline(i_stream, buf, ',');
			statusData.m_uniqueStatus.normalAttackSpeed[i] = stof(buf);
		}

		//通常攻撃の索敵範囲(半径)を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_uniqueStatus.normalAttackSearchRadius = stof(buf);

		//通常攻撃で計算するベクトル同士の内積の比較対象の内積を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_uniqueStatus.normalAttackComparisonDot = stof(buf);


		//シールドの最大耐久値を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_uniqueStatus.maxShieldEnduranceValue = stoi(buf);

		//シールドのノックバック耐性レベルを読み込む。
		getline(i_stream, buf, ',');
		statusData.m_uniqueStatus.knockBackToleranceLevel = stoi(buf);
		

		//現在のシールドの耐久値を設定する
		statusData.SetCurrentShieldEnduranceValue(statusData.m_uniqueStatus.maxShieldEnduranceValue);

		//マップに取得したデータを代入
		StatusDataMAP = statusData;

	}
	//ロードした
	IsLoadedUniqueStatusCSVFile = true;
	return;
}

void SwordShieldStatus::InitUniqueStatus()
{
	//CSVファイルをロードしていないなら
	if (IsLoadedUniqueStatusCSVFile == false)
	{
		//CSVファイルを読み込む
		LoadUniqueStatusCSV();
	}

	//引数と同じ名前のデータを代入
	*this = StatusDataMAP;

	return;
}
