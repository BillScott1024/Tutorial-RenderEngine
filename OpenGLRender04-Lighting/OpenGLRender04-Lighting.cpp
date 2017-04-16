#include "App.h"
#include "GLRenderer.h"
#include "ResourceManager.h"

using namespace X;

struct Vertex
{
	Float3 position;
	Float3 normal;
	Float4 color;
};

void CreateSphere(RenderOp * rop, int rings, int segments, float radius)
{
	int vertexCount = (rings + 1) * (segments + 1);
	int indexCount = rings * segments * 6;

	// vertex layout
	rop->vlayout.push(eVertexSemantic::POSITION, eVertexFormat::FLOAT3);
	rop->vlayout.push(eVertexSemantic::NORMAL, eVertexFormat::FLOAT3);
	rop->vlayout.push(eVertexSemantic::COLOR, eVertexFormat::FLOAT4);

	// vertex buffer
	rop->vbuffer = IRenderer::Instance()->CreateBuffer(vertexCount * sizeof(Vertex));
	Vertex * vert = (Vertex *)rop->vbuffer->Lock(eLockFlag::WRITE);
	{
		float fTileRingAngle = (PI / rings);
		float fTileSegAngle = (PI * 2 / segments);
		float r;
		short i, j;
		Float3 pos, normal;

		for (i = 0; i <= rings; ++i)
		{
			r = radius * sin(i * fTileRingAngle);
			pos.y = radius * cos(i * fTileRingAngle);

			for (j = 0; j <= segments; ++j)
			{
				pos.x = r * cos(j * fTileSegAngle);
				pos.z = r * sin(j * fTileSegAngle);
				normal = pos; normal.Normalize();

				vert->position = pos;
				vert->normal = normal;
				//vert->color = Float3(1, 1, 1);
				vert->color = (normal + Float3(1, 1, 1)) / 2;
				++vert;
			}
		}
	}
	rop->vbuffer->Unlock();

	// index buffer
	rop->ibuffer = IRenderer::Instance()->CreateBuffer(indexCount * sizeof(short));
	short * indices = (short *)rop->ibuffer->Lock(eLockFlag::WRITE);
	{
		short row = 0, row_n = 0;
		short i, j;

		for (i = 0; i < rings; ++i)
		{
			row_n = row + segments + 1;

			for (j = 0; j < segments; ++j)
			{
				*indices++ = row + j;
				*indices++ = row + j + 1;
				*indices++ = row_n + j;

				*indices++ = row_n + j;
				*indices++ = row + j + 1;
				*indices++ = row_n + j + 1;

			}

			row += segments + 1;
		}
	}
	rop->ibuffer->Unlock();

	rop->primCount = indexCount / 3;
	rop->primType = ePrimType::TRI_LIST;
}

class MyApp : public App
{
	ResourceManager * mResourceManager;
	GLRenderer * mRenderer;
	Shader::Ptr mShader;
	RenderOp * mRenderOp;
	Mat4 mWorldMatrix;
	Mat4 mViewMatrix;
	Mat4 mProjMatrix;

public:
	MyApp()
	{
	}

	virtual ~MyApp()
	{
	}

	virtual void OnInit(int w, int h)
	{
		// ������Դ������
		mResourceManager = new ResourceManager;
		mResourceManager->AddFloder("../Media");

		// ������Ⱦ��
		mRenderer = new GLRenderer(mWnd, w, h);

		// ����Shader
		mShader = mRenderer->LoadShader("tutorial04.shader");

		// ��������
		mRenderOp = new RenderOp;
		CreateSphere(mRenderOp, 30, 30, 1);
	}

	virtual void OnUpdate()
	{
		// ��Ⱦ��ʼ
		mRenderer->Begin();

		// ����
		mRenderer->Clear(Float4(0, 0, 0), 1);

		// �������
		mWorldMatrix.MakeRotation(Float3(0, 1, 0), PI * mTime * 0.3f);

		// �������
		Float3 vEye = Float3(0, 3, -3);
		Float3 vAt = Float3(0, 0, 0);
		Float3 vUp = Float3(0, 1, 0);
		mViewMatrix.MakeViewTM(vEye, vAt, vUp);

		// ͶӰ����
		float nearClip = 0.1f;
		float farClip = 1000.0f;
		float fovy = PI / 3;
		float aspect = mWidth / (float)mHeight;
		mProjMatrix.MakePerspectiveTM(fovy, aspect, nearClip, farClip);

		// ���ñ任����
		mRenderer->SetWorldMatrix(mWorldMatrix);
		mRenderer->SetViewMatrix(mViewMatrix);
		mRenderer->SetProjMatrix(mProjMatrix);

		// �������
		Float3 vDir = vAt - vEye;
		vDir.Normalize();
		mRenderer->SetCamera(vEye, vDir);

		// ���õƹ�
		Light light;
		light.Type = Light::DIRECTION;
		light.Direction = Float3(-1, -1, 1);
		light.Diffuse = Float3(1, 1, 1);
		light.Specular = Float3(1, 1, 1);
		light.Direction.Normalize();
		mRenderer->SetLight(&light);

		// ���ò���
		Material material;
		material.Ambient = Float3(1, 0, 0) * 0.3f;
		material.Diffuse = Float3(1, 0, 0);
		material.Specular = Float3(1, 1, 1) * 0.5f;
		material.SpecularPower = 20;
		mRenderer->SetMaterial(material);

		// ����Shader
		Float3 emissive = material.Emissive;
		Float3 ambient = material.Ambient;
		Float3 diffuse = material.Diffuse * light.Diffuse;
		Float3 specular = material.Specular * light.Specular;
		float specularPower = material.SpecularPower;

		mShader->SetUniform("u_CameraPos", mRenderer->GetCameraPos());
		mShader->SetUniform("u_LightDir", -mRenderer->GetLight()->Direction);
		mShader->SetUniform("u_Emissive", emissive);
		mShader->SetUniform("u_Ambient", ambient);
		mShader->SetUniform("u_Diffuse", diffuse);
		mShader->SetUniform("u_Specular", specular);
		mShader->SetUniform("u_SpecularPower", specularPower, 0, 0, 1);

		mRenderer->SetShader(mShader);

		// ������Ⱦ״̬
		RenderState rstate;
		rstate.CullMode = eCullMode::BACK; // ����ü�
		rstate.BlendMode = eBlendMode::OPACITY; // ��͸��
		rstate.DepthMode = eDepthMode::LESS_EQUAL; // ��Ȳ���ģʽ
		rstate.DepthMask = TRUE; // �������д
		mRenderer->SetRenderState(rstate);

		// ��ȾͼԪ
		mRenderer->Render(mRenderOp);

		// ��Ⱦ����
		mRenderer->End();

		// �ύ
		mRenderer->Present();
	}

	virtual void OnShutdown()
	{
		delete mRenderOp;
		delete mRenderer;
		delete mResourceManager;
	}

	virtual void OnResize(int w, int h)
	{
		mRenderer->Resize(w, h);
	}

};

void CreateApplication(App ** ppApp)
{
	*ppApp = new MyApp;
}
