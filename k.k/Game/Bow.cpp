#include "stdafx.h"
#include "Bow.h"
#include "Brave.h"

namespace {
	


}

Bow::Bow()
{
}

Bow::~Bow()
{
}

bool Bow::Start()
{
	m_brave = FindGO<Brave>("Brave");

	//����������
	Init();

	return true;
}

void Bow::Update()
{
	if(m_enWeaponState==enArmed)
	{
		MoveArmed();
	}



	m_bowModelRender.Update();

}

void Bow::Init()
{
	//�|���f���̏�����
	m_bowModelRender.Init("Assets/modelData/character/Player/NewHero/Bow.tkm",
		L"Assets/shader/ToonTextrue/lamp_glay.DDS",
		0,0,enModelUpAxisZ
	);

	//�|�Ɩ�̍��W�ɑΉ�����{�[��ID���擾
	//������Ԃ̎��̃{�[��ID
	m_armedBowBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_l");
	m_armedArrowBoonId = m_brave->GetModelRender().FindBoneID(L"weaponShield_r");

	//���킪���[��ԂȂ�
	if (m_enWeaponState == enStowed)
	{
		ChangeStowedState();
	}
	//���킪������ԂȂ�
	else
	{
		ChangeArmedState();
	}

}

void Bow::ChangeStowedState()
{
	SetCurrentWeaponState(enStowed);
	//���[���̍��W�ɕύX
	m_bowModelRender.SetPosition(m_stowedPosition);

}

void Bow::ChangeArmedState()
{
	SetCurrentWeaponState(enArmed);
}

void Bow::AttackAction()
{
}

void Bow::MoveArmed()
{
	Vector3 swordPos = g_vec3Zero;
	//���̃��[���h���W��ݒ�
	m_bowMatrix = m_brave->GetModelRender().GetBone(m_armedBowBoonId)->GetWorldMatrix();
	m_bowModelRender.SetWorldMatrix(m_bowMatrix);

}

void Bow::Render(RenderContext& rc)
{
	m_bowModelRender.Draw(rc);
}
