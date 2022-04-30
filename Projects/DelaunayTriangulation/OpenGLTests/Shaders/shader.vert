#version 330													

uniform mat4 model;
uniform mat4 projection;
layout (location = 0) in vec3 pos;

out vec4 vColor;
void main(){
	gl_Position = model * vec4(pos.x, pos.y, pos.z, 1.0);
	gl_PointSize = 3.0;
	vColor = vec4(0.0, 0.0, 0.0, 1.0);
}