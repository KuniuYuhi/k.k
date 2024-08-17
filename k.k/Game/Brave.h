#pragma once
#include "CharacterBase.h"

#include "Status_Player.h"

#include "LoadBraveAnimationClips.h"
#include "BraveStateContext.h"

class Status_Player;
class PlayerMovement;
class PlayerController;
class LoadBraveAnimationClips;
class BraveStateContext;
class WeaponBase;



class Brave : public CharacterBase
{
public:
	~Brave();


	bool Start() override;

	void Update() override;

	void Render(RenderContext& rc) override;

	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);

	/// <summary>
	/// ��_���[�W����
	/// </summary>
	virtual void Damage() override;

	/// <summary>
	/// ���S������
	/// </summary>
	virtual void Dead() override;

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

private:

	/// <summary>
	/// ��{�I�ȃR���|�[�l���g���Z�b�e�B���O
	/// </summary>
	void SettingDefaultComponent() override;


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



private:

	Status_Player m_status;					//�v���C���[�̃X�e�[�^�X

	std::unique_ptr<LoadBraveAnimationClips> m_braveAnimClip = nullptr;		//�A�j���[�V�����N���b�v���[�h

	PlayerMovement* m_playerMovement = nullptr;			//�ړ��R���|�[�l���g�C���X�^���X

	PlayerController* m_playerContoller = nullptr;		//�v���C���[�R���g���[���[�C���X�^���X

	std::unique_ptr<BraveStateContext> m_braveStateCotext = nullptr;	//�X�e�[�g�R���e�L�X�g

	WeaponBase* m_armedWeapon = nullptr;		//�������Ă��镐��

	


	bool m_isActionFlag = false;		//�A�N�V����t�����̃t���O
	

	float m_starkTimer = 0.0f;			//�d���^�C�}�[

};

