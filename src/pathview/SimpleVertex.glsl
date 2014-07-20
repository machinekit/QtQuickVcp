uniform mat4 projectionMatrix;    // projection matrix
uniform mat4 modelMatrix;         // modelview matrix
uniform mat4 viewMatrix;          // view matrix
uniform vec3 lightPos;            // position of the light source
uniform vec4 color;               // per-vertex color

attribute vec4 position;    // per-vertex position
attribute vec3 normal;      // per-vertex normal information

varying vec4 destinationColor;  // the output colors

void main(void) {
    mat4 modelviewMatrix = viewMatrix * modelMatrix;
    vec3 modelViewVertex = vec3(modelviewMatrix * position);          // transform vertex into eye space
    vec3 modelViewNormal = vec3(modelviewMatrix * vec4(normal, 0.0));            // transform normals orientation into eye space
    float distance = length(lightPos - modelViewVertex);        // will be used for attenuation
    vec3 lightVector = normalize(lightPos - modelViewVertex);   // get a lighting direction vector from the light to the vertex
    // Calculate the dot product of the light vector and vertex normal.
    // If the normal and light vector are pointing in the same direction then it will get max illumination.
    float diffuse = max(dot(modelViewNormal, lightVector), 0.1);// right hand value is ambience light
    diffuse = diffuse * (1.0 / (1.0 + (0.01 * distance * distance)));   // Attenuate the light based on distance

    destinationColor = color * diffuse;
    destinationColor[3] = color[3]; // correct alpha
    gl_Position = projectionMatrix * modelviewMatrix * position;
}
