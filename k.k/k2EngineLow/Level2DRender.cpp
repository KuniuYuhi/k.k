/*!
 *@brief	���x���B
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
		//std::unique_ptr���ړ�������ɂ�std::move()���g���B
		m_mapChip2DPtrList.emplace_back(std::move(mapClip2DPtr));
	}

	void Level2DRender::Init(
		const char* filePath,
		std::function<bool(Level2DObjectData& objData)> hookFunc)
	{
		//Casl�t�@�C�������[�h����
		CaslFile casl;
		casl.Load(filePath);

		//�\�z
		for (int i = 0; i < casl.GetNumCaslData(); i++)
		{
			auto caslData = casl.GetCaslData(i);
			Level2DObjectData objData;
			//�g�嗦
			objData.scale.x = caslData->scale.x;
			objData.scale.y = caslData->scale.y;
			//���W
			objData.position.x = caslData->position.x;
			objData.position.y = caslData->position.y;
			//�����c��
			objData.width = caslData->width;
			objData.height = caslData->height;
			//���O
			objData.name = caslData->name.get();
			//dds�t�@�C��
			objData.ddsFilePath = caslData->ddsFilePath.get();
			//�s�{�b�g
			objData.pivot = caslData->pivot;
			auto isHook = false;
			if (hookFunc != nullptr) {
				//hook�֐����w�肳��Ă���̂�hook�֐����Ăяo��
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
			//TRS���X�V
			mapClip.get()->Update();
		}
	}

	void Level2DRender::Draw(RenderContext& renderContext) const
	{
		for (auto& mapClip : m_mapChip2DPtrList)
		{
			//�X�v���C�g��`�揈��
			mapClip.get()->Draw(renderContext);
		}
	}


}