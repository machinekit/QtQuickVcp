uniform lowp sampler2D texture;             // The input texture.

varying lowp vec4 destinationColor;         // the output colors
varying mediump vec2 destinationTexCoordinate; // the output texture coordinate

// selection mode
uniform lowp vec4 idColor;             // color to be used for selection mode
uniform bool selectionMode;       // enables or disables the selection mode

void main(void)
{
    if (!selectionMode)
    {
        gl_FragColor = destinationColor * texture2D(texture, destinationTexCoordinate);
    }
    else
    {
        gl_FragColor = idColor * texture2D(texture, destinationTexCoordinate);
    }
}
