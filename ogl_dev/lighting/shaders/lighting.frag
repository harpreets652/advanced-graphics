#version 330

in vec3 Normal0;
in vec3 WorldPos0;
in vec2 TextureCoord0;

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

vec4 CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal) {
    vec4 AmbientColor = vec4(Light.Color * Light.AmbientIntensity, 1.0f);
    float DiffuseFactor = dot(Normal, -LightDirection);

    vec4 DiffuseColor  = vec4(0, 0, 0, 0);
    if (DiffuseFactor > 0) {
        DiffuseColor = vec4(Light.Color * Light.DiffuseIntensity * DiffuseFactor, 1.0f);
    }

    return (AmbientColor + DiffuseColor);
}

vec4 CalcDirectionalLight(vec3 Normal) {
    return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, Normal);
}

vec4 CalcPointLight(PointLight l, vec3 Normal) {
    vec3 LightDirection = WorldPos0 - l.Position;
    float Distance = length(LightDirection);
    LightDirection = normalize(LightDirection);

    vec4 Color = CalcLightInternal(l.Base, LightDirection, Normal);
    float attenuation =  l.Atten.Constant + l.Atten.Linear * Distance + l.Atten.Exp * Distance * Distance;

    return Color / attenuation;
}

vec4 CalcSpotLight(SpotLight l, vec3 Normal) {
    vec3 LightToPixel = normalize(WorldPos0 - l.Base.Position);
    float SpotFactor = dot(LightToPixel, l.Direction);

    if (SpotFactor > l.Cutoff) {
        vec4 Color = CalcPointLight(l.Base, Normal);
        return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - l.Cutoff));
    }
    else {
        return vec4(0,0,0,0);
    }
}

void main(void) {
    vec3 Normal = normalize(Normal0);
    vec4 TotalLight = CalcDirectionalLight(Normal);

    for (int i = 0; i < gNumPointLights; i++) {
        TotalLight += CalcPointLight(gPointLights[i], Normal);
    }

    for (int i = 0 ; i < gNumSpotLights ; i++) {
        TotalLight += CalcSpotLight(gSpotLights[i], Normal);
    }

    frag_color = texture(gSampler, TextureCoord0.xy) * TotalLight;
}
