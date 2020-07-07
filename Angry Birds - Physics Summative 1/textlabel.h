

#pragma once

#ifndef TEXTLABEL_H
#define TEXTLABEL_H

// Local includes
#include "utils.h"
#include "program.h"

// Third-party includes
#include <ft2build.h>
#include FT_FREETYPE_H

// Library includes
#include <map>
#include <string>
#include <iostream>

struct Character {
	GLuint TextureID;
	glm::ivec2 Size; // Size of glyph
	glm::ivec2 Bearing;
	GLuint Advance;
};

class TextLabel 
{
public:
	TextLabel(std::string text, glm::vec2 position, float scale, glm::vec3 colour, char* font);
	~TextLabel();

	void Render();
	//add setPosition, setColor, setScale and setText functions
	void SetPosition(glm::vec2 position);
	void SetColor(glm::vec3 Color);
	void SetScale(GLfloat scale);
	void SetText(std::string text);

	bool GetIsActive();
	void SwitchActive();

private:
	std::string m_text;
	GLfloat m_scale;
	glm::vec3 m_colour;
	glm::vec2 m_position;

	bool m_isActive;
	Program* m_program;
	GLuint m_vao, m_vbo;
	std::map<GLchar, Character> Characters;
};

#endif