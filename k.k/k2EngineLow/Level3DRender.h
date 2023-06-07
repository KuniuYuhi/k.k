/*!
 *@brief	レベル。
 */

#pragma once
#include "level3D/TklFile.h"

namespace nsK2EngineLow {

	//wchar_t=ワイド文字。文字をバイトに変換する。
	//		  文字列の場合は最後にヌル文字が加わる。
	//　　　　文字、文字列の手前にLを付ける。

	/// <summary>
	/// レベルオブジェクト。
	/// </summary>
	struct LevelObjectData {
		Vector3			position;			//座標
		Quaternion		rotation;			//回転
		Vector3			scale;			//拡大率
		const			wchar_t* name;		//名前
		int				number = 0;			//番号

		/// <summary>
		/// 引数で渡したオブジェクト名が同じオブジェクトか調べる
		/// </summary>
		/// <param name="objName">調べるオブジェクト名</param>
		/// <returns>オブジェクト名が同じ場合trueを返す</returns>
		bool EqualObjectName(const wchar_t* objName)
		{
			//ワイド文字列の比較
			return wcscmp(objName, name) == 0;
		}

		/// <summary>
		/// 名前が前方一致か調べる
		/// </summary>
		/// <param name="n">調べる名前</param>
		/// <returns>一致していたらtrueを返す</returns>
		bool ForwardMatchName(const wchar_t* n)
		{
			auto len = wcslen(n);
			auto namelen = wcslen(name);
			if (len > namelen) {
				//名前が長いので不一致
				return false;
			}
			//ワイド文字列の比較
			return wcsncmp(n, name, len) == 0;
		}
	};


	class Level3DRender
	{
	public:
		~Level3DRender();
		/*!
		 * @brief	レベルを初期化。
		 *@param[in]	levelDataFilePath		tklファイルのファイルパス。
		 *@param[in] hookFunc				オブジェクトを作成する時の処理をフックするための関数オブジェクト。
		 *   フックしないならnullptrを指定すればよい、
		 * この関数オブジェクトがfalseを返すと、オブジェクトの情報から、
		 * 静的オブジェクトのMapChipクラスのインスタンスが生成されます。
		 * オブジェクトの名前などで、ドアやジャンプ台、アイテムなどの特殊なクラスのインスタンスを生成したときに、
		 * デフォルトで作成されるMapChipクラスのインスタンスが不要な場合はtrueを返してください。
		 * 例えば、フック関数の中で、渡されたオブジェクトデータの名前のモデルを描画するクラスのインスタンスを
		 * 生成したときに、falseを返してしまうと、同じモデルが二つ描画されることになります。
		 */


		/// <summary>
		/// レベルを初期化
		/// </summary>
		/// <param name="filePath">tklファイルのファイルパス</param>
		/// <param name="hookFunk">オブジェクトを作成する時の処理をフックするための関数オブジェクト</param>
		void Init(
			const char* filePath,
			std::function<bool(LevelObjectData& objData)> hookFunc
		);

	private:
		//tklファイルの行列を変換する
		void MatrixTklToLevel();
	private:
		using BonePtr = std::unique_ptr<Bone>;
		std::vector<BonePtr> m_bonelist;
		TklFile m_tklFile;
	};
}


