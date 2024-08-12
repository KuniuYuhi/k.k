#pragma once
#include "WeaponBase.h"


/// <summary>
/// 武器：ソード＆シールドクラス
/// </summary>
class SwordShield : public WeaponBase
{
public:
    SwordShield();

    ~SwordShield();


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


    void InitCollision();




private:

    ModelRender m_swordModelRender;         //ソードのモデルレンダー
    ModelRender m_shieldModelRender;        //シールドのモデルレンダー

    CollisionObject* m_swordCollision = nullptr;	//片手剣の当たり判定
    CollisionObject* m_shieldCollision = nullptr;	//盾の当たり判定

    Matrix m_swordMatrix;
    Matrix m_shieldMatrix;


    //武器を持たせる時のボーンID
    int m_armedSwordBoonId = -1;
    int m_armedShieldBoonId = -1;

    int m_swordCenterBoonId = -1;           //剣の中心のボーンID

};

