#pragma once


/// <summary>
/// 攻撃を与えるオブジェクトの基底クラス。
/// ボスが生成するダークボールなど
/// </summary>
class IAttackObject:public IGameObject
{
public:
	IAttackObject(){}
	virtual ~IAttackObject(){}

	/// <summary>
	/// 攻撃力の設定
	/// </summary>
	/// <param name="attack"></param>
	void SetAttack(int attack)
	{
		m_attack = attack;
	}
	/// <summary>
	/// 攻撃力の取得
	/// </summary>
	/// <returns></returns>
	const int& GetAttack() const
	{
		return m_attack;
	}

	/// <summary>
	/// 変動する攻撃力の取得
	/// </summary>
	/// <returns></returns>
	virtual const int& GetVariableAttack()
	{
		return 0;
	}

protected:

	/// <summary>
	/// 当たり判定の名前を取得
	/// </summary>
	/// <returns></returns>
	const char* GetCollisionName() const
	{
		return m_collisionName;
	}

protected:
	int m_attack = 0;	//攻撃力

	const char* m_collisionName = "attackobject";	//当たり判定を生成する時の名前

};

