#pragma once

////このコリジョンを作ったアクターを検索
// クリエイターを取得
//Damageable = FindGO<Damageable>(knightcollision->GetCreatorName());
//Damageableを持っていたらダメージ処理


/// <summary>
/// 被ダメージするキャラクターに追加する。インターフェースの実装
/// </summary>
class Damageable
{
public:

	/// <summary>
	/// 被ダメージ処理
	/// </summary>
	virtual void Damage() = 0;

	/// <summary>
	/// 死亡時処理
	/// </summary>
	virtual void Dead() = 0;
};

