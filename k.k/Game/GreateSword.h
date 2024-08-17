#pragma once
#include "WeaponBase.h"
#include "GreateSwordStatus.h"

class GreateSwordStatus;
class PlayerController;
class PlayerMovement;

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
    /// �����蔻�菉����
    /// </summary>
    void InitCollision();


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



    bool IsSkillFlightTimeOver();


private:
    GreateSwordStatus m_uniqueStatus;

    PlayerController* m_playerController = nullptr;
    PlayerMovement* m_playerMovement = nullptr;

    ModelRender m_swordModelRender;         //�\�[�h�̃��f�������_�[

    CollisionObject* m_swordCollision = nullptr;	//���̓����蔻��


    Matrix m_swordMatrix;


    Vector3 m_defensiveActionDirection = g_vec3Zero;

    Vector3 m_normalAttackMoveDirection = g_vec3Zero;


    //������������鎞�̃{�[��ID
    int m_armedSwordBoonId = -1;

    int m_swordCenterBoonId = -1;           //���̒��S�̃{�[��ID



    float m_skillFlightTimer = 0.0f;




};

