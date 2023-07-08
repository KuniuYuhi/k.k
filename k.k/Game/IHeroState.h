#pragma once

class Hero;

/// <summary>
/// ヒーローのステートの基底クラス
/// </summary>
class IHeroState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	IHeroState(Hero* hero)
	{
		m_hero = hero;
	}
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IHeroState() {}
	/// <summary>
	/// 状態を管理する
	/// </summary>
	virtual void ManageState() = 0;
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	virtual void PlayAnimation() = 0;
protected:
	Hero* m_hero = nullptr; // プレイヤー
};

