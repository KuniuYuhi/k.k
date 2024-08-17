#pragma once
#include "WeaponBase.h"
#include "BowArrowStatus.h"

class BowArrowStatus;
class PlayerController;

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
    /// コンボ攻撃処理を進める
    /// </summary>
    void ProceedComboAttack() override;
    /// <summary>
    /// コンボ攻撃処理をリセットする
    /// </summary>
    void ResetComboAttack() override;

    /// <summary>
    /// 回避、防御アクションを終わるか
    /// </summary>
    /// <returns>終わるならtrue</returns>
    bool IsEndDefensiveAction() override;

    /// <summary>
    /// 回避、防御アクションに入ったときの最初の処理
    /// </summary>
    void EntryDefensiveActionProcess() override;
    /// <summary>
    /// 回避、防御アクション中の更新処理
    /// </summary>
    void UpdateDefensiveActionProcess() override;

    /// <summary>
    /// 通常攻撃ステートに入った時の処理
    /// </summary>
    void EntryNormalAttackProcess(EnComboState comboState) override;
    /// <summary>
    /// 通常攻撃ステートでの更新処理
    /// </summary>
    void UpdateNormalAttackProcess(EnComboState comboState) override;
    /// <summary>
    /// 通常攻撃ステートを抜け出す時の処理
    /// </summary>
    void ExitNormalAttackProcess(EnComboState comboState) override;

    /// <summary>
    /// スキル攻撃ステートに入った時の処理
    /// </summary>
    void EntrySkillAttackProcess(EnSkillProcessState skillProcessState) override;
    /// <summary>
    /// スキル攻撃ステートでの更新処理
    /// </summary>
    void UpdateSkillAttackProcess(EnSkillProcessState skillProcessState) override;
    /// <summary>
    /// スキル攻撃ステートを抜け出す時の処理
    /// </summary>
    void ExitSkillAttackProcess(EnSkillProcessState skillProcessState) override;


private:

    /// <summary>
    /// 装備状態での移動処理
    /// </summary>
    void MoveArmed();

    /// <summary>
    /// スキルスタートステートでのエントリー処理
    /// </summary>
    void EntrySkillStartProcess();
    /// <summary>
    /// スキルスタートステートでの更新処理
    /// </summary>
    void UpdateSkillStartProcess();
    /// <summary>
    /// スキルスタートステートでの終わりの処理
    /// </summary>
    void ExitSkillStartProcess();

    /// <summary>
    /// スキルメインステートでのエントリー処理
    /// </summary>
    void EntrySkillMainProcess();
    /// <summary>
    /// スキルメインステートでの更新処理
    /// </summary>
    void UpdateSkillMainProcess();
    /// <summary>
    /// スキルメインステートでの終わりの処理
    /// </summary>
    void ExitSkillMainProcess();

    /// <summary>
    /// スキルチャージ中の処理
    /// </summary>
    void SkillChargeTimeProcess();

private:
    BowArrowStatus m_uniqueStatus;

    PlayerController* m_playerController = nullptr;

    ModelRender m_bowModelRender;         //ボウのモデルレンダー

    Matrix m_bowMatrix;


    Vector3 m_defensiveActionDirection = g_vec3Zero;

    Vector3 m_normalAttackMoveDirection = g_vec3Zero;

    //武器を持たせる時のボーンID
    int m_armedBowBoonId = -1;
    int m_armedArrowBoonId = -1;


    float m_skillChargeTimer = 0.0f;


};

