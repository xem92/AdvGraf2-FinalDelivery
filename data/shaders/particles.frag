#version 330

//out color
out vec4 fragColor;

uniform sampler2D u_diffuse_map;

void main(){
    fragColor = texture(u_diffuse_map, gl_PointCoord);
}
