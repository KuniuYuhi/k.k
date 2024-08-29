#pragma once
#include "CharacterBase.h"

#include "Status_Player.h"

#include "LoadBraveAnimationClips.h"
#include "BraveStateContext.h"

#include "KnockBackInfo.h"

class Status_Player;
class PlayerMovement;
class PlayerController;
class DamageProvider;
class LoadBraveAnimationClips;
class BraveStateContext;
class WeaponBase;

using namespace DamageInformaiton;
using namespace KnockBackInfo;

class Brave : public CharacterBase
{
public:
	~Brave();


	bool Start() override;

	void Update() override;

	void Render(RenderContext& rc) override;

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	/// <summary>
	/// ���݂̍U���͂��擾
	/// </summary>
	/// <returns></returns>
	int GetCurrentPower() override;


	/// <summary>
	/// ��]���邽�߂̕����x�N�g����ݒ�
	/// </summary>
	/// <param name="rotateDirection"></param>
	void SetRotateDirection(Vector3 rotateDirection)
	{
		m_rotateDirection = rotateDirection;
	}

	/// <summary>
	/// ���݂̕���̃A�j���[�V�����̍ŏ��̔ԍ����擾
	/// </summary>
	/// <returns></returns>
	const int GetCurrentAnimationStartIndexNo()
	{
		return m_braveAnimClip.get()->GetCurrentAnimationStartIndexNo();
	}

	/// <summary>
	/// 
	/// </summary>
	void SetCurrentAnimationStartIndexNoForMainWeaponType();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="moveSpeed"></param>
	void CharaConExecute(Vector3 moveSpeed);

	/// <summary>
	/// �A�N�V�������ɂ���
	/// </summary>
	void ActionActive()
	{
		m_isActionFlag = true;
	}
	/// <summary>
	/// �A�N�V�������I���
	/// </summary>
	void ActionDeactive()
	{
		m_isActionFlag = false;
	}
	/// <summary>
	/// �A�N�V��������
	/// </summary>
	/// <returns></returns>
	const bool IsAction() const
	{
		return m_isActionFlag;
	}

	/// <summary>
	/// ���G�L����
	/// </summary>
	void EnableInvincible()
	{
		m_isInvincible = true;
	}
	/// <summary>
	/// ���G������
	/// </summary>
	void DisableInvincible()
	{
		m_isInvincible = false;
	}
	/// <summary>
	/// ���G���H
	/// </summary>
	/// <returns></returns>
	const bool IsInvincible() const
	{
		return m_isInvincible;
	}

	/// <summary>
	/// �X�L�����C���ɐi�ނ��̃t���O
	/// </summary>
	/// <param name="flag"></param>
	void SetProceedSkillMainFlag(bool flag)
	{
		m_isProceedSkillMain = flag;
	}
	/// <summary>
	/// �X�L�����C���ɐi�ނ̂��H
	/// </summary>
	/// <returns></returns>
	bool IsProceedSkillMain()
	{
		return m_isProceedSkillMain;
	}

	/// <summary>
	/// ������Ԃ̕�����擾
	/// </summary>
	/// <returns></returns>
	WeaponBase* GetArmedWeapon()
	{
		return m_armedWeapon;
	}

	/// <summary>
	/// �v���C���[�R���g���[���[���擾
	/// </summary>
	/// <returns></returns>
	PlayerController* GetPlayerController()
	{
		return m_playerContoller;
	}
	/// <summary>
	/// �v���C���[���[�u�����g���擾
	/// </summary>
	/// <returns></returns>
	PlayerMovement* GetPlayerMovement()
	{
		return m_playerMovement;
	}
	/// <summary>
	/// �_���[�W�v���o�C�_�[���擾
	/// </summary>
	/// <returns></returns>
	DamageProvider* GetDamageProvider()
	{
		return m_damageProvider;
	}

	/// <summary>
	/// �X�e�[�^�X�̎擾
	/// </summary>
	/// <returns></returns>
	Status_Player& GetStatus()
	{
		return m_status;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="position"></param>
	void SetDamageProviderPosition(Vector3 position)
	{
		m_damageProviderPosition = position;
	}


	bool IsButtonAction();

	/// <summary>
	/// ���ʂ̃X�e�[�g����
	/// </summary>
	void ProcessCommonStateTransition();

	/// <summary>
	/// �ʏ�U������
	/// </summary>
	void NormalAttackProcess();

	/// <summary>
	/// �U���A�N�V�����𔲂��o��
	/// </summary>
	void ExitAttackAction();

	/// <summary>
	/// �X�e�[�g��؂�ւ���
	/// </summary>
	/// <param name="nextState">���̃X�e�[�g</param>
	void ChangeBraveState(BraveState::EnBraveState nextState);


	void EntryHitActionProcess();

	void UpdateHitActionProcess();

	void ExitHitActionProcess();
	
	/// <summary>
	/// �m�b�N�o�b�N�̏����_���[�W�����g���Đݒ�
	/// </summary>
	/// <param name="damageInfo"></param>
	void SettingKnockBackInfoForDamageInfo(DamageInfo damageInfo);

private:

	/// <summary>
	/// ��{�I�ȃR���|�[�l���g���Z�b�e�B���O
	/// </summary>
	void SettingDefaultComponent() override;


	/// <summary>
	/// �_���[�W���󂯂����̏���
	/// </summary>
	void ProcessHit(DamageInfo damageInfo);

	/// <summary>
	/// �ړ�����
	/// </summary>
	void Movement();
	/// <summary>
	/// ��]����
	/// </summary>
	void Rotation();

	/// <summary>
	/// �{�^���A�N�V�����̏���
	/// </summary>
	void ButtonAction();

	/// <summary>
	/// �U������
	/// </summary>
	void AttackAction();

	/// <summary>
	/// ����A�h�䏈��
	/// </summary>
	void DefensiveAction();

	/// <summary>
	/// ����؂�ւ�
	/// </summary>
	void ChangeWeaponAction();

	/// <summary>
	/// �d�����Ԃ�
	/// </summary>
	/// <returns>�d�����Ȃ�true�A�d�����łȂ��Ȃ�false</returns>
	bool IsStarkTime();

	/// <summary>
	/// �X�^�~�i�̎�����
	/// </summary>
	void AutoRecoveryStamina();

	/// <summary>
	/// �R���W�����̃`�F�b�N
	/// </summary>
	void CheckSelfCollision();

	/// <summary>
	/// �m�b�N�o�b�N����O�̏���
	/// </summary>
	void SettingKnockBackProcess();

	void KnockBackMove(int listNum);

	void KnockBackGravityFall();

private:

	Status_Player m_status;					//�v���C���[�̃X�e�[�^�X

	std::unique_ptr<LoadBraveAnimationClips> m_braveAnimClip = nullptr;		//�A�j���[�V�����N���b�v���[�h

	PlayerMovement* m_playerMovement = nullptr;			//�ړ��R���|�[�l���g�C���X�^���X

	DamageProvider* m_damageProvider = nullptr;

	PlayerController* m_playerContoller = nullptr;		//�v���C���[�R���g���[���[�C���X�^���X

	std::unique_ptr<BraveStateContext> m_braveStateCotext = nullptr;	//�X�e�[�g�R���e�L�X�g

	WeaponBase* m_armedWeapon = nullptr;		//�������Ă��镐��

	std::vector<CurvePoint> m_curvePointList;			//�m�b�N�o�b�N�J�[�u�|�C���g���X�g

	Vector2 m_knockBackSpeed = g_vec2Zero;				//�m�b�N�o�b�N�̃X�s�[�h

	Vector3 m_damageProviderPosition = g_vec3Zero;		//�_���[�W�񋟎ҁi�U���ҁj�̍��W

	Vector3 m_saveForward = g_vec3Zero;					//�ۑ��p�O����

	EnKnockBackPattern m_hitKnockBackPattern;			//�m�b�N�o�b�N�̃p�^�[��
	float m_knockBackTimeScale = 0.0f;					//�m�b�N�o�b�N�̐i�s���x
	
	bool m_isProceedSkillMain = false;					//�X�L�����C���X�e�[�g�ɐi�ނ̂�

	bool m_isActionFlag = false;		//�A�N�V����t�����̃t���O
	

	bool m_isInvincible = false;		//���G���H

	float m_starkTimer = 0.0f;			//�d���^�C�}�[

	float count = 0.0f;

	int oldAttackId = -1;

};

