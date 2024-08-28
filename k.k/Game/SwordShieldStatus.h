#pragma once

/// <summary>
/// 武器：ソード＆シールドの固有のステータス
/// </summary>
class SwordShieldStatus
{
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

		float normalAttackSpeed[3] = { 0.0f };	//通常攻撃の移動速度

		float normalAttackSearchRadius = 0.0f;	//通常攻撃の索敵範囲(半径)

		float normalAttackComparisonDot = 0.0f;	//通常攻撃で計算するベクトル同士の内積の比較対象の内積

		int maxShieldEnduranceValue = 0;			//シールドの最大耐久値
		int currentShieldEnduranceValue = 0;		//現在のシールドの耐久値
	};

	UniqueStatus m_uniqueStatus;


	/// <summary>
	/// ソード＆シールドの固有ステータスcsvファイルの読み込み
	/// </summary>
	void LoadUniqueStatusCSV();

public:

	const UniqueStatus& GetUniqueStatus()
	{
		return m_uniqueStatus;
	}

	/// <summary>
	/// ソード＆シールドの固有ステータスを初期化
	/// </summary>
	void InitUniqueStatus();



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


	const int& GetShieldEnduranceValue()
	{
		return m_uniqueStatus.maxShieldEnduranceValue;
	}

	const int& GetCurrentShieldEnduranceValue()
	{
		return m_uniqueStatus.currentShieldEnduranceValue;
	}

	void SetCurrentShieldEnduranceValue(int value)
	{
		m_uniqueStatus.currentShieldEnduranceValue = value;
	}




	/// <summary>
	/// シールドの耐久値の減算
	/// </summary>
	/// <param name="subValue">減算したい値</param>
	void SubShieldEnduranceValue(int subValue)
	{
		m_uniqueStatus.currentShieldEnduranceValue -= subValue;

		if (m_uniqueStatus.currentShieldEnduranceValue <= 0)
		{
			m_uniqueStatus.currentShieldEnduranceValue = 0;
		}

	}


	//CSVの情報を保存したマップ
	static SwordShieldStatus StatusDataMAP;
	//ソード＆シールドの固有ステータスの初期化で使う静的な型
	static const std::string uniqueStatusCsvFilePath;
	static bool IsLoadedUniqueStatusCSVFile;


};

