#pragma once

extern const char* fishVS = \
"#version 430\n"
"layout(location = 0) in vec2 v_position;\n"
"layout(location = 1) uniform vec3 v_color;\n"
"layout(location = 2) uniform mat4 modelMatrix;\n"
"layout(location = 3) uniform mat4 projectionMatrix;\n"
"out vec4 f_color;\n"
"void main(void)\n"
"{\n"
"	f_color = vec4(v_color, 1.0f);\n"
"	gl_Position = projectionMatrix * modelMatrix * vec4(v_position, 0.0f, 1.0f);\n"
"}\n";

extern const char* fishFS = \
"#version 430\n"
"in vec4 f_color;\n"
"out vec4 color;\n"
"void main(void)\n"
"{\n"
"	color = f_color;\n"
"}\n";

//-------------------------------------------

extern const char* ringVS = \
"#version 430\n"
"layout(location = 0) in vec2 v_position;\n"
"layout(location = 1) uniform vec3 v_color;\n"
"layout(location = 2) uniform mat4 projectionMatrix;\n"
"out vec4 f_color;\n"
"void main(void)\n"
"{\n"
"	f_color = vec4(v_color, 1.0f);\n"
"	gl_Position = projectionMatrix * vec4(v_position, 0.0f, 1.0f);\n"
"}\n";

extern const char* ringFS = \
"#version 430\n"
"in vec4 f_color;\n"
"out vec4 color;\n"
"void main(void)\n"
"{\n"
"	color = f_color;\n"
"}\n";

//-------------------------------------------

extern const char* eyeVS = \
"#version 430\n"
"layout(location = 0) in vec2 v_position;\n"
"void main(void)\n"
"{\n"
"	gl_Position = vec4(v_position, 0.0f, 1.0f);\n"
"}\n";

extern const char* eyeFS = \
"#version 430\n"
"out vec4 color;\n"
"void main(void)\n"
"{\n"
"	color = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\n";

//-------------------------------------------
