#include "stdafx.h"
#include "CalcCharacterForward.h"

#include "CharactersInfoManager.h"
#include "Boss.h"

void CalcCharacterForward::CalcForwardOfNearMonster(
    Vector3 charPosition, 
    Vector3& forward, 
    Vector3& moveSpeed, 
    float chaseDistance
)
{
    //まずモンスターのリストクリア
    m_monstersPosition.clear();

    //モンスターの座標をリストに追加
    for (auto monster : CharactersInfoManager::GetInstance()->GetMobMonsters())
    {
        //モンスターの座標を取得
        Vector3 monsterPos = monster->GetPosition();
        //リストに追加
        m_monstersPosition.emplace_back(monsterPos);
    }

    //ボスの座標をリストに追加
    Vector3 bossPos = CharactersInfoManager::GetInstance()->
        GetBossInstance()->GetPosition();
    //リストに追加
    m_monstersPosition.emplace_back(bossPos);

    //仮の前方向
    Vector3 temporaryForward = forward;
    //追跡距離内カウント
    int count = 0;

    for (Vector3 monsterPosition : m_monstersPosition)
    {
        //プレイヤーからモンスターに向かうベクトルを計算
        Vector3 diff = monsterPosition - charPosition;
        //ベクトルの長さが追跡距離内だったら
        if (diff.Length() < chaseDistance)
        {
            temporaryForward = diff;
            count++;
        }
    }

    //何にもないならforawrdいじらない

    //前方向を正規化
    temporaryForward.Normalize();

    Vector3 forwardVector = charPosition;
    forwardVector.y = 0.0f;
    forwardVector.Add(forward * 20.0f);
    //自身の座標から前方向に向かうベクトル
    Vector3 forwardDiff = forwardVector - charPosition;
    //正規化
    forwardDiff.Normalize();

    

    Vector3 moveSpeedVector = charPosition;
    moveSpeedVector.y = 0.0f;
    moveSpeedVector.Add(moveSpeed);
    //自身の座標から移動方向に向かうベクトル
    Vector3 moveSpeedDiff = moveSpeedVector - charPosition;
    //正規化
    moveSpeedDiff.Normalize();

    //前方向と敵に向かう前方向の内積を求める
    float t1 = Dot(temporaryForward, forwardDiff);
   
    //前方向と移動方向の内積を求める
    float t2 = Dot(moveSpeedDiff, forwardDiff);



    //敵の方向を調べる
    //前方向を調べる
    // 
    //移動方向を調べる  
    //移動方向と前方向の内積の相似0.0以上なら移動方向を前方向にする
    // 
    // 移動方向と敵の方向がかなり似ていたら、前方向を敵の方向に書き換える
    // 
    //敵の方向と前方向の内積の相似が0.0以上なら前方向を変更！！


    // //X方向で直角のベクトル
    //Vector3 rightangleX = forwardDiff;
    ////rightangleX *= -1.0f;
    //float X = rightangleX.x;
    //rightangleX.x = rightangleX.z;
    //rightangleX.z = X;
    //rightangleX.y = 0.0f;

    //rightangleX.x *= moveSpeedDiff.x;
   
    //rightangleX.z *= moveSpeedDiff.z;

    //forward = rightangleX;
    //forward.Normalize();

    //return;

    //移動方向の入力がないなら敵の方向を優先する
    if (fabsf(moveSpeed.x) < 0.1f && fabsf(moveSpeed.z) < 0.1f)
    {
        //敵に向かうベクトルが真横までなら
        if (t1 >= 0.0f)
        {
            //前方向を正規化
            temporaryForward.Normalize();
            forward = temporaryForward;
        }

        return;
    }

    //移動の入力があったら

    //単位ベクトル同士の内積は-1から1の範囲。1になるほど似ている
    //内積が0.0f以上なら前方向を変更する
    if (t2 >= 0.0f)
    {
        //敵を一体も見つけていなかったら
        if (count == 0)
        {
            forward = moveSpeed;
            forward.Normalize();
            return;
        }

        //移動方向の内積と敵に向かう内積がかなり似ていたら、
        //前方向を敵に向かうベクトルに書き換える
        if (abs(t1 - t2) < 0.5)
        {
            //前方向を正規化
            temporaryForward.Normalize();
            forward = temporaryForward;
        }
        else
        {
            //似ていなかったら引数の移動方向を前方向に設定
            forward = moveSpeed;
            forward.Normalize();
            return;
        }
    }
    else
    {
        //ベクトルが敵に向ける範囲を超えているので
        //真横のベクトルにする

        

        //正の数の時は正常、負の時は逆二回転

         //前方向から入力方向に向かって90度のベクトルを作成
        Vector3 rightangleX = forwardDiff;
        //rightangleX *= -1.0f;

        float X = rightangleX.x;
        rightangleX.x = rightangleX.z;
        rightangleX.z = X;
        rightangleX.y = 0.0f;

        Quaternion rot90 = g_quatIdentity;

        rot90.SetRotationYFromDirectionXZ(forwardDiff);
        rot90.AddRotationDegY(90.0f);
        rot90.Apply(rightangleX);

        //前方向と入力方向の内積でなす角を求める3.14がmax
        float kakudo = Dot(forwardDiff, moveSpeedDiff);
        kakudo = acos(kakudo);
        //マイナスかプラスかで判断

        //if (kakudo > 2.0f)
        //{
        //    //反転させる
        //    rightangleX *= -1.0f;
        //}


        //if (moveSpeedDiff.x > moveSpeedDiff.z)
        //{
        //    rightangleX.z *= -1.0f;
        //    //rightangleX.z *= moveSpeedDiff.z;
        //}
        //else
        //{
        //    rightangleX.z *= -1.0f;
        //    //rightangleX.x *= moveSpeedDiff.x;
        //}

       

        forward = rightangleX;
        forward.Normalize();

        return;
    }

  
   

  

}
