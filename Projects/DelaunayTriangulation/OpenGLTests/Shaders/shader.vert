#version 330													

uniform mat4 model;
uniform mat4 projection;
layout (location = 0) in vec3 pos;

out vec4 vColor;
void main(){
	gl_Position = projection * model * vec4(pos.x, pos.y, pos.z, 1.0);
	vColor = vec4(clamp(2 * pos, 0.0f, 1.0f), 1.0);
}