

// This include
#include "TextLabel.h"

/*
*	Textlabel Constructor - sets variables
*	Parameters - text to display, the position, the size, the colour, and the filepath of the font
*	Return - none
*/
TextLabel::TextLabel(std::string text, glm::vec2 position, float scale, glm::vec3 colour, char* font) :
	m_isActive(true)
{
	m_text = text;
	m_colour = colour;
	m_scale = scale / 1000.0f;
	m_position = position / 100.0f;

	// Load shaders
	std::vector<Shader> shaders;
	shaders.push_back(Shader::GetShaderFromFile("Assets\\Shaders\\text-vertex-shader.vs", GL_VERTEX_SHADER));
	shaders.push_back(Shader::GetShaderFromFile("Assets\\Shaders\\text-fragment-shader.fs", GL_FRAGMENT_SHADER));
	m_program = new Program(shaders);

	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}

	FT_Face face; // Load font as face
	if (FT_New_Face(ft, font, 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}

	// Set size to load glyphs as 
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII
	for (GLubyte c = 0; c < 128; c++) {
		// Load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl; continue;
		}

		// Generate texture
		GLuint texture; glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
			face->glyph->bitmap.rows, 0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Now store character for late
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}// for loop end

	glBindTexture(GL_TEXTURE_2D, 0);

	// Destroy FreeType once we're finished 
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	// Configure VAO/VBO for texture quads 
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

/*
*	Textlabel Destructor - deletes the program
*	Parameters - none
*	Return - none
*/
TextLabel::~TextLabel()
{
	delete m_program;
	m_program = 0;
}

/*
*	Renders the textlabel
*	Parameters - none
*	Return - void
*/
void TextLabel::Render()
{
	if (m_isActive)
	{
		glm::vec2 textPos = m_position;
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Activate corresponding render state 
		m_program->Use();
		m_program->setUniform("textColour", m_colour);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(m_vao);

		std::string::const_iterator c;

		for (c = m_text.begin(); c != m_text.end(); c++)
		{
			Character ch = Characters[*c];
			GLfloat xpos = textPos.x + ch.Bearing.x * m_scale;

			GLfloat ypos = textPos.y - (ch.Size.y - ch.Bearing.y) * m_scale;
			GLfloat w = ch.Size.x * m_scale;
			GLfloat h = ch.Size.y * m_scale;
			// Update VBO for each character 
			GLfloat vertices[6][4] = {
				{ xpos, ypos + h, 0.0, 0.0 },
				{ xpos, ypos, 0.0, 1.0 },
				{ xpos + w, ypos, 1.0, 1.0 },
				{ xpos, ypos + h, 0.0, 0.0 },
				{ xpos + w, ypos, 1.0, 1.0 },
				{ xpos + w, ypos + h, 1.0, 0.0 }
			};

			// Render glyph texture over quad

			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// Now advance cursors for next glyph 
			textPos.x += (ch.Advance >> 6) * m_scale;
		} // end of for loop

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);
	}
}

/*
*	Sets the position of the textlabel
*	Parameters - the new position to move to
*	Return - void
*/
void TextLabel::SetPosition(glm::vec2 position)
{
	m_position = position;
}

/*
*	Sets the colour of the textlabel
*	Parameters - the new colour to set
*	Return - void
*/
void TextLabel::SetColor(glm::vec3 Color)
{
	m_colour = Color;
}

/*
*	Sets the size of the textlabel
*	Parameters - the new size to scale to
*	Return - void
*/
void TextLabel::SetScale(GLfloat scale)
{
	m_scale = scale/1000;
}

/*
*	Sets the text of the textlabel
*	Parameters - the new text to display
*	Return - void
*/
void TextLabel::SetText(std::string text)
{
	m_text = text;
}

/*
*	Turns the textlabel on or off
*	Parameters - none
*	Return - void
*/
void TextLabel::SwitchActive()
{
	m_isActive = !m_isActive;
}

/*
*	returns whether the textlabel is active or not
*	Parameters - none
*	Return - bool
*/
bool TextLabel::GetIsActive()
{
	return m_isActive;
}