#pragma once

/// <summary>
/// キャラクターの前方向を計算するクラス
/// </summary>
class CalcCharacterForward
{
public:


	/// <summary>
	/// 一番近いモンスターに向かうベクトルを前方向にして取得
	/// </summary>
	/// <param name="charPosition">キャラクターの座標</param>
	/// <param name="forward">前方向</param>
	/// <param name="moveSpeed">移動方向</param>
	/// <param name="chaseDistance">追いかける距離(範囲)</param>
	void CalcForwardOfNearMonster(
		Vector3 charPosition,
		Vector3& forward,
		Vector3& moveSpeed,
		float chaseDistance
	);

private:

	/// <summary>
	/// モンスターの座標をリストに追加。
	/// </summary>
	void PushBackMonsterPositionList();
	
	/// ベクトルを作成
	/// </summary>
	/// <param name="startPosition">ベクトルの始点</param>
	/// <param name="endPosition">ベクトルの終点</param>
	/// <param name="isNormalizeFlag">正規化するかのフラグ。デフォルトはtrue</param>
	/// <returns></returns>
	Vector3 GetCreateVector(
		Vector3 startPosition, 
		Vector3 endPosition,
		bool isNormalizeFlag = true
	);


private:

	std::vector<Vector3> m_monstersPosition;

	Vector3 monsterPos;

};

