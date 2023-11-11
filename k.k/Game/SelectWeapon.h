#pragma once
#include "MyWeapon.h"
#include "SpringCamera.h"

class Fade;

class SelectWeapon:public IGameObject
{
public:
	SelectWeapon();
	~SelectWeapon();

	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:

	/// <summary>
	/// �����I�ԏ��ԃX�e�[�g
	/// </summary>
	enum EnSelectWeaponorder
	{
		enSelectWeaponorder_MainWeapon,	//��ڂ̕���B���C������
		EnSelectWeaponorder_SubWeapon,	//��ڂ̕���B�T�u����
		EnSelectWeaponorder_Complete	//�������߂��̂ŁA����
	};


	/// <summary>
	/// ����I�𕔉����f���̏�����
	/// </summary>
	void InitWeaponRoom();
	/// <summary>
	/// ���탂�f���̏�����
	/// </summary>
	void InitWeapon();
	/// <summary>
	/// ����̖��O�̏�����
	/// </summary>
	void InitWeaponName();
	/// <summary>
	/// �J�������̏�����
	/// </summary>
	void InitCamera();

	/// <summary>
	/// ��ʏ�̕���̉�]����
	/// </summary>
	void RotationWeapon();

	/// <summary>
	/// ����I���֘A�̏���
	/// </summary>
	void ProcessSelectWeapon();
	/// <summary>
	/// ����I���B�I�����̐؂�ւ�
	/// </summary>
	void ProcessChoice();
	/// <summary>
	/// ��������ԂɑI�ԃX�e�[�g�̊Ǘ�
	/// </summary>
	void SelectWeaponManageState();

	/// <summary>
	/// ����̖��O�֘A�̏����܂Ƃ�
	/// </summary>
	void ProcessWeaponName();

	void GoToPlayMode();

private:

	Vector2 m_namePosForSelectMainWeapon[enWeaponType_Num] = {
		{0.0f,200.0f},
		{50.0f,100.0f},
		{70.0f,0.0f},
	};

	Vector2 m_namePosForSelectSubWeapon[enWeaponType_Num] = {
		{200.0f,200.0f},
		{250.0f,0.0f},
		{300.0f,-200.0f}
	};



	struct WeaponInfo
	{
		ModelRender m_weaponModel;//����̃��f�����i�[����z��
		FontRender m_weaponNameFont;
		bool m_isSelect = false;		//�I�΂ꂽ��
	};

	WeaponInfo m_weaponInfo[enWeaponType_Num];

	WeaponInfo aaa;

	std::vector<WeaponInfo*> m_canSelectWeapon;		//�I�ׂ镐��
	std::vector<WeaponInfo*> m_chosenSelectWeapon;	//�I�΂ꂽ����

	

	int					m_notChoseWeapon = enWeaponType_Num;	//�I�΂�Ă��Ȃ�����̎��

	
	FontRender			m_font[1];

	

	Fade*				m_fade = nullptr;

	EnWeaponType		m_enWeaponTipes[enWeaponType_Num];		//����̎�ނ��i�[����z��

	ModelRender			m_weaponRoomModel;	//�w�i

	SpringCamera		m_springCamera;
	Vector3				m_toCameraPos = g_vec3Zero;
	Vector3				m_positionCamera = g_vec3Zero;
	Vector3				m_targetPosition = g_vec3Zero;

	Vector3				m_scaleWeapon = g_vec3One;
	Vector3				m_weaponPosition = g_vec3Zero;
	Quaternion			m_rotation = g_quatIdentity;

	Vector3				m_positionRoom = g_vec3Zero;

	int					m_nowSelectWeaponNumber = 0;




	bool				m_goToGameFlag = false;

	//const wchar_t m_weaponName[enWeaponType_Num];		//����̖��O���i�[����z��

};

