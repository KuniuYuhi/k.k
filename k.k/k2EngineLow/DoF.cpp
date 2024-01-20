#include "k2EngineLowPreCompile.h"
#include "DoF.h"

void nsK2EngineLow::DoF::OnInit(
	RenderTarget& mainRenderTarget, RenderTarget& zprepassRenderTarget)
{
	//各種レンダリングターゲットの初期化
	InitRenderTarget(mainRenderTarget);
	//六角形ブラーをするための各種スプライトの初期化
	InitBlurSprite(mainRenderTarget);
}

void nsK2EngineLow::DoF::OnRender(RenderContext& rc, RenderTarget& mainRenderTarget)
{
	if (m_isEnable == false) {
		return;
	}
	
	//2枚のレンダリングターゲットを設定してモデルを描画する
	RenderTarget* rts[] = {
		&mainRenderTarget,
		&m_depthRenderTarget
	};

	//被写界深度用のレンダリングターゲットに変更
	//レンダリングターゲットとして利用できるまで待つ
	rc.WaitUntilToPossibleSetRenderTargets(2, rts);
	//レンダリングターゲットを設定
	rc.SetRenderTargetsAndViewport(2, rts);
	// レンダリングターゲットをクリア
	rc.ClearRenderTargetViews(2, rts);
	//モデルを描画
	g_renderingEngine->ModelRendering(rc);
	//レンダリングターゲットへの書き込み終了待ち
	rc.WaitUntilFinishDrawingToRenderTargets(2, rts);

	//垂直、対角線ブラーをかける
	RenderTarget* blurRts[] = {
		&m_rtVerticalBlur,
		&m_rtDiagonalBlur
	};

	rc.WaitUntilToPossibleSetRenderTargets(2, blurRts);
	rc.SetRenderTargetsAndViewport(2, blurRts);
	rc.ClearRenderTargetViews(2, blurRts);
	//垂直ブラーと対角線ブラーのレンダリングターゲットを書き込む
	m_vertDIagonalBlurSprite.Draw(rc);
	// レンダリングターゲットへの書き込み終了待ち
	rc.WaitUntilFinishDrawingToRenderTargets(2, blurRts);

	//六角形ブラーをかける
	rc.WaitUntilToPossibleSetRenderTarget(m_rtPhomboidBlur);
	rc.SetRenderTargetAndViewport(m_rtPhomboidBlur);
	m_phomboidBlurSprite.Draw(rc);
	// レンダリングターゲットへの書き込み終了待ち
	rc.WaitUntilFinishDrawingToRenderTarget(m_rtPhomboidBlur);

	// ボケ画像と深度テクスチャを利用して、ボケ画像を描きこんでいく
	//レンダリングターゲットとして利用できるまで待つ
	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	// メインレンダリングターゲットを設定
	rc.SetRenderTargetAndViewport(mainRenderTarget);
	// スプライトを描画
	m_combineBokeImageSprite.Draw(rc);
	// レンダリングターゲットへの書き込み終了待ち
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
}

void nsK2EngineLow::DoF::Render(RenderContext& rc, RenderTarget& mainRenderTarget, RenderTarget& zprepassRenderTarget)
{
	///////////////////////////////////////////////////////////////////
	////メインレンダリングターゲットのボケ画像を作成
	//m_blur.ExecuteOnGPU(rc, 10);

	////ボケ画像と深度テクスチャを利用して、ボケ画像を描きこんでいく
	////メインレンダリングターゲットを設定
	//rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	//rc.SetRenderTargetAndViewport(mainRenderTarget);

	//m_combineBokeImageSprite.Draw(rc);

	////レンダリングターゲットへの書き込み終了待ち
	//rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
}

void nsK2EngineLow::DoF::InitRenderTarget(RenderTarget& mainRenderTarget)
{
	//シーンのカメラ空間でのZ値を書き込むレンダリングターゲットを作成
	float clearColor[4] = { 10000.0f,10000.0f,10000.0f,1.0f };
	m_depthRenderTarget.Create(
		mainRenderTarget.GetWidth(),
		mainRenderTarget.GetHeight(),
		1,
		1,
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_UNKNOWN,
		clearColor
	);
	
	//垂直ブラーをかけるためのレンダリングターゲット
	m_rtVerticalBlur.Create(
		mainRenderTarget.GetWidth(),
		mainRenderTarget.GetHeight(),
		1,
		1,
		mainRenderTarget.GetColorBufferFormat(),
		DXGI_FORMAT_UNKNOWN
	);
	//対角線ブラーをかけるためのレンダリングターゲット
	m_rtDiagonalBlur.Create(
		mainRenderTarget.GetWidth(),
		mainRenderTarget.GetHeight(),
		1,
		1,
		mainRenderTarget.GetColorBufferFormat(),
		DXGI_FORMAT_UNKNOWN
	);
	//六角形ブラーをかけるためのレンダリングターゲット
	m_rtPhomboidBlur.Create(
		mainRenderTarget.GetWidth(),
		mainRenderTarget.GetHeight(),
		1,
		1,
		mainRenderTarget.GetColorBufferFormat(),
		DXGI_FORMAT_UNKNOWN
	);

}

void nsK2EngineLow::DoF::InitBlurSprite(RenderTarget& mainRenderTarget)
{
	//垂直、対角線ブラーをかけるためのスプライトを初期化
	SpriteInitData vertDiagonalBlurSpriteInitData;
	vertDiagonalBlurSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	vertDiagonalBlurSpriteInitData.m_width = mainRenderTarget.GetWidth();
	vertDiagonalBlurSpriteInitData.m_height = mainRenderTarget.GetHeight();
	vertDiagonalBlurSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect/hexaBlur.fx";
	//垂直、対角線ブラー用のピクセルシェーダーを指定する
	vertDiagonalBlurSpriteInitData.m_psEntryPoinFunc = "PSVerticalDiagonalBlur";
	vertDiagonalBlurSpriteInitData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();
	vertDiagonalBlurSpriteInitData.m_colorBufferFormat[1] = mainRenderTarget.GetColorBufferFormat();
	//スプライトの初期化
	m_vertDIagonalBlurSprite.Init(vertDiagonalBlurSpriteInitData);

	//六角形ブラーをかけるためのスプライトを初期化
	SpriteInitData phomboidBlurSpriteInitData;
	phomboidBlurSpriteInitData.m_textures[0] = &m_rtVerticalBlur.GetRenderTargetTexture();
	phomboidBlurSpriteInitData.m_textures[1] = &m_rtDiagonalBlur.GetRenderTargetTexture();
	phomboidBlurSpriteInitData.m_width = mainRenderTarget.GetWidth();
	phomboidBlurSpriteInitData.m_height = mainRenderTarget.GetHeight();
	phomboidBlurSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect/hexaBlur.fx";

	//六角形ブラー用のピクセルシェーダーを指定する
	phomboidBlurSpriteInitData.m_psEntryPoinFunc = "PSRhomboidBlur";
	phomboidBlurSpriteInitData.m_colorBufferFormat[0] = mainRenderTarget.GetColorBufferFormat();

	m_phomboidBlurSprite.Init(phomboidBlurSpriteInitData);

	//ボケ画像合成用のスプライトを初期化する
	SpriteInitData  combineBokeImageSpriteInitData;
	//使用するテクスチャは2枚
	combineBokeImageSpriteInitData.m_textures[0] = &m_rtPhomboidBlur.GetRenderTargetTexture();
	combineBokeImageSpriteInitData.m_textures[1] = &m_depthRenderTarget.GetRenderTargetTexture();
	combineBokeImageSpriteInitData.m_width = mainRenderTarget.GetWidth();
	combineBokeImageSpriteInitData.m_height = mainRenderTarget.GetHeight();
	//合成用のシェーダーを指定する
	combineBokeImageSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect/DepthOfFeild.fx";
	combineBokeImageSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//距離を利用してボケ画像をアルファブレンディングするので、半透明モードにする
	combineBokeImageSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;
	//スプライトの初期化
	m_combineBokeImageSprite.Init(combineBokeImageSpriteInitData);

}
