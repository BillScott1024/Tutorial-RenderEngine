#include "App.h"
#include "GLRenderer.h"
#include "ResourceManager.h"

using namespace X;

struct Vertex
{
	Float3 position;
	Float4 color;
};

class MyApp : public App
{
	ResourceManager * mResourceManager;
	GLRenderer * mRenderer;
	Shader::Ptr mShader;
	RenderOp * mRenderOp;
	IRenderBuffer::Ptr mTriList;
	IRenderBuffer::Ptr mLineList;
	int mPrimType;

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
		mShader = mRenderer->LoadShader("tutorial02.shader");

		// ����һ��������
		mRenderOp = new RenderOp;
		mRenderOp->vlayout.push(eVertexSemantic::POSITION, eVertexFormat::FLOAT3);
		mRenderOp->vlayout.push(eVertexSemantic::COLOR, eVertexFormat::FLOAT4);
		mRenderOp->vbuffer = mRenderer->CreateBuffer(sizeof(Vertex) * 4);
		mRenderOp->primCount = 2;
		mRenderOp->primType = ePrimType::TRI_LIST;

		Vertex * vb = (Vertex *)mRenderOp->vbuffer->Lock(eLockFlag::WRITE);
		{
			vb[0].position = Float3(-1, +1, 0) * 0.5f;
			vb[1].position = Float3(+1, +1, 0) * 0.5f;
			vb[2].position = Float3(-1, -1, 0) * 0.5f;
			vb[3].position = Float3(+1, -1, 0) * 0.5f;
			vb[0].color = Float4(1, 0, 0, 1);
			vb[1].color = Float4(1, 0, 0, 1);
			vb[2].color = Float4(0, 1, 0, 1);
			vb[3].color = Float4(0, 1, 0, 1);
		}
		mRenderOp->vbuffer->Unlock();

		// �������б�
		mTriList = mRenderer->CreateBuffer(6 * sizeof(short));
		short * ib = (short *)mTriList->Lock(eLockFlag::WRITE);
		{
			*ib++ = 0, *ib++ = 1, *ib++ = 2;
			*ib++ = 2, *ib++ = 1, *ib++ = 3;
		}
		mTriList->Unlock();

		// ���б�
		mLineList = mRenderer->CreateBuffer(8 * sizeof(short));
		ib = (short *)mLineList->Lock(eLockFlag::WRITE);
		{
			*ib++ = 0, *ib++ = 1;
			*ib++ = 1, *ib++ = 3;
			*ib++ = 3, *ib++ = 2;
			*ib++ = 2, *ib++ = 0;
		}
		mLineList->Unlock();

		// ���б�ֱ��ʹ�ö�����б����ö��ⶨ��

		mPrimType = ePrimType::TRI_LIST;
	}

	virtual void OnUpdate()
	{
		// ��Ⱦ��ʼ
		mRenderer->Begin();

		// ����
		mRenderer->Clear(Float4(0, 0, 0), 1);

		// ����ʱ����ת
		Mat4 mWorldMatrix;
		mWorldMatrix.MakeRotation(Float3(0, 0, 1), PI * mTime * 0.3f);

		// ���ñ任����
		mRenderer->SetWorldMatrix(mWorldMatrix);

		// ����Shader Uniform
		float aspect = (float)mHeight / mWidth;
		mShader->SetUniform("u_Asecpt", aspect, 0, 0, 0);

		// ����Shader
		mRenderer->SetShader(mShader);

		// ��ȾͼԪ
		switch (mPrimType)
		{
			case ePrimType::TRI_LIST:
				mRenderOp->ibuffer = mTriList;
				mRenderOp->primCount = 2;
				mRenderOp->primType = ePrimType::TRI_LIST;
				mRenderer->Render(mRenderOp);
				break;

			case ePrimType::LINE_LIST:
				mRenderOp->ibuffer = mLineList;
				mRenderOp->primCount = 4;
				mRenderOp->primType = ePrimType::LINE_LIST;
				mRenderer->Render(mRenderOp);
				break;

			case ePrimType::POINT_LIST:
				mRenderOp->ibuffer = NULL;
				mRenderOp->primCount = 4;
				mRenderOp->primType = ePrimType::POINT_LIST;
				mRenderer->Render(mRenderOp);
				break;
		}

		// ��Ⱦ����
		mRenderer->End();

		// �ύ
		mRenderer->Present();
	}

	virtual void OnShutdown()
	{
		mTriList = NULL;
		mLineList = NULL;
		delete mRenderOp;
		delete mRenderer;
		delete mResourceManager;
	}

	virtual void OnResize(int w, int h)
	{
		mRenderer->Resize(w, h);
	}

	virtual void OnKeyUp(int vk)
	{
		if (vk == VK_TAB)
		{
			mPrimType += 1;
			mPrimType %= 3;
		}
	}
};

void CreateApplication(App ** ppApp)
{
	*ppApp = new MyApp;
}
