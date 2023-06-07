/*!
 *@brief	レベル。
 */
#include "k2EngineLowPreCompile.h"
#include "Level2DRender.h"
#include "level2D/CaslFile.h"
#include "level2D/MapChip2D.h"

namespace nsK2EngineLow {
	void Level2DRender::AddMapChip2D(CaslData* caslData)
	{
		auto mapClip2DPtr = std::make_unique<MapChip2D>();
		mapClip2DPtr->get()->Init(caslData);
		//std::unique_ptrを移動させるにはstd::move()を使う。
		m_mapChip2DPtrList.emplace_back(std::move(mapClip2DPtr));
	}

	void Level2DRender::Init(
		const char* filePath,
		std::function<bool(Level2DObjectData& objData)> hookFunc)
	{
		//Caslファイルをロードする
		CaslFile casl;
		casl.Load(filePath);

		//構築
		for (int i = 0; i < casl.GetNumCaslData(); i++)
		{
			auto caslData = casl.GetCaslData(i);
			Level2DObjectData objData;
			//拡大率
			objData.scale.x = caslData->scale.x;
			objData.scale.y = caslData->scale.y;
			//座標
			objData.position.x = caslData->position.x;
			objData.position.y = caslData->position.y;
			//横幅縦幅
			objData.width = caslData->width;
			objData.height = caslData->height;
			//名前
			objData.name = caslData->name.get();
			//ddsファイル
			objData.ddsFilePath = caslData->ddsFilePath.get();
			//ピボット
			objData.pivot = caslData->pivot;
			auto isHook = false;
			if (hookFunc != nullptr) {
				//hook関数が指定されているのでhook関数を呼び出す
				isHook = hookFunc(objData);
				if (!isHook)
				{
					AddMapChip2D(caslData);
				}
			}
			else
			{
				AddMapChip2D(caslData);
			}


		}
	}

	void Level2DRender::Update() const
	{
		for (auto& mapClip : m_mapChip2DPtrList)
		{
			//TRSを更新
			mapClip.get()->Update();
		}
	}

	void Level2DRender::Draw(RenderContext& renderContext) const
	{
		for (auto& mapClip : m_mapChip2DPtrList)
		{
			//スプライトを描画処理
			mapClip.get()->Draw(renderContext);
		}
	}


}