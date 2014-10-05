// global
uniform highp mat4 projectionMatrix;    // projection matrix
uniform highp mat4 viewMatrix;          // view matrix

// model specific
uniform highp mat4 modelMatrix;         // model matrix
uniform lowp vec4 color;               // per-vertex color
uniform mediump float aspectRatio;        // aspect ratio
uniform lowp int alignment;            // text alignment

// vertex specific
attribute highp vec4 position;          // per-vertex position
attribute mediump vec2 texCoordinate;     // per-vertex texture coordinate

varying lowp vec4 destinationColor;         // the output colors
varying mediump vec2 destinationTexCoordinate; // the output texture coordinate

void main(void)
{
    mat4 modelviewMatrix = viewMatrix * modelMatrix;

    destinationTexCoordinate = texCoordinate;
    destinationColor = color;

    vec4 scaledPosition = position;
    scaledPosition[0] *= aspectRatio;

    if (alignment == 1) // centered
    {
        scaledPosition[0] -= aspectRatio/2.0;
    }
    else if (alignment == 2) // left
    {
        scaledPosition[0] -= aspectRatio;
    }

    gl_Position = projectionMatrix * modelviewMatrix * scaledPosition;
}
