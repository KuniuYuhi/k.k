#pragma once
#include "IComponent.h"
#include "EffectItemStatus.h"

class EffectItemStatus;

/// <summary>
/// プレイヤーに何らかの「効果」を与えるアイテムの基底クラス
/// </summary>
class EffectItemBase : public IComponent
{
public:

	/// <summary>
	/// コンポーネントを追加した時の最初の処理
	/// </summary>
	virtual void Start() override {}

	/// <summary>
	/// 更新処理。自動で行ってくれる。消したいならGameObjectManagerへ
	/// </summary>
	virtual void UpdateComponent() override {}

	/// <summary>
	/// 初期化。
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 削除する時の処理
	/// </summary>
	virtual void DeleteProcess() {}

protected:
	EffectItemStatus m_status;
};

