#pragma once
#include "GameUIBase.h"

/// <summary>
/// ����UI�̃x�[�X�N���X
/// </summary>
class WeaponUIBase : public GameUIBase
{
public:
	


	virtual void UIUpdate() override {}

	virtual void MainWeaponUpdate() {}

	virtual void SubWeaponUpdate() {}

	virtual void SubWeapon2Update() {}

	virtual void Draw(RenderContext& rc) override {}

	virtual void DeleteSelf() override {}

protected:





protected:


	

};

