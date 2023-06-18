#pragma once
namespace nsK2EngineLow {



	class ToonMap
	{
	public:
		ToonMap();
		~ToonMap();

		/// <summary>
		/// 初期化
		/// </summary>
		void Init();

		/// <summary>
		/// テクスチャをピクセルシェーダーに設定する。
		/// </summary>
		void SetShaderResourceView();

	private:
	};
}

