#version 330 core

// Data from vertex shader.
// --------------------------------------------------------
// Add your data for interpolation.
in vec3 iPosWorld;
in vec3 iNormalWorld;
// --------------------------------------------------------

// --------------------------------------------------------
// Add your uniform variables.
uniform vec3 cameraPos;
uniform mat4 viewMatrix;

// Material properties.
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Ns;

// Light data.
uniform vec3 ambientLight;
uniform vec3 dirLightDir;
uniform vec3 dirLightRadiance;

uniform vec3 pointLightPos;
uniform vec3 pointLightIntensity;

uniform vec3 spotLightPos;
uniform vec3 spotLightIntensity;
uniform vec3 spotLightDir;
uniform float spotLightCutoffStartInDegree;
uniform float spotLightTotalWidthInDegree;
// --------------------------------------------------------

out vec4 FragColor;

vec3 Diffuse(vec3 Kd, vec3 I, vec3 N, vec3 lightDir)
{
    return Kd * I * max(0, dot(N, lightDir));
}

vec3 Specular(vec3 Ks, vec3 I, vec3 N, vec3 lightDir, float Ns, vec3 viewDir)
{
    vec3 vH = normalize(lightDir + viewDir);
    return Ks * I * pow(max(0.0, dot(N, vH)), Ns);
}

void main()
{
    vec3 N = normalize(iNormalWorld);

    // Ambient light.
    vec3 ambient = Ka * ambientLight;
    // -------------------------------------------------------------

    // [Directional light]
    vec3 vsLightDir = normalize(-dirLightDir);
    // Diffuse.
    vec3 diffuse = Diffuse(Kd, dirLightRadiance, N, vsLightDir);
    // Specular.
    vec3 specular = Specular(Ks, dirLightRadiance, N, vsLightDir, Ns, normalize(cameraPos-iPosWorld));
    vec3 dirLight = diffuse + specular;
    // -------------------------------------------------------------
    
    // [Point light]
    vsLightDir = normalize(pointLightPos - iPosWorld);   
    float distSurfaceToLight = distance(pointLightPos, iPosWorld); 
    float attenuation = 1.0f / (distSurfaceToLight * distSurfaceToLight);
    vec3 radiance = pointLightIntensity * attenuation;

    // Diffuse.
    diffuse = Diffuse(Kd, radiance, N, vsLightDir);

    // Specular.
    specular = Specular(Ks, radiance, N, vsLightDir, Ns, normalize(cameraPos-iPosWorld));
    vec3 pointLight = diffuse + specular;
    // -------------------------------------------------------------
    
    // [Spot light]
    vsLightDir = normalize(spotLightPos - iPosWorld);  
    float theta = dot(vsLightDir, normalize(-spotLightDir));
    distSurfaceToLight = distance(spotLightPos, iPosWorld);

    float epsion = spotLightCutoffStartInDegree - spotLightTotalWidthInDegree;
    attenuation = clamp((theta - spotLightTotalWidthInDegree) / epsion , 0.0, 1.0 ) / (distSurfaceToLight*distSurfaceToLight);
    radiance = spotLightIntensity*attenuation;

    // Diffuse.
    diffuse = Diffuse(Kd, radiance, N, vsLightDir);

    // Specular.
    specular = Specular(Ks, radiance, N, vsLightDir, Ns, normalize(cameraPos-iPosWorld));
    vec3 spotLight = diffuse + specular;
    
    FragColor = vec4(ambient + dirLight + pointLight + spotLight, 1.0);
    // --------------------------------------------------------
}
