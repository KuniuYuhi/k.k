#pragma once
#include "WeaponBase.h"
#include "BowArrowStatus.h"

class BowArrowStatus;
class PlayerController;

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
    /// �R���{�U��������i�߂�
    /// </summary>
    void ProceedComboAttack() override;
    /// <summary>
    /// �R���{�U�����������Z�b�g����
    /// </summary>
    void ResetComboAttack() override;

    /// <summary>
    /// ����A�h��A�N�V�������I��邩
    /// </summary>
    /// <returns>�I���Ȃ�true</returns>
    bool IsEndDefensiveAction() override;

    /// <summary>
    /// ����A�h��A�N�V�����ɓ������Ƃ��̍ŏ��̏���
    /// </summary>
    void EntryDefensiveActionProcess() override;
    /// <summary>
    /// ����A�h��A�N�V�������̍X�V����
    /// </summary>
    void UpdateDefensiveActionProcess() override;

    /// <summary>
    /// �ʏ�U���X�e�[�g�ɓ��������̏���
    /// </summary>
    void EntryNormalAttackProcess(EnComboState comboState) override;
    /// <summary>
    /// �ʏ�U���X�e�[�g�ł̍X�V����
    /// </summary>
    void UpdateNormalAttackProcess(EnComboState comboState) override;
    /// <summary>
    /// �ʏ�U���X�e�[�g�𔲂��o�����̏���
    /// </summary>
    void ExitNormalAttackProcess(EnComboState comboState) override;

    /// <summary>
    /// �X�L���U���X�e�[�g�ɓ��������̏���
    /// </summary>
    void EntrySkillAttackProcess(EnSkillProcessState skillProcessState) override;
    /// <summary>
    /// �X�L���U���X�e�[�g�ł̍X�V����
    /// </summary>
    void UpdateSkillAttackProcess(EnSkillProcessState skillProcessState) override;
    /// <summary>
    /// �X�L���U���X�e�[�g�𔲂��o�����̏���
    /// </summary>
    void ExitSkillAttackProcess(EnSkillProcessState skillProcessState) override;


private:

    /// <summary>
    /// ������Ԃł̈ړ�����
    /// </summary>
    void MoveArmed();

    /// <summary>
    /// �X�L���X�^�[�g�X�e�[�g�ł̃G���g���[����
    /// </summary>
    void EntrySkillStartProcess();
    /// <summary>
    /// �X�L���X�^�[�g�X�e�[�g�ł̍X�V����
    /// </summary>
    void UpdateSkillStartProcess();
    /// <summary>
    /// �X�L���X�^�[�g�X�e�[�g�ł̏I���̏���
    /// </summary>
    void ExitSkillStartProcess();

    /// <summary>
    /// �X�L�����C���X�e�[�g�ł̃G���g���[����
    /// </summary>
    void EntrySkillMainProcess();
    /// <summary>
    /// �X�L�����C���X�e�[�g�ł̍X�V����
    /// </summary>
    void UpdateSkillMainProcess();
    /// <summary>
    /// �X�L�����C���X�e�[�g�ł̏I���̏���
    /// </summary>
    void ExitSkillMainProcess();

    /// <summary>
    /// �X�L���`���[�W���̏���
    /// </summary>
    void SkillChargeTimeProcess();

private:
    BowArrowStatus m_uniqueStatus;

    PlayerController* m_playerController = nullptr;

    ModelRender m_bowModelRender;         //�{�E�̃��f�������_�[

    Matrix m_bowMatrix;


    Vector3 m_defensiveActionDirection = g_vec3Zero;

    Vector3 m_normalAttackMoveDirection = g_vec3Zero;

    //������������鎞�̃{�[��ID
    int m_armedBowBoonId = -1;
    int m_armedArrowBoonId = -1;


    float m_skillChargeTimer = 0.0f;


};

