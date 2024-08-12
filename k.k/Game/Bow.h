#pragma once
#include "WeaponBase.h"

/// <summary>
/// 武器：ボウクラス
/// </summary>
class Bow : public WeaponBase
{
public:
	Bow();
	~Bow();

    bool Start() override;

    void Update() override;

    void Render(RenderContext& rc) override;


    void Init() override;

    /// <summary>
    /// 収納状態に切り替える
    /// </summary>
    void ChangeStowedState() override;

    /// <summary>
    /// 装備状態に切り替える
    /// </summary>
    void ChangeArmedState() override;



    void AttackAction() override;

    /// <summary>
    /// 装備状態での移動処理
    /// </summary>
    void MoveArmed();


private:
    ModelRender m_bowModelRender;         //ボウのモデルレンダー

    Matrix m_bowMatrix;

    //武器を持たせる時のボーンID
    int m_armedBowBoonId = -1;
    int m_armedArrowBoonId = -1;


};

