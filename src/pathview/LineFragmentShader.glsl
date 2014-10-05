uniform bool stipple;  // enable or disable stippling
uniform lowp float stippleLength;

// selection mode
uniform lowp vec4 idColor;             // color to be used for selection mode
uniform bool selectionMode;       // enables or disables the selection mode

varying lowp vec4 destinationColor;
varying highp vec4 currentPosition;
varying highp vec4 sourcePosition;

void main() {
    const mediump float pi = 3.1415;
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
