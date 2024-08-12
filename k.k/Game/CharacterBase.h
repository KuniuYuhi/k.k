#pragma once
#include "ICharacter.h"
#include "Damageable.h"

/// <summary>
/// キャラクターのベースクラス
/// キャラクターで共通する処理を実装する
/// </summary>
class CharacterBase : public ICharacter,Damageable
{
public:
	//virtual ~CharacterBase() = default;

	/// <summary>
	/// 被ダメージ処理
	/// </summary>
	virtual void Damage() override {}

	/// <summary>
	/// 死亡時処理
	/// </summary>
	virtual void Dead() override {}
	
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

};

