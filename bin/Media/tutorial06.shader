
Uniform
{
	u_WorldMatrix WORLD_MATRIX
	u_ViewMatrix VIEW_MATRIX
	u_ProjMatrix PROJ_MATRIX

	u_Time FLOAT1
}

Sampler
{
	// ע��Index��Ӧ��SetSamplerTexture�Ĳ���
	Index 0
	Name u_Sampler0;
	Filter LINEAR
	AdressMode WRAP
	BorderColor 0 0 0 0
}

VertexShader {

	attribute vec4 POSITION;
	attribute vec4 COLOR;
	attribute vec2 TEXCOORD0;

	varying vec2 v_uv;

	uniform mat4 u_WorldMatrix;
	uniform mat4 u_ViewMatrix;
	uniform mat4 u_ProjMatrix;

	uniform float u_Time;

	void main()
	{
		const float PI = 3.1415926;

		// ����
		POSITION.y += POSITION.y * sin(POSITION.x * PI + u_Time * PI) * 0.15;

		// ����任
		gl_Position = u_WorldMatrix * POSITION;

		// ����任
		gl_Position = u_ViewMatrix * gl_Position;

		// ͶӰ�任
		gl_Position = u_ProjMatrix * gl_Position;

		// �����������
		v_uv = TEXCOORD0;
	}

}

PixelShader {

	varying vec2 v_uv;

	uniform sampler2D u_Sampler0;

	void main()
	{
		// ��������
		vec4 texColor = texture2D(u_Sampler0, v_uv);

		// �����ɫ
		gl_FragColor = texColor;
	}

}