// global
uniform highp mat4 projectionMatrix;    // projection matrix
uniform highp mat4 viewMatrix;          // view matrix

// line specific
uniform highp mat4 modelMatrix;         // model matrix
uniform lowp vec4 color;          // per-vertex color

// vertex specific
attribute highp vec4 position;    // per-vertex position
uniform bool stipple;       // enable or disable stippling

varying lowp vec4 destinationColor;
varying highp vec4 currentPosition;
varying highp vec4 sourcePosition;

void main() {
    highp mat4 modelviewMatrix = viewMatrix * modelMatrix;

    destinationColor = color;

    if (stipple)
    {
        const highp vec4 sourcePoint = vec4(0.0, 0.0, 0.0, 1.0);
        sourcePosition = projectionMatrix * modelviewMatrix * sourcePoint;
    }

    currentPosition = projectionMatrix * modelviewMatrix * position;

    gl_Position = currentPosition;
}
