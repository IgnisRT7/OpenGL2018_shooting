#version 410

uniform vec4 stencilColor = vec4(1,1,0,1);

layout(location=0) out vec4 fragStencil;

void main(){

	fragStencil = stencilColor;
}