#include "stdafx.h"
#include "Status_Player.h"
#include <fstream>
#include <sstream>


std::unordered_map<std::string, Status_Player> Status_Player::StatusDataMAP = {};
//ステータスを記載したファイルパス
const std::string Status_Player::playerStatusCsvFilePath = "Assets/CSV/PlayerStatus.csv";

bool Status_Player::IsLoadedPlayerStatusCSVFile = false;



//プレイヤーのステータス
//[名前][体力][攻撃力][スタミナ][スタミナの回復速度倍率][基礎スピード][加速スピード][回避スピード]


void Status_Player::InitPlayerStatus(const std::string& name)
{
	//CSVファイルをロードしていないなら
	if (IsLoadedPlayerStatusCSVFile == false)
	{
		//CSVファイルを読み込む
		LoadPlayerStatusCSV(name);
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

void Status_Player::LoadPlayerStatusCSV(const std::string& name)
{
	//読み込んだcsvファイルのデータを格納するstring型
	std::string str_buf;
	//「,」までを読み込んで格納するstring型
	std::string buf;
	// 読み込むcsvファイルを開く(std::ifstreamのコンストラクタで開く)
	std::ifstream csvFile(playerStatusCsvFilePath);

	//最初の1行は飛ばす。
	getline(csvFile, str_buf);
	// getline関数で1行ずつ読み込む(読み込んだ内容はstr_bufに格納)
	while (getline(csvFile, str_buf))
	{
		Status_Player statusData;
		// 「,」区切りごとにデータを読み込むためにistringstream型にする
		std::istringstream i_stream(str_buf);
		//名前を読み込む。
		getline(i_stream, buf, ',');
		statusData.name = buf;
		//体力を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_playerStatus.maxHp = stoi(buf);
		//攻撃力を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_playerStatus.defaultPower = stoi(buf);
		//スタミナを読み込む。
		getline(i_stream, buf, ',');
		statusData.m_playerStatus.maxStamina = stof(buf);
		//スタミナの回復速度倍率を読み込む。
		getline(i_stream, buf, ',');
		statusData.m_playerStatus.staminaRecoveryRate = stof(buf);
		//スピードを読み込む。
		getline(i_stream, buf, ',');
		statusData.m_playerStatus.defaultSpeed = stof(buf);
		//加速スピードを読み込む。
		getline(i_stream, buf, ',');
		statusData.m_playerStatus.dashSpeed = stof(buf);
		//回避スピードを読み込む。
		getline(i_stream, buf, ',');
		statusData.m_playerStatus.evasionSpeed = stof(buf);


		//現在のHPを設定
		statusData.m_playerStatus.currentHp = statusData.m_playerStatus.maxHp;
		//現在の攻撃力を設定
		statusData.m_playerStatus.currentPower = statusData.m_playerStatus.defaultPower;
		//現在のスタミナを設定
		statusData.m_playerStatus.currentStamina = statusData.m_playerStatus.maxStamina;

		//マップに取得したデータを代入
		StatusDataMAP[statusData.name] = statusData;

	}
	//ロードした
	IsLoadedPlayerStatusCSVFile = true;
	return;
}


bool Status_Player::TryConsumeStamina(float consumeValue)
{
	//仮のスタミナを定義
	float tempStamina = m_playerStatus.currentStamina;

	//スタミナを消費する
	tempStamina -= consumeValue;

	//スタミナが0以下になったら
	if (tempStamina < 0)
	{
		//スタミナを消費できない。行動できない
		return false;
	}

	//消費された後のスタミナをステータスのスタミナに代入
	m_playerStatus.currentStamina = tempStamina;

	return true;
}


bool Status_Player::CheckConsumeStamina(float checkValue)
{
	//仮のスタミナを定義
	float tempStamina = m_playerStatus.currentStamina;

	//スタミナを消費する
	tempStamina -= checkValue;

	//スタミナが0以下になったら
	if (tempStamina < 0)
	{
		//スタミナを消費できない。行動できない
		return false;
	}
	//消費できる状態にある
	return true;
}

void Status_Player::RecoveryStamina(float recoveryValue)
{
	//既に最大値まで回復しているなら処理しない
	if (m_playerStatus.currentStamina == m_playerStatus.maxStamina)
	{
		return;
	}

	m_playerStatus.currentStamina += recoveryValue;
	//スタミナが最大値を超えないようにする
	if (m_playerStatus.currentStamina >= m_playerStatus.maxStamina)
	{
		m_playerStatus.currentStamina = m_playerStatus.maxStamina;
	}

}
