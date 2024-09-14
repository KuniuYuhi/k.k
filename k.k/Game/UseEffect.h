#pragma once

#include "EffectNumbers.h"




using namespace EffectNumbers;

class CharacterBase;

//エフェクトのプール
//このクラスのプールを作る




//このクラス
// その場でいるだけ
//座標を中心に追いかける
//ボーンの位置を取得して追いかける
//強制的に削除する処理

/// <summary>
/// エフェクトを使うオブジェクトが読み込む
/// </summary>
class UseEffect:public IGameObject
{
public:



public:

	UseEffect();
	~UseEffect();


	bool Start() override;

	void Update() override;

	/// <summary>
	/// エフェクトをプレイ
	/// </summary>
	/// <param name="initEffectNumber">初期化するエフェクトの番号</param>
	/// <param name="position">初期座標</param>
	/// <param name="SetScale">拡大率</param>
	/// <param name="rotation">回転</param>
	/// <param name="isAutoMove">自動で移動するか</param>
	void PlayEffect(
		EnEFK initEffectNumber,
		Vector3 position,
		Vector3 SetScale,
		Quaternion rotation,
		bool isAutoMove
	);

	/// <summary>
	/// エフェクトをプレイ
	/// </summary>
	/// <param name="initEffectNumber">初期化するエフェクトの番号</param>
	/// <param name="characterBase">キャラクターベースインスタンス</param>
	/// <param name="position">初期座標</param>
	/// <param name="SetScale">拡大率</param>
	/// <param name="rotation">回転</param>
	/// <param name="isAutoMove">自動で移動するか</param>
	void PlayEffect(
		EnEFK initEffectNumber,
		CharacterBase* characterBase,
		Vector3 position,
		Vector3 SetScale,
		Quaternion rotation,
		bool isAutoMove
	);

	/// <summary>
	/// エフェクトをプレイ
	/// </summary>
	/// <param name="initEffectNumber">初期化するエフェクトの番号</param>
	/// <param name="characterBase">キャラクターベースインスタンス</param>
	/// <param name="position">初期座標</param>
	/// <param name="SetScale">拡大率</param>
	/// <param name="rotation">回転</param>
	/// <param name="isAutoMove">自動で移動するか</param>
	void PlayEffect(
		EnEFK initEffectNumber,
		CharacterBase* characterBase,
		int boonId,
		Vector3 position,
		Vector3 SetScale,
		Quaternion rotation,
		bool isAutoMove
	);

	/// <summary>
	/// 削除
	/// </summary>
	void Delete();

	/// <summary>
	/// 移動座標を設定
	/// </summary>
	/// <param name="position"></param>
	void SetMovePosition(Vector3 position);


private:

	/// <summary>
	/// モデルの中心を追いかける移動
	/// </summary>
	void ChaseMoveForCenterPosition();


	/// <summary>
	/// 指定された座標を使って追いかける移動
	/// </summary>
	void ChaseMoveForSpecifiedPosition();




private:

	EffectEmitter* m_effect = nullptr;		//エフェクト

	CharacterBase* m_character = nullptr;


	int m_boonId = -1;

	bool m_isAutoMove = false;		//自動で移動させるか？


};

