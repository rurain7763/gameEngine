#version 150

#ifdef GL_ES
precision mediump float;
#endif

uniform mat4 tvpMat;
uniform mat4 transMat;

out vec3 worldPos;
out vec3 normalV;
out vec2 uvV;

in vec3 position;
in vec3 normal;
in vec2 uv;

void main()
{
	gl_Position = tvpMat * vec4(position, 1.0);
	uvV = uv;
	normalV = mat3(transpose(inverse(transMat))) * normal;
	worldPos = (transMat * vec4(position, 1.0)).xyz;
}

