  #version 330
  
  in vec4 vertexColor;
  in vec2 texCoord;
  in vec3 normal;
  in vec3 fragPos;
  
  out vec4 color;

  const int MAX_POINT_LIGHT = 3;

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
  struct Material
  {
    float specularIntensity;
    float shininess;
  };
  uniform int pointLightCount;
  uniform sampler2D theTexture;
  uniform DirectionalLight directionalLight;
  uniform PointLight pointLight[MAX_POINT_LIGHT];
  uniform Material material;
  uniform vec3 eyePosition;
 
 vec4 CalcLightByDirection(Light light, vec3 direction)
 {
    vec4 ambiantColor = vec4(light.color,1.0f) * light.ambiantIntensity;
    float diffuseFactor = max(dot(normalize(normal),normalize(direction)), 0.0f);
    vec4 diffuseColor = vec4(light.color,1.0f) * light.diffuseIntensity * diffuseFactor;

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
    return (ambiantColor + diffuseColor+specularColor);
 }
 vec4 CalcDirectionalLight()
 {
  return CalcLightByDirection(directionalLight.base,directionalLight.direction);
 }
 vec4 CalcPointLights()
 {
  vec4 totalColor = vec4(0,0,0,0);
  for(int i = 0; i < pointLightCount;i++)
  {
    vec3 direction = fragPos - pointLight[i].position;
    float distance = length(direction);
    direction = normalize(direction);

    vec4 calcColor = CalcLightByDirection(pointLight[i].base, direction);
    float attenuation = pointLight[i].exponent*distance*distance+pointLight[i].linear*distance+pointLight[i].constant;

    totalColor+=(calcColor/attenuation);
  }
  return totalColor;
 }
  void main()
  {
    vec4 finalColor = CalcDirectionalLight();
    finalColor += CalcPointLights();
    color = texture(theTexture,texCoord) * finalColor;
  }