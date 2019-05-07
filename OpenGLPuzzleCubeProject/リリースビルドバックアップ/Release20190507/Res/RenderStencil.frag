#version 410

uniform vec4 fillColor = vec4(1,1,1,1);

layout(location=0) out vec4 fragStencil;

void main(){

	fragStencil = fillColor;
}