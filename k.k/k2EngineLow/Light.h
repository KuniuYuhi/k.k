#pragma once

#include "MyRenderer.h"

namespace nsK2EngineLow {

	
	/// <summary>
	/// ディレクションライト構造体
	/// </summary>
	struct DirectionalLight
	{
		Vector3 lightDirection;		//ライトの方向
		float pad0;					//パティング
		Vector3 color;				//ライトのカラー
		float pad1;					//パティング
		
	};

	/// <summary>
	/// ポイントライト構造体
	/// </summary>
	struct PointLight
	{
		Vector3 position;		//座標
		float pad0;				//パティング0
		Vector3 color;			//カラー
		float pad1;				//パティング1
		Vector3 attn;			//xに影響範囲、yに影響率に累乗するパラメータ
		int	isUse = false;		//使用中フラグ

	};

	/// <summary>
	/// スポットライト構造体
	/// </summary>
	struct SpotLight
	{
		Vector3 position;		//座標
		float pad0;				//パティング0
		Vector3 angle;			//xに射出角度、yに影響に累乗するパラメータ
		float pad1;				//パティング1
		Vector3 color;			//カラー
		float pad2;				//パティング2
		Vector3 attn;			//xに影響範囲、yに影響率に累乗するパラメータ
		int	isUse = false;		//使用中フラグ
		Vector3 direction;		//照射方向
		float pad3;				//パティング3
	};

	/// <summary>
	/// 半球ライト
	/// </summary>
	struct HemiSphereLight
	{
		Vector3 groundColor;		//地面色
		float pad0;					//パティング0
		Vector3 skyColor;			//天球色
		int isUse = false;			//使用フラグ
		Vector3 groundNormal;		//地面の法線
		float pad1;
	};

	/// <summary>
	/// ライト構造体
	/// </summary>
	struct Light
	{
		DirectionalLight	directionalLight;		//ディレクションライト
		PointLight			pointLight;				//ポイントライト
		SpotLight			spotLight;				//スポットライト				
		HemiSphereLight		hemiSphereLight;		//半球ライト
		Vector3             ambientLight;			//環境光(アンビエントライト)
		float				pad1;					//パティング１
		Vector3				cameraEyePos;			//カメラの座標
		float				pad2;
		Matrix				mLVP[NUM_SHADOW_MAP];					//ライトビュープロジェクション行列
	};


	class SceneLight
	{
	public:
		void Init();
		Light& GetSceneLight()
		{
			return m_light;
		}

		/// <summary>
		/// ライトビュープロジェクション行列を設定する
		/// </summary>
		/// <param name="LVP">ライトビュープロジェクション行列</param>
		void SetmLVP(Matrix LVP)
		{
			for (int mapNo = 0; mapNo < NUM_SHADOW_MAP; mapNo++)
			{
				m_light.mLVP[mapNo] = LVP;
			}
			
		}
		//////////////////////////////////////////////////////////////////////
		///ディレクションライトの関数
		////////////////////////////////////////////////////////////////////// 
		/// <summary>
		/// ディレクションライトの設定
		/// </summary>
		/// <param name="lightNo">ライト番号</param>
		/// <param name="direction">ライトの方向</param>
		/// <param name="color">ライトの色</param>
		void SetDirectionLight(int lightNo, Vector3 direction, Vector3 color)
		{
			SetDirLightDirection(direction);
			SetDirLightColor(color);
		}

		/// <summary>
		/// ディレクションライトの光の方向を設定する
		/// </summary>
		/// <param name="direction">方向</param>
		void SetDirLightDirection(Vector3 direction)
		{
			m_light.directionalLight.lightDirection = direction;
		}
		/// <summary>
		/// ディレクションライトの光の色を設定する
		/// </summary>
		/// <param name="color">色</param>
		void SetDirLightColor(Vector3 color)
		{
			m_light.directionalLight.color = color;
		}
		/// <summary>
		/// ディレクションライトの光の方向を取得する
		/// </summary>
		/// <returns>光の方向</returns>
		const Vector3& GetDirLigDirection() const
		{
			return m_light.directionalLight.lightDirection;
		}
		/// <summary>
		/// ディレクションライトの光の色を取得する
		/// </summary>
		/// <returns>色</returns>
		const Vector3& GetDirLigColor() const
		{
			return m_light.directionalLight.color;
		}

		////////////////////////////////////////////////////////
		///環境光の関数
		////////////////////////////////////////////////////////

		/// <summary>
		/// 環境光の設定
		/// </summary>
		/// <param name="ambient">環境光の強さ</param>
		void SetAmbient(Vector3 ambient)
		{
			m_light.ambientLight = ambient;
		}

		////////////////////////////////////////////////////////
		///ポイントライトの関数
		////////////////////////////////////////////////////////

		/// <summary>
		/// ポイントライトの設定
		/// </summary>
		/// <param name="pos">座標</param>
		/// <param name="color">色</param>
		/// <param name="attn">Xに影響範囲,Yに累乗するパラメータ</param>
		void SetPointLight(Vector3 pos, Vector3 color, Vector3 attn)
		{
			SetPointLightPosition(pos);
			SetPointLightColor(color);
			SetPointLightAttn(attn);
			UsePointLight();
		}

		/// <summary>
		/// ポイントライトの座標
		/// </summary>
		/// <param name="pos">座標</param>
		void SetPointLightPosition(Vector3 pos)
		{
			m_light.pointLight.position = pos;
		}

		/// <summary>
		/// ポイントライトの色
		/// </summary>
		/// <param name="color">色</param>
		void SetPointLightColor(Vector3 color)
		{
			m_light.pointLight.color = color;
		}

		/// <summary>
		/// 影響範囲と影響率に累乗するパラメータを設定
		/// </summary>
		/// <param name="attn">Xに影響範囲,Yに累乗するパラメータ</param>
		void SetPointLightAttn(Vector3 attn)
		{
			m_light.pointLight.attn = attn;
		}

		/// <summary>
		/// ポイントライトを使用する
		/// </summary>
		void UsePointLight()
		{
			m_light.pointLight.isUse = true;
		}

		/// <summary>
		/// ポイントライトを使用しない
		/// </summary>
		void UnUsePointLight()
		{
			m_light.pointLight.isUse = false;
		}

		/// <summary>
		/// ポイントライトの位置を取得する
		/// </summary>
		/// <returns>座標</returns>
		const Vector3& GetPointLightPosition() const
		{
			return m_light.pointLight.position;
		}

		/// <summary>
		/// ポイントライトの光の色を取得
		/// </summary>
		/// <returns>色</returns>
		const Vector3& GetPointLightColor() const
		{
			return m_light.pointLight.color;
		}

		/// <summary>
		/// スポットライトの影響範囲と累乗するパラメータを取得
		/// </summary>
		/// <returns>Xに影響範囲,Yに累乗するパラメータ</returns>
		const Vector3& GetPointLightAttn() const
		{
			return m_light.pointLight.attn;
		}

		/// <summary>
		/// ポイントライトは使用中?
		/// </summary>
		/// <returns>使用中ならtrue</returns>
		const int PointLightIsUse() const
		{
			return m_light.pointLight.isUse;
		}

		////////////////////////////////////////////////////////
		///スポットライトの関数
		////////////////////////////////////////////////////////

		/// <summary>
		/// スポットライトを設定する
		/// </summary>
		/// <param name="pos">位置</param>
		/// <param name="color">色</param>
		/// <param name="range">Xに影響範囲,Yに累乗するパラメータ</param>
		/// <param name="direction">方向</param>
		/// <param name="angle">角度</param>
		void SetSpotLight(Vector3 pos, Vector3 color, Vector3 attn, Vector3 direction, Vector3 angle)
		{
			SetSpotLightPosition(pos);
			SetSpotLightColor(color);
			SetSpotLightAttn(attn);
			SetSpotLightDirection(direction);
			SetSpotLightAngle(angle);
			UseSpotLight();
		}

		/// <summary>
		/// スポットライトの位置を設定
		/// </summary>
		/// <param name="pos">座標</param>
		void SetSpotLightPosition(Vector3 pos)
		{
			m_light.spotLight.position = pos;
		}

		/// <summary>
		/// スポットライトのライト色の設定
		/// </summary>
		/// <param name="color">色</param>
		void SetSpotLightColor(Vector3 color)
		{
			m_light.spotLight.color = color;
		}

		/// <summary>
		/// 影響範囲と累乗するパラメータを設定
		/// </summary>
		/// <param name="attn">Xに影響範囲,Yに累乗するパラメータ</param>
		void SetSpotLightAttn(Vector3 attn)
		{
			m_light.spotLight.attn = attn;
		}

		/// <summary>
		/// スポットライトのライトの方向を設定
		/// </summary>
		/// <param name="direction">方向</param>
		void SetSpotLightDirection(Vector3 direction)
		{
			m_light.spotLight.direction = direction;
		}

		/// <summary>
		/// スポットライトのライトの角度を設定
		/// </summary>
		/// <param name="angle">角度</param>
		void SetSpotLightAngle(Vector3 angle)
		{
			m_light.spotLight.angle = angle;
		}

		/// <summary>
		/// スポットライトを使用する
		/// </summary>
		void UseSpotLight()
		{
			m_light.spotLight.isUse = true;
		}

		/// <summary>
		/// スポットライトを使用しない
		/// </summary>
		void UnUseSpotLight()
		{
			m_light.spotLight.isUse = false;
		}

		/// <summary>
		/// スポットライトの位置を取得
		/// </summary>
		/// <returns>座標</returns>
		const Vector3& GetSpotLightPosition() const
		{
			return m_light.spotLight.position;
		}

		/// <summary>
		/// スポットライトの光の色を取得
		/// </summary>
		/// <returns>色</returns>
		const Vector3& GetSpotLightColor() const
		{
			return m_light.spotLight.color;
		}

		/// <summary>
		/// スポットライトの影響範囲と累乗するパラメータを取得
		/// </summary>
		/// <returns>Xに影響範囲,Yに累乗するパラメータ</returns>
		const Vector3& GetSpotLightAttn()const
		{
			return m_light.spotLight.attn;
		}

		/// <summary>
		/// スポットライトの光の方向を取得
		/// </summary>
		/// <returns>光の方向</returns>
		const Vector3& GetSpotLightDirection() const
		{
			return m_light.spotLight.direction;
		}

		/// <summary>
		/// スポットライトの角度を取得する
		/// </summary>
		/// <returns>角度</returns>
		const Vector3& GetSpotLightAngle() const
		{
			return m_light.spotLight.angle;
		}

		/// <summary>
		/// スポットライトは使用中？
		/// </summary>
		/// <returns>使用中の場合true</returns>
		const int SpotLightIsUse() const
		{
			return m_light.spotLight.isUse;
		}

		////////////////////////////////////////////////////////
		///半球ライトの関数
		////////////////////////////////////////////////////////

		/// <summary>
		/// 半球ライトを設定する
		/// </summary>
		/// <param name="groundColor">地面色</param>
		/// <param name="skyColor">天球色</param>
		/// <param name="groundNormal">地面の法線</param>
		void SetHemiLight(Vector3 groundColor, Vector3 skyColor, Vector3 groundNormal)
		{
			SetHemiLightGroundColor(groundColor);
			SetHemiLightSkyColor(skyColor);
			SetHemiLightGroundNormal(groundNormal);
			UseHemiLight();
		}

		/// <summary>
		/// 半球ライトの地面色を設定
		/// </summary>
		/// <param name="groundColor">地面色</param>
		void SetHemiLightGroundColor(Vector3 groundColor)
		{
			m_light.hemiSphereLight.groundColor = groundColor;
		}

		/// <summary>
		/// 半球ライトの天球色を設定
		/// </summary>
		/// <param name="skyColor">天球色</param>
		void SetHemiLightSkyColor(Vector3 skyColor)
		{
			m_light.hemiSphereLight.skyColor = skyColor;
		}

		/// <summary>
		/// 半球ライトの地面の法線
		/// </summary>
		/// <param name="normal">地面の法線</param>
		void SetHemiLightGroundNormal(Vector3 normal)
		{
			m_light.hemiSphereLight.groundNormal = normal;
		}

		/// <summary>
		/// 半球ライトを使用する
		/// </summary>
		void UseHemiLight()
		{
			m_light.hemiSphereLight.isUse = true;
		}

		/// <summary>
		/// 半球ライトを使用しない
		/// </summary>
		void UnUseHemiLight()
		{
			m_light.hemiSphereLight.isUse = false;
		}

		/// <summary>
		/// 半球ライトの地面色を取得する
		/// </summary>
		/// <returns>地面色</returns>
		const Vector3& GetHemiLightGroundColor() const
		{
			return m_light.hemiSphereLight.groundColor;
		}

		/// <summary>
		/// 半球ライトの天球色を取得する
		/// </summary>
		/// <returns></returns>
		const Vector3& GetHemiLightSkyColor() const
		{
			return m_light.hemiSphereLight.skyColor;
		}

		/// <summary>
		/// 半球ライトの地面の法線を取得する
		/// </summary>
		/// <returns></returns>
		const Vector3& GetHemiLightGroundNormal() const
		{
			return m_light.hemiSphereLight.groundNormal;
		}

		/// <summary>
		/// 半球ライトは使用中?
		/// </summary>
		/// <returns>使用中のときtrue</returns>
		const int HemiLightIsUse() const
		{
			return m_light.hemiSphereLight.isUse;
		}

		////////////////////////////////////////////////////////
		///カメラの位置の関数
		////////////////////////////////////////////////////////

		/// <summary>
		/// カメラの位置を設定する
		/// </summary>
		/// <param name="eyePos"></param>
		void SetEyePos(Vector3 eyePos)
		{
			m_light.cameraEyePos = eyePos;
		}

	private:
		Light m_light;
	};
}



