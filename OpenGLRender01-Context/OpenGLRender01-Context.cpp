#include "App.h"
#include "GLRenderer.h"

using namespace X;

class MyApp : public App
{
	GLRenderer * mRenderer;

public:
	MyApp()
	{
	}

	virtual ~MyApp()
	{
	}

	virtual void OnInit(int w, int h)
	{
		mRenderer = new GLRenderer(mWnd, w, h);
	}

	virtual void OnUpdate()
	{
		// ��Ⱦ��ʼ
		mRenderer->Begin();

		// ����
		mRenderer->Clear(Float4(1, 0, 0), 1);

		// ��Ⱦ����
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
};

void CreateApplication(App ** ppApp)
{
	*ppApp = new MyApp;
}
