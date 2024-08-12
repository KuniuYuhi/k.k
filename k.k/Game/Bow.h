#pragma once
#include "WeaponBase.h"

/// <summary>
/// ����F�{�E�N���X
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


private:
    ModelRender m_bowModelRender;         //�{�E�̃��f�������_�[

    Matrix m_bowMatrix;

    //������������鎞�̃{�[��ID
    int m_armedBowBoonId = -1;
    int m_armedArrowBoonId = -1;


};

