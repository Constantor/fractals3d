#version 140

#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
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
uniform float ZoomCoefficient = 1.0;

const int MAX_STEPS = 255;
const float MAX_DIST = 2000.0;
const float MIN_DIST = 0.0001;
const int OTHER_ITER = 200;
const int MANDEL_ITER = 50;

float sphere(vec3 point, vec3 center, float radius) {
    return length(point - center) - radius;
}

float plane(vec3 point) {
    return point.y;
}

vec4 Add(vec4 z1, vec4 z2) {
    return z1 + z2;
}

vec4 Mul(vec4 z1, vec4 z2) {
    return vec4(z1.x * z2.x - dot(z1.yzw, z2.yzw), z2.x * z1.yzw + z1.x * z2.yzw + cross(z1.yzw, z2.yzw));
}

vec4 Pow(vec4 z, int n) {
    vec4 z0 = vec4(1.0, vec3(0.0));
    for(int i = 0; i < n; i++)
        z0 = Mul(z0, z);
    return z0;
}

struct QDual {
    vec4 q;
    vec4 d;
};

QDual qdAdd(QDual qd1, QDual qd2) {
    return QDual(Add(qd1.q, qd2.q), Add(qd1.d, qd2.d));
}

QDual qdMul(QDual qd1, QDual qd2) {
    return QDual(Mul(qd1.q, qd2.q), Add(Mul(qd1.d, qd2.q), Mul(qd1.q, qd2.d)));
}

QDual qdPow(QDual qd, int n) {
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


float mandelbrot(vec4 c, vec4 z) {
    QDual zd = QDual(z, vec4(0.0, vec3(0.0)));
    QDual cd = QDual(c, vec4(1.0, vec3(0.0)));
    for(int i = 0; i < MANDEL_ITER; i++) {
        zd = qdAdd(qdPow(zd, POWER), cd);
        if (length(zd.q) > RADIUS) break;
    }

    return 0.5 * length(zd.q) * log(length(zd.q)) / length(zd.d);
}

float circleFractal(vec4 point, vec4 CriticalPoint) {
    vec4 z = point;
    float dr = 1.0;
    float r = 0.0;
    for (int i = 0; i < OTHER_ITER; i++) {
        r = length(z);
        if (RADIUS < r)
            break;

        dr = pow(r, POWER - 1.0) * POWER * dr + 1.0;

        float zr = pow(r, POWER);
        float theta;
        float phi = atan(z.x, z.y) * POWER;
        if ((i & 1) == 0)
            theta = asin(-z.z / r) * POWER;
        else
            theta = asin(z.z / r) * POWER;
        z = zr * vec4(
        cos(theta) * cos(phi),
        sin(phi) * cos(theta),
        sin(theta),
        0.0);
        z += CriticalPoint;
    }
    return 0.5 * log(r) * r / dr;
}

float spongeFractal(vec4 point, vec4 CriticalPoint) {
    vec4 z = point;
    float dr = 1.0;
    float r = 0.0;
    for (int i = 0; i < OTHER_ITER; ++i) {
        r = length(z);
        if (r > RADIUS) break;

        dr = pow(r, POWER - 1.0) * POWER * dr + 1.0;

        float zr = pow(r, POWER);
        float theta = atan(z.x, z.y) * POWER;
        float phi;
        if (i % 2 == 0) {
            phi = asin(-z.z / r) * POWER;
        }
        else {
            phi = asin(z.z / r) * POWER;
        }
        z = zr * vec4(
        cos(theta) * cos(phi),
        sin(phi) * cos(theta),
        sin(theta),
        0.0);
        z += CriticalPoint;
    }
    return 0.5 * log(r) * r / dr;
}

float GetDist(vec3 point, vec3 CriticalPoint) {
    float fractalDist;
    switch(TYPE) {
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
        case 4:
            fractalDist = circleFractal(vec4(point, 0.0), vec4(CriticalPoint, 0.0));
            break;
        case 5:
            fractalDist = spongeFractal(vec4(point, 0.0), vec4(CriticalPoint, 0.0));
            break;
    }
    return fractalDist;
}

float RayMarch(vec3 CameraPosition, vec3 RayDirection, vec3 CriticalPoint) {
    float dist = 0.0;
    for(int i = 0; i < MAX_STEPS; i++) {
        vec3 RayPosition = CameraPosition + RayDirection * dist;
        float distNear = GetDist(RayPosition, CriticalPoint);
        dist += distNear;
        if (MAX_DIST < dist || distNear < MIN_DIST)
            break;
    }
    return dist;
}

vec2 linmap(vec2 point, vec2 leftCorner, vec2 rightCorner, vec2 newLeftCorner, vec2 newRightCorner) {
    return (point - leftCorner) / (rightCorner - leftCorner) * (newRightCorner - newLeftCorner) + newLeftCorner;
}

out vec4 FragColor;

void main() {
    float resolutionMin = min(Resolution.x, Resolution.y);
    float resolutionMax = max(Resolution.x, Resolution.y);
    if(resolutionMin < 1) {
        FragColor = vec4(Ambience, 1.0);
        return;
    }

    // horizontal
    vec2 bounds = vec2(Resolution.y / Resolution.x, 1);
    vec2 shift = vec2((resolutionMax - resolutionMin) * 0.5, 0);
    if(Resolution.x < Resolution.y) { // vertical
        bounds.y = 1 / bounds.x;
        bounds.x = 1;
        shift.y = shift.x;
        shift.x = 0;
    }
    //bounds /= ZoomCoefficient;
    vec2 FragCoord = linmap(gl_FragCoord.xy - shift, vec2(0), vec2(resolutionMin), -bounds, bounds);

    vec3 CriticalPoint = vec3(CriticalPointX, CriticalPointY, CriticalPointZ);
    vec3 RayDirection = normalize((inverse(mvp_matrix) * vec4(FragCoord, 1.0, 1.0)).xyz);
    float distance = RayMarch(CameraPosition, RayDirection, CriticalPoint);
    if(MAX_DIST * 0.75 < distance) {
        FragColor = vec4(Ambience, 1);
        return;
    }
    const float zoomCorrector = 1.28;
    FragColor = vec4(zoomCorrector * ZoomCoefficient * pow(1 + distance, 0.5) / pow(3, 0.5) * ColorFractal, 1.0);

    /*const int method = 0;
    if(method == 0) {
        const float zoomCorrector = 1.3;
        FragColor = vec4(zoomCorrector * ZoomCoefficient * pow(1 + distance, 0.5) / pow(3, 0.5) * ColorFractal, 1.0);
    } else if(method == 1) {
        FragColor = vec4(1.1 * distance * ColorFractal, 1.0);
    } else if(method == 2) {
        float colorCoefficient = 1.5;
        FragColor = vec4(colorCoefficient * (pow(2, 1.5) - pow(distance, 1.5)) / pow(2, 1.5) * ColorFractal / pow(ZoomCoefficient, 1.5 - distance), 1.0);
    }*/
}