#version 100
precision mediump float;

uniform sampler2D uTexture;
varying vec2 vTexCoord;

void main() {
    vec4 color = texture2D(uTexture, vTexCoord);
    
    // Convert to grayscale
    float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
    
    // Simple edge detection using Sobel operator
    vec2 texelSize = vec2(1.0) / vec2(textureSize(uTexture, 0));
    
    float tl = texture2D(uTexture, vTexCoord + vec2(-texelSize.x, -texelSize.y)).r;
    float tm = texture2D(uTexture, vTexCoord + vec2(0.0, -texelSize.y)).r;
    float tr = texture2D(uTexture, vTexCoord + vec2(texelSize.x, -texelSize.y)).r;
    float ml = texture2D(uTexture, vTexCoord + vec2(-texelSize.x, 0.0)).r;
    float mr = texture2D(uTexture, vTexCoord + vec2(texelSize.x, 0.0)).r;
    float bl = texture2D(uTexture, vTexCoord + vec2(-texelSize.x, texelSize.y)).r;
    float bm = texture2D(uTexture, vTexCoord + vec2(0.0, texelSize.y)).r;
    float br = texture2D(uTexture, vTexCoord + vec2(texelSize.x, texelSize.y)).r;
    
    float sobelX = tl + 2.0 * ml + bl - tr - 2.0 * mr - br;
    float sobelY = tl + 2.0 * tm + tr - bl - 2.0 * bm - br;
    
    float edge = sqrt(sobelX * sobelX + sobelY * sobelY);
    edge = step(0.1, edge);
    
    gl_FragColor = vec4(edge * vec3(0.0, 1.0, 1.0), 1.0);
}
