#pragma once
#include "ICharacter.h"
#include "DamageFont.h"
#include "DamageProvider.h"

/// <summary>
/// キャラクターのベースクラス
/// キャラクターで共通する処理を実装する
/// </summary>
class CharacterBase : public ICharacter//,DamageProvider
{
public:
	
	/// <summary>
	/// 現在の攻撃力を取得
	/// </summary>
	/// <returns></returns>
	virtual int GetCurrentPower() { return 0; }
	
	

	/// <summary>
	/// キャラコンの作成
	/// </summary>
	void CreateCharacterController();

	/// <summary>
	/// 移動量を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetMoveSpeed()
	{


		return m_moveSpeed;
	}

	/// <summary>
	/// キャラコンを取得
	/// </summary>
	/// <returns></returns>
	CharacterController* GetCharaCon()
	{
		return m_charaCon.get();
	}

	/// <summary>
	/// 回転方向を設定
	/// </summary>
	/// <param name="direction"></param>
	void SetRotateDirection(Vector3 direction);

	/// <summary>
	/// ダメージフォントを作成
	/// </summary>
	/// <param name="hitDamage">表示するダメージ</param>
	/// <param name="hitActor">誰がダメージを受けるのか</param>
	void CreateDamageFont(int hitDamage, DamageFont::EnDamageActor hitActor);


	/// <summary>
	/// 死亡フラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetDieFlag(bool flag)
	{
		m_isDieFlag = flag;
	}
	/// <summary>
	/// 死亡したか？
	/// </summary>
	/// <returns></returns>
	bool IsDie()
	{
		return m_isDieFlag;
	}





protected:

	/// <summary>
	/// 基本的なコンポーネントをセッティング
	/// </summary>
	virtual void SettingDefaultComponent(){}

	
	

protected:
	
	//CharacterController m_charaCon;		//キャラクターコントローラー
	
	std::unique_ptr<CharacterController> m_charaCon = nullptr;


	Vector3 m_moveSpeed = Vector3::Zero;		//移動量

	Vector3 m_rotateDirection = Vector3::Zero;	//回転する方向ベクトル


	bool m_isDieFlag = false;				//死亡フラグ


};

