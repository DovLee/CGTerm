#include "sample/frag_lighting_view.h"

#include "basic/basic_file_loader.h"

using namespace std;

void FragLightingView::OnInit()
{
	string vs = FileLoader::GetInstance()->ReadTxtFile("shader/view_f_lit/f_lit.vs");
	string fs = FileLoader::GetInstance()->ReadTxtFile("shader/view_f_lit/f_lit.fs");
	mViewRenderer->SetProgram(vs, fs);

	string ball = FileLoader::GetInstance()->ReadTxtFile("obj3d/sphere3");

	TexData textJ;
	FileLoader::GetInstance()->ReadTexture("tex/soccer.jpg", textJ);

	float scale = 30.0f;
	mViewRenderer->SetNewModel(ball, scale);
	mViewRenderer->SetTexture(TEXDATA_GENERAL, &textJ);

	mViewRenderer->Initialize();
	glClearColor(0.1f, 0.1f, 0.3f, 0.0f);

	mViewRenderer->GetCamera()->SetEye(0, 0, 800);
	mViewRenderer->GetCamera()->SetAt(0, 0, 0);
}

void FragLightingView::OnStep()
{
	mViewRenderer->RenderFrame();
}

