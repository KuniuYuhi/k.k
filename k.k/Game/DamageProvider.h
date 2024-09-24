#pragma once
#include "IComponent.h"

#include "KnockBackInfo.h"
#include "WeaponInfo.h"



class CharacterBase;
class IMagicBall;

using namespace KnockBackInfo;
using namespace WeaponAttribute;

/// <summary>
/// ダメージプロバイダーの構造体
/// </summary>
namespace DamageInformaiton {

	struct DamageInfo
	{
		
		int attackId = -1;						//攻撃ID
		int attackPower = 0;					//攻撃力
		float knockBackTimeScale = 0.0f;			//ノックバックの時間間隔
		EnKnockBackPattern knockBackPattern;							//ノックバックパターン
		EnWeaponAttribute weaponAttribute = enWeaponAttribute_None;		//武器の属性
	};
}

using namespace DamageInformaiton;


/// <summary>
/// ダメージに関する情報を提供するクラス。コンポーネント
/// </summary>
class DamageProvider:public IComponent
{
public:

	DamageProvider();
	~DamageProvider();

	/// <summary>
	/// コンポーネントを追加した時の最初の処理
	/// </summary>
	void Start() override;

	/// <summary>
	/// 更新処理。自動で行ってくれる。消したいならGameObjectManagerへ
	/// </summary>
	void UpdateComponent() override;

	/// <summary>
	/// 提供者のキャラクターのインスタンスを設定
	/// </summary>
	/// <param name="characterBase"></param>
	void SetProviderCharacterInstance(CharacterBase* characterBase)
	{
		m_characterBase = characterBase;
	}
	/// <summary>
	/// 提供者の魔法球のインスタンスを設定
	/// </summary>
	/// <param name="magicBall"></param>
	void SetProviderMagicBallInstance(IMagicBall* magicBall)
	{
		m_magicBall = magicBall;
	}


	/// <summary>
	/// ダメージ提供者の座標を取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetProviderPostion();

	/// <summary>
	/// 提供者のダメージ情報を取得
	/// </summary>
	/// <returns></returns>
	DamageInfo GetProviderDamageInfo()
	{
		return m_damageInfo;
	}

	/// <summary>
	/// 攻撃IDを設定
	/// </summary>
	/// <param name="idNum"></param>
	void SetAttackId(int  idNum)
	{
		m_damageInfo.attackId = idNum;
	}
	/// <summary>
	/// 攻撃IDを取得
	/// </summary>
	/// <returns></returns>
	int GetAttackId()
	{
		return m_damageInfo.attackId;
	}

	/// <summary>
	/// ランダムに調整後のダメージを取得
	/// </summary>
	/// <returns></returns>
	int GetAdjustedDamage()
	{
		AdjustPowerRandomly();
		return m_damageInfo.attackPower;
	}


	/// <summary>
	/// ダメージ情報を設定
	/// </summary>
	/// <param name="attackId"></param>
	/// <param name="attackPower"></param>
	/// <param name="knockBackPattern"></param>
	/// <param name="weaponAttribute"></param>
	void SetDamageInfo(
		int attackId,
		int attackPower,
		float knockBackTimeScale,
		EnKnockBackPattern knockBackPattern,
		EnWeaponAttribute weaponAttribute = enWeaponAttribute_None
	)
	{
		m_damageInfo.attackId = attackId;
		//ランダムに変える
		m_damageInfo.attackPower = attackPower;
		//AdjustPowerRandomly();

		m_damageInfo.knockBackTimeScale = knockBackTimeScale;
		m_damageInfo.knockBackPattern = knockBackPattern;
		m_damageInfo.weaponAttribute = weaponAttribute;
	}

	/// <summary>
	/// 攻撃力をランダムに調整
	/// </summary>
	void AdjustPowerRandomly();

	/// <summary>
	/// 最終的なノックバックパターンを計算
	/// </summary>
	/// <param name="knockBackToleranceLevel">ノックバック耐性レベル</param>
	void CalcFinalKnockBackPattern(int knockBackToleranceLevel);

	/// <summary>
	/// ヒットした
	/// </summary>
	void Hit()
	{
		m_isHit = true;
	}

	/// <summary>
	/// ヒットしたか？
	/// </summary>
	/// <returns></returns>
	const bool IsHit() const
	{
		return m_isHit;
	}
	

	//todo　　情報を構造体にして渡したい
	//被ダメージ量、攻撃ID、ノックバックデータ

protected:
	
	CharacterBase* m_characterBase = nullptr;	//このコンポーネントを持っているキャラクター

	IMagicBall* m_magicBall = nullptr;		//このコンポーネントを持っているマジックボール

	DamageInfo m_damageInfo;


	bool m_isHit = false;

};

