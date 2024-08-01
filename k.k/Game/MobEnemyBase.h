#pragma once
#include "EnemyBase.h"



/// <summary>
/// モブエネミー用のベースクラス
/// モブエネミーで共通する処理を実装する
/// </summary>
class MobEnemyBase : public EnemyBase
{
public:

	virtual ~MobEnemyBase() override = default;


	/// <summary>
	/// 更新処理。使ってない！！！！
	/// </summary>
	virtual void UpdateCharacter() override {}

protected:

	/// <summary>
	/// 基本的なコンポーネントをセッティング
	/// </summary>
	virtual void SettingDefaultComponent() override {}


protected:



};

