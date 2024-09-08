#pragma once
#include "WeaponBase.h"
#include "SwordShieldStatus.h"

class PlayerController;
class SwordShieldStatus;

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

    void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);


    void Init() override;

    /// <summary>
    /// ���g���폜����
    /// </summary>
    void DeleteThis() override;

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
    /// ����A�h��A�N�V�����𔲂��o�����̏���
    /// </summary>
    void ExitDefensiveActionProcess() override;

    /// <summary>
    /// ����A�h��q�b�g�ɓ������Ƃ��̍ŏ��̏���
    /// </summary>
    void EntryDefensiveHitProcess() override;
    /// <summary>
    /// ����A�h��q�b�g���̍X�V����
    /// </summary>
    void UpdateDefensiveHitProcess() override;
    /// <summary>
    /// ����A�h��q�b�g�𔲂��o�����̏���
    /// </summary>
    void ExitDefensiveHitProcess() override;


    /// <summary>
    /// ����A�h��A�N�V�������s���邩
    /// </summary>
    /// <returns>�s����Ȃ�true</returns>
    bool CanDefensiveAction() override;
    /// <summary>
    /// �X�L���U�����s���邩
    /// </summary>
    /// <returns>�s����Ȃ�true</returns>
    bool CanSkillAttack() override;

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

    /// <summary>
    /// �U���̏u�Ԃ̏���
    /// </summary>
    /// <param name="startOrEnd">start��true</param>
    void AttackImpactProcess(bool startOrEnd) override;


private:

    /// <summary>
    /// ������Ԃł̈ړ�����
    /// </summary>
    void MoveArmed();
    /// <summary>
    /// �����蔻�菉����
    /// </summary>
    void InitCollision();
    /// <summary>
    /// �X�L���U���p�R���W�����̍쐬
    /// </summary>
    void CreateSkillAttackCollision();


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
    /// ���̓����蔻����`�F�b�N
    /// </summary>
    void CheckShieldCollision();

    /// <summary>
    /// �V�[���h�Ƀq�b�g�������̏���
    /// </summary>
    void ProcessShieldHit();

private:

    SwordShieldStatus m_uniqueStatus;

    PlayerController* m_playerController = nullptr;

    ModelRender m_swordModelRender;         //�\�[�h�̃��f�������_�[
    ModelRender m_shieldModelRender;        //�V�[���h�̃��f�������_�[

    CollisionObject* m_swordCollision = nullptr;	//�Ў茕�̓����蔻��
    CollisionObject* m_shieldCollision = nullptr;	//���̓����蔻��

    Matrix m_swordMatrix;
    Matrix m_swordCenterMatrix;

    Matrix m_shieldMatrix;

    Vector3 m_normalAttackMoveDirection = g_vec3Zero;


    //������������鎞�̃{�[��ID
    int m_armedSwordBoonId = -1;
    int m_armedShieldBoonId = -1;

    int m_swordCenterBoonId = -1;           //���̒��S�̃{�[��ID


    bool m_isHitShield = false;

};

