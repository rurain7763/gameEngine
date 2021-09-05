#version 150
#define MAX_DIFFUSE_NUM		3
#define MAX_SPECULAR_NUM	1

#ifdef GL_ES
precision mediump float;
#endif

#if MAX_DIFFUSE_NUM == 1
uniform sampler2D diffuse0;
#elif MAX_DIFFUSE_NUM == 2
uniform sampler2D diffuse0;
uniform sampler2D diffuse1;
#elif MAX_DIFFUSE_NUM == 3
uniform sampler2D diffuse0;
uniform sampler2D diffuse1;
uniform sampler2D diffuse2;
#elif MAX_DIFFUSE_NUM == 4
uniform sampler2D diffuse0;
uniform sampler2D diffuse1;
uniform sampler2D diffuse2;
uniform sampler2D diffuse3;
#elif MAX_DIFFUSE_NUM == 5
uniform sampler2D diffuse0;
uniform sampler2D diffuse1;
uniform sampler2D diffuse2;
uniform sampler2D diffuse3;
uniform sampler2D diffuse4;
#endif

#if MAX_SPECULAR_NUM == 1
uniform sampler2D specular0;
#elif MAX_SPECULAR_NUM == 2
uniform sampler2D specular0;
uniform sampler2D specular1;
#elif MAX_SPECULAR_NUM == 3
uniform sampler2D specular0;
uniform sampler2D specular1;
uniform sampler2D specular2;
#elif MAX_SPECULAR_NUM == 4
uniform sampler2D specular0;
uniform sampler2D specular1;
uniform sampler2D specular2;
uniform sampler2D specular3;
#elif MAX_SPECULAR_NUM == 5
uniform sampler2D specular0;
uniform sampler2D specular1;
uniform sampler2D specular2;
uniform sampler2D specular3;
uniform sampler2D specular4;
#endif

in vec3 normalV;
in vec2 uvV;

out vec4 throwColor;

void main()
{ 
	vec4 diffuse = texture(diffuse0, uvV);
	vec4 specular = texture(specular0, uvV);

	throwColor = vec4(1.0);
	//throwColor = vec4((diffuse + specular) * throwColor);
}

