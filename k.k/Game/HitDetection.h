#pragma once

//相手に攻撃をヒットさせられるかの判定を行う
//多段ヒットする攻撃で使う
struct HitDetection
{
private:
	float timer = 0.0f;			//タイマー
	bool hittableFlag = false;

public:
	/// <summary>
	/// 多段ヒット可能か
	/// </summary>
	/// <param name="hittableTime">ヒット可能になる制限時間</param>
	/// <param name="hittableFlag">ヒット可能か結果をほぞんするフラグ</param>
	/// <returns>ヒット可能でtrue、不可能でfalse</returns>
	bool IsHittable(const float hittableTime);

	/// <summary>
	/// 多段ヒット可能かフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetHittableFlag(bool flag)
	{
		hittableFlag = flag;
	}
	/// <summary>
	/// 多段ヒット可能かフラグを取得
	/// </summary>
	/// <returns></returns>
	const bool& GetHittableFlag() const
	{
		return hittableFlag;
	}
};

