#pragma once

#include "WeaponInfo.h"
#include "KnockBackInfo.h"

using namespace WeaponAttribute;
using namespace KnockBackInfo;

//攻撃の仕組み
//
//ダメージを与えたら武器ステータスのコンボごとのノックバックパターンを使って
// カーブデータクラスからvector型のデータを取得

/// <summary>
/// 武器のステータス
/// </summary>
class WeaponStatus
{
private:

	enum EnCombo
	{
		FirstCombo = 1,
		SecondCombo,
		ThirdCombo
	};

	/// <summary>
	/// ステータス
	/// </summary>
	struct CommonStatus
	{
		int defaultPower = 0;				//基礎攻撃力
		int currentPower = 0;				//現在の攻撃力
		int skillPower = 0;					//スキル攻撃力
		float skillStaminaCost = 0;			//スキルのスタミナ消費量
		float defensiveStaminaCost = 0;		//回避、防御のスタミナ消費量

		EnWeaponAttribute weaponAttribute;	//武器の属性

		//通常攻撃のノックバックパターン
		EnKnockBackPattern firstComboPattern;		//1コンボ
		EnKnockBackPattern secondComboPattern;	//2コンボ
		EnKnockBackPattern thirdComboPattern;		//3コンボ

		//スキルのノックバックパターン
		EnKnockBackPattern m_SkillPattern;			//スキル

	};

	CommonStatus m_status;

public:


	const CommonStatus GetCommonStatus()
	{
		return m_status;
	}

	/// <summary>
	/// 武器のステータスを初期化
	/// </summary>
	/// <param name="name">ステータスを初期化したい武器の名前</param>
	void InitWeaponCommonStatus(const std::string& name);





	/// <summary>
	/// 現在の攻撃力の設定
	/// </summary>
	/// <param name="setAtk"></param>
	void SetCurrentPower(const int setCurrentPower)
	{
		m_status.currentPower = setCurrentPower;
	}
	/// <summary>
	/// 現在の攻撃力の取得
	/// </summary>
	/// <returns></returns>
	const int& GetCurrentPower() const
	{
		return m_status.currentPower;
	}

	/// <summary>
	/// スキル攻撃力の設定
	/// </summary>
	/// <param name="setAtk"></param>
	void SetSkillPower(const int setSkillPower)
	{
		m_status.skillPower = setSkillPower;
	}
	/// <summary>
	/// スキル攻撃力の取得
	/// </summary>
	/// <returns></returns>
	const int& GetSkillPower() const
	{
		return m_status.skillPower;
	}

	/// <summary>
	/// 武器の属性を取得
	/// </summary>
	/// <returns></returns>
	const EnWeaponAttribute& GetWeaponAttribute()
	{
		return m_status.weaponAttribute;
	}

	/// <summary>
	/// スキルのスタミナ消費量を取得
	/// </summary>
	/// <returns></returns>
	const float& GetSkillStaminaCost()
	{
		return m_status.skillStaminaCost;
	}

	/// <summary>
	/// 回避、防御のスタミナ消費量を取得
	/// </summary>
	/// <returns></returns>
	const float& GetDefensiveStaminaCost()
	{
		return m_status.defensiveStaminaCost;
	}

	const EnKnockBackPattern& GetComboKnockBackPattern(EnCombo comboCount)
	{
		switch (comboCount)
		{
		case WeaponStatus::FirstCombo:
			return m_status.firstComboPattern;
		case WeaponStatus::SecondCombo:
			return m_status.secondComboPattern;;
		case WeaponStatus::ThirdCombo:
			return m_status.thirdComboPattern;;
		default:
			std::abort();
			break;
		}
	}

	//csvは使わない
	std::string name;          //名前

	//CSVの情報を保存したマップ
	static std::unordered_map<std::string, WeaponStatus> StatusDataMAP;
	//プレイヤーのステータスの初期化で使う静的な型
	static const std::string weaponCommonStatusCsvFilePath;
	static bool IsLoadedWeaponCommonStatusCSVFile;



private:

	/// <summary>
	/// 武器ステータスcsvファイルの読み込み
	/// </summary>
	/// <param name="name">読み込みたい武器の名前</param>
	void LoadWeaponCommonStatusCSV(const std::string& name);



	EnWeaponAttribute StringToWeaponAttribute(std::string& attributeName);

	EnKnockBackPattern StringToKnockBackPattern(std::string& knockBackPattern);



};

