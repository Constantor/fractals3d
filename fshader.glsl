#version 140

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;
uniform vec2 Resolution = vec2(600, 600); //размеры окна
uniform float RADIUS = 2.5; //радиус сходимости, если хочешь
uniform int POWER = 2; //степень
uniform int TYPE = 0;
uniform float CriticalPointX; //координаты точки
uniform float CriticalPointY;
uniform float CriticalPointZ;
uniform vec3 CameraPosition;
uniform vec3 Ambience = vec3(0.6, 0.8, 0.8);
uniform vec3 ColorFractal = vec3(0.6, 0.5, 0.8);


#define MAX_STEPS 255
#define MAX_DIST 2000.0
#define MIN_DIST 0.0001
#define OTHER_ITER 200
#define MANDEL_ITER 50

float sphere(vec3 point, vec3 center, float radius) {
    return length(point - center) - radius;
}

float plane(vec3 point) {
    return point.y;
}

vec4 Add(vec4 z1, vec4 z2)
{
    return z1 + z2;
}

vec4 Mul(vec4 z1, vec4 z2)
{
    return vec4(z1.x * z2.x - dot(z1.yzw, z2.yzw), z2.x * z1.yzw + z1.x * z2.yzw + cross(z1.yzw, z2.yzw));
}

vec4 Pow(vec4 z, int n)
{
    vec4 z0 = vec4(1.0, vec3(0.0));
    for (int i = 0; i < n; ++i)
    {
        z0 = Mul(z0, z);
    }
    return z0;
}

struct QDual
{
    vec4 q;
    vec4 d;
};

QDual qdAdd(QDual qd1, QDual qd2)
{
    return QDual(Add(qd1.q, qd2.q), Add(qd1.d, qd2.d));
}

QDual qdMul(QDual qd1, QDual qd2)
{
    return QDual(Mul(qd1.q, qd2.q), Add(Mul(qd1.d, qd2.q), Mul(qd1.q, qd2.d)));
}

QDual qdPow(QDual qd, int n)
{
    QDual p = QDual(vec4(1.0, vec3(0.0)), vec4(0.0, vec3(0.0)));
    for (int i = 0; i < n; ++i)
    {
        p = qdMul(p, qd);
    }
    return p;
}

float psychoFractal(vec4 point, vec4 CriticalPoint) {
    vec4 z = point;
    float dr = 1.0;
    float r = 0.0;
    for (int i = 0; i < OTHER_ITER; ++i) {
        r = length(z);
        if (r > RADIUS) break;

        dr = pow(r, POWER - 1.0) * POWER * dr + 1.0;

        float zr = pow(r, POWER);
        float theta = acos(z.z / r) * POWER;
        float phi = atan(z.y, z.x) * POWER;

        z = zr * vec4(
        sin(theta) * sin(phi),
        sin(phi) * sin(theta),
        sin(theta),
        1.0);
        z += CriticalPoint;
    }
    return 0.5 * log(r) * r / dr;
}

float flowerFractal(vec4 point, vec4 CriticalPoint) {
    vec4 z = point;
    float dr = 1.0;
    float r = 0.0;
    for (int i = 0; i < OTHER_ITER; ++i) {
        r = length(z);
        if (r > RADIUS) break;

        dr = pow(r, POWER - 1.0) * POWER * dr + 1.0;

        float zr = pow(r, POWER);
        float theta = acos(z.z / r) * POWER;
        float phi = atan(z.y, z.x) * POWER;

        z = zr * vec4(
        sin(theta) * cos(phi),
        sin(phi) * sin(theta),
        cos(theta),
        1.0);
        z += CriticalPoint;
    }
    return 0.5 * log(r) * r / dr;
}

float anotherFractal(vec4 point, vec4 CriticalPoint) {
    vec4 z = point;
    float dr = 1.0;
    float r = 0.0;
    for (int i = 0; i < OTHER_ITER; ++i) {
        r = length(z);
        if (r > RADIUS) break;

        dr = pow(r, POWER - 1.0) * POWER * dr + 1.0;

        float zr = pow(r, POWER);
        float theta = acos(z.z / r) * POWER;
        float phi = atan(z.y, z.x) * POWER;

        z = zr * vec4(
        cos(theta) * cos(phi),
        sin(phi) * sin(theta),
        cos(theta),
        1.0);
        z += CriticalPoint;
    }
    return 0.5 * log(r) * r / dr;
}


float mandelbrot(vec4 c, vec4 z)
{
    QDual zd = QDual(z, vec4(0.0, vec3(0.0)));
    QDual cd = QDual(c, vec4(1.0, vec3(0.0)));
    for (int i = 0; i < MANDEL_ITER; ++i)
    {
        zd = qdAdd(qdPow(zd, POWER), cd);
        if (length(zd.q) > RADIUS) break;
    }

    return (length(zd.q) * log(length(zd.q))) / (2 * length(zd.d));
}

float GetDist(vec3 point, vec3 CriticalPoint) {
    float fractalDist;
    switch(TYPE){
        case 0:
        fractalDist = mandelbrot(vec4(point, 0.0), vec4(CriticalPoint, 0.0));
        break;
        case 1:
        fractalDist = psychoFractal(vec4(point, 0.0), vec4(CriticalPoint, 0.0));
        break;
        case 2:
        fractalDist = anotherFractal(vec4(point, 0.0), vec4(CriticalPoint, 0.0));
        break;
        case 3:
        fractalDist = flowerFractal(vec4(point, 0.0), vec4(CriticalPoint, 0.0));
        break;
    }
    return fractalDist;
}

float RayMarch(vec3 CameraPosition, vec3 RayDirection, vec3 CriticalPoint) {
    float dist = 0.0;
    for (int i = 0; i < MAX_STEPS; ++i) {
        vec3 RayPosition = CameraPosition + RayDirection * dist;
        float distNear = GetDist(RayPosition, CriticalPoint);
        dist += distNear;
        if (dist > MAX_DIST || distNear < MIN_DIST) {
            break;
        }
    }

    return dist;
}

vec2 linmap(vec2 point, vec2 leftCorner, vec2 rightCorner, vec2 newLeftCorner, vec2 newRightCorner)
{
    return (point - leftCorner) / (rightCorner - leftCorner) * (newRightCorner - newLeftCorner) + newLeftCorner;
}

out vec4 FragColor;

void main() {
    float MinY = -1.0;
    float MaxY = 1.0;
    float MinX = -Resolution.x / Resolution.y;
    float MaxX = Resolution.x / Resolution.y;
    vec2 FragCoord = linmap(gl_FragCoord.xy, vec2(0, 0), Resolution, vec2(MinX, MinY), vec2(MaxX, MaxY));
    vec3 result = vec3(0);
    vec3 CriticalPoint = vec3(CriticalPointX, CriticalPointY, CriticalPointZ);
    vec3 RayDirection = normalize((inverse(mvp_matrix) * vec4(FragCoord, 1.0, 1.0)).xyz);

    float distance = RayMarch(CameraPosition, RayDirection, CriticalPoint);
    result = 1.1 * vec3(distance * ColorFractal.x, distance * distance * ColorFractal.y, distance * ColorFractal.z);
    if (distance > MAX_DIST * 0.75) {
        result = Ambience;
    }
    FragColor = vec4(result, 1.0);
}