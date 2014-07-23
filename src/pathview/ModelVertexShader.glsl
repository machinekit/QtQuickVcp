// global
uniform mat4 projectionMatrix;    // projection matrix
uniform mat4 viewMatrix;          // view matrix

// model specific
uniform mat4 modelMatrix;         // model matrix
uniform vec4 color;               // per-vertex color

// vertex specific
attribute vec4 position;    // per-vertex position
attribute vec3 normal;      // per-vertex normal information

// lighting
//uniform vec3 lightPos;            // position of the light source
//uniform bool enableLighting;      // whether lighting is enabled or not

uniform struct Light {
   vec3 position;       // position of the light source
   vec3 intensities;    //a.k.a the color of the light
   float attenuation;
   float ambientCoefficient;
   bool enabled;         // whether lighting is enabled or not
} light;

// selection mode
uniform vec4 idColor;             // color to be used for selection mode
uniform bool selectionMode;       //

varying vec4 destinationColor;  // the output colors

void main(void) {
    mat4 modelviewMatrix = viewMatrix * modelMatrix;

    if (selectionMode)
    {
        destinationColor = idColor;
    }
    else if (light.enabled)
    {
        vec3 modelViewVertex = vec3(modelMatrix * position);          // transform vertex into eye space
        vec3 modelViewNormal = vec3(modelMatrix * vec4(normal, 0.0));            // transform normals orientation into eye space
        vec3 modelViewLight = light.position;//vec3(modelviewMatrix * vec4(light.position, 0.0));
        vec3 lightVector = normalize(modelViewLight - modelViewVertex);   // get a lighting direction vector from the light to the vertex
        // Calculate the dot product of the light vector and vertex normal.
        // If the normal and light vector are pointing in the same direction then it will get max illumination.

        //ambient
        vec3 ambient = light.ambientCoefficient * color.rgb * light.intensities;

        //diffuse
        float diffuseCoefficient = max(dot(modelViewNormal, lightVector), 0.0);// right hand value is ambience light
        vec3 diffuse = diffuseCoefficient * color.rgb * light.intensities;

        //attenuation
        float distanceToLight = length(modelViewLight - modelViewVertex);        // will be used for attenuation
        float attenuation = 1.0 / (1.0 + light.attenuation * distanceToLight * distanceToLight);

        //float diffuse = max(dot(modelViewNormal, lightVector), 0.0);// right hand value is ambience light
        //diffuse = diffuse * (1.0 / (1.0 + (light.attenuation * pow(distanceToLight, 2)));   // Attenuate the light based on distance

        //linear color (color before gamma correction)
        vec3 linearColor = ambient + attenuation * diffuse;

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
