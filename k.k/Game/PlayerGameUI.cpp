#include "stdafx.h"
#include "PlayerGameUI.h"

#include "Brave.h"


#include "WeaponManager.h"

#include "GreateSwordUI.h"
#include "SwordShieldUI.h"
#include "BowArrowUI.h"



namespace {

    //////////////////////////////////////////////////////////////////
    //ゲージ
    ///////////////////////////////////////////////////////////////////
    const Vector2 GAUGE_PIBOT = { 0.0f,0.5f };							//HPかMPのピボット
    ///////////////////////////////////////////////////////////////////
    //ステータスに関する値
    ///////////////////////////////////////////////////////////////////
    const Vector2 HP_CENTER_POS = { -650.0f, -470.0f };     //HPに関するスプライトの中心
    const Vector2 HP_REFT_POS = { -940.0f, -470.0f };       //HPの左側

    const float WHITE_HP_MUL_SPEED = 25.0f;

    const Vector2 STAMINA_CENTER_POS = { 0.0f,-380.0f };
    const Vector2 STAMINA_REFT_POS = { -171.0f,-380.0f };

    const Vector2 STAMINA_INSUFFCIENT_POS = { -350.0f,-290.0f };

    ///////////////////////////////////////////////////////////////////
    //バフ
    ///////////////////////////////////////////////////////////////////

    const Vector2 ATTACK_BUFF_AMOUNT_POS = { -870.0f,-370.0f };
    const Vector2 ATTACK_BUFF_ICON_POS = { -860.0f,-360.0f };
}

PlayerGameUI::~PlayerGameUI()
{
    for (auto weaponUI : m_weaponUIMap)
    {
        //削除
        weaponUI.second->DeleteSelf();
    }

    m_weaponUIMap.clear();

}

bool PlayerGameUI::Start()
{
    m_player = FindGO<Brave>("Brave");


    m_whiteHP = m_player->GetStatus().GetCurrentHp();

    //初期化
    Init();

    WeaponUIInit();

   



    return true;
}

void PlayerGameUI::UIUpdate()
{
    //プレイヤーのステータスのUIを更新
    UpdatePlayerStatusUI();

    //武器UIの更新
    UpdateWeaponUI();

    //バフの更新処理
    UpdateHaveBuffs();

}

void PlayerGameUI::Draw(RenderContext& rc)
{
    m_spriteList[enSpriteName_HPBar_Back].Draw(rc);

    m_spriteList[enSpriteName_HPBar_White].Draw(rc);

    m_spriteList[enSpriteName_HPBar_Front].Draw(rc);

    m_spriteList[enSpriteName_HPBar_Frame].Draw(rc);


    if (m_isVeiwStaminaGauge)
    {
        m_spriteList[enSpriteName_StaminaBar_Back].Draw(rc);
        m_spriteList[enSpriteName_StaminaBar_Front].Draw(rc);
        m_spriteList[enSpriteName_StaminaBar_Frame].Draw(rc);
    }



    if (m_isStaminaInsufficient)
    {
        m_fontList[enFontName_StaminaInsufficient].Draw(rc);
    }
   

    m_spriteList[enSpriteName_MainWeaponFlame].Draw(rc);
    m_spriteList[enSpriteName_SubWeaponFlame].Draw(rc);
    m_spriteList[enSpriteName_SubWeapon2Flame].Draw(rc);


    for (auto weaponUI : m_weaponUIMap)
    {
        //描画
        weaponUI.second->Draw(rc);
    }

    if (!m_isViewBuffSpritsAndFonts) return;

    m_spriteList[enSpriteName_AttackBuffIcon].Draw(rc);
    m_fontList[enFontName_AttackIconAmount].Draw(rc);

}

void PlayerGameUI::DeleteSelf()
{
    DeleteGO(this);
}

void PlayerGameUI::UpdatePlayerStatusUI()
{
    //HPの更新処理
    UpdatePlayerHP();
    //スタミナの更新処理
    UpdatePlayerStamina();

    //スタミナが不足しているなら
    if (m_isStaminaInsufficient)
    {
        //タイマーが止まったら
        if (!IsTimerRunning(m_staminaInsufficientViewTimeLimit, m_staminaInsufficientViewTimer))
        {
            //スタミナが不足フラグをリセット
            m_isStaminaInsufficient = false;
            //タイマーリセット
            m_staminaInsufficientViewTimer = 0.0f;
        }

        
    }
   

}

void PlayerGameUI::UpdatePlayerHP()
{
    //最大HPと現在のHPを取得
    int maxHP = m_player->GetStatus().GetMaxHp();
    int currentHP = m_player->GetStatus().GetCurrentHp();


    //HPバーの減っていく割合。
    Vector3 HpScale = CalcGaugeRatio(maxHP, currentHP);
    //拡大率を設定
    m_spriteList[enSpriteName_HPBar_Front].SetScale(HpScale);
    //更新
    m_spriteList[enSpriteName_HPBar_Front].Update();


    //白いHPが現在のHPより小さいなら//白いHPに現在のHPを代入する
    if (m_whiteHP <= currentHP) 
    {
        m_whiteHP = currentHP;
    }
    else
    {
        m_whiteHP -= g_gameTime->GetFrameDeltaTime() * WHITE_HP_MUL_SPEED;
    }

    Vector3 whiteBarScale= CalcGaugeRatio(maxHP, m_whiteHP);

    m_spriteList[enSpriteName_HPBar_White].SetScale(whiteBarScale);

    m_spriteList[enSpriteName_HPBar_White].Update();

}

void PlayerGameUI::UpdatePlayerStamina()
{
    //スタミナが変動した時だけ処理する
    if (m_player->GetStatus().GetMaxStamina() == m_player->GetStatus().GetCurrentStamina())
    {
        m_isVeiwStaminaGauge = false;
        return;
    }

    //最大HPと現在のHPを取得
    int maxStamina = m_player->GetStatus().GetMaxStamina();
    int currentStamina = m_player->GetStatus().GetCurrentStamina();

    //スタミナバーの減っていく割合。
    Vector3 staminaScale = CalcGaugeRatio(maxStamina, currentStamina);

    m_spriteList[enSpriteName_StaminaBar_Front].SetScale(staminaScale);
    m_spriteList[enSpriteName_StaminaBar_Front].Update();

    //ここまで来ているということはスタミナが減っているので、ゲージを表示する
    m_isVeiwStaminaGauge = true;

    //スタミナが不足しているフラグが立ったなら
    if (m_player->IsStaminaInsufficient())
    {
        //こっちのフラグを立てる
        m_isStaminaInsufficient = true;



        //プレイヤー側のスタミナ不足フラグは元に戻す
        m_player->SetStaminaInsufficientFlag(false);
    }
   

}

void PlayerGameUI::UpdateWeaponUI()
{
    //メイン武器の更新
    m_weaponUIMap[WeaponManager::GetInstance()->GetMainWeaponType()]
        ->MainWeaponUpdate();
    //サブ武器の更新
    m_weaponUIMap[WeaponManager::GetInstance()->GetSubWeaponType()]
        ->SubWeaponUpdate();
    //サブ武器２の更新
    m_weaponUIMap[WeaponManager::GetInstance()->GetSubWeaponType2()]
        ->SubWeapon2Update();
}

void PlayerGameUI::UpdateHaveBuffs()
{
    //バフがないなら処理しない
    if (m_player->GetAttackBuffAmount() == 0)
    {
        m_isViewBuffSpritsAndFonts = false;
        return;
    }

    m_isViewBuffSpritsAndFonts = true;

    wchar_t attackBuffAmount[255];
    swprintf_s(attackBuffAmount, 255, L"x%d", m_player->GetAttackBuffAmount());
    //ダメージ量を設定
    m_fontList[enFontName_AttackIconAmount].SetText(attackBuffAmount);



}

void PlayerGameUI::Init()
{

    InitSpriteRenders();


    InitFontRenders();

}

void PlayerGameUI::WeaponUIInit()
{
    //武器のUIインスタンスを生成

   //ソード＆シールド
    m_weaponUIMap.insert(std::make_pair(enSwordShield, NewGO<SwordShieldUI>(0, "SwordShieldUI")));
    //グレイトソード
    m_weaponUIMap.insert(std::make_pair(enGreateSword, NewGO<GreateSwordUI>(0, "GreateSwordUI")));
    //ボウ＆アロー
    m_weaponUIMap.insert(std::make_pair(enBowArrow, NewGO<BowArrowUI>(0, "BowArrowUI")));


}

void PlayerGameUI::InitSpriteRenders()
{
    //HPフレームの初期化
    InitSpriteRender(
        m_spriteList[enSpriteName_HPBar_Frame],
        "Assets/sprite/InGame/Character/Player_HP_Flame.DDS", 608, 88, HP_CENTER_POS);
    //////////////////////////////////////////////////////////////////////////////////////////
    //HPバーの表
    InitSpriteRender(
        m_spriteList[enSpriteName_HPBar_Front],
        "Assets/sprite/InGame/Character/Player_HP_Front.DDS", 588, 78, HP_REFT_POS);
    //ピボットの設定
    m_spriteList[enSpriteName_HPBar_Front].SetPivot(GAUGE_PIBOT);
    ///////////////////////////////////////////////////////////////////////////////////////////
    //HPが減った時に出てくる白いHPバー
    InitSpriteRender(
        m_spriteList[enSpriteName_HPBar_White],
        "Assets/sprite/InGame/Character/Player_HP_White.DDS", 588, 78, HP_REFT_POS);
    //ピボットの設定
    m_spriteList[enSpriteName_HPBar_White].SetPivot(GAUGE_PIBOT);
    ////////////////////////////////////////////////////////////////////////////////////////////
    //HPバーの裏側
    InitSpriteRender(
        m_spriteList[enSpriteName_HPBar_Back],
        "Assets/sprite/InGame/Character/Player_HP_Back.DDS", 600, 80, HP_CENTER_POS);


    ////////////////////////////////////////////////////////////////////////////////////////////
    //スタミナバーのフレーム
    InitSpriteRender(
        m_spriteList[enSpriteName_StaminaBar_Frame],
        "Assets/sprite/InGame/Character/Player_Stamina_Flame.DDS", 354, 58, STAMINA_CENTER_POS);
    ////////////////////////////////////////////////////////////////////////////////////////////
    //スタミナバーの表
    InitSpriteRender(
        m_spriteList[enSpriteName_StaminaBar_Front],
        "Assets/sprite/InGame/Character/Player_Stamina_Front.DDS", 342, 48, STAMINA_REFT_POS);
    //ピボットの設定
    m_spriteList[enSpriteName_StaminaBar_Front].SetPivot(GAUGE_PIBOT);
    m_spriteList[enSpriteName_StaminaBar_Front].Update();
    ////////////////////////////////////////////////////////////////////////////////////////////
    //スタミナバーの裏側
    InitSpriteRender(
        m_spriteList[enSpriteName_StaminaBar_Back],
        "Assets/sprite/InGame/Character/Player_Stamina_Back.DDS", 354, 58, STAMINA_CENTER_POS);


    /////////////////////////////////////////////////////////////////////////////////////////////
    //メイン武器のフレーム
    InitSpriteRender(
        m_spriteList[enSpriteName_MainWeaponFlame],
        "Assets/sprite/InGame/Character/MainWeaponFlame.DDS", 285, 284, MAIN_WEAPON_CENTER_POS);
    /////////////////////////////////////////////////////////////////////////////////////////////
    //サブ武器のフレーム
    InitSpriteRender(
        m_spriteList[enSpriteName_SubWeaponFlame],
        "Assets/sprite/InGame/Character/SubWeaponFlame.DDS", 156, 155, SUB_WEAPON_CENTER_POS);
    /////////////////////////////////////////////////////////////////////////////////////////////
    //サブ武器2のフレーム
    InitSpriteRender(
        m_spriteList[enSpriteName_SubWeapon2Flame],
        "Assets/sprite/InGame/Character/SubWeaponFlame.DDS", 156, 155, SUB_WEAPON2_COENTER_POS);



    /////////////////////////////////////////////////////////////////////////////////////////////
     //サブ武器2のフレーム
    InitSpriteRender(
        m_spriteList[enSpriteName_AttackBuffIcon],
        "Assets/sprite/InGame/Character/AttackBuffEffectIcon.DDS", 110, 110, ATTACK_BUFF_ICON_POS);



}

void PlayerGameUI::InitFontRenders()
{
    //「スタミナが不足しています！」
    InitFontRender(
        m_fontList[enFontName_StaminaInsufficient],
        STAMINA_INSUFFCIENT_POS,
        1.0f,
        g_vec4Red,
        L"スタミナが不足しています"
    );
   
    //攻撃バフの数を表示
    InitFontRender(
        m_fontList[enFontName_AttackIconAmount],
        ATTACK_BUFF_AMOUNT_POS,
        1.0f,
        g_vec4White
    );

}