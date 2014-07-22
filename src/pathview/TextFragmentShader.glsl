uniform sampler2D texture;             // The input texture.

varying vec4 destinationColor;         // the output colors
varying vec2 destinationTexCoordinate; // the output texture coordinate

void main(void)
{
    gl_FragColor = destinationColor * texture2D(texture, destinationTexCoordinate);
}
