#pragma once

/// <summary>
/// 武器：ボウ＆アローの固有のステータス
/// </summary>
class BowArrowStatus
{
public:

	enum EnSkillChargeStage
	{
		enStage_1,
		enStage_2,
		enStage_max,
	};

private:

	/// <summary>
	/// 固有ステータス
	/// </summary>
	struct UniqueStatus
	{
		float firstAttackTimeScale = 1.0f;		//一コンボ目ノックバックの進行速度
		float secondAttackTimeScale = 1.0f;		//二コンボ目ノックバックの進行速度
		float thirdAttackTimeScale = 1.0f;		//三コンボ目ノックバックの進行速度
		float skillAttackTimeScale = 1.0f;		//スキル攻撃ノックバックの進行速度

		float defenciveMoveSpeed = 0.0f;		//回避スピード

		float normalAttackSpeed[3] = { 0.0f };	//通常攻撃の移動速度

		float normalAttackSearchRadius = 0.0f;	//通常攻撃の索敵範囲(半径)

		float normalAttackComparisonDot = 0.0f;	//通常攻撃で計算するベクトル同士の内積の比較対象の内積


		float skillChargeCompletionTime[enStage_max] = { 0.0f };//チャージ完了時間。配列は段階

		float attackInfoUpdateInterval = 0.0f;			//攻撃情報更新インターバル

		int arrowStack = 0;						//矢のストック
	};

	UniqueStatus m_uniqueStatus;

	EnSkillChargeStage m_enSkillChargeStage = enStage_1;

	/// <summary>
	/// ボウ＆アローの固有ステータスcsvファイルの読み込み
	/// </summary>
	void LoadUniqueStatusCSV();

public:

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const UniqueStatus& GetUniqueStatus()
	{
		return m_uniqueStatus;
	}

	/// <summary>
	/// ボウ＆アローの固有ステータスを初期化
	/// </summary>
	void InitUniqueStatus();

	
	void SetSkillChargeStage(EnSkillChargeStage setStage)
	{
		m_enSkillChargeStage = setStage;
	}

	EnSkillChargeStage GetCurrentSkillChargeStage()
	{
		return m_enSkillChargeStage;
	}


	const float& GetAttackTimeScale(int num)
	{
		switch (num)
		{
		case 0:
			return FirstAttackTimeScale();
		case 1:
			return SecondAttackTimeScale();
		case 2:
			return ThirdAttackTimeScale();
		default:
			break;
		}
	}

	const float& FirstAttackTimeScale()
	{
		return m_uniqueStatus.firstAttackTimeScale;
	}

	const float& SecondAttackTimeScale()
	{
		return m_uniqueStatus.secondAttackTimeScale;
	}

	const float& ThirdAttackTimeScale()
	{
		return m_uniqueStatus.thirdAttackTimeScale;
	}

	const float& SkillAttackTimeScale()
	{
		return m_uniqueStatus.skillAttackTimeScale;
	}

	const float& GetDefenciveMoveSpeed()
	{
		return m_uniqueStatus.defenciveMoveSpeed;
	}

	float& GetNormalAttackSpeed(int num)
	{
		return m_uniqueStatus.normalAttackSpeed[num];
	}

	float& GetNormalAttackSearchRadius()
	{
		return m_uniqueStatus.normalAttackSearchRadius;
	}

	const float& GetNormalAttackComparisonDot()
	{
		return m_uniqueStatus.normalAttackComparisonDot;
	}

	const float& GetSkillChargeCompletionTime(EnSkillChargeStage stage)
	{
		return m_uniqueStatus.skillChargeCompletionTime[stage];
	}

	const float& GetAttackInfoUpdateInterval()
	{
		return m_uniqueStatus.attackInfoUpdateInterval;
	}

	const int& GetArrowStack()
	{
		return m_uniqueStatus.arrowStack;
	}



	//CSVの情報を保存したマップ
	static BowArrowStatus StatusDataMAP;
	//ボウ＆アローの固有ステータスの初期化で使う静的な型
	static const std::string uniqueStatusCsvFilePath;
	static bool IsLoadedUniqueStatusCSVFile;


};

