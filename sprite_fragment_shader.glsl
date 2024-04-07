// Source code of fragment shader
#version 130

// Attributes passed from the vertex shader
in vec4 color_interp;
in vec2 uv_interp;

// Texture sampler
uniform sampler2D onetex;

uniform int ghost_mode;
uniform float tiling;

void main()
{
    // Sample texture
    vec2 uv = uv_interp;
    uv = uv * tiling;
    uv = fract(uv);
    vec4 color = texture2D(onetex, uv);

    // Assign color to fragment
    if(ghost_mode == 0){
        gl_FragColor = vec4(color.r, color.g, color.b, color.a);
    }else{
        float luminence = color.r * 0.299 + color.g * 0.587 + color.b * 0.114;
        gl_FragColor = vec4(luminence, luminence, luminence, color.a);
    }

    // Check for transparency
    if(color.a < 0.25)
    {
         discard;
    }
}
