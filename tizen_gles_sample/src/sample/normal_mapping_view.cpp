#include "sample/normal_mapping_view.h"

#include "basic/basic_file_loader.h"

using namespace std;

void NormalMappingView::OnInit()
{

		string vs = FileLoader::GetInstance()->ReadTxtFile("shader/view_f_lit/f_lit.vs");
		string fs = FileLoader::GetInstance()->ReadTxtFile("shader/view_f_lit/f_lit.fs");
		mViewRenderer->SetProgram(vs, fs);

		/*
		string vs = FileLoader::GetInstance()->ReadTxtFile("shader/view_nor/nor.vs");
		string fs = FileLoader::GetInstance()->ReadTxtFile("shader/view_nor/nor.fs");
		mViewRenderer->SetProgram(vs, fs);
		*/

		string ball = FileLoader::GetInstance()->ReadTxtFile("obj3d/sphere3");
		//string teapot = FileLoader::GetInstance()->ReadTxtFile("obj3d/teapot");

		/*
		TexData textJ;
		FileLoader::GetInstance()->ReadTexture("tex/soccer.jpg", textJ);
		*/
		TexData tex, normal_tex;
		//height_tex;
		FileLoader::GetInstance()->ReadTexture("tex/soccer.jpg", tex);
		//FileLoader::GetInstance()->ReadTexture("tex/photosculpt-graystonewall-normal.jpg", normal_tex);
		//FileLoader::GetInstance()->ReadTexture("tex/photosculpt-graystonewall-ambientocclusion.jpg", height_tex);

		/*
		float scale = 30.0f;
		mViewRenderer->SetNewModel(ball, scale);
		mViewRenderer->SetTexture(TEXDATA_GENERAL, &textJ);
		*/
		float scale = 30.0f;
		mViewRenderer->SetNewModel(ball, scale, 0);
		//mViewRenderer->SetNewModel(teapot, scale, 1);
		mViewRenderer->SetTexture(TEXDATA_GENERAL, &tex);
		//mViewRenderer->SetTexture(TEXDATA_NORMAL_MAP, &normal_tex);

		mViewRenderer->Initialize();
		// glClearColor(0.1f, 0.1f, 0.3f, 0.0f);

		mViewRenderer->GetCamera()->SetEye(0, 0, 800);
		mViewRenderer->GetCamera()->SetAt(0, 0, 0);
}

void NormalMappingView::OnStep()
{
	mViewRenderer->RenderFrame();
}

