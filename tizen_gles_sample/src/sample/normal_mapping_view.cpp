#include "sample/normal_mapping_view.h"

#include "basic/basic_file_loader.h"

using namespace std;

void NormalMappingView::OnInit() {
	string vs = FileLoader::GetInstance()->ReadTxtFile(
			"shader/view_nor/nor.vs");
	string fs = FileLoader::GetInstance()->ReadTxtFile(
			"shader/view_nor/nor.fs");
	mViewRenderer->SetProgram(vs, fs);

	string ball = FileLoader::GetInstance()->ReadTxtFile("obj3d/sphere3");
	string obstacle1 = FileLoader::GetInstance()->ReadTxtFile("obj3d/cube");
	string obstacle2 = FileLoader::GetInstance()->ReadTxtFile("obj3d/cube");
	string goal = FileLoader::GetInstance()->ReadTxtFile("obj3d/sphere3");

	TexData ball_tex, ball_normal_tex;
	TexData obstacle1_tex, obstacle1_normal_tex;
	TexData obstacle2_tex, obstacle2_normal_tex;
	TexData goal_tex, goal_normal_tex;
	FileLoader::GetInstance()->ReadTexture("tex/soccer.jpg", ball_tex);
	FileLoader::GetInstance()->ReadTexture(
			"tex/photosculpt-mixedstones-diffuse.jpg", obstacle1_tex);
	FileLoader::GetInstance()->ReadTexture(
			"tex/photosculpt-mixedstones-diffuse.jpg", obstacle2_tex);
	FileLoader::GetInstance()->ReadTexture("tex/black.jpg", goal_tex);

	FileLoader::GetInstance()->ReadTexture("tex/plain.jpg", ball_normal_tex);
	FileLoader::GetInstance()->ReadTexture(
			"tex/photosculpt-mixedstones-normal.jpg", obstacle1_normal_tex);
	FileLoader::GetInstance()->ReadTexture(
			"tex/photosculpt-mixedstones-normal.jpg", obstacle2_normal_tex);
	FileLoader::GetInstance()->ReadTexture("tex/plain.jpg", goal_normal_tex);

	float scale = 30.0f;
	mViewRenderer->SetNewModel(ball, obstacle1, obstacle2, goal, scale);
	mViewRenderer->SetTexture(TEXDATA_GENERAL, &ball_tex, &obstacle1_tex,
			&obstacle2_tex, &goal_tex);
	mViewRenderer->SetTexture(TEXDATA_NORMAL_MAP, &ball_normal_tex,
			&obstacle1_normal_tex, &obstacle2_normal_tex, &goal_normal_tex);

	mViewRenderer->Initialize();
	glClearColor(0.5f, 0.5f, 0.1f, 0.0f);

	mViewRenderer->GetCamera()->SetEye(0, 0, 800);
	mViewRenderer->GetCamera()->SetAt(0, 0, 0);
}

void NormalMappingView::OnStep() {
	mViewRenderer->RenderFrame();
}

