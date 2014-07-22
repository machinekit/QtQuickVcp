uniform bool stipple;  // enable or disable stippling
uniform float stippleLength;

varying lowp vec4 destinationColor;
varying lowp vec4 currentPosition;
varying lowp vec4 sourcePosition;

void main() {
    const float pi = 3.1415;
    if (stipple && (sin(pi*abs(distance(sourcePosition.xyz, currentPosition.xyz))/stippleLength) < 0.0)) {
        gl_FragColor = vec4(0,0,0,0);
    } else {
        gl_FragColor = destinationColor;
    }
}
