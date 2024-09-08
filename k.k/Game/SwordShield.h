#pragma once
#include "WeaponBase.h"
#include "SwordShieldStatus.h"

class PlayerController;
class SwordShieldStatus;

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

    void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);


    void Init() override;

    /// <summary>
    /// 自身を削除する
    /// </summary>
    void DeleteThis() override;

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
    /// 回避、防御アクションを抜け出す時の処理
    /// </summary>
    void ExitDefensiveActionProcess() override;

    /// <summary>
    /// 回避、防御ヒットに入ったときの最初の処理
    /// </summary>
    void EntryDefensiveHitProcess() override;
    /// <summary>
    /// 回避、防御ヒット中の更新処理
    /// </summary>
    void UpdateDefensiveHitProcess() override;
    /// <summary>
    /// 回避、防御ヒットを抜け出す時の処理
    /// </summary>
    void ExitDefensiveHitProcess() override;


    /// <summary>
    /// 回避、防御アクションが行えるか
    /// </summary>
    /// <returns>行えるならtrue</returns>
    bool CanDefensiveAction() override;
    /// <summary>
    /// スキル攻撃が行えるか
    /// </summary>
    /// <returns>行えるならtrue</returns>
    bool CanSkillAttack() override;

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

    /// <summary>
    /// 攻撃の瞬間の処理
    /// </summary>
    /// <param name="startOrEnd">startはtrue</param>
    void AttackImpactProcess(bool startOrEnd) override;


private:

    /// <summary>
    /// 装備状態での移動処理
    /// </summary>
    void MoveArmed();
    /// <summary>
    /// 当たり判定初期化
    /// </summary>
    void InitCollision();
    /// <summary>
    /// スキル攻撃用コリジョンの作成
    /// </summary>
    void CreateSkillAttackCollision();


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
    /// 盾の当たり判定をチェック
    /// </summary>
    void CheckShieldCollision();

    /// <summary>
    /// シールドにヒットした時の処理
    /// </summary>
    void ProcessShieldHit();

private:

    SwordShieldStatus m_uniqueStatus;

    PlayerController* m_playerController = nullptr;

    ModelRender m_swordModelRender;         //ソードのモデルレンダー
    ModelRender m_shieldModelRender;        //シールドのモデルレンダー

    CollisionObject* m_swordCollision = nullptr;	//片手剣の当たり判定
    CollisionObject* m_shieldCollision = nullptr;	//盾の当たり判定

    Matrix m_swordMatrix;
    Matrix m_swordCenterMatrix;

    Matrix m_shieldMatrix;

    Vector3 m_normalAttackMoveDirection = g_vec3Zero;


    //武器を持たせる時のボーンID
    int m_armedSwordBoonId = -1;
    int m_armedShieldBoonId = -1;

    int m_swordCenterBoonId = -1;           //剣の中心のボーンID


    bool m_isHitShield = false;

};

