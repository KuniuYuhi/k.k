#pragma once
#include "WeaponBase.h"


/// <summary>
/// ����F�\�[�h���V�[���h�N���X
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
    /// ���[��Ԃɐ؂�ւ���
    /// </summary>
    void ChangeStowedState() override;

    /// <summary>
    /// ������Ԃɐ؂�ւ���
    /// </summary>
    void ChangeArmedState() override;

    void AttackAction() override;

    /// <summary>
    /// ������Ԃł̈ړ�����
    /// </summary>
    void MoveArmed();


    void InitCollision();




private:

    ModelRender m_swordModelRender;         //�\�[�h�̃��f�������_�[
    ModelRender m_shieldModelRender;        //�V�[���h�̃��f�������_�[

    CollisionObject* m_swordCollision = nullptr;	//�Ў茕�̓����蔻��
    CollisionObject* m_shieldCollision = nullptr;	//���̓����蔻��

    Matrix m_swordMatrix;
    Matrix m_shieldMatrix;


    //������������鎞�̃{�[��ID
    int m_armedSwordBoonId = -1;
    int m_armedShieldBoonId = -1;

    int m_swordCenterBoonId = -1;           //���̒��S�̃{�[��ID

};

