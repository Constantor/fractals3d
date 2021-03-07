#version 140

#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;
uniform float uElapsedSeconds = 3.;
uniform mat4 uViewMatrix = mat4(1.0);
uniform mat4 uProjectionMatrix = mat4(1.0);
uniform vec2 uResolution = vec2(600, 600);
uniform vec3 uCameraPosition = vec3(0,0,5);
uniform vec3 uLightDirection = vec3(-1.0, -1.0, -1.0);
uniform vec4 uLightAmbientColor = vec4(vec3(0.0), 1.0);
uniform vec4 uLightDiffuseColor = vec4(vec3(0.5), 1.0);
uniform vec4 uLightSpecularColor = vec4(vec3(1.0), 1.0);
uniform vec4 uMaterialAmbientColor = vec4(vec3(1.0), 1.0);
uniform vec4 uMaterialDiffuseColor = vec4(vec3(1.0), 1.0);
uniform vec4 uMaterialSpecularColor = vec4(vec3(1.0), 1.0);
uniform vec4 uMaterialEmissionColor = vec4(vec3(0.0), 1.0);
uniform float uShininess = 64.0;
uniform int juliaIterations = 10;
uniform int mandelbrotIterations = 10;
uniform int marchingIterations = 500;
uniform float convergenceCriteria = 1e-4;
out vec4 oFragColor;


// ---------------- utilities ---------------- //

vec2 linmap(vec2 in_val, vec2 in_min, vec2 in_max, vec2 out_min, vec2 out_max)
{
    return (in_val - in_min) / (in_max - in_min) * (out_max - out_min) + out_min;
}

// ---------------- complex ---------------- //

vec2 cAdd(vec2 c1, vec2 c2)
{
    // return vec2(c1.x + c2.x, c1.y + c2.y);
    return c1 + c2;
}

vec2 cSub(vec2 c1, vec2 c2)
{
    // return vec2(c1.x - c2.x, c1.y - c2.y);
    return c1 - c2;
}

vec2 cMul(vec2 c1, vec2 c2)
{
    return vec2(c1.x * c2.x - c1.y * c2.y, c1.y * c2.x + c1.x * c2.y);
}

vec2 cConj(vec2 c)
{
    return vec2(c.x, -c.y);
}

vec2 cInv(vec2 c)
{
    return cConj(c) / pow(length(c), 2);
}

vec2 cDiv(vec2 c1, vec2 c2)
{
    return cMul(c1, cInv(c2));
}

vec2 cPow(vec2 c, int n)
{
    vec2 p = vec2(1.0, 0.0);
    for (int i = 0; i < n; ++i)
    {
        p = cMul(p, c);
    }
    return p;
}

// ---------------- quaternion ---------------- //

vec4 qAdd(vec4 q1, vec4 q2)
{
    // return vec4(q1.x + q2.x, q1.yzw + q2.yzw);
    return q1 + q2;
}

vec4 qSub(vec4 q1, vec4 q2)
{
    // return vec4(q1.x - q2.x, q1.yzw - q2.yzw);
    return q1 - q2;
}

vec4 qMul(vec4 q1, vec4 q2)
{
    return vec4(q1.x * q2.x - dot(q1.yzw, q2.yzw), q2.x * q1.yzw + q1.x * q2.yzw + cross(q1.yzw, q2.yzw));
}

vec4 qConj(vec4 q)
{
    return vec4(q.x, -q.yzw);
}

vec4 qInv(vec4 q)
{
    return qConj(q) / pow(length(q),2);
}

vec4 qDiv(vec4 q1, vec4 q2)
{
    return qMul(q1, qInv(q2));
}

vec4 qPow(vec4 q, int n)
{
    vec4 p = vec4(1.0, vec3(0.0));
    for (int i = 0; i < n; ++i)
    {
        p = qMul(p, q);
    }
    return p;
}

// ---------------- dual ---------------- //

struct QDual
{
    vec4 q;
    vec4 d;
};

QDual qdAdd(QDual qd1, QDual qd2)
{
    return QDual(qAdd(qd1.q, qd2.q), qAdd(qd1.d, qd2.d));
}

QDual qdSub(QDual qd1, QDual qd2)
{
    return QDual(qSub(qd1.q, qd2.q), qSub(qd1.d, qd2.d));
}

QDual qdMul(QDual qd1, QDual qd2)
{
    return QDual(qMul(qd1.q, qd2.q), qAdd(qMul(qd1.d, qd2.q), qMul(qd1.q, qd2.d)));
}

QDual qdDiv(QDual qd1, QDual qd2)
{
    return QDual(qDiv(qd1.q, qd2.q), qDiv(qSub(qMul(qd1.d, qd2.q), qMul(qd1.q, qd2.d)), qMul(qd2.q, qd2.q)));
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

// ---------------- fractals ---------------- //

float sdfJulia(vec4 z, vec4 c)
{
    QDual zd = QDual(z, vec4(1.0, vec3(0.0)));
    QDual cd = QDual(c, vec4(0.0, vec3(0.0)));
    for (int i = 0; i < juliaIterations; ++i)
    {
        // forward-mode automatic differentiation
        zd = qdAdd(qdPow(zd, 2), cd);
        if (length(zd.q) > 2.0) break;
    }

    return (length(zd.q) * log(length(zd.q))) / (2 * length(zd.d));
}

vec4 normalJulia(vec4 z, vec4 c)
{
    // jacobian
    mat4 J = mat4(1.0);
    for (int i = 0; i < juliaIterations; ++i)
    {
        // forward-mode automatic differentiation
        // NOTE: glsl uses column-major matrices
        J = 2 * mat4(
        +z.x, z.y, z.z, z.w,
        -z.y, z.x, 0.0, 0.0,
        -z.z, 0.0, z.x, 0.0,
        -z.w, 0.0, 0.0, z.x
        ) * J;
        z = qAdd(qPow(z, 2), c);
        if (length(z) > 2.0) break;
    }

    return z * J;
}

float sdfMandelbrot(vec4 c, vec4 z)
{
    QDual zd = QDual(z, vec4(0.0, vec3(0.0)));
    QDual cd = QDual(c, vec4(1.0, vec3(0.0)));
    for (int i = 0; i < mandelbrotIterations; ++i)
    {
        // forward-mode automatic differentiation
        zd = qdAdd(qdPow(zd, 2), cd);
        if (length(zd.q) > 2.0) break;
    }

    return (length(zd.q) * log(length(zd.q))) / (2 * length(zd.d));
}

vec4 normalMandelbrot(vec4 c, vec4 z)
{
    // jacobian
    mat4 J = mat4(0.0);
    for (int i = 0; i < mandelbrotIterations; ++i)
    {
        // forward-mode automatic differentiation
        // NOTE: glsl uses column-major matrices
        J = 2 * mat4(
        +z.x, z.y, z.z, z.w,
        -z.y, z.x, 0.0, 0.0,
        -z.z, 0.0, z.x, 0.0,
        -z.w, 0.0, 0.0, z.x
        ) * J + mat4(1.0);
        z = qAdd(qPow(z, 2), c);
        if (length(z) > 2.0) break;
    }

    return z * J;
}

// ---------------- reflection ---------------- //

vec4 phongReflection(
vec3 surfaceNormal,
vec3 eyeDirection,
vec3 lightDirection,
vec4 lightAmbientColor,
vec4 lightDiffuseColor,
vec4 lightSpecularColor,
vec4 materialAmbientColor,
vec4 materialDiffuseColor,
vec4 materialSpecularColor,
vec4 materialEmissionColor,
float shineness
)
{
    surfaceNormal = normalize(surfaceNormal);
    eyeDirection = normalize(eyeDirection);
    lightDirection = normalize(lightDirection);
    vec4 ambientColor = lightAmbientColor * materialAmbientColor;
    vec4 diffuseColor = lightDiffuseColor * materialDiffuseColor;
    vec4 specularColor = lightSpecularColor * materialSpecularColor;
    diffuseColor *= max(dot(-lightDirection, surfaceNormal), 0.0);
    specularColor *= pow(max(dot(reflect(lightDirection, surfaceNormal), -eyeDirection), 0.0), shineness);
    vec4 color = clamp(ambientColor + diffuseColor + specularColor + materialEmissionColor, 0.0, 1.0);
    return color;
}

void main()
{
    //	oFragColor = vec4(vec3(1.0), 1.0);
    //	return;

    vec2 fragCoord = linmap(gl_FragCoord.xy, vec2(0, 0), uResolution, vec2(-1, -1), vec2(1, 1));

    // why this does not work?
    // vec3 rayDirection = normalize((inverse(uViewMatrix) * inverse(uProjectionMatrix) * vec4(vec3(fragCoord, 1.0), 0.0)).xyz);

    vec3 rayDirection = normalize((inverse(mat3(uViewMatrix)) * inverse(mat3(mvp_matrix)) * vec3(fragCoord, 1.0)).xyz);
    vec3 rayPosition = uCameraPosition;

    vec4 juliaType = 0.45 * cos(vec4(0.5, 3.9, 1.4, 1.1) + uElapsedSeconds * 0.15 * vec4(1.2, 1.7, 1.3, 2.5)) - vec4(0.3, 0.0, 0.0, 0.0);
    vec4 criticalPoint = vec4(0.0);

    for (int i = 0; i < marchingIterations; ++i)
    {
        float sdf = sdfJulia(vec4(rayPosition, 0.0), juliaType);
        // float sdf = sdfMandelbrot(vec4(rayPosition, 0.0), criticalPoint);
        rayPosition += sdf * rayDirection;
        if (abs(sdf) < convergenceCriteria)
        {
            vec3 surfaceNormal = normalize(normalJulia(vec4(rayPosition, 0.0), juliaType).xyz);
            // vec3 surfaceNormal = normalize(normalMandelbrot(vec4(rayPosition, 0.0), criticalPoint).xyz);
            oFragColor = phongReflection(
            surfaceNormal,
            rayDirection,
            uLightDirection,
            uLightAmbientColor,
            uLightDiffuseColor,
            uLightSpecularColor,
            uMaterialAmbientColor,
            uMaterialDiffuseColor,
            uMaterialSpecularColor,
            uMaterialEmissionColor,
            uShininess
            );
            return;
        }
    }
    oFragColor = vec4(vec3(1.0), 1.0);
}