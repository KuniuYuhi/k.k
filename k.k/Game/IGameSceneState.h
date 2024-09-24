#pragma once

/// <summary>
/// ゲームシーンステートの基底クラス
/// </summary>
class IGameSceneState
{
public:

	//デストラクタ
	virtual ~IGameSceneState(){}

	/// <summary>
	/// ステート開始時の処理
	/// </summary>
	virtual void Entry() {}

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() {}

	/// <summary>
	/// ステート終了時の処理
	/// </summary>
	virtual void Exit() {}

protected:

};