#pragma once

namespace SummonerActions {
	/// <summary>
	/// アクションの名前
	/// </summary>
	enum EnActionName
	{
		enActionName_Idle,			//待機
		enActionName_Walk,			//歩き
		enActionName_Darkball,		//ダークボール(遠距離攻撃)
		enActionName_DarkWall,		//ダークウォール(近距離攻撃)
		enActionName_KnockBack,		//ノックバック攻撃(近距離攻撃)
		enActionName_NormalAttack_1,//通常攻撃１(近距離攻撃)
		enActionName_NormalAttack_2,//通常攻撃２(近距離攻撃)
		enActionName_NormalAttack_3,//通常攻撃３(近距離攻撃)
		enActionName_DarkMeteo_Start,//ダークメテオスタート
		enActionName_DarkMeteo_Main,//ダークメテオメイン
		enActionName_Command,		//モブに号令をかける
		enActionName_Summon,		//召喚
		enActionName_CriticalHit,	//スーパーアーマー割られた
		enActionName_Die,			//やられた
		enActionName_Victory,		//勝利
		enActionName_Angry,			//怒りモード
		enActionName_Warp,			//ワープ(逃げる)
	};
}


