
Uniform
{
	u_WorldMatrix WORLD_MATRIX
	u_ViewMatrix VIEW_MATRIX
	u_ProjMatrix PROJ_MATRIX
}

VertexShader {

	attribute vec4 POSITION;
	attribute vec4 COLOR;

	varying vec4 v_color;

	uniform mat4 u_WorldMatrix;
	uniform mat4 u_ViewMatrix;
	uniform mat4 u_ProjMatrix;

	void main()
	{
		// ����任
		gl_Position = u_WorldMatrix * POSITION;

		// ����任
		gl_Position = u_ViewMatrix * gl_Position;

		// ͶӰ�任
		gl_Position = u_ProjMatrix * gl_Position;

		// �����ɫ
		v_color = COLOR;
	}

}

PixelShader {

	varying vec4 v_color;

	void main()
	{
		gl_FragColor = v_color;
	}

}