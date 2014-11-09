// global
uniform highp mat4 projectionMatrix;    // projection matrix
uniform highp mat4 viewMatrix;          // view matrix

// model specific
uniform highp mat4 modelMatrix;         // model matrix
uniform lowp vec4 color;                // per-vertex color

// vertex specific
attribute highp vec4 position;    // per-vertex position
attribute highp vec3 normal;      // per-vertex normal information

// lighting
//uniform vec3 lightPos;            // position of the light source
//uniform bool enableLighting;      // whether lighting is enabled or not

struct Light {
    highp vec3 position;       // position of the light source
    lowp vec3 intensities;    //a.k.a the color of the light
    lowp float attenuation;
    lowp float ambientCoefficient;
    bool enabled;         // whether lighting is enabled or not
};

uniform Light light;

// selection mode
uniform lowp vec4 idColor;             // color to be used for selection mode
uniform bool selectionMode;       // enables or disables the selection mode

varying lowp vec4 destinationColor;  // the output colors

void main(void) {
    highp mat4 modelviewMatrix = viewMatrix * modelMatrix;

    if (selectionMode)
    {
        destinationColor = idColor;
    }
    else if (light.enabled)
    {
        highp vec3 modelViewVertex = vec3(modelMatrix * position);          // transform vertex into eye space
        highp vec3 modelViewNormal = vec3(modelMatrix * vec4(normal, 0.0));            // transform normals orientation into eye space
        highp vec3 modelViewLight = light.position;//vec3(modelviewMatrix * vec4(light.position, 0.0));
        highp vec3 lightVector = normalize(modelViewLight - modelViewVertex);   // get a lighting direction vector from the light to the vertex
        // Calculate the dot product of the light vector and vertex normal.
        // If the normal and light vector are pointing in the same direction then it will get max illumination.

        //ambient
        lowp vec3 ambient = light.ambientCoefficient * color.rgb * light.intensities;

        //diffuse
        lowp float diffuseCoefficient = max(dot(modelViewNormal, lightVector), 0.0);// right hand value is ambience light
        lowp vec3 diffuse = diffuseCoefficient * color.rgb * light.intensities;

        //attenuation
        lowp float distanceToLight = length(modelViewLight - modelViewVertex);        // will be used for attenuation
        lowp float attenuation = 1.0 / (1.0 + light.attenuation * distanceToLight * distanceToLight);

        //float diffuse = max(dot(modelViewNormal, lightVector), 0.0);// right hand value is ambience light
        //diffuse = diffuse * (1.0 / (1.0 + (light.attenuation * pow(distanceToLight, 2)));   // Attenuate the light based on distance

        //linear color (color before gamma correction)
        lowp vec3 linearColor = ambient + attenuation * diffuse;

        //final color (after gamma correction)
        //vec3 gamma = vec3(1.0/2.2);
        //destinationColor = vec4(pow(linearColor, gamma), color.a);
        destinationColor = vec4(linearColor, color.a);
        //destinationColor = color * diffuse;
        //destinationColor[3] = color[3]; // correct alpha
    }
    else
    {
        destinationColor = color;
    }

    gl_Position = projectionMatrix * modelviewMatrix * position;
}
