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
	/// 武器を選ぶ順番ステート
	/// </summary>
	enum EnSelectWeaponorder
	{
		enSelectWeaponorder_MainWeapon,	//一つ目の武器。メイン武器
		EnSelectWeaponorder_SubWeapon,	//二つ目の武器。サブ武器
		EnSelectWeaponorder_Complete	//両方決めたので、完了
	};


	/// <summary>
	/// 武器選択部屋モデルの初期化
	/// </summary>
	void InitWeaponRoom();
	/// <summary>
	/// 武器モデルの初期化
	/// </summary>
	void InitWeapon();
	/// <summary>
	/// 武器の名前の初期化
	/// </summary>
	void InitWeaponName();
	/// <summary>
	/// カメラ情報の初期化
	/// </summary>
	void InitCamera();

	/// <summary>
	/// 画面上の武器の回転処理
	/// </summary>
	void RotationWeapon();

	/// <summary>
	/// 武器選択関連の処理
	/// </summary>
	void ProcessSelectWeapon();
	/// <summary>
	/// 武器選択。選択肢の切り替え
	/// </summary>
	void ProcessChoice();
	/// <summary>
	/// 武器を順番に選ぶステートの管理
	/// </summary>
	void SelectWeaponManageState();

	/// <summary>
	/// 武器の名前関連の処理まとめ
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
		ModelRender m_weaponModel;//武器のモデルを格納する配列
		FontRender m_weaponNameFont;
		bool m_isSelect = false;		//選ばれたか
	};

	WeaponInfo m_weaponInfo[enWeaponType_Num];

	WeaponInfo aaa;

	std::vector<WeaponInfo*> m_canSelectWeapon;		//選べる武器
	std::vector<WeaponInfo*> m_chosenSelectWeapon;	//選ばれた武器

	

	int					m_notChoseWeapon = enWeaponType_Num;	//選ばれていない武器の種類

	
	FontRender			m_font[1];

	

	Fade*				m_fade = nullptr;

	EnWeaponType		m_enWeaponTipes[enWeaponType_Num];		//武器の種類を格納する配列

	ModelRender			m_weaponRoomModel;	//背景

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

	//const wchar_t m_weaponName[enWeaponType_Num];		//武器の名前を格納する配列

};

