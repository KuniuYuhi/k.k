#pragma once

class Wizard;

/// <summary>
/// ウィザードのステートの基底クラス
/// </summary>
class IWizardState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	IWizardState(Wizard* wizard)
	{
		m_wizard = wizard;
	}
	virtual ~IWizardState() {}
	/// <summary>
	/// 状態を管理する
	/// </summary>
	virtual void ManageState() = 0;
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	virtual void PlayAnimation() = 0;

protected:
	Wizard* m_wizard = nullptr;
};

