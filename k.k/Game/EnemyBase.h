#pragma once
#include "CharacterBase.h"


/// <summary>
/// エネミー用のベースクラス
/// エネミーで共通する処理を実装する
/// </summary>
class EnemyBase : public CharacterBase
{
public:

	virtual ~EnemyBase() override = default;

	/// <summary>
	/// 更新処理。使ってない！！！！
	/// </summary>
	virtual void UpdateCharacter() override {}


protected:


	/// <summary>
	/// 基本的なコンポーネントをセッティング
	/// </summary>
	//virtual void SettingDefaultComponent() {}


protected:

	//基本的なステータス

};

