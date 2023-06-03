#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow {

	void ModelRender::Init(const char* tkmFilepath)
	{
		//tkmファイルパスを設定
		m_modelInitData.m_tkmFilePath = tkmFilepath;
		//シェーダーのファイルパスを設定
		m_modelInitData.m_fxFilePath = "Assets/shader/model.fx";

		//作成した初期化データを元にモデルを初期化する
		m_model.Init(m_modelInitData);
	}


}