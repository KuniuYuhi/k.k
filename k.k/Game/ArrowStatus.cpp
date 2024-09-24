#include "stdafx.h"
#include "ArrowStatus.h"
#include <fstream>
#include <sstream>

std::unordered_map<std::string, ArrowStatus> ArrowStatus::StatusDataMAP = {};
//ステータスを記載したファイルパス
const std::string ArrowStatus::arrowStatusCsvFilePath = "Assets/CSV/WeaponUniqueStatus/ArrowStatus.csv";

bool ArrowStatus::IsLoadedArrowStatusCSVFile = false;


void ArrowStatus::InitArrowStatus(const std::string& name)
{
	//CSVファイルをロードしていないなら
	if (IsLoadedArrowStatusCSVFile == false)
	{
		//CSVファイルを読み込む
		LoadArrowStatusCSV(name);
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

void ArrowStatus::LoadArrowStatusCSV(const std::string& name)
{
	//読み込んだcsvファイルのデータを格納するstring型
	std::string str_buf;
	//「,」までを読み込んで格納するstring型
	std::string buf;
	// 読み込むcsvファイルを開く(std::ifstreamのコンストラクタで開く)
	std::ifstream csvFile(arrowStatusCsvFilePath);

	//最初の1行は飛ばす。
	getline(csvFile, str_buf);
	// getline関数で1行ずつ読み込む(読み込んだ内容はstr_bufに格納)
	while (getline(csvFile, str_buf))
	{
		ArrowStatus statusData;
		// 「,」区切りごとにデータを読み込むためにistringstream型にする
		std::istringstream i_stream(str_buf);
		//名前を読み込む。
		getline(i_stream, buf, ',');
		statusData.name = buf;

		//矢のスピードを読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status.arrowSpeed = stof(buf);
	
		// コリジョンのサイズを読み込む
		if (i_stream.peek() == '"') {
			// 先頭が"の場合、その中のテキストをすべて読み取る
			i_stream.get(); // 最初の"を取り除く
			getline(i_stream, buf, '"'); // "までを読み込む
			i_stream.get(); // 最後の"を取り除く
		}

		// コリジョンサイズをコンマで分割し、それぞれの値を設定する
		std::replace(buf.begin(), buf.end(), ',', ' ');

		std::istringstream sizeStream(buf);

		sizeStream >> statusData.m_status.collisionSize.x
			>> statusData.m_status.collisionSize.y
			>> statusData.m_status.collisionSize.z;

		//マップに取得したデータを代入
		StatusDataMAP[statusData.name] = statusData;

	}
	//ロードした
	IsLoadedArrowStatusCSVFile = true;
	return;
}
