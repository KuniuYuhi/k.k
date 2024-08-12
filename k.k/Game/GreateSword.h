#pragma once
#include "WeaponBase.h"


/// <summary>
/// 武器：グレイトソードクラス
/// </summary>
class GreateSword : public WeaponBase
{
public:
	GreateSword();
	~GreateSword();



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

    CollisionObject* m_swordCollision = nullptr;	//剣の当たり判定


    Matrix m_swordMatrix;

    //武器を持たせる時のボーンID
    int m_armedSwordBoonId = -1;

    int m_swordCenterBoonId = -1;           //剣の中心のボーンID

};

