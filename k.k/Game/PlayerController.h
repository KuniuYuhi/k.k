#pragma once
#include "IComponent.h"

class PlayerController:public IComponent
{
public:

	PlayerController();

	~PlayerController();

	/// <summary>
	/// コンポーネントを追加した時の最初の処理
	/// </summary>
	void Start() override;

	/// <summary>
	/// 更新処理。自動で行ってくれる。消したいならGameObjectManagerへ
	/// </summary>
	void UpdateComponent() override;





	/// <summary>
	/// コントローラーの入力を受け付けるかのフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetControllerInputEnabledFlag(bool flag)
	{
		m_isControllerInputEnabled = flag;
	}

	/// <summary>
	/// コントローラーの入力を受け付けるかのフラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& IsControllerInputEnabledFlag() const
	{
		return m_isControllerInputEnabled;
	}



	/// <summary>
	/// 通常攻撃ボタンを押したか
	/// </summary>
	/// <returns></returns>
	bool IsTriggerNromalAttackButton();

	/// <summary>
	/// 回避、防御ボタンを押したか
	/// </summary>
	/// <returns></returns>
	bool IsTriggerDefensiveActionButton();

	/// <summary>
	/// 回避、防御ボタンを押し続けているか
	/// </summary>
	/// <returns></returns>
	bool IsPressDefensiveActionButton();

	/// <summary>
	/// 特定のボタンのトリガー判定
	/// </summary>
	/// <param name="button">判定したいボタン</param>
	/// <returns></returns>
	bool IsButtonTrigger(EnButton button);

	/// <summary>
	/// 特定のボタンのプレス判定
	/// </summary>
	/// <param name="button">判定したいボタン</param>
	/// <returns></returns>
	bool IsButtonPress(EnButton button);

	/// <summary>
	/// 左スティックの入力量を取得
	/// </summary>
	/// <returns>入力量を-1～1の範囲で取得。入力していないときは0を取得</returns>
	Vector3 GetLStickInput();

	/// <summary>
	/// 右スティックの入力量を取得
	/// </summary>
	/// <returns>入力量を-1～1の範囲で取得。入力していないときは0を取得</returns>
	Vector3 GetRStickInput();




private:

	bool m_isControllerInputEnabled = true;		//コントローラーの入力を受け付けるか

};

