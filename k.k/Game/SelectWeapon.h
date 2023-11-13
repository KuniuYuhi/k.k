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
	enum EnSelectWeaponOrder
	{

		enSelectWeaponOrder_MainWeapon,	//一つ目の武器。メイン武器
		enSelectWeaponOrder_SubWeapon,	//二つ目の武器。サブ武器
		enSelectWeaponOrder_Complete,	//両方決めたので、完了
		enSelectWeaponOrder_None		//何も選択されていない
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
	/// 画像の初期化
	/// </summary>
	void InitSprite();

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
	/// <param name="weaponOrder">選択する武器の順番。メインかサブか</param>
	void ProcessChoice(EnSelectWeaponOrder weaponOrder);
	/// <summary>
	/// 武器を順番に選ぶステートの管理
	/// </summary>
	void SelectWeaponManageState();
	/// <summary>
	/// メイン、サブ武器を選んだ後の処理
	/// </summary>
	void ProcessComplete();

	/// <summary>
	/// 武器の名前関連の処理まとめ
	/// </summary>
	void ProcessWeaponName();

	/// <summary>
	/// 
	/// </summary>
	void GoToPlayMode();

	/// <summary>
	/// フォントの初期化
	/// </summary>
	/// <param name="fontRender">初期化したいフォントレンダー</param>
	/// <param name="position">座標</param>
	/// <param name="name">設定したい文字、文字列</param>
	/// <param name="color">文字の色</param>
	/// <param name="isShadowParam">影をつけるか(輪郭)</param>
	/// <param name="shadowOffset">オフセット量</param>
	/// <param name="shadowColor">影の色</param>
	

	/// <summary>
	/// フォントの初期化
	/// </summary>
	/// <param name="fontRender">初期化したいフォントレンダー<</param>
	/// <param name="position">座標</param>
	/// <param name="scale">拡大率</param>
	/// <param name="name">設定したい文字、文字列</param>
	/// <param name="color">文字の色</param>
	/// <param name="isShadowParam">影をつけるか(輪郭)</param>
	/// <param name="shadowOffset">オフセット量</param>
	/// <param name="shadowColor">影の色</param>
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
	/// 選択している武器
	/// </summary>
	/// <param name="weaponOrder"></param>
	void SetEnSelectWeaponOrder(EnSelectWeaponOrder weaponOrder)
	{
		m_enSelectWeaponOrder = weaponOrder;
	}
	/// <summary>
	/// 現在どの武器を選択しているか取得
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
		ModelRender m_weaponModel;//武器のモデルを格納する配列
		FontRender m_weaponNameFont;
		bool m_isSelect = false;		//選ばれたか
		EnSelectWeaponOrder m_weaponOrder;
	};

	WeaponInfo m_weaponInfo[enWeaponType_Num];

	WeaponInfo aaa;

	std::vector<WeaponInfo*> m_canSelectWeapon;		//選べる武器
	std::vector<WeaponInfo*> m_chosenSelectWeapon;	//選ばれた武器

	

	int					m_notChoseWeapon = enWeaponType_Num;	//選ばれていない武器の種類

	

	Fade*				m_fade = nullptr;

	EnWeaponType		m_enWeaponTipe;

	EnSelectWeaponOrder m_enSelectWeaponOrder = enSelectWeaponOrder_MainWeapon;	//武器選択

	ModelRender			m_weaponRoomModel;	//背景
	SpriteRender		m_selectBarSprite;	//選択バー
	SpriteRender		m_selectBackSprite;	//選択背景

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

