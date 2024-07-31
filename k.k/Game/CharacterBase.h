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
	/// 更新処理。使ってない！！！！
	/// </summary>
	virtual void UpdateCharacter() = 0;



	//テンプレートでステータスを取得する


protected:

	/// <summary>
	/// 基本的なコンポーネントをセッティング
	/// </summary>
	virtual void SettingDefaultComponent(){}

	


protected:
	//ステータス

	//デフォルトコンポーネント

};

