#include "stdafx.h"
#include "PlayerGameUI.h"

#include "Brave.h"


#include "WeaponManager.h"

#include "GreateSwordUI.h"
#include "SwordShieldUI.h"
#include "BowArrowUI.h"



namespace {

    //////////////////////////////////////////////////////////////////
    //�Q�[�W
    ///////////////////////////////////////////////////////////////////
    const Vector2 GAUGE_PIBOT = { 0.0f,0.5f };							//HP��MP�̃s�{�b�g
    ///////////////////////////////////////////////////////////////////
    //�X�e�[�^�X�Ɋւ���l
    ///////////////////////////////////////////////////////////////////
    const Vector2 HP_CENTER_POS = { -650.0f, -470.0f };     //HP�Ɋւ���X�v���C�g�̒��S
    const Vector2 HP_REFT_POS = { -940.0f, -470.0f };       //HP�̍���

    const float WHITE_HP_MUL_SPEED = 25.0f;

    const Vector2 STAMINA_CENTER_POS = { 0.0f,-380.0f };
    const Vector2 STAMINA_REFT_POS = { -171.0f,-380.0f };

    const Vector2 STAMINA_INSUFFCIENT_POS = { -350.0f,-290.0f };

    ///////////////////////////////////////////////////////////////////
    //�o�t
    ///////////////////////////////////////////////////////////////////

    const Vector2 ATTACK_BUFF_AMOUNT_POS = { -870.0f,-370.0f };
    const Vector2 ATTACK_BUFF_ICON_POS = { -860.0f,-360.0f };
}

PlayerGameUI::~PlayerGameUI()
{
    for (auto weaponUI : m_weaponUIMap)
    {
        //�폜
        weaponUI.second->DeleteSelf();
    }

    m_weaponUIMap.clear();

}

bool PlayerGameUI::Start()
{
    m_player = FindGO<Brave>("Brave");


    m_whiteHP = m_player->GetStatus().GetCurrentHp();

    //������
    Init();

    WeaponUIInit();

   



    return true;
}

void PlayerGameUI::UIUpdate()
{
    //�v���C���[�̃X�e�[�^�X��UI���X�V
    UpdatePlayerStatusUI();

    //����UI�̍X�V
    UpdateWeaponUI();

    //�o�t�̍X�V����
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
        //�`��
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
    //HP�̍X�V����
    UpdatePlayerHP();
    //�X�^�~�i�̍X�V����
    UpdatePlayerStamina();

    //�X�^�~�i���s�����Ă���Ȃ�
    if (m_isStaminaInsufficient)
    {
        //�^�C�}�[���~�܂�����
        if (!IsTimerRunning(m_staminaInsufficientViewTimeLimit, m_staminaInsufficientViewTimer))
        {
            //�X�^�~�i���s���t���O�����Z�b�g
            m_isStaminaInsufficient = false;
            //�^�C�}�[���Z�b�g
            m_staminaInsufficientViewTimer = 0.0f;
        }

        
    }
   

}

void PlayerGameUI::UpdatePlayerHP()
{
    //�ő�HP�ƌ��݂�HP���擾
    int maxHP = m_player->GetStatus().GetMaxHp();
    int currentHP = m_player->GetStatus().GetCurrentHp();


    //HP�o�[�̌����Ă��������B
    Vector3 HpScale = CalcGaugeRatio(maxHP, currentHP);
    //�g�嗦��ݒ�
    m_spriteList[enSpriteName_HPBar_Front].SetScale(HpScale);
    //�X�V
    m_spriteList[enSpriteName_HPBar_Front].Update();


    //����HP�����݂�HP��菬�����Ȃ�//����HP�Ɍ��݂�HP��������
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
    //�X�^�~�i���ϓ�������������������
    if (m_player->GetStatus().GetMaxStamina() == m_player->GetStatus().GetCurrentStamina())
    {
        m_isVeiwStaminaGauge = false;
        return;
    }

    //�ő�HP�ƌ��݂�HP���擾
    int maxStamina = m_player->GetStatus().GetMaxStamina();
    int currentStamina = m_player->GetStatus().GetCurrentStamina();

    //�X�^�~�i�o�[�̌����Ă��������B
    Vector3 staminaScale = CalcGaugeRatio(maxStamina, currentStamina);

    m_spriteList[enSpriteName_StaminaBar_Front].SetScale(staminaScale);
    m_spriteList[enSpriteName_StaminaBar_Front].Update();

    //�����܂ŗ��Ă���Ƃ������Ƃ̓X�^�~�i�������Ă���̂ŁA�Q�[�W��\������
    m_isVeiwStaminaGauge = true;

    //�X�^�~�i���s�����Ă���t���O���������Ȃ�
    if (m_player->IsStaminaInsufficient())
    {
        //�������̃t���O�𗧂Ă�
        m_isStaminaInsufficient = true;



        //�v���C���[���̃X�^�~�i�s���t���O�͌��ɖ߂�
        m_player->SetStaminaInsufficientFlag(false);
    }
   

}

void PlayerGameUI::UpdateWeaponUI()
{
    //���C������̍X�V
    m_weaponUIMap[WeaponManager::GetInstance()->GetMainWeaponType()]
        ->MainWeaponUpdate();
    //�T�u����̍X�V
    m_weaponUIMap[WeaponManager::GetInstance()->GetSubWeaponType()]
        ->SubWeaponUpdate();
    //�T�u����Q�̍X�V
    m_weaponUIMap[WeaponManager::GetInstance()->GetSubWeaponType2()]
        ->SubWeapon2Update();
}

void PlayerGameUI::UpdateHaveBuffs()
{
    //�o�t���Ȃ��Ȃ珈�����Ȃ�
    if (m_player->GetAttackBuffAmount() == 0)
    {
        m_isViewBuffSpritsAndFonts = false;
        return;
    }

    m_isViewBuffSpritsAndFonts = true;

    wchar_t attackBuffAmount[255];
    swprintf_s(attackBuffAmount, 255, L"x%d", m_player->GetAttackBuffAmount());
    //�_���[�W�ʂ�ݒ�
    m_fontList[enFontName_AttackIconAmount].SetText(attackBuffAmount);



}

void PlayerGameUI::Init()
{

    InitSpriteRenders();


    InitFontRenders();

}

void PlayerGameUI::WeaponUIInit()
{
    //�����UI�C���X�^���X�𐶐�

   //�\�[�h���V�[���h
    m_weaponUIMap.insert(std::make_pair(enSwordShield, NewGO<SwordShieldUI>(0, "SwordShieldUI")));
    //�O���C�g�\�[�h
    m_weaponUIMap.insert(std::make_pair(enGreateSword, NewGO<GreateSwordUI>(0, "GreateSwordUI")));
    //�{�E���A���[
    m_weaponUIMap.insert(std::make_pair(enBowArrow, NewGO<BowArrowUI>(0, "BowArrowUI")));


}

void PlayerGameUI::InitSpriteRenders()
{
    //HP�t���[���̏�����
    InitSpriteRender(
        m_spriteList[enSpriteName_HPBar_Frame],
        "Assets/sprite/InGame/Character/Player_HP_Flame.DDS", 608, 88, HP_CENTER_POS);
    //////////////////////////////////////////////////////////////////////////////////////////
    //HP�o�[�̕\
    InitSpriteRender(
        m_spriteList[enSpriteName_HPBar_Front],
        "Assets/sprite/InGame/Character/Player_HP_Front.DDS", 588, 78, HP_REFT_POS);
    //�s�{�b�g�̐ݒ�
    m_spriteList[enSpriteName_HPBar_Front].SetPivot(GAUGE_PIBOT);
    ///////////////////////////////////////////////////////////////////////////////////////////
    //HP�����������ɏo�Ă��锒��HP�o�[
    InitSpriteRender(
        m_spriteList[enSpriteName_HPBar_White],
        "Assets/sprite/InGame/Character/Player_HP_White.DDS", 588, 78, HP_REFT_POS);
    //�s�{�b�g�̐ݒ�
    m_spriteList[enSpriteName_HPBar_White].SetPivot(GAUGE_PIBOT);
    ////////////////////////////////////////////////////////////////////////////////////////////
    //HP�o�[�̗���
    InitSpriteRender(
        m_spriteList[enSpriteName_HPBar_Back],
        "Assets/sprite/InGame/Character/Player_HP_Back.DDS", 600, 80, HP_CENTER_POS);


    ////////////////////////////////////////////////////////////////////////////////////////////
    //�X�^�~�i�o�[�̃t���[��
    InitSpriteRender(
        m_spriteList[enSpriteName_StaminaBar_Frame],
        "Assets/sprite/InGame/Character/Player_Stamina_Flame.DDS", 354, 58, STAMINA_CENTER_POS);
    ////////////////////////////////////////////////////////////////////////////////////////////
    //�X�^�~�i�o�[�̕\
    InitSpriteRender(
        m_spriteList[enSpriteName_StaminaBar_Front],
        "Assets/sprite/InGame/Character/Player_Stamina_Front.DDS", 342, 48, STAMINA_REFT_POS);
    //�s�{�b�g�̐ݒ�
    m_spriteList[enSpriteName_StaminaBar_Front].SetPivot(GAUGE_PIBOT);
    m_spriteList[enSpriteName_StaminaBar_Front].Update();
    ////////////////////////////////////////////////////////////////////////////////////////////
    //�X�^�~�i�o�[�̗���
    InitSpriteRender(
        m_spriteList[enSpriteName_StaminaBar_Back],
        "Assets/sprite/InGame/Character/Player_Stamina_Back.DDS", 354, 58, STAMINA_CENTER_POS);


    /////////////////////////////////////////////////////////////////////////////////////////////
    //���C������̃t���[��
    InitSpriteRender(
        m_spriteList[enSpriteName_MainWeaponFlame],
        "Assets/sprite/InGame/Character/MainWeaponFlame.DDS", 285, 284, MAIN_WEAPON_CENTER_POS);
    /////////////////////////////////////////////////////////////////////////////////////////////
    //�T�u����̃t���[��
    InitSpriteRender(
        m_spriteList[enSpriteName_SubWeaponFlame],
        "Assets/sprite/InGame/Character/SubWeaponFlame.DDS", 156, 155, SUB_WEAPON_CENTER_POS);
    /////////////////////////////////////////////////////////////////////////////////////////////
    //�T�u����2�̃t���[��
    InitSpriteRender(
        m_spriteList[enSpriteName_SubWeapon2Flame],
        "Assets/sprite/InGame/Character/SubWeaponFlame.DDS", 156, 155, SUB_WEAPON2_COENTER_POS);



    /////////////////////////////////////////////////////////////////////////////////////////////
     //�T�u����2�̃t���[��
    InitSpriteRender(
        m_spriteList[enSpriteName_AttackBuffIcon],
        "Assets/sprite/InGame/Character/AttackBuffEffectIcon.DDS", 110, 110, ATTACK_BUFF_ICON_POS);



}

void PlayerGameUI::InitFontRenders()
{
    //�u�X�^�~�i���s�����Ă��܂��I�v
    InitFontRender(
        m_fontList[enFontName_StaminaInsufficient],
        STAMINA_INSUFFCIENT_POS,
        1.0f,
        g_vec4Red,
        L"�X�^�~�i���s�����Ă��܂�"
    );
   
    //�U���o�t�̐���\��
    InitFontRender(
        m_fontList[enFontName_AttackIconAmount],
        ATTACK_BUFF_AMOUNT_POS,
        1.0f,
        g_vec4White
    );

}