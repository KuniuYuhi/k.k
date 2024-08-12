#pragma once
#include "WeaponBase.h"


/// <summary>
/// ����F�O���C�g�\�[�h�N���X
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

    CollisionObject* m_swordCollision = nullptr;	//���̓����蔻��


    Matrix m_swordMatrix;

    //������������鎞�̃{�[��ID
    int m_armedSwordBoonId = -1;

    int m_swordCenterBoonId = -1;           //���̒��S�̃{�[��ID

};

