#include "App.h"
#include "SoftRenderer.h"

using namespace X;

class MyApp : public App
{
	SoftRenderer * mRenderer;
	int mPrimType;
	Vertex mQuad[4];
	int mLineList[8];
	int mTriList[8];

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
		Vertex * vb = mQuad;
		vb[0].position = Float3(-1, +1, 0) * 0.5f;
		vb[1].position = Float3(+1, +1, 0) * 0.5f;
		vb[2].position = Float3(-1, -1, 0) * 0.5f;
		vb[3].position = Float3(+1, -1, 0) * 0.5f;
		vb[0].color = Float4(1, 0, 0, 1);
		vb[1].color = Float4(1, 0, 0, 1);
		vb[2].color = Float4(0, 1, 0, 1);
		vb[3].color = Float4(0, 1, 0, 1);

		// �������б�
		int * ib = mTriList;
		{
			*ib++ = 0, *ib++ = 1, *ib++ = 2;
			*ib++ = 2, *ib++ = 1, *ib++ = 3;
		}

		//  ���б�
		ib = mLineList;
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

		Vertex * vb = mQuad;
		vb[0].position = Float3(-1, +1, 0) * 0.5f;
		vb[1].position = Float3(+1, +1, 0) * 0.5f;
		vb[2].position = Float3(-1, -1, 0) * 0.5f;
		vb[3].position = Float3(+1, -1, 0) * 0.5f;

		mQuad[0].position.TransformA(form);
		mQuad[1].position.TransformA(form);
		mQuad[2].position.TransformA(form);
		mQuad[3].position.TransformA(form);

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
				mRenderer->Render(vb, 4, mTriList, 2, ePrimType::TRI_LIST);
				break;

			case ePrimType::LINE_LIST:
				mRenderer->Render(vb, 4, mLineList, 4, ePrimType::LINE_LIST);
				break;

			case ePrimType::POINT_LIST:
				mRenderer->Render(vb, 4, NULL, 4, ePrimType::POINT_LIST);
				break;
		}

		mRenderer->End();

		// �ύ
		mRenderer->Present();
	}

	virtual void OnShutdown()
	{
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
