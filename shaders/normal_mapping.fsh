#version 330
out vec4 frag_colour;
in vec3 Normal;
in vec3 FragPos;
in vec2 textureCoordinatesOut;
in mat3 tbn;
uniform vec3 cameraPosition;
uniform vec3 objectColor;
uniform float foggy;
uniform sampler2D objectTexture;
uniform sampler2D normalTexture;

struct lightStruct {
    int type;
    vec3 position;
    vec3 direction;
    vec3 color;

    float cutoff;
};

const int MAX_LIGHTS = 16;
uniform lightStruct lights[MAX_LIGHTS];
uniform int how_many_lights;

vec3 point_light(lightStruct light, vec3 norm) {
    float ambientStrength = 0.1f;
    vec3 computedColor = objectColor * light.color;
    vec3 lightDir = normalize(light.position - FragPos);

    /* AMBIENT */
    vec3 ambient = (ambientStrength * computedColor);

    /* DIFFUSE */
    vec3 diffuse = max(dot(norm, lightDir), 0.0) * computedColor;

    /* SPECULAR */
    float specularStrength = 0.3f;
    vec3 viewDir = normalize(cameraPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    vec3 specular = specularStrength * spec * computedColor;

    if (dot(norm, lightDir) < 0.0){
        specular = vec3(0.0f, 0.0f, 0.0f);
    }
    return ambient + diffuse + specular;
}

vec3 directional_light(lightStruct light, vec3 norm) {
    float ambientStrength = 0.1;
    vec3 computedColor = objectColor * light.color;
    vec3 lightDir = normalize(-light.direction);

    /* AMBIENT */
    vec3 ambient = (ambientStrength * computedColor);

    /* DIFFUSE */
    vec3 diffuse = max(dot(norm, lightDir), 0.0) * computedColor;

    /* SPECULAR */
    float specularStrength = 0.3f;
    vec3 viewDir = normalize(cameraPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    vec3 specular = specularStrength * spec * computedColor;

    if (dot(norm, lightDir) < 0.0){
        specular = vec3(0.0f, 0.0f, 0.0f);
    }
    return ambient + diffuse + specular;
}

vec3 spot_light(lightStruct light, vec3 norm) {
    float ambientStrength = 0.1;
    vec3 computedColor = objectColor * light.color;
    vec3 lightDir = normalize(light.position - FragPos);

    /* AMBIENT */
    vec3 ambient = (ambientStrength * computedColor);

    float theta = dot(lightDir, normalize(-light.direction));
    if(theta > cos(radians(light.cutoff))){

        float distance = length(light.position - FragPos);
        // <0, cutoff>  -->  <1, 0>
        float attenuation = 5.0 / (3.0 + 0.09 * distance + 0.012 * (distance * distance));
        float intensity = (1.0 - (1.0 - theta) / (1.0 - cos(radians(light.cutoff))));

        /* DIFFUSE */
        vec3 diffuse = max(dot(norm, lightDir), 0.0) * computedColor * attenuation * intensity;

        /* SPECULAR */
        float specularStrength = 0.2f;
        vec3 viewDir = normalize(cameraPosition - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
        vec3 specular = specularStrength * spec * computedColor * attenuation * intensity;

        if (dot(norm, lightDir) < 0.0){
            specular = vec3(0.0f, 0.0f, 0.0f);
        }
        return ambient + diffuse + specular;
    }

    return ambient;
}

vec4 fog(vec4 fragmentColor){
    float fogMinDistance = 100;
    float fogMaxDistance = 180;
    vec4 fogColor = vec4(0.0, 0.0, 0.0, 0.0);

    // Calculate fog
    float fogDistance = length(FragPos - cameraPosition);
    float fogFactor = (fogMaxDistance - fogDistance) / (fogMaxDistance - fogMinDistance);
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    return mix(fogColor, fragmentColor, fogFactor);
}

void main () {
    vec3 encodedNormal = texture(normalTexture, textureCoordinatesOut).rgb;
    encodedNormal = 2.0 * encodedNormal - 1.0;    // RGB to vector <0,1> -> <-1,1>
    encodedNormal = normalize(encodedNormal * vec3(1, 1, 5));
    vec3 norm = normalize(tbn * encodedNormal);

    vec4 texColor = texture(objectTexture, textureCoordinatesOut);
    if(texColor.a < 0.1) discard;
    frag_colour = texColor;

    for(int i = 0; i < how_many_lights; ++i) {
        lights[i].type == 1 ? (frag_colour = frag_colour + vec4(point_light(lights[i], norm), 1.0f)) : frag_colour;
        lights[i].type == 2 ? (frag_colour = frag_colour + vec4(directional_light(lights[i], norm), 1.0f)) : frag_colour;
        lights[i].type == 3 ? (frag_colour = frag_colour + vec4(spot_light(lights[i], norm), 1.0f)) : frag_colour;
    }
    if (foggy == 1.0f)
        frag_colour = fog(frag_colour);
}