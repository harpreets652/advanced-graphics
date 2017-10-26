#version 330

in vec3 Normal0;
in vec3 WorldPos0;
in vec2 TextureCoord0;
in vec3 Tangent0;
smooth in vec3 f_color;
in vec4 LightSpacePos;

out vec4 frag_color;

const int MAX_POINT_LIGHTS = 2;
const int MAX_SPOT_LIGHTS = 2;

struct BaseLight {
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
};

struct DirectionalLight {
    BaseLight Base;
    vec3 Direction;
};

struct Attenuation {
    float Constant;
    float Linear;
    float Exp;
};

struct PointLight {
    BaseLight Base;
    vec3 Position;
    Attenuation Atten;
};

struct SpotLight {
    PointLight Base;
    vec3 Direction;
    float Cutoff;
};

uniform int gNumPointLights;
uniform int gNumSpotLights;
uniform DirectionalLight gDirectionalLight;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];
uniform sampler2D gSampler;
uniform sampler2D gShadowSampler;
uniform sampler2D gNormalMapSampler;


vec3 CalcBumpedNormal() {
    vec3 normalizedNormal = normalize(Normal0);
    vec3 normalizedTangent = normalize(Tangent0);
    normalizedTangent = normalize(normalizedTangent - dot(normalizedTangent, normalizedNormal) * normalizedNormal);
    vec3 bitangent = cross(normalizedTangent, normalizedNormal);
    vec3 bumpMapNormal = texture(gNormalMapSampler, TextureCoord0).xyz;
    bumpMapNormal = 2.0 * bumpMapNormal - vec3(1.0, 1.0, 1.0);
    mat3 tangentBitangentNormal = mat3(normalizedTangent, bitangent, normalizedNormal);
    vec3 newNormal = tangentBitangentNormal * bumpMapNormal;
    newNormal = normalize(newNormal);

    return newNormal;
}

float CalcShadowFactor(vec4 pLightSpacePos) {
    vec3 projCoords = pLightSpacePos.xyz / pLightSpacePos.w;
    vec2 shadowUVCoords;
    shadowUVCoords.x = 0.5 * projCoords.x + 0.5;
    shadowUVCoords.y = 0.5 * projCoords.y + 0.5;
    float z = 0.5 * projCoords.z + 0.5;
    float shadowDepth = texture(gShadowSampler, shadowUVCoords).x;
    if (shadowDepth < z + 0.00001)
        return 0.0625;
    else
        return 1.0;
}

vec4 CalcLightInternal(BaseLight pLight, vec3 pLightDirection, vec3 pNormal, float pShadowFactor) {
    vec4 ambientColor = vec4(pLight.Color * pLight.AmbientIntensity, 1.0f);
    float diffuseFactor = dot(pNormal, -pLightDirection);

    vec4 diffuseColor  = vec4(0, 0, 0, 0);
    if (diffuseFactor > 0) {
        diffuseColor = vec4(pLight.Color * pLight.DiffuseIntensity * diffuseFactor, 1.0f);
    }

    return ambientColor + (pShadowFactor * diffuseColor);
}

vec4 CalcDirectionalLight(vec3 pNormal) {
    return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, pNormal, 1.0);
}

vec4 CalcPointLight(PointLight l, vec3 pNormal, vec4 pLightSpacePos) {
    vec3 lightDirection = WorldPos0 - l.Position;
    float distance = length(lightDirection);
    lightDirection = normalize(lightDirection);

    vec4 Color = CalcLightInternal(l.Base, lightDirection, pNormal, 1.0);
    float attenuation = l.Atten.Constant + l.Atten.Linear * distance + l.Atten.Exp * distance * distance;

    return Color / attenuation;
}

vec4 CalcSpotLight(SpotLight l, vec3 pNormal, vec4 pLightSpacePos) {
    vec3 LightToPixel = normalize(WorldPos0 - l.Base.Position);
    float SpotFactor = dot(LightToPixel, l.Direction);

    if (SpotFactor > l.Cutoff) {
        vec4 Color = CalcPointLight(l.Base, pNormal, pLightSpacePos);
        return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - l.Cutoff));
    }
    else {
        return vec4(0,0,0,0);
    }
}

void main(void) {
    vec3 normal = CalcBumpedNormal(); //normalize(Normal0); //
    vec4 totalLight = CalcDirectionalLight(normal);

    for (int i = 0; i < gNumPointLights; i++) {
        totalLight += CalcPointLight(gPointLights[i], normal, LightSpacePos);
    }

    for (int i = 0 ; i < gNumSpotLights ; i++) {
        totalLight += CalcSpotLight(gSpotLights[i], normal, LightSpacePos);
    }

    frag_color = texture(gSampler, TextureCoord0.xy) * totalLight;
//    frag_color = vec4(f_color.xyz, 1);
}
