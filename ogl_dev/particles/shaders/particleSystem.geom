#version 330

layout(points) in;
layout(points) out;
layout(max_vertices = 30) out;

in float Type0[];
in vec3 Position0[];
in vec3 Velocity0[];
in float Age0[];

out float Type1;
out vec3 Position1;
out vec3 Velocity1;
out float Age1;

uniform float gDeltaTimeMillis;
uniform float gTime;
uniform sampler1D gRandomTexture;
uniform float gLauncherLifetime;
uniform float gShellLifetime;
uniform int numToGenerate;

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0f

vec3 GetRandomDir(float TexCoord) {
     vec3 Dir = texture(gRandomTexture, TexCoord).xyz;
     return Dir;
}

void main() {
    float Age = Age0[0] + gDeltaTimeMillis;

    if (Type0[0] == PARTICLE_TYPE_LAUNCHER) {
        if (Age >= gLauncherLifetime) {
            Age = 0.0;

            for (int i = 0; i < numToGenerate; i ++) {
                Type1 = PARTICLE_TYPE_SHELL;
                Position1 = Position0[0];
                vec3 Dir = GetRandomDir((gTime + i)/1000.0);
                Velocity1 = normalize(Dir) * 2.0;
                Age1 = 0.0;
                EmitVertex();
                EndPrimitive();
            }
        }

        Type1 = PARTICLE_TYPE_LAUNCHER;
        Position1 = Position0[0];
        Velocity1 = Velocity0[0];
        Age1 = Age;
        EmitVertex();
        EndPrimitive();
    } else {
        float DeltaTimeSecs = gDeltaTimeMillis / 1000.0f;

        vec3 DeltaP = DeltaTimeSecs * Velocity0[0];
        vec3 newPos = Position0[0] + DeltaP - (0.5 * vec3(0.0, -9.81, 0.0) * (DeltaTimeSecs * DeltaTimeSecs));

        vec3 DeltaV = vec3(DeltaTimeSecs) * vec3(0.0, -9.81, 0.0) * .05;
        vec3 newVel = Velocity0[0] + DeltaV;

        if (Age < gShellLifetime) {
            Type1 = PARTICLE_TYPE_SHELL;
            Position1 = newPos;
            Velocity1 = newVel;
            Age1 = Age;
            EmitVertex();
            EndPrimitive();
        }
//        else {
//             Type1 = PARTICLE_TYPE_SECONDARY_SHELL;
//             Position1 = vec3(0.0f, -5.0f, 0.0f);
//             vec3 Dir = GetRandomDir(gTime/1000.0);
//             Velocity1 = normalize(Dir) / 2.0;
//             Age1 = 0.0f;
//             EmitVertex();
//             EndPrimitive();
//        }
    }
}