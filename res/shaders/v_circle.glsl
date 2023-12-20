 #version 330 core
    in vec2 coord;
    in vec4 color;

    uniform float x_scale;
    uniform float y_scale;
    
    out vec4 vert_color;

    void main() {
        vec3 position = vec3(coord, 1.0) * mat3(x_scale,0,0,0,y_scale,0,0,0,1);
        gl_Position = vec4(position[0],position[1], 0.0, 1.0);
        vert_color = color;
    }