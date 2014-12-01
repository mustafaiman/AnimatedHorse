#version 150
uniform mat4 ModelView;
uniform mat4 Projection;
in vec4 vPosition;
uniform vec4 vColor;
in vec3 vNormal;
out vec4 color;
in vec2 texcoord;
out vec2 st;

uniform bool designMode;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform vec4 LightPosition;
uniform float Shininess;
void main()
{
    if(designMode) {
        color = vColor;
    } else {
        vec3 pos = (ModelView * vPosition).xyz;

        vec3 L = normalize( LightPosition.xyz - pos );
        vec3 E = normalize( -pos );
        vec3 H = normalize( L + E );

        // Transform vertex normal into eye coordinates
        vec3 N = normalize( ModelView*vec4(vNormal, 0.0) ).xyz;

        // Compute terms in the illumination equation
        vec4 ambient = AmbientProduct;

        float Kd = max( dot(L, N), 0.0 );
        vec4  diffuse = Kd*DiffuseProduct;

        float Ks = pow( max(dot(N, H), 0.0), Shininess );
        vec4  specular = Ks * SpecularProduct;

        if( dot(L, N) < 0.0 ) {
            specular = vec4(0.0, 0.0, 0.0, 1.0);
        }

        color = ambient + diffuse + specular;
        color.a = 1.0;
    }
    st = texcoord;
    gl_Position = Projection*ModelView*vPosition;
}
