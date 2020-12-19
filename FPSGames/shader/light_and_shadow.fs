#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

// uniform vec3 lightPos;
uniform vec3 lightDirection;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // ͸�ӳ���
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // ӳ�䵽[0,1]����
    projCoords = projCoords * 0.5 + 0.5;
    // �ӹ�ĽǶȵõ�ָ��ǰƬԪ��·���ϵ���������ֵ��ʹ��[0,1]����fragPosLight���꣩
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // �ӹ�ĽǶȵõ���ǰƬԪ�����
    float currentDepth = projCoords.z;
    
    // ����ƫ��
    vec3 normal = normalize(fs_in.Normal);
    float bias = max(0.001 * (1.0 - dot(normal, lightDirection)), 0.00001);
    
    // ��⵱ǰfrag�Ƿ�����Ӱ��
    // float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    // PCF �Ż������Ӱ��Ե
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    
    // ��ƬԪ��ȳ���1.0ʱ����������Ӱ������Զ��ȫ����Ӱ������
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main()
{
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.8);

    // ambient ������
    vec3 ambient = 0.3 * color;
    
    // diffuse ������
    float diff = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = diff * color;  // ��ͼ��ɫԽ�������Խ��
    
    // specular ���淴��
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDirection, normal);
    
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDirection + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    
    // ������Ӱ
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    
    FragColor = vec4(lighting, 1.0);
}
