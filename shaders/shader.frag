  #version 330
  
  in vec4 vertexColor;
  in vec2 texCoord;
  in vec3 normal;
  in vec3 fragPos;
  in vec4 directionalLightSpacePos;
  
  out vec4 color;

  const int MAX_POINT_LIGHT = 3;
  const int MAX_SPOT_LIGHT = 3;

  struct Light
  {
     vec3 color;
      float ambiantIntensity;
      float diffuseIntensity;
  };  
  struct DirectionalLight
  {
    Light base;
    vec3 direction;
  };
  struct PointLight
  {
    Light base;
    vec3 position;
    float constant;
    float linear;
    float exponent;
  };
  struct SpotLight
  {
    PointLight base;
    vec3 direction;
    float edge;
  };
  struct Material
  {
    float specularIntensity;
    float shininess;
  };
  uniform int pointLightCount;
  uniform int spotLightCount;

  uniform sampler2D theTexture;
  uniform sampler2D directionalShadowMap;
  uniform DirectionalLight directionalLight;
  uniform PointLight pointLight[MAX_POINT_LIGHT];
  uniform SpotLight spotLight[MAX_SPOT_LIGHT];
  uniform Material material;
  uniform vec3 eyePosition;

float CalcDirectionalShadowFactor(DirectionalLight Light)
  {
  vec3 projCoords = directionalLightSpacePos.xyz / directionalLightSpacePos.w;
  projCoords = (projCoords * 0.5) + 0.5;
  
  float current = projCoords.z;
  
  vec3 newNormal = normalize(normal);
  vec3 lightDir = normalize(directionalLight.direction);
  
  float bias = max(0.05 * (1.0 - dot(newNormal, lightDir)), 0.0005);
  float shadow = 0.0f;
  
  vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
  for(int x = -1; x <= 1; ++x)
  {
    for(int y = -1; y <= 1; ++y)
    {
      float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x,y) * texelSize).r;
      shadow += current - bias > pcfDepth ? 1.0 : 0.0;
    }
  }

  shadow /= 9.0;
  
  if(projCoords.z > 1.0)
  {
    shadow = 0.0;
  }                 
  
  return shadow;
  }
 
 vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor)
 {
    vec4 ambiantColor = vec4(light.color,1.0f) * light.ambiantIntensity;
    float diffuseFactor = max(dot(normalize(normal),normalize(direction)), 0.0f);
    vec4 diffuseColor = vec4(light.color * light.diffuseIntensity * diffuseFactor,1.0f);

    vec4 specularColor = vec4(0,0,0,0);
    if(diffuseFactor > 0.0f)
    {
      vec3 fragToEye = normalize(eyePosition-fragPos);
      vec3 reflectedVertex = normalize(reflect(direction,normalize(normal)));
      float specularFactor = dot(fragToEye,reflectedVertex);
      if(specularFactor > 0.0f)
      {
        specularFactor = pow(specularFactor,material.shininess);
        specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
      }
    }
    return (ambiantColor + (1.0-shadowFactor) * (diffuseColor+specularColor));
 }
 vec4 CalcDirectionalLight()
 {
  float shadowFactor = CalcDirectionalShadowFactor(directionalLight);
  return CalcLightByDirection(directionalLight.base,directionalLight.direction,shadowFactor);
 }
 vec4 CalcPointLight(PointLight pLight)
 {
    vec3 direction = fragPos - pLight.position;
    float distance = length(direction);
    direction = normalize(direction);

    vec4 calcColor = CalcLightByDirection(pLight.base, direction,0.0f);
    float attenuation = pLight.exponent*distance*distance+pLight.linear*distance+pLight.constant;

    return (calcColor/attenuation);
 }

 vec4 CalcSpotLight(SpotLight sLight)
 {
  vec3 rayDirection = normalize(fragPos-sLight.base.position);
  float slFactor=dot(rayDirection,sLight.direction);
  if(slFactor > sLight.edge)
  {
    vec4 color = CalcPointLight(sLight.base);

    return color * (1.0 - (1.0f-slFactor)/(1.0f-sLight.edge));
  }else
  {
    return vec4(0,0,0,0);
  }
 }

 vec4 CalcSpotLights()
 {
  vec4 totalColor = vec4(0,0,0,0);
  for(int i = 0;i<spotLightCount;i++)
  {
    totalColor += CalcSpotLight(spotLight[i]);
  }
  return totalColor;
 }

 vec4 CalcPointLights()
 {
  vec4 totalColor = vec4(0,0,0,0);
  for(int i = 0; i < pointLightCount;i++)
  {
    totalColor += CalcPointLight(pointLight[i]);
  }
  return totalColor;
 }
 
  void main()
  {
    vec4 finalColor = CalcDirectionalLight();
    finalColor += CalcPointLights();
    finalColor += CalcSpotLights();
    color = texture(theTexture,texCoord) * finalColor;
  }