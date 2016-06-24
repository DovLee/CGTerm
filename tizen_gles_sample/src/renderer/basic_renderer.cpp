#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

#include "basic/basic_renderer.h"

using namespace std;
using namespace glm;
float velocity_x = 0;
float velocity_y = 0;
float position_x = -263;
float position_y = -428;

static int num2_x = 0;
static int num2_y = 0;
static int num3 = 0;
static int generate = 0;
Evas_Object *popup, *parent;

BasicRenderer::BasicRenderer() {
	mWidth = 0, mHeight = 0, mDeltaTime = 0, mTimer = NULL, mCamera = NULL, mShader =
			NULL, mIsAutoRotateEye = true, mIsFill = true, mIsTouchOn = false, mTouchPoint =
			glm::vec2(0), mVboVertices = 0, mVboIndices = 0, mVboTangents = 0, mVboInstTransforms =
			0, mHasTexture = false, mHasCubemap = false, mHasNorMap = false, mHasHeightmap =
			false, mTexId = 0, mTexCubeId = 0, mTexNorId = 0, mTexHeightId = 0, mInstCount =
			1;

	popup = elm_popup_add(parent);
	elm_object_style_set(popup, "toast");

	mTimer = new BasicTimer();
	mCamera = new BasicCamera();
	mShader = new BasicShader();

	Mines[0].mTimer = NULL, Mines[0].mCamera = NULL, Mines[0].mShader = NULL,

	Mines[1].mTimer = NULL, Mines[1].mCamera = NULL, Mines[1].mShader = NULL,

	Mines[2].mTimer = NULL, Mines[2].mCamera = NULL, Mines[2].mShader = NULL;

	Mines[3].mTimer = NULL, Mines[3].mCamera = NULL, Mines[3].mShader = NULL;

	for (int i = 0; i < 4; i++) {
		Mines[i].mTimer = new BasicTimer();
		Mines[i].mCamera = new BasicCamera();
		Mines[i].mShader = new BasicShader();
		Mines[i].mWidth = 0;
		Mines[i].mHeight = 0;
		Mines[i].mDeltaTime = 0;
		Mines[i].mIsAutoRotateEye = true;
		Mines[i].mIsFill = true;
		Mines[i].mIsTouchOn = false;
		Mines[i].mTouchPoint = glm::vec2(0);
		Mines[i].mVboVertices = 0;
		Mines[i].mVboIndices = 0;
		Mines[i].mVboTangents = 0;
		Mines[i].mVboInstTransforms = 0;
		Mines[i].mHasTexture = false;
		Mines[i].mHasCubemap = false;
		Mines[i].mHasNorMap = false;
		Mines[i].mHasHeightmap = false;
		Mines[i].mTexId = 0;
		Mines[i].mTexCubeId = 0;
		Mines[i].mTexNorId = 0;
		Mines[i].mTexHeightId = 0;
		Mines[i].mInstCount = 1;
	}

}

BasicRenderer::~BasicRenderer() {
	delete mTimer;
	delete mShader;
	delete mCamera;

	delete Mines[0].mTimer;
	delete Mines[0].mCamera;
	delete Mines[0].mShader;

	delete Mines[1].mTimer;
	delete Mines[1].mCamera;
	delete Mines[1].mShader;

	delete Mines[2].mTimer;
	delete Mines[2].mCamera;
	delete Mines[2].mShader;

	delete Mines[3].mTimer;
	delete Mines[3].mCamera;
	delete Mines[3].mShader;

}

// Interface functionsfd
/// Sets vertex shader and fragment shader for rendering
bool BasicRenderer::SetProgram(const std::string& vertexSource,
		const std::string& fragmentSource) const {
	for (int i = 0; i < 4; i++) {
		Mines[i].mShader->CreateProgram(vertexSource, fragmentSource);
		//Mines[my_number].mShader->CreateProgram(vertexSource, fragmentSource);
		if (!Mines[i].mShader->GetProgram()) {
			LOGE("Could not create program.\n");
			return false;
		}
		/*if (!Mines[my_number].mShader->GetProgram())
		 {
		 LOGE("Could not create program.\n");
		 return false;
		 }*/

		Mines[i].mShader->Use();
		//Mines[my_number].mShader->Use();

		LOGI("minjae_setprogram");

	}

	return true;
}

bool BasicRenderer::SetProgram(const char* vertexSource,
		const char* fragmentSource) const {
	for (int i = 0; i < 4; i++) {
		Mines[i].mShader->CreateProgram(vertexSource, fragmentSource);
		//Mines[my_number].mShader->CreateProgram(vertexSource, fragmentSource);

		if (!Mines[i].mShader->GetProgram()) {
			LOGE("Could not create program.\n");
			return false;
		}
		/*if (!Mines[my_number].mShader->GetProgram())
		 {
		 LOGE("Could not create program.\n");
		 return false;
		 }*/

		Mines[i].mShader->Use();
		//Mines.mShader[my_number]->Use();
		LOGI("minjae_no std char");

	}
	return true;

}

/****************************
 **** Interface functions ***
 ****************************/
void BasicRenderer::SetNewModel(const std::string& objSource,
		const std::string& objSource2, const std::string& objSource3,
		const std::string& objSource4, const float& scale) {
	LOGI("SetNewModel");
	char* buffer = new char[objSource.length() + 1];
	char* buffer2 = new char[objSource2.length() + 1];
	char* buffer3 = new char[objSource3.length() + 1];
	char* buffer4 = new char[objSource4.length() + 1];

	strcpy(buffer, objSource.c_str());
	strcpy(buffer2, objSource2.c_str());
	strcpy(buffer3, objSource3.c_str());
	strcpy(buffer4, objSource4.c_str());

	SetNewModel(buffer, buffer2, buffer3, buffer4, scale);

	delete (buffer);
	delete (buffer2);
	delete (buffer3);
	delete (buffer4);
	LOGI("minjae_setnew_model");
}

void BasicRenderer::SetNewModel(char* objSource, char* objSource2,
		char* objSource3, char* objSource4) {
	ImportModel(objSource, objSource2, objSource3, objSource4, this);
}

void BasicRenderer::SetNewModel(char* objSource, char* objSource2,
		char* objSource3, char* objSource4, const float& scale) {
	ImportModelScale(objSource, this, scale, 0);
	ImportModelScale2(objSource2, this, 20.0f, 1);
	ImportModelScale3(objSource3, this, 15.0f, 2);
	ImportModelScale3(objSource4, this, 30.0f, 3);

	Mines[0].mHasTexture = true;
	Mines[1].mHasTexture = true;
	Mines[2].mHasTexture = true;
	Mines[3].mHasTexture = true;

}
void BasicRenderer::SetTexture(TexData_Type type, const TexData* newTex,
		const TexData* newTex2, const TexData* newTex3,
		const TexData* newTex4) {
	LOGI("SETTEXTURE _____");
	switch (type) {
	case TEXDATA_GENERAL: // general texture
		LOGI("Set Texture : general\n");
		Mines[0].mHasTexture = true;
		Mines[1].mHasTexture = true;
		Mines[2].mHasTexture = true;
		Mines[3].mHasTexture = true;

		//Mines[my_number].mHasTexture = true;

		CreateTexBuffer(*newTex, Mines[0].mTexId);
		//		CreateTexBuffer(*newTex, Mines[my_number].mTexId);
		CreateTexBuffer(*newTex2, Mines[1].mTexId);
		CreateTexBuffer(*newTex3, Mines[2].mTexId);
		CreateTexBuffer(*newTex4, Mines[3].mTexId);

		break;
	case TEXDATA_NORMAL_MAP: // normal map
		LOGI("Set Texture : normal map\n");
		Mines[0].mHasNorMap = true;
		Mines[1].mHasNorMap = true;
		Mines[2].mHasNorMap = true;
		Mines[3].mHasNorMap = true;

		//Mines[my_number].mHasNorMap = true;

		if (Mines[0].mTangentBuffer.empty() || Mines[1].mTangentBuffer.empty()
				|| Mines[2].mTangentBuffer.empty()
				|| Mines[3].mTangentBuffer.empty())
			ComputeTangent();
		//if (Mines[my_number].mTangentBuffer.empty()) ComputeTangent();

		CreateTexBuffer(*newTex, Mines[0].mTexNorId);
		CreateTexBuffer(*newTex2, Mines[1].mTexNorId);

		CreateTexBuffer(*newTex3, Mines[2].mTexNorId);
		CreateTexBuffer(*newTex4, Mines[3].mTexNorId);

		//CreateTexBuffer(*newTex, Mines[my_number].mTexNorId)
		break;
	default:
		break;
	}
	LOGI("SETTEXTURE _____Done");

}

bool BasicRenderer::Initialize() {
	LOGI(" renderer.\n");
	LogInfo();

	CountTickInit();

	CreateVbo();
	SetState();

	return true;
}

void BasicRenderer::SetViewPort(int w, int h) {
	LOGI("SetViewPort(%d, %d)\n", w, h);
	Mines[0].mWidth = w;
	Mines[0].mHeight = h;
	//mWidth = w;
	//mHeight = h;
	glViewport(0, 0, w, h);
	check_gl_error("glViewport");

	mCamera->ComputePerspective(60.0f, w, h);
	//Mines[my_number].mCamera->ComputePerspective(60.0f, w, h);
}

void BasicRenderer::RenderFrame() {
	LOGI("renderframe");

	//LOGI("BasicRenderer::RenderFrame()");
	ComputeTick();

	//if (mIsAutoRotateEye) mCamera->RotateAuto(mDeltaTime); //
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	check_gl_error("glClear");
	for (int i = 0; i < 4; i++) {
		if (i == 0 && gameWin)
			continue; // dont draw the ball anymore

		Mines[i].mShader->Use();
		PassUniform(i);
		SetAttrib(i);

		Draw(i);
	}
}
void BasicRenderer::SetAttrib(int i) const {
	glBindBuffer(GL_ARRAY_BUFFER, Mines[i].mVboVertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mines[i].mVboIndices);
	int stride = sizeof(VertexStruct); // stride: sizeof(float) * number of components
	int offset = 0;

	if (i == 0) {
		glEnableVertexAttribArray(V_ATTRIB_POSITION);
		glVertexAttribPointer(V_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, stride,
				reinterpret_cast<GLvoid *>(offset));
		check_gl_error("glVertexAttribPointer");

		offset += sizeof(vec3);

		glEnableVertexAttribArray(V_ATTRIB_NORMAL);
		glVertexAttribPointer(V_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, stride,
				reinterpret_cast<GLvoid *>(offset));
		check_gl_error("glVertexAttribPointer");

		offset += sizeof(vec3);

		if (Mines[i].mHasTexture || Mines[i].mHasNorMap) {
			glEnableVertexAttribArray(V_ATTRIB_TEX);
			glVertexAttribPointer(V_ATTRIB_TEX, 2, GL_FLOAT, GL_FALSE, stride,
					reinterpret_cast<GLvoid *>(offset));
			check_gl_error("glVertexAttribPointer");

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Mines[i].mTexId);
		}

		if (Mines[i].mHasNorMap) {
			// Bump mapping need to change space (world and TBN)
			// mTangentBuffer calculated by ComputeTangent() when normal texture has set
			glBindBuffer(GL_ARRAY_BUFFER, Mines[i].mVboTangents);

			offset = 0;
			stride = sizeof(vec3);
			glEnableVertexAttribArray(V_ATTRIB_TANGENT);
			glVertexAttribPointer(V_ATTRIB_TANGENT, 3, GL_FLOAT, GL_FALSE,
					stride, reinterpret_cast<GLvoid *>(offset));

			glActiveTexture(GL_TEXTURE0 + TEX_POS_NORMAL);
			glBindTexture(GL_TEXTURE_2D, Mines[i].mTexNorId);
		}
	} else if (i == 1) {
		glEnableVertexAttribArray(V_ATTRIB_POSITION2);
		glVertexAttribPointer(V_ATTRIB_POSITION2, 3, GL_FLOAT, GL_FALSE, stride,
				reinterpret_cast<GLvoid *>(offset));
		check_gl_error("glVertexAttribPointer");
		offset += sizeof(vec3);

		glEnableVertexAttribArray(V_ATTRIB_NORMAL2);
		glVertexAttribPointer(V_ATTRIB_NORMAL2, 3, GL_FLOAT, GL_FALSE, stride,
				reinterpret_cast<GLvoid *>(offset));
		check_gl_error("glVertexAttribPointer");

		offset += sizeof(vec3);

		if (Mines[i].mHasTexture || Mines[i].mHasNorMap) {
			glEnableVertexAttribArray(V_ATTRIB_TEX2);
			glVertexAttribPointer(V_ATTRIB_TEX2, 2, GL_FLOAT, GL_FALSE, stride,
					reinterpret_cast<GLvoid *>(offset));
			check_gl_error("glVertexAttribPointer");

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Mines[i].mTexId);
		}

		if (Mines[i].mHasNorMap) {
			// Bump mapping need to change space (world and TBN)
			// mTangentBuffer calculated by ComputeTangent() when normal texture has set
			glBindBuffer(GL_ARRAY_BUFFER, Mines[i].mVboTangents);

			offset = 0;
			stride = sizeof(vec3);
			glEnableVertexAttribArray(V_ATTRIB_TANGENT2);
			glVertexAttribPointer(V_ATTRIB_TANGENT2, 3, GL_FLOAT, GL_FALSE,
					stride, reinterpret_cast<GLvoid *>(offset));

			glActiveTexture(GL_TEXTURE0 + TEX_POS_NORMAL2);
			glBindTexture(GL_TEXTURE_2D, Mines[i].mTexNorId);
		}

	} else if (i == 2) {
		glEnableVertexAttribArray(V_ATTRIB_POSITION3);
		glVertexAttribPointer(V_ATTRIB_POSITION3, 3, GL_FLOAT, GL_FALSE, stride,
				reinterpret_cast<GLvoid *>(offset));
		check_gl_error("glVertexAttribPointer");

		offset += sizeof(vec3);

		glEnableVertexAttribArray(V_ATTRIB_NORMAL3);
		glVertexAttribPointer(V_ATTRIB_NORMAL3, 3, GL_FLOAT, GL_FALSE, stride,
				reinterpret_cast<GLvoid *>(offset));
		check_gl_error("glVertexAttribPointer");

		offset += sizeof(vec3);

		if (Mines[i].mHasTexture || Mines[i].mHasNorMap) {
			glEnableVertexAttribArray(V_ATTRIB_TEX3);
			glVertexAttribPointer(V_ATTRIB_TEX3, 2, GL_FLOAT, GL_FALSE, stride,
					reinterpret_cast<GLvoid *>(offset));
			check_gl_error("glVertexAttribPointer");

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Mines[i].mTexId);
		}
		if (Mines[i].mHasNorMap) {
			// Bump mapping need to change space (world and TBN)
			// mTangentBuffer calculated by ComputeTangent() when normal texture has set
			glBindBuffer(GL_ARRAY_BUFFER, Mines[i].mVboTangents);

			offset = 0;
			stride = sizeof(vec3);
			glEnableVertexAttribArray(V_ATTRIB_TANGENT3);
			glVertexAttribPointer(V_ATTRIB_TANGENT3, 3, GL_FLOAT, GL_FALSE,
					stride, reinterpret_cast<GLvoid *>(offset));

			glActiveTexture(GL_TEXTURE0 + TEX_POS_NORMAL3);
			glBindTexture(GL_TEXTURE_2D, Mines[i].mTexNorId);
		}

	} else if (i == 3) {
		glEnableVertexAttribArray(V_ATTRIB_POSITION4);
		glVertexAttribPointer(V_ATTRIB_POSITION4, 3, GL_FLOAT, GL_FALSE, stride,
				reinterpret_cast<GLvoid *>(offset));
		check_gl_error("glVertexAttribPointer");

		offset += sizeof(vec3);

		glEnableVertexAttribArray(V_ATTRIB_NORMAL4);
		glVertexAttribPointer(V_ATTRIB_NORMAL4, 3, GL_FLOAT, GL_FALSE, stride,
				reinterpret_cast<GLvoid *>(offset));
		check_gl_error("glVertexAttribPointer");

		offset += sizeof(vec3);

		if (Mines[i].mHasTexture || Mines[i].mHasNorMap) {
			glEnableVertexAttribArray(V_ATTRIB_TEX4);
			glVertexAttribPointer(V_ATTRIB_TEX4, 2, GL_FLOAT, GL_FALSE, stride,
					reinterpret_cast<GLvoid *>(offset));
			check_gl_error("glVertexAttribPointer");

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Mines[i].mTexId);
		}
		if (Mines[i].mHasNorMap) {
			// Bump mapping need to change space (world and TBN)
			// mTangentBuffer calculated by ComputeTangent() when normal texture has set
			glBindBuffer(GL_ARRAY_BUFFER, Mines[i].mVboTangents);

			offset = 0;
			stride = sizeof(vec3);
			glEnableVertexAttribArray(V_ATTRIB_TANGENT4);
			glVertexAttribPointer(V_ATTRIB_TANGENT4, 3, GL_FLOAT, GL_FALSE,
					stride, reinterpret_cast<GLvoid *>(offset));

			glActiveTexture(GL_TEXTURE0 + TEX_POS_NORMAL4);
			glBindTexture(GL_TEXTURE_2D, Mines[i].mTexNorId);
		}

	}
}

void BasicRenderer::TogglePolygonMode() {
#ifdef BASIC_OPENGL
	mIsFill = !mIsFill;
	glPolygonMode(GL_FRONT_AND_BACK, mIsFill ? GL_FILL : GL_LINE);
#endif
}

/*****************************
 ***** Texture functions *****
 *****************************/
void BasicRenderer::CreateTexBuffer(const TexData& newTex,
		GLuint& target) const {
	LOGI("BasicRenderer::CreateTexBuffer\n");
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	check_gl_error("glPixelStorei");
	glGenTextures(1, &target);
	check_gl_error("glGenTextures");

	glBindTexture(GL_TEXTURE_2D, target);
	check_gl_error("glBindTexture");

	TexBuffer(GL_TEXTURE_2D, newTex);

	glGenerateMipmap(GL_TEXTURE_2D);
	check_gl_error("glGenerateMipmap");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);
	check_gl_error("glTexParameteri");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	check_gl_error("glTexParameteri");

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	check_gl_error("glTexParameteri");

	glBindTexture(GL_TEXTURE_2D, 0);
}

void BasicRenderer::TexBuffer(GLenum type, const TexData& newTex) const {
	LOGI("BasicRenderer::TexBuffer");

	glTexImage2D(type, 0, newTex.format, newTex.width, newTex.height, 0,
			static_cast<GLenum>(newTex.format), GL_UNSIGNED_BYTE,
			static_cast<void *>(newTex.pixels));

	check_gl_error("glTexImage2D");
}

/*******************************
 ***** Rendering functions *****
 *******************************/
void BasicRenderer::SetState() const {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
}

void BasicRenderer::CreateVbo() {
	for (int i = 0; i < 4; i++) {
		LOGI("BasicRenderer::createVbo\n");
		glGenBuffers(1, &Mines[i].mVboVertices);
		glGenBuffers(1, &Mines[i].mVboIndices);
		if (i == 1)
			LOGI("BasicRenderer::createVbo 123\n");

		glBindBuffer(GL_ARRAY_BUFFER, Mines[i].mVboVertices);
		check_gl_error("glcreate_error");

		if (i == 1)
			LOGI("BasicRenderer::createVbo 567\n");

		glBufferData(GL_ARRAY_BUFFER,
				static_cast<GLsizeiptr>(Mines[i].mVertexBuffer.size()
						* sizeof(VertexStruct)),
				&(Mines[i].mVertexBuffer.at(0)), GL_STATIC_DRAW);

		////////////////////여기서Mines[i]. mvertexBuffer i로 하면 에러 , i를 0으로 하면 뽑히긴 뽑히는데
		if (i == 1)
			LOGI("BasicRenderer::createVbo 456\n");

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mines[i].mVboIndices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				static_cast<GLsizeiptr>(Mines[i].mIndexBuffer.size()
						* sizeof(GLushort)), &(Mines[i].mIndexBuffer.at(0)),
				GL_STATIC_DRAW);
		LOGI("BasicRenderer::createVbo 1\n");

		int stride = sizeof(VertexStruct); // stride: sizeof(float) * number of components
		int offset = 0;
		if (i == 0) {
			glEnableVertexAttribArray(V_ATTRIB_POSITION);
			glVertexAttribPointer(V_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE,
					stride, reinterpret_cast<GLvoid *>(offset));

			offset += sizeof(vec3);
			glEnableVertexAttribArray(V_ATTRIB_NORMAL);
			glVertexAttribPointer(V_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE,
					stride, reinterpret_cast<GLvoid *>(offset));

			// If renderer has texture, we should enable vertex attribute for texCoord
			if (Mines[i].mHasTexture || Mines[i].mHasNorMap) {
				offset += sizeof(vec3);
				glEnableVertexAttribArray(V_ATTRIB_TEX);
				glVertexAttribPointer(V_ATTRIB_TEX, 2, GL_FLOAT, GL_FALSE,
						stride, reinterpret_cast<GLvoid *>(offset));

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, Mines[i].mTexId);
			}
			LOGI("BasicRenderer::createVbo 2\n");

			if (Mines[i].mHasNorMap) {
				// Bump mapping need to change space (world and TBN)
				// mTangentBuffer calculated by ComputeTangent() when normal texture has set
				glGenBuffers(1, &Mines[i].mVboTangents);
				glBindBuffer(GL_ARRAY_BUFFER, Mines[i].mVboTangents);
				glBufferData(GL_ARRAY_BUFFER,
						static_cast<GLsizeiptr>(Mines[i].mTangentBuffer.size()
								* sizeof(vec3)),
						&(Mines[i].mTangentBuffer.at(0)), GL_STATIC_DRAW);

				offset = 0;
				stride = sizeof(vec3);
				glEnableVertexAttribArray(V_ATTRIB_TANGENT);
				glVertexAttribPointer(V_ATTRIB_TANGENT, 3, GL_FLOAT, GL_FALSE,
						stride, reinterpret_cast<GLvoid *>(offset));

				glActiveTexture(GL_TEXTURE0 + TEX_POS_NORMAL);
				glBindTexture(GL_TEXTURE_2D, Mines[i].mTexNorId);
			}

			LOGI("BasicRenderer::createVbo 3\n");

		} else if (i == 1) {
			LOGI("BasicRenderer::createVbo 4\n");

			glEnableVertexAttribArray(V_ATTRIB_POSITION2);
			glVertexAttribPointer(V_ATTRIB_POSITION2, 3, GL_FLOAT, GL_FALSE,
					stride, reinterpret_cast<GLvoid *>(offset));

			offset += sizeof(vec3);
			glEnableVertexAttribArray(V_ATTRIB_NORMAL2);
			glVertexAttribPointer(V_ATTRIB_NORMAL2, 3, GL_FLOAT, GL_FALSE,
					stride, reinterpret_cast<GLvoid *>(offset));

			// If renderer has texture, we should enable vertex attribute for texCoord
			if (Mines[i].mHasTexture || Mines[i].mHasNorMap) {
				offset += sizeof(vec3);
				glEnableVertexAttribArray(V_ATTRIB_TEX2);
				glVertexAttribPointer(V_ATTRIB_TEX2, 2, GL_FLOAT, GL_FALSE,
						stride, reinterpret_cast<GLvoid *>(offset));

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, Mines[i].mTexId);
			}
			LOGI("BasicRenderer::createVbo 5\n");

			if (Mines[i].mHasNorMap) {
				// Bump mapping need to change space (world and TBN)
				// mTangentBuffer calculated by ComputeTangent() when normal texture has set
				glGenBuffers(1, &Mines[i].mVboTangents);
				glBindBuffer(GL_ARRAY_BUFFER, Mines[i].mVboTangents);
				glBufferData(GL_ARRAY_BUFFER,
						static_cast<GLsizeiptr>(Mines[i].mTangentBuffer.size()
								* sizeof(vec3)),
						&(Mines[i].mTangentBuffer.at(0)), GL_STATIC_DRAW);

				offset = 0;
				stride = sizeof(vec3);
				glEnableVertexAttribArray(V_ATTRIB_TANGENT2);
				glVertexAttribPointer(V_ATTRIB_TANGENT2, 3, GL_FLOAT, GL_FALSE,
						stride, reinterpret_cast<GLvoid *>(offset));

				glActiveTexture(GL_TEXTURE0 + TEX_POS_NORMAL2);
				glBindTexture(GL_TEXTURE_2D, Mines[i].mTexNorId);
			}

			LOGI("BasicRenderer::createVbo 6\n");

		} else if (i == 2) {
			LOGI("BasicRenderer::createVbo 7\n");

			glEnableVertexAttribArray(V_ATTRIB_POSITION3);
			glVertexAttribPointer(V_ATTRIB_POSITION3, 3, GL_FLOAT, GL_FALSE,
					stride, reinterpret_cast<GLvoid *>(offset));

			offset += sizeof(vec3);
			glEnableVertexAttribArray(V_ATTRIB_NORMAL3);
			glVertexAttribPointer(V_ATTRIB_NORMAL3, 3, GL_FLOAT, GL_FALSE,
					stride, reinterpret_cast<GLvoid *>(offset));

			// If renderer has texture, we should enable vertex attribute for texCoord
			if (Mines[i].mHasTexture || Mines[i].mHasNorMap) {
				offset += sizeof(vec3);
				glEnableVertexAttribArray(V_ATTRIB_TEX3);
				glVertexAttribPointer(V_ATTRIB_TEX3, 2, GL_FLOAT, GL_FALSE,
						stride, reinterpret_cast<GLvoid *>(offset));

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, Mines[i].mTexId);
			}

			LOGI("BasicRenderer::createVbo 8\n");

			if (Mines[i].mHasNorMap) {
				// Bump mapping need to change space (world and TBN)
				// mTangentBuffer calculated by ComputeTangent() when normal texture has set
				glGenBuffers(1, &Mines[i].mVboTangents);
				glBindBuffer(GL_ARRAY_BUFFER, Mines[i].mVboTangents);
				glBufferData(GL_ARRAY_BUFFER,
						static_cast<GLsizeiptr>(Mines[i].mTangentBuffer.size()
								* sizeof(vec3)),
						&(Mines[i].mTangentBuffer.at(0)), GL_STATIC_DRAW);

				offset = 0;
				stride = sizeof(vec3);
				glEnableVertexAttribArray(V_ATTRIB_TANGENT3);
				glVertexAttribPointer(V_ATTRIB_TANGENT3, 3, GL_FLOAT, GL_FALSE,
						stride, reinterpret_cast<GLvoid *>(offset));

				glActiveTexture(GL_TEXTURE0 + TEX_POS_NORMAL3);
				glBindTexture(GL_TEXTURE_2D, Mines[i].mTexNorId);
			}
			LOGI("BasicRenderer::createVbo 9\n");

		} else if (i == 3) {
			LOGI("BasicRenderer::createVbo 7\n");

			glEnableVertexAttribArray(V_ATTRIB_POSITION4);
			glVertexAttribPointer(V_ATTRIB_POSITION4, 3, GL_FLOAT, GL_FALSE,
					stride, reinterpret_cast<GLvoid *>(offset));

			offset += sizeof(vec3);
			glEnableVertexAttribArray(V_ATTRIB_NORMAL4);
			glVertexAttribPointer(V_ATTRIB_NORMAL4, 3, GL_FLOAT, GL_FALSE,
					stride, reinterpret_cast<GLvoid *>(offset));

			// If renderer has texture, we should enable vertex attribute for texCoord
			if (Mines[i].mHasTexture || Mines[i].mHasNorMap) {
				offset += sizeof(vec3);
				glEnableVertexAttribArray(V_ATTRIB_TEX4);
				glVertexAttribPointer(V_ATTRIB_TEX4, 2, GL_FLOAT, GL_FALSE,
						stride, reinterpret_cast<GLvoid *>(offset));

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, Mines[i].mTexId);
			}

			LOGI("BasicRenderer::createVbo 8\n");

			if (Mines[i].mHasNorMap) {
				// Bump mapping need to change space (world and TBN)
				// mTangentBuffer calculated by ComputeTangent() when normal texture has set
				glGenBuffers(1, &Mines[i].mVboTangents);
				glBindBuffer(GL_ARRAY_BUFFER, Mines[i].mVboTangents);
				glBufferData(GL_ARRAY_BUFFER,
						static_cast<GLsizeiptr>(Mines[i].mTangentBuffer.size()
								* sizeof(vec3)),
						&(Mines[i].mTangentBuffer.at(0)), GL_STATIC_DRAW);

				offset = 0;
				stride = sizeof(vec3);
				glEnableVertexAttribArray(V_ATTRIB_TANGENT4);
				glVertexAttribPointer(V_ATTRIB_TANGENT4, 3, GL_FLOAT, GL_FALSE,
						stride, reinterpret_cast<GLvoid *>(offset));

				glActiveTexture(GL_TEXTURE0 + TEX_POS_NORMAL4);
				glBindTexture(GL_TEXTURE_2D, Mines[i].mTexNorId);
			}
			LOGI("BasicRenderer::createVbo 9\n");

		}
	}
	LOGI("BasicRenderer::createVbo is finished \n");
}

void BasicRenderer::RebindVbo() const {
	LOGI("BasicRenderer::First_  RebindVbo\n");

	for (int i = 0; i < 4; i++) {
		LOGI("BasicRenderer::RebindVbo\n");
		glBindBuffer(GL_ARRAY_BUFFER, Mines[i].mVboVertices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Mines[i].mVboIndices);

		int stride = sizeof(VertexStruct); // stride: sizeof(float) * number of components
		int offset = 0;
		glEnableVertexAttribArray(V_ATTRIB_POSITION);
		glVertexAttribPointer(V_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, stride,
				reinterpret_cast<GLvoid *>(offset));

		offset += sizeof(vec3);
		glEnableVertexAttribArray(V_ATTRIB_NORMAL);
		glVertexAttribPointer(V_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, stride,
				reinterpret_cast<GLvoid *>(offset));

		// If renderer has texture, we should enable vertex attribute for texCoord
		if (Mines[i].mHasTexture || Mines[i].mHasNorMap) {
			offset += sizeof(vec3);
			glEnableVertexAttribArray(V_ATTRIB_TEX);
			glVertexAttribPointer(V_ATTRIB_TEX, 2, GL_FLOAT, GL_FALSE, stride,
					reinterpret_cast<GLvoid *>(offset));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Mines[i].mTexId);
		}

		if (Mines[i].mHasNorMap) {
			// Bump mapping need to change space (world and TBN)
			// mTangentBuffer calculated by ComputeTangent() when normal texture has set
			glBindBuffer(GL_ARRAY_BUFFER, Mines[i].mVboTangents);

			offset = 0;
			stride = sizeof(vec3);
			glEnableVertexAttribArray(V_ATTRIB_TANGENT);
			glVertexAttribPointer(V_ATTRIB_TANGENT, 3, GL_FLOAT, GL_FALSE,
					stride, reinterpret_cast<GLvoid *>(offset));

			glActiveTexture(GL_TEXTURE0 + TEX_POS_NORMAL);
			glBindTexture(GL_TEXTURE_2D, Mines[i].mTexNorId);
		}

	}
}

mat4 BasicRenderer::GetInverseTranspose(const mat4& m) const {
	return transpose(inverse(m));

}

void BasicRenderer::PassUniform(int i) const {
	mat4 worldMat;
	LOGI("passuniform");

	int my_rand_x = 0;
	int my_rand_y = 0;

	if (i == 0)
		worldMat = GetWorldMatrix();
	else if (i == 1) {
		my_rand_x = rand() % 5;
		my_rand_y = rand() % 5;
		if (my_rand_x == 0) {
			num2_x += 1;
		} else if (my_rand_x == 1) {
			num2_x -= 1;
		} else if (my_rand_x == 2) {
			num2_x += 3;
		} else if (my_rand_x == 3) {
			num2_x -= -2.8;
		} else if (my_rand_x == 4) {
			num2_x += 1.4;
		}
		if (my_rand_y == 0) {
			num2_y += 1;
		} else if (my_rand_y == 1) {
			num2_y -= 1;
		} else if (my_rand_y == 2) {
			num2_y += 3;
		} else if (my_rand_y == 3) {
			num2_y -= -4.1;
		} else if (my_rand_y == 4) {
			num2_y += 0.1;
		}
		if (num2_x == -500)
			num2_x = 0;
		if (num2_x == 500)
			num2_x = 0;
		if (num2_y == -500)
			num2_y = 0;
		if (num2_y == 500)
			num2_y = 0;

		worldMat = GetWorldMatrix2(num2_x, num2_y);

	} else if (i == 2) {
		num3 = num3 + 1;
		if (num3 == 500)
			num3 = 0;
		worldMat = GetWorldMatrix3(num3);

	} else if (i == 3) {
		worldMat = GetWorldMatrix4();

	}
	mat4 viewMat = mCamera->GetViewMat();
	mat4 projMat = mCamera->GetPerspectiveMat();
	mat4 invTransWorldMat = GetInverseTranspose(worldMat);

	Mines[i].mShader->SetUniform("worldMat", worldMat);
	Mines[i].mShader->SetUniform("viewMat", viewMat);
	Mines[i].mShader->SetUniform("projMat", projMat);
	Mines[i].mShader->SetUniform("invTransWorldMat", invTransWorldMat);
	Mines[i].mShader->SetUniform("s_tex0", 0);
	Mines[i].mShader->SetUniform("s_texNor", TEX_POS_NORMAL);
	Mines[i].mShader->SetUniform("s_texCube", TEX_POS_CUBEMAP);
	Mines[i].mShader->SetUniform("eyePos", mCamera->GetEye());
	Mines[i].mShader->SetUniform("lightPos", vec3(100.0f, 100.0f, 100.0f));
	Mines[i].mShader->SetUniform("materialDiff", vec3(0.8f, 1.0f, 0.7f));
	Mines[i].mShader->SetUniform("materialSpec", vec3(0.8f, 1.0f, 0.7f));
	Mines[i].mShader->SetUniform("materialAmbi", vec3(0.0f, 0.0f, 0.0f));
	Mines[i].mShader->SetUniform("materialEmit", vec3(0.0f, 0.0f, 0.0f));
	Mines[i].mShader->SetUniform("materialSh", 100.0f);
	Mines[i].mShader->SetUniform("sourceDiff", vec3(0.7f, 0.7f, 0.7f));
	Mines[i].mShader->SetUniform("sourceSpec", vec3(1.0f, 1.0f, 1.0f));
	Mines[i].mShader->SetUniform("sourceAmbi", vec3(0.0f, 0.0f, 0.0f));
}

void BasicRenderer::Draw(int i) const {
	//glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Mines[my_number].mIndexBuffer.size()), GL_UNSIGNED_SHORT, static_cast<GLvoid *>(NULL));
	LOGI("I drawed!!!!!!");

	glDrawElements(GL_TRIANGLES,
			static_cast<GLsizei>(Mines[i].mIndexBuffer.size()),
			GL_UNSIGNED_SHORT, static_cast<GLvoid *>(NULL));

}

/*****************************
 ***** Utility functions *****
 *****************************/
void BasicRenderer::LogInfo() const {
	print_gl_string("Version", GL_VERSION);
	print_gl_string("Vendor", GL_VENDOR);
	print_gl_string("Renderer", GL_RENDERER);
	print_gl_string("Extensions", GL_EXTENSIONS);
	print_gl_string("GLSLversion", GL_SHADING_LANGUAGE_VERSION);
}

void BasicRenderer::CountTickInit() const {
	//Mines[my_number].mTimer->InitTimer();
	mTimer->InitTimer();
}

void BasicRenderer::ComputeTick() {
	static double lastTick = 0;
	double currTick = mTimer->GetElapsedTime();
	//double currTick = Mines[my_number].mTimer->GetElapsedTime();

	//Mines[my_number].mDeltaTime = currTick - lastTick;
	mDeltaTime = currTick - lastTick;
	lastTick = currTick;
	//LOGI("Tick: %f\n", mDeltaTime);
}

/// Computes Tangent vector by using position and texCoord
//  These vectors will be used for making TBN rotation matrix
void BasicRenderer::ComputeTangent() {

	LOGI("ComputeTangent working!!!");
	vector<vec3> triTangents;
	vector<vec3> triTangents2;
	vector<vec3> triTangents3;
	// Compute Tangent Basis
	int j = 0;

	for (unsigned int i = 0; i < Mines[j].mIndexBuffer.size(); i += 3) {
		vec3 p0 = Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i)).pos;
		vec3 p1 = Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i + 1)).pos;
		vec3 p2 = Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i + 2)).pos;

		vec3 uv0 = vec3(
				Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i)).tex, 0);
		vec3 uv1 = vec3(
				Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i + 1)).tex,
				0);
		vec3 uv2 = vec3(
				Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i + 2)).tex,
				0);

		vec3 deltaPos1 = p1 - p0;
		vec3 deltaPos2 = p2 - p0;

		vec3 deltaUV1 = uv1 - uv0;
		vec3 deltaUV2 = uv2 - uv0;

		// Compute the tangent
		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		vec3 computedTangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)
				* r;

		triTangents.push_back(computedTangent);
		triTangents.push_back(computedTangent);
		triTangents.push_back(computedTangent);
	}

	// Initialize mTangents
	for (int i = 0; i < Mines[j].mVertexBuffer.size(); ++i) {
		Mines[j].mTangentBuffer.push_back(vec3(0));
	}

	// Accumulate tangents by indices
	for (unsigned int i = 0; i < Mines[j].mIndexBuffer.size(); ++i) {
		Mines[j].mTangentBuffer.at(Mines[j].mIndexBuffer.at(i)) =
				Mines[j].mTangentBuffer.at(Mines[j].mIndexBuffer.at(i))
						+ triTangents.at(i);
	}

	j = 1;

	for (unsigned int i = 0; i < Mines[j].mIndexBuffer.size(); i += 3) {
		vec3 p02 = Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i)).pos;
		vec3 p12 =
				Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i + 1)).pos;
		vec3 p22 =
				Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i + 2)).pos;

		vec3 uv02 = vec3(
				Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i)).tex, 0);
		vec3 uv12 = vec3(
				Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i + 1)).tex,
				0);
		vec3 uv22 = vec3(
				Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i + 2)).tex,
				0);

		vec3 deltaPos12 = p12 - p02;
		vec3 deltaPos22 = p22 - p02;

		vec3 deltaUV12 = uv12 - uv02;
		vec3 deltaUV22 = uv22 - uv02;

		// Compute the tangent
		float r2 = 1.0f
				/ (deltaUV12.x * deltaUV22.y - deltaUV12.y * deltaUV22.x);
		vec3 computedTangent2 = (deltaPos12 * deltaUV22.y
				- deltaPos22 * deltaUV12.y) * r2;

		triTangents2.push_back(computedTangent2);
		triTangents2.push_back(computedTangent2);
		triTangents2.push_back(computedTangent2);
	}

	// Initialize mTangents
	for (int i = 0; i < Mines[j].mVertexBuffer.size(); ++i) {
		Mines[j].mTangentBuffer.push_back(vec3(0));
	}

	// Accumulate tangents by indices
	for (unsigned int i = 0; i < Mines[j].mIndexBuffer.size(); ++i) {
		Mines[j].mTangentBuffer.at(Mines[j].mIndexBuffer.at(i)) =
				Mines[j].mTangentBuffer.at(Mines[j].mIndexBuffer.at(i))
						+ triTangents.at(i);
	}

	j = 2;

	for (unsigned int i = 0; i < Mines[j].mIndexBuffer.size(); i += 3) {
		vec3 p03 = Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i)).pos;
		vec3 p13 =
				Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i + 1)).pos;
		vec3 p23 =
				Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i + 2)).pos;

		vec3 uv03 = vec3(
				Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i)).tex, 0);
		vec3 uv13 = vec3(
				Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i + 1)).tex,
				0);
		vec3 uv23 = vec3(
				Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i + 2)).tex,
				0);

		vec3 deltaPos13 = p13 - p03;
		vec3 deltaPos23 = p23 - p03;

		vec3 deltaUV13 = uv13 - uv03;
		vec3 deltaUV23 = uv23 - uv03;

		// Compute the tangent
		float r3 = 1.0f
				/ (deltaUV13.x * deltaUV23.y - deltaUV13.y * deltaUV23.x);
		vec3 computedTangent3 = (deltaPos13 * deltaUV23.y
				- deltaPos23 * deltaUV13.y) * r3;

		triTangents3.push_back(computedTangent3);
		triTangents3.push_back(computedTangent3);
		triTangents3.push_back(computedTangent3);
	}

	// Initialize mTangents
	for (int i = 0; i < Mines[j].mVertexBuffer.size(); ++i) {
		Mines[j].mTangentBuffer.push_back(vec3(0));
	}

	// Accumulate tangents by indices
	for (unsigned int i = 0; i < Mines[j].mIndexBuffer.size(); ++i) {
		Mines[j].mTangentBuffer.at(Mines[j].mIndexBuffer.at(i)) =
				Mines[j].mTangentBuffer.at(Mines[j].mIndexBuffer.at(i))
						+ triTangents.at(i);
	}

	j = 3;

	for (unsigned int i = 0; i < Mines[j].mIndexBuffer.size(); i += 3) {
		vec3 p03 = Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i)).pos;
		vec3 p13 =
				Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i + 1)).pos;
		vec3 p23 =
				Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i + 2)).pos;

		vec3 uv03 = vec3(
				Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i)).tex, 0);
		vec3 uv13 = vec3(
				Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i + 1)).tex,
				0);
		vec3 uv23 = vec3(
				Mines[j].mVertexBuffer.at(Mines[j].mIndexBuffer.at(i + 2)).tex,
				0);

		vec3 deltaPos13 = p13 - p03;
		vec3 deltaPos23 = p23 - p03;

		vec3 deltaUV13 = uv13 - uv03;
		vec3 deltaUV23 = uv23 - uv03;

		// Compute the tangent
		float r3 = 1.0f
				/ (deltaUV13.x * deltaUV23.y - deltaUV13.y * deltaUV23.x);
		vec3 computedTangent3 = (deltaPos13 * deltaUV23.y
				- deltaPos23 * deltaUV13.y) * r3;

		triTangents3.push_back(computedTangent3);
		triTangents3.push_back(computedTangent3);
		triTangents3.push_back(computedTangent3);
	}

	// Initialize mTangents
	for (int i = 0; i < Mines[j].mVertexBuffer.size(); ++i) {
		Mines[j].mTangentBuffer.push_back(vec3(0));
	}

	// Accumulate tangents by indices
	for (unsigned int i = 0; i < Mines[j].mIndexBuffer.size(); ++i) {
		Mines[j].mTangentBuffer.at(Mines[j].mIndexBuffer.at(i)) =
				Mines[j].mTangentBuffer.at(Mines[j].mIndexBuffer.at(i))
						+ triTangents.at(i);
	}

}

void BasicRenderer::ImportModel(char* objSource, char* objSource2,
		char* objSource3, char* objSource4, BasicRenderer* renderer) const {
	LOGI("imported model");
	ImportModelScale(objSource, renderer, 1.0f, 0);
	ImportModelScale2(objSource2, renderer, 1.0f, 1);
	ImportModelScale3(objSource3, renderer, 1.0f, 2);
	ImportModelScale4(objSource4, renderer, 1.0f, 3);

}
void BasicRenderer::ImportModelScale(char* objSource, BasicRenderer* renderer,
		const float& scale, int i) const {
	LOGI("funcition : getObjModel\n");

	vec3 sVec = vec3(scale);

	vector<vec3> posCoords;
	vector<vec3> norCoords;
	vector<vec2> texCoords;

	vector < string > strIndexer;

	float x, y, z;
	char *line, *linePtr;

	LOGI("GOOOD");
	line = util_strtok(objSource, "\r\n", &linePtr);
	while (line) {
		char *word, *wordPtr;
		word = util_strtok(line, " ", &wordPtr);

		switch (word[0]) {
		case 'v':
			switch (word[1]) {
			case 'n': //normal coordinate
				word = util_strtok(NULL, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				z = static_cast<float>(atof(word));
				norCoords.push_back(vec3(x, y, z));
				break;
			case 't': //texture coordinate
				word = util_strtok(NULL, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				texCoords.push_back(vec2(x, y));
				break;
			default: //vertex position
				word = util_strtok(NULL, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				z = static_cast<float>(atof(word));
				posCoords.push_back(sVec * vec3(x, y, z));
				break;
			}

			break;

		case 'f':
			while ((word = util_strtok(NULL, " ", &wordPtr))) {
				string face = word;
				vector<string>::iterator findIter = find(strIndexer.begin(),
						strIndexer.end(), word);

				if (findIter != strIndexer.end()) {
					// this face already in vertex data
					renderer->Mines[i].mIndexBuffer.push_back(
							std::distance(strIndexer.begin(), findIter));
				} else {
					unsigned short face_index =
							static_cast<unsigned short>(strIndexer.size());
					renderer->Mines[i].mIndexBuffer.push_back(face_index);
					strIndexer.push_back(face);

					VertexStruct newVertex;
					char *num, *numPtr;
					num = util_strtok(word, "/", &numPtr); // position index
					newVertex.pos = posCoords.at(atoi(num) - 1);
					num = util_strtok(NULL, "/", &numPtr); // texture index
					newVertex.tex = texCoords.at(atoi(num) - 1);
					num = util_strtok(NULL, "/", &numPtr); // normal index
					newVertex.nor = norCoords.at(atoi(num) - 1);
					LOGI("yes done");
					renderer->Mines[i].mVertexBuffer.push_back(newVertex);
				}
			}

			break;

		case 'g':
			LOGI("name : %s\n", util_strtok(NULL, " ", &wordPtr));
			break;

		default:
			break;
		}
		line = util_strtok(NULL, "\r\n", &linePtr);
	}

}
void BasicRenderer::ImportModelScale2(char* objSource, BasicRenderer* renderer,
		const float& scale, int i) const {
	LOGI("funcition : getObjModel\n");

	vec3 sVec = vec3(scale);

	vector<vec3> posCoords;
	vector<vec3> norCoords;
	vector<vec2> texCoords;

	vector < string > strIndexer;

	float x, y, z;
	char *line, *linePtr;

	line = util_strtok(objSource, "\r\n", &linePtr);
	while (line) {
		char *word, *wordPtr;
		word = util_strtok(line, " ", &wordPtr);

		switch (word[0]) {
		case 'v':
			switch (word[1]) {
			case 'n': //normal coordinate
				word = util_strtok(NULL, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				z = static_cast<float>(atof(word));
				norCoords.push_back(vec3(x, y, z));
				break;
			case 't': //texture coordinate
				word = util_strtok(NULL, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				texCoords.push_back(vec2(x, y));
				break;
			default: //vertex position
				word = util_strtok(NULL, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				z = static_cast<float>(atof(word));
				posCoords.push_back(sVec * vec3(x, y, z));
				break;
			}

			break;

		case 'f':
			while ((word = util_strtok(NULL, " ", &wordPtr))) {
				string face = word;
				vector<string>::iterator findIter = find(strIndexer.begin(),
						strIndexer.end(), word);

				if (findIter != strIndexer.end()) {
					// this face already in vertex data
					renderer->Mines[i].mIndexBuffer.push_back(
							std::distance(strIndexer.begin(), findIter));
				} else {
					unsigned short face_index =
							static_cast<unsigned short>(strIndexer.size());
					renderer->Mines[i].mIndexBuffer.push_back(face_index);
					strIndexer.push_back(face);

					VertexStruct newVertex;
					char *num, *numPtr;
					num = util_strtok(word, "/", &numPtr); // position index
					newVertex.pos = posCoords.at(atoi(num) - 1);
					num = util_strtok(NULL, "/", &numPtr); // texture index
					newVertex.tex = texCoords.at(atoi(num) - 1);
					num = util_strtok(NULL, "/", &numPtr); // normal index
					newVertex.nor = norCoords.at(atoi(num) - 1);
					LOGI("yes done");
					renderer->Mines[i].mVertexBuffer.push_back(newVertex);
				}
			}

			break;

		case 'g':
			LOGI("name : %s\n", util_strtok(NULL, " ", &wordPtr));
			break;

		default:
			break;
		}
		line = util_strtok(NULL, "\r\n", &linePtr);
	}

}

void BasicRenderer::ImportModelScale3(char* objSource, BasicRenderer* renderer,
		const float& scale, int i) const {
	LOGI("funcition : getObjModel\n");

	vec3 sVec = vec3(scale);

	vector<vec3> posCoords;
	vector<vec3> norCoords;
	vector<vec2> texCoords;

	vector < string > strIndexer;

	float x, y, z;
	char *line, *linePtr;

	line = util_strtok(objSource, "\r\n", &linePtr);
	while (line) {
		char *word, *wordPtr;
		word = util_strtok(line, " ", &wordPtr);

		switch (word[0]) {
		case 'v':
			switch (word[1]) {
			case 'n': //normal coordinate
				word = util_strtok(NULL, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				z = static_cast<float>(atof(word));
				norCoords.push_back(vec3(x, y, z));
				break;
			case 't': //texture coordinate
				word = util_strtok(NULL, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				texCoords.push_back(vec2(x, y));
				break;
			default: //vertex position
				word = util_strtok(NULL, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				z = static_cast<float>(atof(word));
				posCoords.push_back(sVec * vec3(x, y, z));
				break;
			}

			break;

		case 'f':
			while ((word = util_strtok(NULL, " ", &wordPtr))) {
				string face = word;
				vector<string>::iterator findIter = find(strIndexer.begin(),
						strIndexer.end(), word);

				if (findIter != strIndexer.end()) {
					// this face already in vertex data
					renderer->Mines[i].mIndexBuffer.push_back(
							std::distance(strIndexer.begin(), findIter));
				} else {
					unsigned short face_index =
							static_cast<unsigned short>(strIndexer.size());
					renderer->Mines[i].mIndexBuffer.push_back(face_index);
					strIndexer.push_back(face);

					VertexStruct newVertex;
					char *num, *numPtr;
					num = util_strtok(word, "/", &numPtr); // position index
					newVertex.pos = posCoords.at(atoi(num) - 1);
					num = util_strtok(NULL, "/", &numPtr); // texture index
					newVertex.tex = texCoords.at(atoi(num) - 1);
					num = util_strtok(NULL, "/", &numPtr); // normal index
					newVertex.nor = norCoords.at(atoi(num) - 1);
					LOGI("yes done");
					renderer->Mines[i].mVertexBuffer.push_back(newVertex);
				}
			}

			break;

		case 'g':
			LOGI("name : %s\n", util_strtok(NULL, " ", &wordPtr));
			break;

		default:
			break;
		}
		line = util_strtok(NULL, "\r\n", &linePtr);
	}

}
void BasicRenderer::ImportModelScale4(char* objSource, BasicRenderer* renderer,
		const float& scale, int i) const {
	LOGI("funcition : getObjModel\n");

	vec3 sVec = vec3(scale);

	vector<vec3> posCoords;
	vector<vec3> norCoords;
	vector<vec2> texCoords;

	vector < string > strIndexer;

	float x, y, z;
	char *line, *linePtr;

	line = util_strtok(objSource, "\r\n", &linePtr);
	while (line) {
		char *word, *wordPtr;
		word = util_strtok(line, " ", &wordPtr);

		switch (word[0]) {
		case 'v':
			switch (word[1]) {
			case 'n': //normal coordinate
				word = util_strtok(NULL, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				z = static_cast<float>(atof(word));
				norCoords.push_back(vec3(x, y, z));
				break;
			case 't': //texture coordinate
				word = util_strtok(NULL, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				texCoords.push_back(vec2(x, y));
				break;
			default: //vertex position
				word = util_strtok(NULL, " ", &wordPtr);
				x = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				y = static_cast<float>(atof(word));
				word = util_strtok(NULL, " ", &wordPtr);
				z = static_cast<float>(atof(word));
				posCoords.push_back(sVec * vec3(x, y, z));
				break;
			}

			break;

		case 'f':
			while ((word = util_strtok(NULL, " ", &wordPtr))) {
				string face = word;
				vector<string>::iterator findIter = find(strIndexer.begin(),
						strIndexer.end(), word);

				if (findIter != strIndexer.end()) {
					// this face already in vertex data
					renderer->Mines[i].mIndexBuffer.push_back(
							std::distance(strIndexer.begin(), findIter));
				} else {
					unsigned short face_index =
							static_cast<unsigned short>(strIndexer.size());
					renderer->Mines[i].mIndexBuffer.push_back(face_index);
					strIndexer.push_back(face);

					VertexStruct newVertex;
					char *num, *numPtr;
					num = util_strtok(word, "/", &numPtr); // position index
					newVertex.pos = posCoords.at(atoi(num) - 1);
					num = util_strtok(NULL, "/", &numPtr); // texture index
					newVertex.tex = texCoords.at(atoi(num) - 1);
					num = util_strtok(NULL, "/", &numPtr); // normal index
					newVertex.nor = norCoords.at(atoi(num) - 1);
					LOGI("yes done");
					renderer->Mines[i].mVertexBuffer.push_back(newVertex);
				}
			}

			break;

		case 'g':
			LOGI("name : %s\n", util_strtok(NULL, " ", &wordPtr));
			break;

		default:
			break;
		}
		line = util_strtok(NULL, "\r\n", &linePtr);
	}

}

vec3 BasicRenderer::GetArcballVector(const vec2& point) const {
	const float RADIUS = 1.0;

	LOGI("width, height (%f, %f)\n", Mines[0].mWidth, Mines[0].mHeight);

	vec3 P = vec3(1.0 * point.x / Mines[0].mWidth * 2 - 1.0,
			1.0 * point.y / Mines[0].mHeight * 2 - 1.0, 0);
	P.y = -P.y;

	float OP_squared = P.x * P.x + P.y * P.y;

	P.z = sqrt(RADIUS * RADIUS - OP_squared); // Pythagore

	return P;
}

//////////////////// WorldMatrix2 for the wall
mat4 BasicRenderer::GetWorldMatrix2(int i, int j) const {
	LOGI("Minjae_getworldmatrix2222222222");

	mat4 rotationMat = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 200, 250, 0, 1);
	return rotationMat;
}
mat4 BasicRenderer::GetWorldMatrix3(int i) const {
	LOGI("Minjae_getworldmatrix33333333333");

	mat4 rotationMat = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -200, -250, 0,
			1);
	return rotationMat;

}
mat4 BasicRenderer::GetWorldMatrix4() const {
	LOGI("Minjae_getworldmatrix444444");

	mat4 rotationMat = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 262, 427, 0, 1);
	return rotationMat;

}
static void create_popup(Evas_Object *parent) {
	Evas_Object *popup;

	popup = elm_popup_add(parent);
	elm_popup_align_set(popup, ELM_NOTIFY_ALIGN_FILL, 1.0);
	eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK,
			eext_popup_back_cb, parent);
	evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_style_set(popup, "toast");
	elm_object_text_set(popup, "Welcome!");

	evas_object_show(popup);
}

mat4 BasicRenderer::GetWorldMatrix() const {
	static quat startRotQuat = quat_cast(mat3(1));
	static quat lastRotQuat = startRotQuat;
	static vec2 ancPts = Mines[0].mTouchPoint;	//mTouchPoint
	static bool isUpdateAnc = false;
	LOGI("Minjae_getworldmatrix");
	//if (mIsTouchOn)
	if (!mIsTouchOn) {
		if (!isUpdateAnc) {
			ancPts = Mines[0].mTouchPoint;
			isUpdateAnc = true;
			LOGI("Anchor Updated\n");
			LOGI("Anchor X, Y (%f, %f)\n", ancPts.x, ancPts.y);
		} else {
			if (ancPts.x != Mines[0].mTouchPoint.x
					|| ancPts.y != Mines[0].mTouchPoint.y) {

				if (fabs(velocity_x * (1 - 0.05)) >= 0) {
					velocity_x *= (1 - 0.05);
				} else {
					velocity_x = 0;

				}

				if (fabs(velocity_y * (1 - 0.05)) >= 0) {
					velocity_y *= (1 - 0.05);
				} else {
					velocity_y = 0;
				}

				if ((position_x - velocity_x) <= 265
						&& (position_x - velocity_x) >= -265) {
					velocity_x += (Mines[0].mTouchPoint.x - 360) / (-36.0 * 5);
				} else {
					velocity_x = -velocity_x * 1.2;
				}

				if ((position_y - velocity_y) <= 430
						&& (position_y - velocity_y) >= -430) {
					velocity_y += (Mines[0].mTouchPoint.y - 565) / (56.5 * 5);
				} else {
					velocity_y = -velocity_y * 1.2;

				}
				//------------------------------------------------------
				if ((position_x - velocity_x) >= 80) {

					if ((position_y - velocity_y) <= 390
							&& (position_y - velocity_y) >= 110) {

						if ((position_x - velocity_x) <= 90
								&& (position_x - velocity_x) >= 80) {

							velocity_x = -velocity_x * 1.2;

						} else {

							velocity_y = -velocity_y * 1.2;

						}

					}

				}

				if ((position_x - velocity_x) <= -110) {

					if ((position_y - velocity_y) <= -140
							&& (position_y - velocity_y) >= -390) {

						if ((position_x - velocity_x) <= -110
								&& (position_x - velocity_x) >= -120) {

							velocity_x = -velocity_x * 1.2;

						} else {

							velocity_y = -velocity_y * 1.2;

						}

					}

				}

				if ((position_x >= 260 && position_x <= 265)
						&& (position_y >= 425 && position_x <= 430)) {

					LOGI("You did it!");

				}

				LOGI("Mercedes X, Y (%f, %f)\n", (position_x - velocity_x), (position_y - velocity_y));
				//LOGI("Darsan X, Y (%f, %f)\n", Mines[0].mTouchPoint.x,
				//		Mines[0].mTouchPoint.y);
				// Get the vectors on the arcball
				vec3 va = GetArcballVector(ancPts);
				vec3 vb = GetArcballVector(Mines[0].mTouchPoint);

				// Get the rotation axis and the angle between the vector

				float angle = acos(glm::min(1.0f, dot(va, vb))) * 2.0f;

				vec3 axisInCameraSpace = normalize(cross(va, vb));

				mat4 cameraToObjectSpace = inverse(GetCamera()->GetViewMat());
				vec3 axisInObjectSpace = normalize(
						mat3(cameraToObjectSpace) * axisInCameraSpace);

				quat curRotQuat = angleAxis(angle, axisInObjectSpace);
				lastRotQuat = normalize(curRotQuat * startRotQuat);

			}
		}
	} else {
		startRotQuat = lastRotQuat;
		isUpdateAnc = false;
	}

	LOGI("velocity_x, Y (%f, %f)\n", velocity_x, ancPts.x);
	position_x = (position_x - velocity_x);
	position_y = (position_y - velocity_y);

	LOGI("%d", Mines[0].mIndexBuffer.size());
	////////////////////만약에 내가 컨트롤하는 옵젝트가 자동으로 움직이는 옵젝트와 충돌할시 원점으로 돌아간다.
	/*if(Mines[1].mVertexBuffer.pos.x == position_x)
	 {
	 postion_x = 0;
	 }
	 if(Mines[2].mVertexBuffer.pos.y== position_y)
	 {
	 postion_y = 0;
	 }*/
	//////////////////// collision part

	LOGI("my_position_x (%f) my_position_y (%f)\n", position_x, position_y);

	//////////////////FINISH LINE
	if (position_y > 410 && position_y < 430 && position_x < 265
			&& position_x > 245) {
		LOGI("game Win");	//finished
		gameWin = true;

		/*
		 Evas_Object* view = get_sample_view();
		 create_popup(view);
		 */

	}
	mat4 rotationMat = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, position_x,
			position_y, 0, 1) * mat4_cast(lastRotQuat);

	int ij = 0;
	while (ij <= 1000) {
		ij += 1;
	}
	return rotationMat;
}
