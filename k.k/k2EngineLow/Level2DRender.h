#pragma once
#include "level2D/MapChip2D.h"

namespace nsK2EngineLow {

	struct CaslData;
	/// <summary>
	/// 2Dのデータ
	/// </summary>
	struct Level2DObjectData
	{
		Vector3 position = Vector3::Zero;				//座標
		Quaternion rotation = Quaternion::Identity;		//回転
		Vector3 scale = Vector3::One;					//拡大率
		int width = 0;									//横幅
		int height = 0;									//縦幅
		Vector2 pivot = Sprite::DEFAULT_PIVOT;			//ピボット
		const char* name;								//名前
		const char* ddsFilePath;						//ddsファイルのファイルパス

		/// <summary>
		/// 引数で渡したオブジェクト名が同じオブジェクトか調べる
		/// </summary>
		/// <param name="objName">調べるオブジェクト名</param>
		/// <returns>オブジェクト名が同じ場合trueを返す</returns>
		bool EqualObjectName(const char* objName)
		{
			return strcmp(objName, name) == 0;
		}

		/// <summary>
		/// 名前が前方一致か調べる
		/// </summary>
		/// <param name="n">調べる名前</param>
		/// <returns>一致していたらtrueを返す</returns>
		bool ForwardMatchName(const char* n) const
		{
			auto len = strlen(n);
			auto namelen = strlen(name);
			if (len > namelen) {
				//名前が長いので不一致
				return false;
			}
			//文字列の比較
			return strncmp(n, name, len) == 0;
		}

	};

	/// <summary>
	/// レベル2D
	/// </summary>
	class Level2DRender
	{
	private:
		using MapChip2D = std::unique_ptr<MapChip2D>;
	public:

		/// <summary>
		/// レベルを初期化
		/// </summary>
		/// <param name="filePath">caslファイルのファイルパス</param>
		/// <param name="hookFunc">オブジェクトを作成する時の処理をフックするための関数オブジェクト</param>
		void Init(
			const char* filePath,
			std::function<bool(Level2DObjectData& objData)> hookFunc
		);

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update() const;

		/// <summary>
		/// マップチップ2Dの描画処理
		/// </summary>
		/// <param name="renderContext">レンダーコンテキスト</param>
		void Draw(RenderContext& renderContext) const;

	private:

		/// <summary>
		/// マップチップ2Dを追加する
		/// </summary>
		/// <param name="caslData">Caslデータ</param>
		void AddMapChip2D(CaslData* objData);

	private:
		std::vector<MapChip2D> m_mapChip2DPtrList;
	};
}



