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


	bool IsButtonAction();

	/// <summary>
	/// 
	/// </summary>
	void ProcessCommonStateTransition();

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
	/// ����؂�ւ�
	/// </summary>
	void ChangeWeaponAction();


	//void 



private:

	Status_Player m_status;					//�v���C���[�̃X�e�[�^�X

	std::unique_ptr<LoadBraveAnimationClips> m_braveAnimClip = nullptr;		//�A�j���[�V�����N���b�v���[�h

	PlayerMovement* m_playerMovement = nullptr;			//�ړ��R���|�[�l���g�C���X�^���X

	PlayerController* m_playerContoller = nullptr;		//�v���C���[�R���g���[���[�C���X�^���X

	std::unique_ptr<BraveStateContext> m_braveStateCotext = nullptr;	//�X�e�[�g�R���e�L�X�g



	WeaponType::EnWeaponType m_mainWeaponType;		//���C������^�C�v
	WeaponType::EnWeaponType m_subWeaponType;		//�T�u����1
	WeaponType::EnWeaponType m_subWeaponType2;		//�T�u����2
	WeaponType::EnWeaponType m_changeTargetWeaponType;	//�؂�ւ��Ώۂ̕���^�C�v



	bool m_isActionFlag = false;		//�A�N�V����t�����̃t���O
	

};

