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
	enum EnSelectWeaponOrder
	{

		enSelectWeaponOrder_MainWeapon,	//��ڂ̕���B���C������
		enSelectWeaponOrder_SubWeapon,	//��ڂ̕���B�T�u����
		enSelectWeaponOrder_Complete,	//�������߂��̂ŁA����
		enSelectWeaponOrder_None		//�����I������Ă��Ȃ�
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
	/// �摜�̏�����
	/// </summary>
	void InitSprite();

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
	/// <param name="weaponOrder">�I�����镐��̏��ԁB���C�����T�u��</param>
	void ProcessChoice(EnSelectWeaponOrder weaponOrder);
	/// <summary>
	/// ��������ԂɑI�ԃX�e�[�g�̊Ǘ�
	/// </summary>
	void SelectWeaponManageState();
	/// <summary>
	/// ���C���A�T�u�����I�񂾌�̏���
	/// </summary>
	void ProcessComplete();

	/// <summary>
	/// ����̖��O�֘A�̏����܂Ƃ�
	/// </summary>
	void ProcessWeaponName();

	/// <summary>
	/// 
	/// </summary>
	void GoToPlayMode();

	/// <summary>
	/// �t�H���g�̏�����
	/// </summary>
	/// <param name="fontRender">�������������t�H���g�����_�[</param>
	/// <param name="position">���W</param>
	/// <param name="name">�ݒ肵���������A������</param>
	/// <param name="color">�����̐F</param>
	/// <param name="isShadowParam">�e�����邩(�֊s)</param>
	/// <param name="shadowOffset">�I�t�Z�b�g��</param>
	/// <param name="shadowColor">�e�̐F</param>
	

	/// <summary>
	/// �t�H���g�̏�����
	/// </summary>
	/// <param name="fontRender">�������������t�H���g�����_�[<</param>
	/// <param name="position">���W</param>
	/// <param name="scale">�g�嗦</param>
	/// <param name="name">�ݒ肵���������A������</param>
	/// <param name="color">�����̐F</param>
	/// <param name="isShadowParam">�e�����邩(�֊s)</param>
	/// <param name="shadowOffset">�I�t�Z�b�g��</param>
	/// <param name="shadowColor">�e�̐F</param>
	void InitFontRender(
		FontRender& fontRender,
		Vector2 position,
		float scale,
		const wchar_t* name,
		Vector4 color = g_vec4White,
		bool isShadowParam = true,
		float shadowOffset = 2.0f,
		Vector4 shadowColor = g_vec4Black
	);

	/// <summary>
	/// �I�����Ă��镐��
	/// </summary>
	/// <param name="weaponOrder"></param>
	void SetEnSelectWeaponOrder(EnSelectWeaponOrder weaponOrder)
	{
		m_enSelectWeaponOrder = weaponOrder;
	}
	/// <summary>
	/// ���݂ǂ̕����I�����Ă��邩�擾
	/// </summary>
	/// <returns></returns>
	const EnSelectWeaponOrder& GetSelectWeaponOrder() const
	{
		return m_enSelectWeaponOrder;
	}

private:

	Vector2 m_namePosForSelectMainWeapon[enWeaponType_Num] = {
		{50.0f,250.0f},
		{120.0f,50.0f},
		{190.0f,-150.0f},
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
		EnSelectWeaponOrder m_weaponOrder;
	};

	WeaponInfo m_weaponInfo[enWeaponType_Num];

	WeaponInfo aaa;

	std::vector<WeaponInfo*> m_canSelectWeapon;		//�I�ׂ镐��
	std::vector<WeaponInfo*> m_chosenSelectWeapon;	//�I�΂ꂽ����

	

	int					m_notChoseWeapon = enWeaponType_Num;	//�I�΂�Ă��Ȃ�����̎��

	

	Fade*				m_fade = nullptr;

	EnWeaponType		m_enWeaponTipe;

	EnSelectWeaponOrder m_enSelectWeaponOrder = enSelectWeaponOrder_MainWeapon;	//����I��

	ModelRender			m_weaponRoomModel;	//�w�i
	SpriteRender		m_selectBarSprite;	//�I���o�[
	SpriteRender		m_selectBackSprite;	//�I��w�i

	SpringCamera		m_springCamera;
	Vector3				m_toCameraPos = g_vec3Zero;
	Vector3				m_positionCamera = g_vec3Zero;
	Vector3				m_targetPosition = g_vec3Zero;

	Vector3				m_scaleWeapon = g_vec3One;
	Vector3				m_weaponPosition = g_vec3Zero;
	Quaternion			m_rotation = g_quatIdentity;

	Vector3				m_positionRoom = g_vec3Zero;

	Vector3				m_moveSelectBar = g_vec3Zero;
	Vector3				m_selectCompleteBar = g_vec3Zero;


	int					m_nowSelectWeaponNumber = 0;

	bool				m_goToGameFlag = false;

};

