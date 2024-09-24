#pragma once
#include "EffectItemBase.h"

class Brave;
class UseEffect;

/// <summary>
/// プレイヤーの攻撃力を増加させるコンポーネント
/// </summary>
class AttackBuffItem : public EffectItemBase
{
public:

	AttackBuffItem();
	~AttackBuffItem();


	/// <summary>
	/// コンポーネントを追加した時の最初の処理
	/// </summary>
	void Start() override;

	/// <summary>
	/// 更新処理。自動で行ってくれる。消したいならGameObjectManagerへ
	/// </summary>
	void UpdateComponent() override;

	/// <summary>
	/// 初期化。
	/// </summary>
	void Init() override;

	/// <summary>
	/// 削除する時の処理
	/// </summary>
	void DeleteProcess() override;

private:

	bool IsDelete();

private:

	Brave* m_brave = nullptr;

	UseEffect* m_effect = nullptr;

	float m_deleteTimer = 0.0f;

};

