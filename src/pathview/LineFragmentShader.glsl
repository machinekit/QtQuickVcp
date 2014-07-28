uniform bool stipple;  // enable or disable stippling
uniform float stippleLength;

// selection mode
uniform vec4 idColor;             // color to be used for selection mode
uniform bool selectionMode;       // enables or disables the selection mode

varying lowp vec4 destinationColor;
varying lowp vec4 currentPosition;
varying lowp vec4 sourcePosition;

void main() {
    const float pi = 3.1415;
    if (stipple && (sin(pi*abs(distance(sourcePosition.xyz, currentPosition.xyz))/stippleLength) < 0.0))
    {
        gl_FragColor = vec4(0,0,0,0);
    }
    else if (!selectionMode)
    {
        gl_FragColor = destinationColor;
    }
    else
    {
        gl_FragColor = idColor;
    }
}
