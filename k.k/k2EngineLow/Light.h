#pragma once
namespace nsK2EngineLow {

	const int MAM_DIRECTIONAL_LIGHT = 4;   //ディレクションライトの最大数

	struct DirectionalLight
	{
		Vector3 lightDirection;		//ライトの方向
		float pad0;					//パティング
		Vector3 color;				//ライトのカラー
		float pad1;					//パティング
		
	};

	/// <summary>
	/// ライト構造体
	/// </summary>
	struct Light
	{
		DirectionalLight directionalLight;
		float pad0;
		float pad1;
		Matrix mLVP;
		Vector3 cameraEyePos;
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
			m_light.mLVP = LVP;
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



