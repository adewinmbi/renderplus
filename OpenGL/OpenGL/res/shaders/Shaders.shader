#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoords;

out vec2 v_textureCoords;

void main() {
   gl_Position = position;
   v_textureCoords = textureCoords;
};

#shader fragment
#version 330 core
layout(location = 0) out vec4 color;

in vec2 v_textureCoords;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main() {
	vec4 textureColor = texture(u_Texture, v_textureCoords);
    color = u_Color;
};