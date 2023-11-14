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
	/// 武器が決定してからのステート
	/// </summary>
	enum EnCompleteState
	{
		enCompleteState_OffScreenObject,
		enCompleteState_OnScreenObject,
		enCompleteState_Complete,
	};

	/// <summary>
	/// 各種ライトの設定
	/// </summary>
	void SettingLight();
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
		Vector3 position,
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

	/// <summary>
	/// 武器選択に関連するフォント、画像を画面外に移動させる
	/// </summary>
	/// <param name="moveScreenFlag">画面外に移動させるならtrue、画面内に移動させるならfalse</param>
	void ScreenWeaponFontAndSprite(bool moveScreenFlag,Vector3 start,Vector3 end);
	
	/// <summary>
	/// スクリーン(画像、フォント)の移動
	/// </summary>
	/// <param name="moveScreenFlag">画面外に移動させるならtrue、画面内に移動させるならfalse</param>
	/// <param name="movePos">移動する量</param>
	void ProcessMoveScreen(bool moveScreenFlag,const Vector3 movePos);
	/// <summary>
	/// 画面外に移動
	/// </summary>
	/// <param name="movePos"></param>
	void MoveOffScreen(Vector3 movePos);
	/// <summary>
	/// 画面内に移動
	/// </summary>
	/// <param name="movePos"></param>
	void MoveOnScreen(Vector3 movePos);
	/// <summary>
	/// 武器を画面外に移動
	/// </summary>
	void MoveOutWeapon(Vector3 movePos);
	/// <summary>
	/// 武器を画面内に移動
	/// </summary>
	void MoveInWeapon(Vector3 movePos);

	/// <summary>
	/// 武器選択をリセットかゲームモードに切り替え
	/// </summary>
	void ProcessResetOrGoToPlay();

	/// <summary>
	/// 
	/// </summary>
	void FindAndSetMainWeaponInfo();

private:

	struct WeaponInfo
	{
		ModelRender m_weaponModel;//武器のモデルを格納する配列
		FontRender m_weaponNameFont;
		bool m_isSelect = false;		//選ばれたか
		EnSelectWeaponOrder m_weaponOrder=enSelectWeaponOrder_None;
	};

	WeaponInfo m_weaponInfo[enWeaponType_Num];

	WeaponInfo* m_moveWeapon=nullptr;

	std::vector<WeaponInfo*> m_canSelectWeapon;		//選べる武器
	std::vector<WeaponInfo*> m_chosenSelectWeapon;	//選ばれた武器

	EnCompleteState		m_enCompleteState = enCompleteState_OffScreenObject;

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


	int					m_nowSelectWeaponNumber = 0;		//現在選択している武器の種類(順番)


	bool				m_goToGameFlag = false;				//

	float				m_screenTimer = 0.0f;				//画面切り替え時のタイマー

};

