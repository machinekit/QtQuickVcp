attribute vec4 position;
attribute vec4 sourceColor;
uniform mat4 projection;
uniform mat4 modelview;

varying vec4 destinationColor;

void main(void) {
    destinationColor = sourceColor;
    gl_Position = projection * modelview * position;
}
