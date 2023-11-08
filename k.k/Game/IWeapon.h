#pragma once

class Brave;

/// <summary>
/// インターフェースの実装
/// </summary>
class IWeapon:public IGameObject
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	IWeapon(){}
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IWeapon(){}

	/// <summary>
	/// 武器の状態ステート
	/// </summary>
	enum EnWeaponState
	{
		enWeaponState_Stowed,		//収納している状態
		enWeaponState_Armed,		//装備している状態
		enWeaponState_None,			//何も装備していない
	};
	/// <summary>
	/// 武器の防御のタイプ。
	/// </summary>
	enum EnDefendTipe
	{
		enDefendTipe_Defence,	//盾などで防ぐタイプ
		enDefendTipe_avoid,		//回避タイプ
		enDefendTipe_None
	};

	////////////////////////////////////////////////////////////////////////
	///仮想関数、純粋仮想関数
	////////////////////////////////////////////////////////////////////////
	/// <summary>
	/// モデルの初期化
	/// </summary>
	virtual void InitModel() = 0;
	/// <summary>
	/// 武器の移動処理
	/// </summary>
	virtual void MoveWeapon() = 0;
	/// <summary>
	/// 武器を装備している時の移動処理
	/// </summary>
	virtual void MoveArmed() = 0;
	/// <summary>
	/// 武器を収納している時の移動処理
	/// </summary>
	virtual void MoveStowed() = 0;


};

