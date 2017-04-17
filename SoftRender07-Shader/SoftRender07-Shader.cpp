#include "App.h"
#include "SoftRenderer.h"
#include "ResourceManager.h"
#include <array>

using namespace X;

void CreateQuad(RenderOp * rop, float width, float height)
{
	static const int LINE_ELEMS = 16;
	static const int LINE_VERTS = LINE_ELEMS + 1;

	rop->vbuffer.resize(LINE_VERTS * LINE_VERTS);
	Vertex * vb = &rop->vbuffer[0];
	for (int j = 0; j <= LINE_ELEMS; ++j)
	{
		for (int i = 0; i <= LINE_ELEMS; ++i)
		{
			vb->position.x = (i / (float)LINE_ELEMS - 0.5f) * width;
			vb->position.y = (0.5f - j / (float)LINE_ELEMS) * height;
			vb->position.z = 0;
			vb->color = Float4(1, 1, 1);
			vb->uv.x = i / (float)LINE_ELEMS;
			vb->uv.y = j / (float)LINE_ELEMS;

			++vb;
		}
	}

	rop->ibuffer.resize(LINE_ELEMS * LINE_ELEMS * 6);
	short * ib = &rop->ibuffer[0];
	for (int j = 0; j < LINE_ELEMS; ++j)
	{
		for (int i = 0; i < LINE_ELEMS; ++i)
		{
			*ib++ = (j + 0) * LINE_VERTS + (i + 0);
			*ib++ = (j + 0) * LINE_VERTS + (i + 1);
			*ib++ = (j + 1) * LINE_VERTS + (i + 0);

			*ib++ = (j + 1) * LINE_VERTS + (i + 0);
			*ib++ = (j + 0) * LINE_VERTS + (i + 1);
			*ib++ = (j + 1) * LINE_VERTS + (i + 1);
		}
	}

	rop->primCount = LINE_ELEMS * LINE_ELEMS * 2;
	rop->primType = ePrimType::TRI_LIST;
}

struct MyShader : public Shader
{
	float mTime;

	virtual void VertexShader(RasterizerVertex * vo, const Vertex * vi)
	{
		vo->position = Float4(vi->position);
		vo->normal = vi->normal;
		vo->color = vi->color;
		vo->uv = vi->uv;

		// ����
		vo->position.y += vo->position.y * sin(vo->position.x * PI + mTime * PI) * 0.15f;

		// ����任
		vo->position.TransformA(SoftRenderer::Instance()->GetWorldMatrix());

		// ��ͼ�任
		vo->position.TransformA(SoftRenderer::Instance()->GetViewMatrix());

		// ͶӰ�任
		vo->position.Transform(SoftRenderer::Instance()->GetProjMatrix());
	}

	bool PixelShader(RasterizerVertex * vio)
	{
		Texture * tex = SoftRenderer::Instance()->GetTexture();
		const SamplerState & sstate = SoftRenderer::Instance()->GetSamplerState();

		if (tex != NULL)
		{
			vio->color = vio->color * tex->Tex2D(vio->uv.x, vio->uv.y, sstate);
		}

		return true;
	}
};

class MyApp : public App
{
	ResourceManager * mResourceManager;
	SoftRenderer * mRenderer;
	RenderOp * mRenderOp;
	Texture mTexture;
	MyShader mShader;
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
		mResourceManager = new ResourceManager;
		mRenderer = new SoftRenderer(mWnd, w, h);

		mResourceManager->AddFloder("../Media");

		// ����������
		mRenderOp = new RenderOp;
		CreateQuad(mRenderOp, 1, 1);

		// ��������
		mTexture.Load("X.png");
	}

	virtual void OnUpdate()
	{
		mRenderer->Begin();

		mRenderer->Clear(Float3(0, 0, 0), 1);

		// �������
		mWorldMatrix = Mat4::Identity;

		// �������
		Float3 vEye = Float3(0, 0, -2);
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

		// ���ò���
		Material material;
		material.Ambient = Float3(1, 1, 1) * 0.3f;
		material.Diffuse = Float3(1, 1, 1);
		material.Specular = Float3(1, 1, 1) * 0.0f;
		material.SpecularPower = 20;
		mRenderer->SetMaterial(material);

		// ��������
		mRenderer->SetTexture(&mTexture);

		// ���ò���״̬
		SamplerState sstate;
		sstate.Filter = eTexFilter::LINEAR;
		sstate.AddressMode = eTexAddressMode::WRAP;
		mRenderer->SetSampleState(sstate);

		// ������Ⱦ״̬
		RenderState rstate;
		rstate.CullMode = eCullMode::BACK; // ����ü�
		rstate.BlendMode = eBlendMode::OPACITY; // ��͸��
		rstate.DepthMode = eDepthMode::LESS_EQUAL; // ��Ȳ���ģʽ
		rstate.DepthMask = TRUE; // �������д
		mRenderer->SetRenderState(rstate);

		// ����Shader
		mShader.mTime = mTime;
		mRenderer->SetShader(&mShader);

		// ��Ⱦ
		mRenderer->Render(mRenderOp);

		//
		mRenderer->End();

		// �ύ
		mRenderer->Present();
	}

	virtual void OnShutdown()
	{
		delete mRenderOp;
		delete mRenderer;
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
