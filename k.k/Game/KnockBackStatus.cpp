#include "stdafx.h"
#include "KnockBackStatus.h"
#include <fstream>
#include <sstream>


KnockBackStatus KnockBackStatus::StatusDataMAP;

//ステータスを記載したファイルパス
const std::string KnockBackStatus::knockBackStatusCsvFilePath = "Assets/CSV/KnockBackStatus.csv";
bool KnockBackStatus::IsLoadedKnockBackStatusCSVFile = false;


void KnockBackStatus::InitKnockBackStatus()
{
	//CSVファイルをロードしていないなら
	if (IsLoadedKnockBackStatusCSVFile == false)
	{
		//CSVファイルを読み込む
		LoadKnockBackStatusCSV();
	}

	//引数と同じ名前のデータを代入
	*this = StatusDataMAP;

	return;
}

void KnockBackStatus::LoadKnockBackStatusCSV()
{
	//読み込んだcsvファイルのデータを格納するstring型
	std::string str_buf;
	//「,」までを読み込んで格納するstring型
	std::string buf;
	// 読み込むcsvファイルを開く(std::ifstreamのコンストラクタで開く)
	std::ifstream csvFile(knockBackStatusCsvFilePath);

	//最初の1行は飛ばす。
	getline(csvFile, str_buf);


	//インスタンス
	KnockBackStatus statusData;
	
	//配列の番号
	int num = 0;

	// getline関数で1行ずつ読み込む(読み込んだ内容はstr_bufに格納)
	while (getline(csvFile, str_buf))
	{
		// 「,」区切りごとにデータを読み込むためにistringstream型にする
		std::istringstream i_stream(str_buf);

		//パターンの名前は飛ばす
		getline(i_stream, buf, ',');

		//ノックバックレベルを読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status[num].knockBackLevel = stoi(buf);

		//初速度を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status[num].initialVelocity = stof(buf);

		//角度を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status[num].angle = stof(buf);


		//持続時間を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status[num].duration = stof(buf);

		//重力を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_status[num].gravity = stof(buf);

		// コリジョンのサイズを読み込む
		if (i_stream.peek() == '"') {
			// 先頭が"の場合、その中のテキストをすべて読み取る
			i_stream.get(); // 最初の"を取り除く
			getline(i_stream, buf, '"'); // "までを読み込む
			i_stream.get(); // 最後の"を取り除く
		}

		// ノックバックのスピードをコンマで分割し、それぞれの値を設定する
		std::replace(buf.begin(), buf.end(), ',', ' ');

		std::istringstream sizeStream(buf);

		sizeStream >> statusData.m_status[num].knockBackTimeScale.x
			>> statusData.m_status[num].knockBackTimeScale.y;

		//次の配列にする
		num++;

	}


	//マップに取得したデータを代入
	StatusDataMAP = statusData;

	//ロードした
	IsLoadedKnockBackStatusCSVFile = true;
	return;
}


