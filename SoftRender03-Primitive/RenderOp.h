#pragma once

#include "XCore.h"

namespace X {

	struct Vertex
	{
		Float3 position;	// λ��
		Float3 normal;		// ����
		Float4 color;		// ��ɫ
		Float2 uv;			// ��������
	};

	struct RasterizerVertex
	{
		Float4 position;	// λ��
		Float3 normal;		// ����
		Float4 color;		// ��ɫ
		Float2 uv;			// ��������

		static void Lerp(RasterizerVertex & v, const RasterizerVertex & a, const RasterizerVertex & b, float k)
		{
			v.position = a.position + (b.position - a.position) * k;
			v.normal = a.normal + (b.normal - a.normal) * k;
			v.color = a.color + (b.color - a.color) * k;
			v.uv = a.uv + (b.uv - a.uv) * k;
		}
	};

	struct ePrimType
	{
		enum {
			POINT_LIST,
			LINE_LIST,
			TRI_LIST,
		};
	};

	struct RenderOp
	{
		std::vector<Vertex> vbuffer;
		std::vector<short> ibuffer;
		int primType;
		int primCount;
	};

}