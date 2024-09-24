#pragma once
#include "KnockBackInfo.h"
#include "SummonerInfo.h"

using namespace KnockBackInfo;
using namespace SummonerSkillType;

class SummonerSkillStatus
{
private:

	/// <summary>
	/// 技に関するステータス
	/// </summary>
	struct SkillStatus
	{
		int power = 0;									//攻撃力
		float knockBackTimeScale = 0.0f;				//ノックバックの進行速度
		EnKnockBackPattern m_knockBackPattern;			//ノックバックパターン

	};

	SkillStatus m_skillStatus[enSkillType_Num];

public:


	/// <summary>
	/// 技に関するステータスを取得
	/// </summary>
	/// <param name="getKnockBackPattern"></param>
	/// <returns></returns>
	SkillStatus& GetGetSkillTypeStatus(EnSkillSType skillType)
	{
		return m_skillStatus[skillType];
	}
	
	/// <summary>
	/// ノックバックのステータスを初期化
	/// </summary>
	void InitSkillStatus();

	/// <summary>
	/// 技の攻撃力を取得
	/// </summary>
	/// <param name="skillType"></param>
	/// <returns></returns>
	int& GetSkillPower(EnSkillSType skillType)
	{
		return m_skillStatus[skillType].power;
	}
	/// <summary>
	/// ノックバックの進行速度を取得
	/// </summary>
	/// <param name="skillType"></param>
	/// <returns></returns>
	float& GetSkillKnockBackTimeScale(EnSkillSType skillType)
	{
		return m_skillStatus[skillType].knockBackTimeScale;
	}
	/// <summary>
	/// ノックバックのパターンを取得
	/// </summary>
	/// <param name="skillType"></param>
	/// <returns></returns>
	EnKnockBackPattern& GetSkillKnockBackPattern(EnSkillSType skillType)
	{
		return m_skillStatus[skillType].m_knockBackPattern;
	}


	//CSVの情報を保存したインスタンス
	static SummonerSkillStatus StatusDataMAP;
	//プレイヤーのステータスの初期化で使う静的な型
	static const std::string skillTypeStatusCsvFilePath;
	static bool IsLoadedSkillTypeStatusCSVFile;


private:
	/// <summary>
	/// 技ステータスcsvファイルの読み込み
	/// </summary>
	void LoadSkillStatusCSV();


	EnKnockBackPattern StringToKnockBackPattern(std::string& knockBackPattern);

};


//思考用パラメータ
//class 

