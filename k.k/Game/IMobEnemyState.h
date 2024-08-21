#pragma once

/// <summary>
/// モブエネミーのステートの基底クラス(インターフェース)
/// </summary>
class IMobEnemyState
{
public:

	virtual ~IMobEnemyState() {}

	/// <summary>
	/// アニメーションを再生
	/// </summary>
	virtual void PlayAnimation() = 0;
	/// <summary>
	/// 入った時の処理
	/// </summary>
	virtual void Entry() = 0;
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Ubdate() = 0;
	/// <summary>
	/// 終了時の処理
	/// </summary>
	virtual void Exit() = 0;

protected:


};

