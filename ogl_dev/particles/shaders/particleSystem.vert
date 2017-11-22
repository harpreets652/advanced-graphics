#version 330

layout (location = 0) in float Type;
layout (location = 1) in vec3 Position;
layout (location = 2) in vec3 Velocity;
layout (location = 3) in float Age;

out float Type1;
out vec3 Position1;
out vec3 Velocity1;
out float Age1;

uniform float gDeltaTimeMillis;
uniform float gTime;
uniform sampler1D gRandomTexture;
uniform float gLauncherLifetime;
uniform float gShellLifetime;
uniform float gSecondaryShellLifetime;

vec3 GetRandomDir(float TexCoord) {
     vec3 Dir = texture(gRandomTexture, TexCoord).xyz;
     Dir -= vec3(0.5, 0.5, 0.5);
     return Dir;
}

float randZeroOne(vec3 vLocalSeed) {
    uint n = floatBitsToUint(vLocalSeed.y * 214013.0 + vLocalSeed.x * 2531011.0 + vLocalSeed.z * 141251.0);
    n = n * (n * n * 15731u + 789221u);
    n = (n >> 9u) | 0x3F800000u;

    float fRes =  2.0 - uintBitsToFloat(n);
    vLocalSeed = vec3(vLocalSeed.x + 147158.0 * fRes, vLocalSeed.y*fRes  + 415161.0 * fRes, vLocalSeed.z + 324154.0*fRes);
    return fRes;
}

void main() {
    float Age = Age + gDeltaTimeMillis;
    float DeltaTimeSecs = gDeltaTimeMillis / 1000.0f;
    vec3 DeltaP = DeltaTimeSecs * Velocity;
    vec3 DeltaV = vec3(DeltaTimeSecs) * (0.0, -9.81, 0.0);

    if (Age < gSecondaryShellLifetime) {
        Type1 = 0.0;
        Position1 = Position + DeltaP;
        Velocity1 = Velocity + DeltaV;
        Age1 = Age;
    } else {
        Type1 = 0.0;
        float randVal = randZeroOne(Position);
        Position1 = vec3(mod(randVal * 100, 9), 5.0, mod(randVal * 100, 3));
        Velocity1 = vec3(0.0f, -1.0f, 0.0f);
        Age1 = 0;
    }
}