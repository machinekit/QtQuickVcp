// global
uniform mat4 projectionMatrix;    // projection matrix
uniform mat4 viewMatrix;          // view matrix

// model specific
uniform mat4 modelMatrix;         // model matrix
uniform vec4 color;               // per-vertex color
uniform float aspectRatio;        // aspect ratio

// vertex specific
attribute vec4 position;          // per-vertex position
attribute vec2 texCoordinate;     // per-vertex texture coordinate

varying vec4 destinationColor;         // the output colors
varying vec2 destinationTexCoordinate; // the output texture coordinate

void main(void)
{
    mat4 modelviewMatrix = viewMatrix * modelMatrix;

    destinationTexCoordinate = texCoordinate;
    destinationColor = color;

    vec4 scaledPosition = position;
    scaledPosition[0] *= aspectRatio;
    gl_Position = projectionMatrix * modelviewMatrix * scaledPosition;
}
