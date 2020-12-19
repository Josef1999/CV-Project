#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "include/learnopengl/shader_m.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <string>

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class Text
{
public:
	Text() {};
	~Text() {};

	// Setters
	void SetPosition(glm::vec2 pos) { this->x = pos.x; this->y = pos.y; }
	glm::vec2 GetPosition() { return glm::vec2(this->x, this->y); }
	void SetColor(glm::vec3 col) { this->color = col; }
	void SetScale(GLfloat scale) { this->scale = scale; }
	void SetText(std::string text) { this->text = text; }

	void Configure(Shader& text_shader, std::string font);
	void Render(Shader& text_shader);

private:
	std::map<GLchar, Character> characters;
	unsigned int VAO, VBO;

	std::string text;
	float x, y, scale;
	glm::vec3 color;
};
