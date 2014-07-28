// global
uniform mat4 projectionMatrix;    // projection matrix
uniform mat4 viewMatrix;          // view matrix

// line specific
uniform mat4 modelMatrix;         // model matrix
uniform vec4 color;               // per-vertex color

// vertex specific
attribute vec4 position;    // per-vertex position
uniform bool stipple;       // enable or disable stippling

varying vec4 destinationColor;
varying vec4 currentPosition;
varying vec4 sourcePosition;

void main() {
    mat4 modelviewMatrix = viewMatrix * modelMatrix;

    destinationColor = color;

    if (stipple)
    {
        const vec4 sourcePoint = vec4(0.0, 0.0, 0.0, 1.0);
        sourcePosition = projectionMatrix * modelviewMatrix * sourcePoint;
    }

    currentPosition = projectionMatrix * modelviewMatrix * position;

    gl_Position = currentPosition;
}
