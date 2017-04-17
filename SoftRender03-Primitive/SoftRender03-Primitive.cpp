#include "App.h"
#include "SoftRenderer.h"

using namespace X;

class MyApp : public App
{
	SoftRenderer * mRenderer;
	int mPrimType;
	RenderOp * mRenderOp;
	std::vector<short> mTriList;
	std::vector<short> mLineList;

public:
	MyApp()
	{
	}

	virtual ~MyApp()
	{
	}

	virtual void OnInit(int w, int h)
	{
		mRenderer = new SoftRenderer(mWnd, w, h);

		// ����������
		mRenderOp = new RenderOp;
		mRenderOp->vbuffer.resize(4);
		mRenderOp->primCount = 0;
		mRenderOp->primType = ePrimType::TRI_LIST;

		Vertex * vb = &mRenderOp->vbuffer[0];
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

		// �������б�
		mTriList.resize(6);
		short * ib = &mTriList[0];
		{
			*ib++ = 0, *ib++ = 1, *ib++ = 2;
			*ib++ = 2, *ib++ = 1, *ib++ = 3;
		}

		//  ���б�
		mLineList.resize(8);
		ib = &mLineList[0];
		{
			*ib++ = 0, *ib++ = 1;
			*ib++ = 1, *ib++ = 3;
			*ib++ = 3, *ib++ = 2;
			*ib++ = 2, *ib++ = 0;
		}

		// ���б�ֱ��ʹ�ö�����б����ö��ⶨ��

		//
		mPrimType = ePrimType::TRI_LIST;
	}

	virtual void OnUpdate()
	{
		mRenderer->Begin();

		// ����
		mRenderer->Clear(Float3(0, 0, 0), 1);
		
		// ����ʱ����ת
		Mat4 form;
		form.MakeRotation(Float3(0, 0, 1), PI * mTime * 0.3f);

		Vertex * vb = &mRenderOp->vbuffer[0];
		vb[0].position = Float3(-1, +1, 0) * 0.5f;
		vb[1].position = Float3(+1, +1, 0) * 0.5f;
		vb[2].position = Float3(-1, -1, 0) * 0.5f;
		vb[3].position = Float3(+1, -1, 0) * 0.5f;

		vb[0].position.TransformA(form);
		vb[1].position.TransformA(form);
		vb[2].position.TransformA(form);
		vb[3].position.TransformA(form);

		// �����ݺ��
		float aspect = (float)mHeight / mWidth;
		vb[0].position *= Float3(aspect, 1, 0);
		vb[1].position *= Float3(aspect, 1, 0);
		vb[2].position *= Float3(aspect, 1, 0);
		vb[3].position *= Float3(aspect, 1, 0);

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
				mRenderOp->ibuffer.clear();
				mRenderOp->primCount = 4;
				mRenderOp->primType = ePrimType::POINT_LIST;
				mRenderer->Render(mRenderOp);
				break;
		}

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
