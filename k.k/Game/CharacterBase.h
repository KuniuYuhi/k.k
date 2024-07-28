#pragma once
#include "ICharacter.h"

/// <summary>
/// キャラクターのベースクラス
/// キャラクターで共通する処理を実装する
/// </summary>
class CharacterBase : public ICharacter
{
public:
	virtual ~CharacterBase() = default;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void UpdateCharacter() = 0;

protected:


	void SettingDefaultComponent();

	


protected:
	//ステータス

	//デフォルトコンポーネント

};

