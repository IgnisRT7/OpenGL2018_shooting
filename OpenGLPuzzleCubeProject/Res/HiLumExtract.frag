#version 410

layout(location=0) in vec2 inTexCoord;

out vec4 fragColor;

uniform sampler2D colorSampler;

/**
*	‹P“x‚ğŒvZ‚·‚é
*
*	@param rgb RGBƒJƒ‰[
*
*	@return rgb‚ğITU-BT.2020‚É]‚Á‚Ä•ÏŠ·‚µ‚½‹P“x‚Ì‘Î”
*/
float luminance(vec3 rgb){

	return log(dot(rgb, vec3(0.2627, 0.678, 0.0593)) + 0.0001);
}

void main(){

	vec3 threshould = vec3(1.0);
	vec4 ts;
	
	ts.xy = vec2(1.0) / vec2(textureSize(colorSampler,0));
	ts.zw = -ts.xy;

	vec3 rgb;
	rgb = texture(colorSampler, inTexCoord + ts.xy).rgb;

	fragColor.a = luminance(rgb);
	fragColor.rgb = max(rgb, threshould) - threshould;
	rgb = texture(colorSampler, inTexCoord + ts.zy).rgb;

	fragColor.a += luminance(rgb);
	fragColor.rgb += max(rgb,threshould) - threshould;
	rgb = texture(colorSampler, inTexCoord + ts.xw).rgb;

	fragColor.a += luminance(rgb);
	fragColor.rgb += max(rgb,threshould) - threshould;	
	rgb = texture(colorSampler, inTexCoord + ts.zw).rgb;

	fragColor.a += luminance(rgb);
	fragColor.rgb += max(rgb,threshould) - threshould;	


	fragColor.rgb *= 1.0 / 4.0;
}