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
    //前方向の正規化
    forward.Normalize();
    //モンスターの座標をリストに追加
    PushBackMonsterPositionList();

    //ボスの座標を取得
    Vector3 bossPos = CharactersInfoManager::GetInstance()->
        GetBossInstance()->GetPosition();
   
    //キャラクターからボスに向かうベクトルを取得
    Vector3 bossToCharacterVector = GetCreateVector(charPosition, bossPos,false);

    

    //moveSpeedに相似性の高いベクトルを保存するベクトル
    Vector3 nearMoveSpeedVector = moveSpeed;

    //キャラクターから移動方向に向かうベクトルを作成
    Vector3 newMoveSpeed = moveSpeed;
    newMoveSpeed.y = 0.0f;
    newMoveSpeed.Add(moveSpeed);
    //キャラクターから移動方向に向かうベクトルを取得
    Vector3 moveSpeedToCharacterVector = GetCreateVector(charPosition, newMoveSpeed);
    

    //キャラクターからモンスターに向かうベクトル。初期値は移動方向
    Vector3 monsterToCharcterVector = forward;
    //追跡距離内の敵の数のカウント
    int count = 0;
    //一番近いキャラクターからモンスターに向かうベクトルを調べる
    for (Vector3 monsterPosition : m_monstersPosition)
    {
        //キャラクターからモンスターに向かうベクトルを計算
        Vector3 diff = monsterPosition - charPosition;
        //ベクトルの長さが追跡距離内だったら
        if (diff.Length() < chaseDistance)
        {
            //一番近いモンスターに向かうベクトルになる
            monsterToCharcterVector = diff;
            //正規化
            diff.Normalize();

            //移動方向と敵に向かうベクトルの相似性を調べる
            float d = Dot(moveSpeedToCharacterVector,diff);
            //相似性が0.5以上なら
            if (d >= 0.5f)
            {
                //移動方向と相似性の高いベクトルに設定
                nearMoveSpeedVector = diff;
            }
            //追跡距離内の敵の数のカウント加算
            count++;
        }
    }

    //前方向ベクトルの設定
    Vector3 newForward = charPosition;
    newForward.y = 0.0f;
    newForward.Add(forward * 20.0f);
    //キャラクターから前方向に向かうベクトルを取得
    Vector3 forwardToCharacterVector = GetCreateVector(charPosition, newForward);
  
    //前方向と敵に向かう前方向の内積を求める
    float t1 = Dot(monsterToCharcterVector, forwardToCharacterVector);
   
    //前方向と移動方向の内積を求める
    float t2 = Dot(moveSpeedToCharacterVector, forwardToCharacterVector);

    //敵に向かうベクトルと移動方向の内積を求める
    float t3 = Dot(nearMoveSpeedVector, moveSpeedToCharacterVector);

    //移動方向の入力がないなら敵の方向を優先する
    if (fabsf(moveSpeed.x) < 0.1f && fabsf(moveSpeed.z) < 0.1f)
    {
        //入力がないかつ真横までに敵がいたら
        // 前方向を敵に向かうベクトルに変更
        if (t1 >= 0.0f)
        {
            //ボスに向かうベクトルの長さが追跡距離内なら
            if (bossToCharacterVector.Length() < chaseDistance)
            {
                //正規化
                bossToCharacterVector.Normalize();
                //前方向をボスに向かうベクトルに設定
                forward = bossToCharacterVector;
            }
            else
            {
                //モンスターに向かうベクトルを正規化
                monsterToCharcterVector.Normalize();
                //前方向をモンスターに向かうベクトルに設定。
                //モンスターが追跡距離内にいなかったら引数の前方向になる
                forward = monsterToCharcterVector;
            }
        }
        //終了
        return;
    }


    //一番近い敵に向かうベクトルと移動方向の内積がかなり似ているかつ
    //敵が近くにいるカウントが１以上なら
    //前方向をmoveSpeedに相似性の高いベクトルに書き換える
    if (t3 >= 0.5 && count > 0)
    {
        //正規化
        nearMoveSpeedVector.Normalize();
        //前方向をmoveSpeedに相似性の高いベクトルに設定
        forward = nearMoveSpeedVector;
        //終了
        return;
    }
    else
    {
        //似ていなかったら引数の移動方向を前方向に設定
        forward = moveSpeed;
        forward.Normalize();
        //終了
        return;
    }


    //移動の入力があったら
    //単位ベクトル同士の内積は-1から1の範囲。1になるほど似ている
    //内積が0.0f以上なら前方向を変更する
    
    //前方向と移動方向のベクトルの相似性が0.0ｆより大きいなら
    if (t2 >= 0.0f)
    {
        
    }
    else
    {
        //ベクトルが敵に向ける範囲を超えているので
        //真横のベクトルにする

        

        //正の数の時は正常、負の時は逆二回転

         //前方向から入力方向に向かって90度のベクトルを作成
        Vector3 rightangleX = forwardToCharacterVector;
        //rightangleX *= -1.0f;

        float X = rightangleX.x;
        rightangleX.x = rightangleX.z;
        rightangleX.z = X;
        rightangleX.y = 0.0f;

        Quaternion rot90 = g_quatIdentity;

        rot90.SetRotationYFromDirectionXZ(forwardToCharacterVector);
        rot90.AddRotationDegY(90.0f);
        rot90.Apply(rightangleX);

        //前方向と入力方向の内積でなす角を求める3.14がmax
        float kakudo = Dot(forwardToCharacterVector, moveSpeedToCharacterVector);
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

       
        //出来てない
        forward = rightangleX;
        forward.Normalize();


        //似ていなかったら引数の移動方向を前方向に設定
        forward = moveSpeed;
        forward.Normalize();

        return;
    }

  
   

  

}

void CalcCharacterForward::PushBackMonsterPositionList()
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
}

Vector3 CalcCharacterForward::GetCreateVector(
    Vector3 startPosition, Vector3 endPosition, bool isNormalizeFlag)
{
    //始点から終点に向かうベクトルを計算
    Vector3 diff = endPosition - startPosition;

    //正規化フラグがセットされているなら
    if (isNormalizeFlag == true)
    {
        //正規化
        diff.Normalize();
    }

    return diff;
}
