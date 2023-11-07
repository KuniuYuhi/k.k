#include "stdafx.h"
#include "Bow.h"
#include "Brave.h"
#include "Arrow.h"

namespace {
	//���킪���[��Ԃ̎��̍��W
	const Vector3 STOWEDS_POSITION = { 0.0f,-500.0f,0.0f };


}

Bow::Bow()
{
}

Bow::~Bow()
{
	if (m_arrow != nullptr)
	{
		DeleteGO(m_arrow);
	}
}

bool Bow::Start()
{
	//�E�҂̃C���X�^���X��T��
	m_brave = FindGO<Brave>("brave");

	//��̃I�u�W�F�N�g�𐶐�
	m_arrow = NewGO<Arrow>(0, "arrow");
	m_arrow->SetBow(this);
	//��������Ă���̂ŁA�ێ��t���O���Z�b�g
	SetStockArrowFlag(true);

	InitModel();

	//����
	SetWeaponState(enWeaponState_Stowed);
	SetStowedFlag(true);
	//�h��^�C�v�̐ݒ�
	SetEnDefendTipe(enDefendTipe_avoid);


	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_brave->GetModelRender().AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	return true;
}

void Bow::Update()
{
	//���[��ԂȂ�
	if (GetStowedFlag() == true)
	{
		return;
	}

	if (GetStockArrowFlag() != true)
	{
		m_arrow = NewGO<Arrow>(0, "arrow");
		m_arrow->SetBow(this);
		//��������Ă���̂ŁA�ێ��t���O���Z�b�g
		SetStockArrowFlag(true);
	}

	MoveWeapon();

	m_modelBow.Update();
}

void Bow::MoveWeapon()
{
	switch (m_enWeaponState)
	{
	case IWeapon::enWeaponState_Stowed://���[���
		MoveStowed();
		break;
	case IWeapon::enWeaponState_Armed://�������
		MoveArmed();
		break;
	case IWeapon::enWeaponState_None://�Ȃ�
		break;
	default:
		break;
	}
}

bool Bow::IsHitCollision()
{
	return false;
}

void Bow::ProcessSkillAttack()
{
	//�{�^���������Ă���ԃ`���[�W
	if (g_pad[0]->IsPress(enButtonB) == true)
	{
		int a = 0;
	}
	//���������𔭎�
	else
	{
		//�E�҂̃A�j���[�V�������X�L�����C���ɐ؂�ւ�
		m_brave->SetNextAnimationState(Brave::enAnimationState_Skill_Main);
	}
}

void Bow::InitModel()
{
	//�|���f���̏�����
	m_modelBow.Init("Assets/modelData/character/Player/NewHero/Bow.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,
		0,
		enModelUpAxisZ
	);

	//�|�Ɩ�̍��W�ɑΉ�����{�[��ID���擾
	//������Ԃ̎��̃{�[��ID
	m_armedBowBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_l");
	m_armedArrowBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_r");

}

void Bow::MoveArmed()
{
	//�|�̃��[���h���W��ݒ�
	m_bowMatrix = 
		m_brave->GetModelRender().GetBone(m_armedBowBoonId)->GetWorldMatrix();
	m_modelBow.SetWorldMatrix(m_bowMatrix);
	//��̃��[���h���W��ݒ�
	m_arrowMatrix = 
		m_brave->GetModelRender().GetBone(m_armedArrowBoonId)->GetWorldMatrix();
	m_modelBow.SetWorldMatrix(m_bowMatrix);

	if (m_arrow != nullptr)
	{
		//��̃X�e�[�g��ݒ�
		m_arrow->SetWeaponState(enWeaponState_Armed);
	}
	
}

void Bow::MoveStowed()
{
	//�|�̍��W��ݒ�
	m_bowPos = STOWEDS_POSITION;
	m_modelBow.SetPosition(m_bowPos);

	if (m_arrow != nullptr)
	{
		//��̃X�e�[�g��ݒ�
		m_arrow->SetWeaponState(enWeaponState_Stowed);
	}
	

	SetStowedFlag(true);
}

void Bow::ProcessLongRangeAttack()
{
	if (m_arrow != nullptr)
	{
		//��𔭎�
		m_arrow->SetShotArrowSetting(
			true,
			m_brave->GetForward(),
			m_arrow->GetPosition(),
			Arrow::enShotPatternState_Normal
		);
		//���������̂ŁA���̖��ێ��t���O�����Z�b�g����B��������Ă��Ȃ����
		SetStockArrowFlag(false);
		m_arrow = nullptr;
	}
}

void Bow::SkillShot()
{
	if (m_arrow != nullptr)
	{
		//��𔭎�
		m_arrow->SetShotArrowSetting(
			true,
			m_brave->GetForward(),
			m_arrow->GetPosition(),
			Arrow::enShotPatternState_Skill
		);
		//���������̂ŁA���̖��ێ��t���O�����Z�b�g����B��������Ă��Ȃ����
		SetStockArrowFlag(false);
		m_arrow = nullptr;
	}
}

void Bow::Render(RenderContext& rc)
{
	if (GetWeaponState() == enWeaponState_Stowed)
	{
		return;
	}

	m_modelBow.Draw(rc);
}

void Bow::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	//�������U������
	if (wcscmp(eventName, L"LongRangeAttack") == 0)
	{
		ProcessLongRangeAttack();
	}

	//�X�L�����������u��
	if (wcscmp(eventName, L"SkillShot") == 0)
	{
		SkillShot();
	}
}
