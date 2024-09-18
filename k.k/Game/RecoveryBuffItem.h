#pragma once
#include "EffectItemBase.h"

class Brave;
class UseEffect;

/// <summary>
/// プレイヤーのHPを回復させる
/// </summary>
class RecoveryBuffItem : public EffectItemBase
{
public:

	RecoveryBuffItem();
	~RecoveryBuffItem();


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


private:

	Brave* m_brave = nullptr;

	UseEffect* m_effect = nullptr;

	bool isRecoveryPlayerHp = false;


};

