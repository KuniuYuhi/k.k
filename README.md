# FANTASY BATTLE
河原電子ビジネス専門学校　
ゲームクリエイター科2年<br>
氏名：国宇雄飛<br>
## 紹介動画
動画<br>
## 目次
- [FANTASY BATTLE](#fantasy-battle)
  - [紹介動画](#紹介動画)
  - [目次](#目次)
  - [1.作品概要](#1作品概要)
  - [2.担当ソースコード](#2担当ソースコード)
  - [3.操作説明](#3操作説明)
  - [4.ゲーム説明](#4ゲーム説明)
    - [4.1 ゲーム詳細](#41-ゲーム詳細)
    - [4.2 タイトルについて](#42-タイトルについて)
    - [4.3 プレイヤーについて](#43-プレイヤーについて)
    - [4.4 武器について](#44-武器について)
    - [4.5 モンスターの挙動について](#45-モンスターの挙動について)
      - [1.モブモンスターの挙動](#1モブモンスターの挙動)
      - [2.ボスの挙動](#2ボスの挙動)
    - [4.6 フェーズについて](#46-フェーズについて)
  - [5.その他](#5その他)
    - [5.1.デプスシャドウ](#51デプスシャドウ)
    - [5.2.カスケードシャドウ](#52カスケードシャドウ)
    - [5.3.VSM](#53vsm)
    - [5.4.川瀬式ブルームフィルター](#54川瀬式ブルームフィルター)
    - [5.5.GBufferを使ったアウトラインの描画](#55gbufferを使ったアウトラインの描画)
    - [5.6.トゥーン調の表現](#56トゥーン調の表現)
      - [1.アウトラインの描画](#1アウトラインの描画)
      - [2.ランプシェーディング](#2ランプシェーディング)
    - [5.7.被写界深度](#57被写界深度)


## 1.作品概要
* タイトル 
  * FANTASY BATTLE
* 制作人数
  * 1人
* 製作期間
  * 2023年6月～1月
* ゲームジャンル
  * 3Dアクションゲーム
* プレイ人数
  * 1人
* 使用言語
  * C++
  * HLSL
* 使用ツール
  * Visual Studio 2022
  * Visual Studio Code
  * Adobe Photoshop 2024
  * 3ds Max 2023
  * Effekseer
  * GitHub
  * Fork
  * Notion
* 開発環境
  * 学校内製の簡易エンジン
  * Windows11

## 2.担当ソースコード
<details><summary>
  ゲーム部分
  </summary>
<details><summary>
  ゲームの進行
  </summary>

* BattlePhase.cpp
* BattlePhase.h
* BattleStart.cpp
* BattleStart.h
* BossStage1.cpp
* BossStage1.h
* EntryBoss.cpp
* EntryBoss.h
* Fade.cpp
* Fade.h
* Game.cpp
* Game.h
* GameCamera.cpp
* GameCamera.h
* GameFinishCamera.cpp
* GameFinishCamera.h
* GameUI.cpp
* GameUI.h
* Pause.cpp
* Pause.h
* Result.cpp
* Result.h
* Title.cpp
* Title.h
</details>

<details><summary>
  マネージャー
</summary>

* EventManager.cpp
* EventManager.h
* CharactersInfoManager.cpp
* CharactersInfoManager.h
* GameManager.cpp
* GameManager.h
* ManagerPreCompile.cpp
* ManagerPreCompile.h
* WeaponManager.cpp
* WeaponManager.h
</details>
<details><summary>
  プレイヤー
</summary>

* Actor.cpp
* Actor.h
* Player.cpp
* Player.h
<details><summary>
  Brave
</summary>

* Brave.cpp
* Brave.h
* BraveStateAttack_1.cpp
* BraveStateAttack_1.h
* BraveStateAttack_2.cpp
* BraveStateAttack_2.h
* BraveStateAttack_3.cpp
* BraveStateAttack_3.h
* BraveStateChangeSwordShield.cpp
* BraveStateChangeSwordShield.h
* BraveStateDefend.cpp
* BraveStateDefend.h
* BraveStateDefendHit.cpp
* BraveStateDefendHit.h
* BraveStateDie.cpp
* BraveStateDie.h
* BraveStateHit.cpp
* BraveStateHit.h
* BraveStateIdle.cpp
* BraveStateIdle.h
* BraveStateKnockBack.cpp
* BraveStateKnockBack.h
* BraveStateSkill_Main.cpp
* BraveStateSkill_Main.h
* BraveStateSkill_Start.cpp
* BraveStateSkill_Start.h
* BraveStateSprint.cpp
* BraveStateSprint.h
* BraveStateWin_Main.cpp
* BraveStateWin_Main.h
* BraveStateWin_Start.cpp
* BraveStateWin_Start.h
* IBraveState.cpp
* IBraveState.h
</details>

</details>
<details><summary>
  武器
</summary>

* Arrow.cpp
* Arrow.h
* BigSword.cpp
* BigSword.h
* Bow.cpp
* Bow.h
* IWeapon.cpp
* IWeapon.h
* MyWeapon.h
* SwordShield.cpp
* SwordShield.h
* WeaponBase.cpp
* WeaponBase.h
</details>

<details><summary>
  モンスター
</summary>

* AIActor
* AIActor
* Boss
* Boss
* BossBase
* BossBase
* IMobStateMachine
* IMobStateMachine
* MobMonster
* MobMonster
* MobMonsterActionList
* MonsterBase
* MonsterBase
<details><summary>
  Cactus
</summary>

* ICactusState.h
* Cactus.cpp
* Cactus.h
* CactusStateAppear.cpp
* CactusStateAppear.h
* CactusStateAttack_1.cpp
* CactusStateAttack_1.h
* CactusStateChase.cpp
* CactusStateChase.h
* CactusStateDamage.cpp
* CactusStateDamage.h
* CactusStateDie.cpp
* CactusStateDie.h
* CactusStateIdle.cpp
* CactusStateIdle.h
* CactusStatePatrol.cpp
* CactusStatePatrol.h
* CactusStateSkill.cpp
* CactusStateSkill.h
* CactusStateVictory.cpp
* CactusStateVictory.h
</details>
<details><summary>
  Mushroom
</summary>

* IMushroomState.h
* Mushroom.cpp
* Mushroom.h
* MushroomStateAppear.cpp
* MushroomStateAppear.h
* MushroomStateAttack_1.cpp
* MushroomStateAttack_1.h
* MushroomStateChase.cpp
* MushroomStateChase.h
* MushroomStateDamage.cpp
* MushroomStateDamage.h
* MushroomStateDie.cpp
* MushroomStateDie.h
* MushroomStateIdle.cpp
* MushroomStateIdle.h
* MushroomStatePatrol.cpp
* MushroomStatePatrol.h
* MushroomStateSkill.cpp
* MushroomStateSkill.h
* MushroomStateVictory.cpp
* MushroomStateVictory.h
</details>

<details><summary>
  Slime
</summary>

* ISlimeState.h
* Slime.cpp
* Slime.h
* SlimeStateAppear.cpp
* SlimeStateAppear.h
* SlimeStateAttack.cpp
* SlimeStateAttack.h
* SlimeStateChase.cpp
* SlimeStateChase.h
* SlimeStateDamage.cpp
* SlimeStateDamage.h
* SlimeStateDie.cpp
* SlimeStateDie.h
* SlimeStateIdle.cpp
* SlimeStateIdle.h
* SlimeStatePatrol.cpp
* SlimeStatePatrol.h
* SlimeStateSkill.cpp
* SlimeStateSkill.h
* SlimeStateVictory.cpp
* SlimeStateVictory.h
</details>

<details><summary>
  Summoner
</summary>

* ISummonerState.h
* Summoner.cpp
* Summoner.h
* SummonerActionList.h
* SummonerState_Angry.cpp
* SummonerState_Angry.h
* SummonerState_DarkBall.cpp
* SummonerState_DarkBall.h
* SummonerState_DarkSpearEnd.cpp
* SummonerState_DarkSpearEnd.h
* SummonerState_DarkSpearMain.cpp
* SummonerState_DarkSpearMain.h
* SummonerState_DarkSpearStart.cpp
* SummonerState_DarkSpearStart.h
* SummonerState_DarkWall.cpp
* SummonerState_DarkWall.h
* SummonerState_Die.cpp
* SummonerState_Die.h
* SummonerState_DMeteo_Main.cpp
* SummonerState_DMeteo_Main.h
* SummonerState_DMeteo_Start.cpp
* SummonerState_DMeteo_Start.h
* SummonerState_Hit.cpp
* SummonerState_Hit.h
* SummonerState_Idle.cpp
* SummonerState_Idle.h
* SummonerState_KnockBack.cpp
* SummonerState_KnockBack.h
* SummonerState_NAttack_1.cpp
* SummonerState_NAttack_1.h
* SummonerState_NAttack_2.cpp
* SummonerState_NAttack_2.h
* SummonerState_NAttack_3.cpp
* SummonerState_NAttack_3.h
* SummonerState_Summon.cpp
* SummonerState_Summon.h
* SummonerState_Victory.cpp
* SummonerState_Victory.h
* SummonerState_Walk.cpp
* SummonerState_Walk.h
* SummonerState_Warp.cpp
* SummonerState_Warp.h
* SummonerStateEat.cpp
* SummonerStateEat.h
</details>

<details><summary>
  TurtleShell
</summary>

* ITurtleShellState.h
* TurtleShell.cpp
* TurtleShell.h
* TurtleShellStateAppear.cpp
* TurtleShellStateAppear.h
* TurtleShellStateAttack_1.cpp
* TurtleShellStateAttack_1.h
* TurtleShellStateChase.cpp
* TurtleShellStateChase.h
* TurtleShellStateDamage.cpp
* TurtleShellStateDamage.h
* TurtleShellStateDie.cpp
* TurtleShellStateDie.h
* TurtleShellStateIdle.cpp
* TurtleShellStateIdle.h
* TurtleShellStatePatrol.cpp
* TurtleShellStatePatrol.h
* TurtleShellStateSkill.cpp
* TurtleShellStateSkill.h
* TurtleShellStateVictory.cpp
* TurtleShellStateVictory.h
</details>

</details>

<details><summary>
  その他
</summary>

* DamageFont.cpp
* DamageFont.h
* DarkBall.cpp
* DarkBall.h
* DarkMeteo.cpp
* DarkMeteo.h
* DarkSpear.cpp
* DarkSpear.h
* DarkSpearObj.cpp
* DarkSpearObj.h
* DarkWall.cpp
* DarkWall.h
* DecisionMeteoEndpoint.h
* HitDetection.cpp
* HitDetection.h
* InitEffect.cpp
* InitEffect.h
* KnockBack.cpp
* KnockBack.h
* MagicBall.cpp
* MagicBall.h
* Meteo.cpp
* Meteo.h
* SoundFile.cpp
* SoundFile.h
* Status.cpp
* Status.h
</details>

</details>

## 3.操作説明



## 4.ゲーム説明

### 4.1 ゲーム詳細
このゲームは、制限時間内にステージのボスを倒すとゲームクリアになります。  
プレイヤーは、ソード＆シールド、グレイトソード、ボウ＆アローの3つの武器を使い分けて戦います。  
バトルにはフェーズが存在し、一定時間ごとにフェーズが進み、敵を生成します。フェーズが進行すると、前のフェーズに存在したモブモンスターは消滅します。第4フェーズに進行すると、モブモンスターが出現しない時間となりフェーズ終了後、第1フェーズに戻り以降ループします。
#### [目次に戻る](#目次)

### 4.2 タイトルについて

タイトルを3D
また、手前の主人公に注目してもらいたいため、[被写界深度](#57被写界深度)を利用しました。  
バトルを始めるときに主人公が崖から飛び降りる演出を入れることで、


#### [目次に戻る](#目次)

### 4.3 プレイヤーについて
プレイヤーの状態はステートパターンを利用してステートで管理しています。また、ポリモーフィズムを利用することで同じ変数で違う処理を行えるようにし、行動の追加を容易にしました。
同じステートが武器の数だけあると、状態遷移する際に武器の違いによる処理が複雑になるため、全ての武器で同じステートを利用できるようにしました。  
```c++
// アニメーションクリップの番号を表す列挙型。
enum EnAnimationClip {
	enAnimClip_Idle,
	enAnimClip_Sprint,
        ・
        ・
        ・
	enAnimClip_Num
};

//武器のアニメーションクリップの最初の番号
const int m_mainWeaponAnimationStartIndexNo = 0;						//メイン武器のアニメーションクリップの最初の番号
const int m_subWeaponAnimationStartIndexNo = enAnimClip_Num * 1;		//サブ武器のアニメーションクリップの最初の番号
const int m_subWeapon2AnimationStartIndexNo = enAnimClip_Num * 2;		//サブ２武器のアニメーションクリップの最初の番号

AnimationClip	m_animationClip[enAnimClip_Num * AnimationClipGroup_Num];// アニメーションクリップ。アニメーションの数×武器の数。
```  
アニメーションクリップの総数に武器のアニメーションクリップの最初の番号を足してアニメーションをロードしました。
```c++
/// <summary>
/// ソード＆シールドのアニメーションクリップのロード
/// </summary>
/// <param name="mainWeaponAnimationStartIndexNo">アニメーションクリップの最初の番号</param>
void Brave::RoadOneHandSwordAnimationClip(int mainWeaponAnimationStartIndexNo)
{
  // 片手剣のアニメーションクリップをロードするためのペアクラス
  const std::pair<const char*, bool> oneHandedSwordAnimClipFilePaths[] = {  {"Assets/animData/character/Player/OneHandSword/Idle.tka",true},  {"Assets/animData/character/Player/OneHandSword/Sprint.tka",true},
          ・
          ・
          ・
{"None",false}
  };
  //設定したoneHandedSwordAnimClipFilePathsを使ってアニメーションクリップをロード、ループフラグを設定
  //enAnimClip_Numはアニメーションクリップの数
  //mainWeaponAnimationStartIndexNoは武器のアニメーションクリップの最初の番号
  for (int i = 0; i < enAnimClip_Num; i++) {
	  m_animationClip[mainWeaponAnimationStartIndexNo + i].Load(oneHandedSwordAnimClipFilePaths[i].first);
	  m_animationClip[mainWeaponAnimationStartIndexNo + i].SetLoopFlag(oneHandedSwordAnimClipFilePaths[i].second);
  }
}
```
この関数が武器ごとに存在しています。  
最終的にm_animationClip[]にはこのように格納されます。  
```c++
m_animationClip[]={
  ソード＆シールドの時のアニメーション×enAnimClip_Num,
  グレイトソードの時のアニメーション×enAnimClip_Num,
  ボウ＆アローの時のアニメーション×enAnimClip_Num,
};
```
アニメーションを再生する際に、再生したいアニメーションクリップの番号と現在の武器のアニメーションクリップの最初の番号を足すことで、武器ごとに使うステートを変えずにアニメーションを再生できるようにしました。
```c++
void BraveStateIdle::PlayAnimation()
{
  //再生したいアニメーションクリップと現在の武器のアニメーションクリップの最初の番号を足す
	m_brave->GetModelRender().PlayAnimation(
		Brave::enAnimClip_Idle + m_brave->GetCurrentMainWeaponAnimationStartIndexNo(),
		0.4f
	);
}
```

#### [目次に戻る](#目次)

### 4.4 武器について

武器の座標、回転は、主人公のモデルの手のボーンからワールド座標を取得して設定しています。ですが、主人公の向きによって回転の仕方がおかしくなっていました。  
動画  
この時点でのワールド座標の設定
```c++
/// <summary>
/// 行列を設定。
/// </summary>
/// <param name="rotation">行列。</param>
void SetWorldMatrix(const Matrix& matrix)
{
	Vector3 position;
	position.x = matrix.m[3][0];
	position.y = matrix.m[3][1];
	position.z = matrix.m[3][2];
	SetPosition(position);
	Quaternion rotation;
	rotation.SetRotation(matrix);
	SetRotation(rotation);
}
```
原因は,rotation.SetRotation(matrix)で回転を設定する際のmatrixの回転成分が正規化されたものではないからでした。回転成分と拡大縮小行列は混じっているので、
取得するボーンのrootボーンの拡大率が変更されていて、


<div style="text-align: right;">

#### [目次に戻る](#目次)
</div>

### 4.5 モンスターの挙動について
モンスターの状態も主人公と同様ステートパターンで管理しています。
そして、モンスターの行動を管理、調整、追加をしやすくするために、階層化ステートマシンを作成しました。
#### 1.モブモンスターの挙動
モブモンスターは「追跡」と「巡回」の2つのステートマシンを作成しました。また、ステートマシン側での変更をなくすこと、で新しいモブモンスターの作成を容易にしました。
画像

<div style="text-align: right;">

#### [目次に戻る](#目次)
</div>
#### 2.ボスの挙動
ボスは基本的に移動しないので、「攻撃」と「警戒」の2つのステートマシンを作成しました。また、ボスの攻撃の行動パターンを増やしたり、行動の決め方を細かくすることで、プレイヤーが



<div style="text-align: right;">

#### [目次に戻る](#目次)
</div>

### 4.6 フェーズについて

<div style="text-align: right;">

#### [目次に戻る](#目次)
</div>

## 5.その他

### 5.1.デプスシャドウ

<div style="text-align: right;">

#### [目次に戻る](#目次)
</div>

### 5.2.カスケードシャドウ
### 5.3.VSM
デプスシャドウを実装しましたが、影の境界線がジャギーになっていて見栄えがあまり良くなかったので、ソフトシャドウを実装しました。また、ソフトシャドウの中でも品質の高いVSM(Variance Shadow Maps)を実装しました。VSMとはシャドウマップに書き込まれた**深度値の局所的な分散**を利用してソフトシャドウを実現するアルゴリズムです。
<img src="README_IMAGE/VSM.png" width="500" alt="VSM">  

1.シャドウマップにシャドウライトから見たピクセルまでの距離(深度値)と、先ほどの深度値の2乗を書き込みます。深度値を2乗した値は分散を計算するときに使われます。
```h
 float depth = psIn.pos.z ;
 float pos = exp(INFINITY * depth);
 return float4(pos, pos * pos, 0.0f, 1.0f);
```
2.作成したシャドウマップにガウシアンブラーをかけたテクスチャを新しいシャドウマップとして利用します(カスケードシャドウのため、シャドウがマップ3つになります)。  
<img src="README_IMAGE/blur_near.png" width="200" alt="近影のブラー"> 
<img src="README_IMAGE/blur_center.png" width="200" alt="中影のブラー"> 
<img src="README_IMAGE/blur_far.png" width="200" alt="遠影のブラー"> 

3.チェビシェフの不等式を使用して光が当たる確率を求めます。
```h
//moments...シャドウマップからサンプリングした値
//depth...ピクセルの深度値

// 遮蔽されているなら、チェビシェフの不等式を利用して光が当たる確率を求める
float depth_sq = moments.x * moments.x;
// このグループの分散具合を求める
// 分散が大きいほど、varianceの数値は大きくなる
float variance = moments.y-depth_sq;
//このピクセルのライトから見た深度値とシャドウマップの平均の深度値の差を求める 
float md  = depth - moments.x;
// 光が届く確率を求める
float lig_factor = variance / (variance + md * md);
float lig_factor_min = 0.3f;
// 光が届く確率の下限以下は影になるようにする。
lig_factor = saturate((lig_factor - lig_factor_min) / (1.0f - lig_factor_min));
// 光が届く確率から影になる確率を求める。影の濃さ
return (1.0f - lig_factor);
```
4.影の濃さをライティングしたカラーに乗算する。
```h
 albedoColor.xyz *= (1.0f - shadow);
```


<div style="text-align: right;">

#### [目次に戻る](#目次)
</div>

### 5.4.川瀬式ブルームフィルター
### 5.5.GBufferを使ったアウトラインの描画
### 5.6.トゥーン調の表現
  #### 1.アウトラインの描画
  #### 2.ランプシェーディング
### 5.7.被写界深度
