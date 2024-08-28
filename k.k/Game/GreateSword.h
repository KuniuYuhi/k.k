#pragma once
#include "WeaponBase.h"
#include "GreateSwordStatus.h"

class GreateSwordStatus;
class PlayerController;
class PlayerMovement;

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

    void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);



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
    /// 回避、防御アクションを終わる時の処理
    /// </summary>
    void ExitDefensiveActionProcess() override;

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
    /// スキル攻撃用コリジョン作成
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
    /// 
    /// </summary>
    /// <returns></returns>
    bool IsSkillFlightTimeOver();


private:
    GreateSwordStatus m_uniqueStatus;

    PlayerController* m_playerController = nullptr;
    PlayerMovement* m_playerMovement = nullptr;

    ModelRender m_swordModelRender;         //ソードのモデルレンダー

    CollisionObject* m_swordCollision = nullptr;	//剣の当たり判定


    Matrix m_swordMatrix;
    Matrix m_swordCenterMatrix;


    Vector3 m_defensiveActionDirection = g_vec3Zero;

    Vector3 m_normalAttackMoveDirection = g_vec3Zero;


    //武器を持たせる時のボーンID
    int m_armedSwordBoonId = -1;

    int m_swordCenterBoonId = -1;           //剣の中心のボーンID



    float m_skillFlightTimer = 0.0f;        //スキルスタート中空に浮いている時間計測タイマー
    bool isMoveSkillAttack = false;         //スキル中移動できるか




};

