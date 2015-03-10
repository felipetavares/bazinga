#include "cache.h"
#include "text.h"
#include "gl/glwin.h"
#include "console.h"
#include <bitset>
using namespace bazinga;

text::AreaBuffer::AreaBuffer (int w, int h) {
	freeAreas.push_back(Area({0, 0, w, h}));

	console << LINEINFO << "initial area: (" << freeAreas[0].x << ", " <<
												freeAreas[0].y << ", " <<
												freeAreas[0].w << ", " <<
												freeAreas[0].h << ")" << outline;
}

text::Area text::AreaBuffer::insert (int w, int h) {
	for (int i=0;i<freeAreas.size();i++) {
		auto a = freeAreas[i];

		if (a.w >= w && a.h >= h) {
			freeAreas.erase(freeAreas.begin()+i);

			auto newAreas = createChildren(a, w, h);

			for (auto na :newAreas)
				freeAreas.push_back(na);

			return a;
		}
	}

	return Area({-1,-1,-1,-1});
}

vector <text::Area> text::AreaBuffer::createChildren (Area a, int w, int h) {
	vector <Area> areas;

	if (a.w <= w && a.h <= h) {
		return areas;
	} else {
		if (a.w > w) {
			Area a1 = Area ({
				w+a.x, a.y,
				a.w-w, h
			});

			areas.push_back(a1);
		}

		if (a.h > h) {
			Area a2 = Area ({
				a.x, h+a.y,
				a.w, a.h-h
			});

			areas.push_back(a2);
		}

		return areas;
	}
}

// Font
text::Font::Font (Path file):
	color(1,1,1,1) {
	if (FT_New_Face(freetype, file.getPath().c_str(), 0, &face)) {
		console << LINEINFO << "bazinga: text: font: cannot open " << file.getPath() << outline;
	} else {
		console << LINEINFO << "bazinga: text: font: loaded from " << file.getPath() << outline;

		FT_Select_Charmap(face, FT_ENCODING_UNICODE);

		setSize(12);

		newAtlas();
	}
}

text::Font::~Font () {
	console << LINEINFO << "bazinga: text: font: closing font...";

	if (FT_Done_Face(face)) {
		console << " fail" << outline;
	} else {
		console << " ok" << outline;
	}

	for (auto i :charCache) {
		delete i.second;
	}

	for (auto texture :textures)
		glDeleteTextures(1, &texture);

	for (auto areaManager :areaManagers) {
		delete areaManager;
	}
}

void text::Font::setColor (video::Color color) {
	this->color = color;
}

void text::Font::setSize (int size) {
	FT_Set_Pixel_Sizes(face, 0, size);
	fontSize = size;
}

uint32_t text::Font::utf82unicode(uint32_t utf8) {
	uint32_t unicode;
	// First byte
	uint32_t fb1 = (utf8>>0);
	uint32_t fb2 = (utf8>>8);
	uint32_t fb3 = (utf8>>16);
	uint32_t fb4 = (utf8>>24);

	if ((fb1&0b10000000) == 0) { // ASCII
		return (unsigned long)fb1;
	} else
	if ((fb2&0b11100000) == 0b11000000) { // Two bytes
		return (((utf8>>8)&0b00011111)<<6) | (utf8&0b00111111);
	} else
	if ((fb3&0b11110000) == 0b11100000) { // Three bytes
		return (((utf8>>16)&0b00001111)<<12) | (((utf8>>8)&0b00111111)<<6) | (utf8&0b00111111);
	} else
	if ((fb4&0b11111000) == 0b11110000) { // Four bytes
		return (((utf8>>24)&0b00000111)<<18) | (((utf8>>16)&0b00111111)<<12) | (((utf8>>8)&0b00111111)<<6) | (utf8&0b00111111);
	} else {
		return 0;
	}
}

int text::Font::utf8asint(const char* str, int len, uint32_t& utf8) {
	int advance = 1;

	if (len < 1) {
		return -1;
	}

	if ((str[0]&0b10000000) == 0) { // ASCII
		advance = 1;
		utf8 = (unsigned char)str[0];
	} else
	if ((str[0]&0b11100000) == 0b11000000) { // Two bytes
		advance = 2;
		if (len < 2)
			return -1;
		utf8 = (uint32_t((unsigned char)str[0])<<8) | uint32_t((unsigned char)str[1]);
	} else
	if ((str[0]&0b11110000) == 0b11100000) { // Three bytes
		advance = 3;
		if (len < 3)
			return -1;
		utf8 = (uint32_t((unsigned char)str[0])<<16) | ((unsigned char)uint32_t(str[1])<<8) | (unsigned char)str[2];
	} else
	if ((str[0]&0b11111000) == 0b11110000) { // Four bytes
		advance = 4;
		if (len < 4)
			return -1;
		utf8 = (uint32_t((unsigned char)str[0])<<24) | (uint32_t((unsigned char)str[1])<<16) | (uint32_t((unsigned char)str[2])<<8) | (unsigned char)str[3];
	}

	return advance;
}

int text::Font::measure (const char* str, int len, float& w, float& h, float &dh) {
	Char *bchar;
	uint64_t c;
	uint32_t utf8Char;

	int advance = utf8asint(str, len, utf8Char);

	c = (uint64_t(fontSize)<<32) | uint64_t(utf8Char);

	if (advance < 0)
		return advance;

	try {
		bchar = charCache.at(c);
	} catch (exception e) {
		// Load the character
		if (FT_Load_Char (face, utf82unicode(c), FT_LOAD_RENDER)) {
			console << LINEINFO << "cannot load character " << c << outline;
		} else {
			//cout << "bazinga: text: font: loading character " << c << endl;

			// Get the pre-rendered glyph
			FT_GlyphSlot g = face->glyph;

			bchar = new Char();

			bchar->fontSize = fontSize;
			bchar->w = g->bitmap.width;
			bchar->h = g->bitmap.rows;
			bchar->dw = g->bitmap_left;
			bchar->dh = -int(g->bitmap_top);
			bchar->adw = g->advance.x >> 6;
			bchar->adh = g->advance.y >> 6;

			Area position = areaManagers[areaManagers.size()-1]->insert(bchar->w, bchar->h);

			if (position.x < 0) {
				newAtlas();
				position = areaManagers[areaManagers.size()-1]->insert(bchar->w, bchar->h);
			}

			bchar->tn = areaManagers.size()-1;

			bchar->x = position.x;
			bchar->y = position.y;

			//cout << "bazinga: text: font: free area at (" << bchar->x << ", " << bchar->y << ")" << endl;
			//cout << "\t" << "uploading char sized (" << bchar->w << ", " << bchar->h << ") to GPU" << endl;

			glBindTexture(GL_TEXTURE_2D, textures[bchar->tn]);
			glTexSubImage2D(GL_TEXTURE_2D, 0, bchar->x, bchar->y, bchar->w, bchar->h, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

			charCache[c] = bchar;
		}
	}

	w = bchar->adw>bchar->w?bchar->adw:bchar->w;
	h = bchar->h+bchar->dh;
	dh = bchar->dh;

    return advance;
}

int text::Font::render (const char *str, int len) {
	Char *bchar;
	uint64_t c;
	uint32_t utf8Char;

	int advance = utf8asint(str, len, utf8Char);

	c = (uint64_t(fontSize)<<32) | uint64_t(utf8Char);

	if (advance < 0)
		return advance;

	try {
		bchar = charCache.at(c);
	} catch (exception e) {
		// Load the character
		if (FT_Load_Char (face, utf82unicode(c), FT_LOAD_RENDER)) {
			console << LINEINFO << "cannot load character " << c << outline;
		} else {
			//cout << "bazinga: text: font: loading character " << c << endl;

			// Get the pre-rendered glyph
			FT_GlyphSlot g = face->glyph;

			bchar = new Char();

			bchar->fontSize = fontSize;
			bchar->w = g->bitmap.width;
			bchar->h = g->bitmap.rows;
			bchar->dw = g->bitmap_left;
			bchar->dh = -int(g->bitmap_top);
			bchar->adw = g->advance.x >> 6;
			bchar->adh = g->advance.y >> 6;

			Area position = areaManagers[areaManagers.size()-1]->insert(bchar->w, bchar->h);

			if (position.x < 0) {
				newAtlas();
				position = areaManagers[areaManagers.size()-1]->insert(bchar->w, bchar->h);
			}

			bchar->tn = areaManagers.size()-1;

			bchar->x = position.x;
			bchar->y = position.y;

			//cout << "bazinga: text: font: free area at (" << bchar->x << ", " << bchar->y << ")" << endl;
			//cout << "\t" << "uploading char sized (" << bchar->w << ", " << bchar->h << ") to GPU" << endl;

			glBindTexture(GL_TEXTURE_2D, textures[bchar->tn]);
			glTexSubImage2D(GL_TEXTURE_2D, 0, bchar->x, bchar->y, bchar->w, bchar->h, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

			charCache[c] = bchar;
		}
	}

    bazinga::cache::getShaderProgram("text")->use();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable (GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[bchar->tn]);

	cache::getShaderProgram("text")->setUniform("sampler", 0);
	cache::getShaderProgram("text")->setUniform("color", color.r, color.g, color.b, 1);

	float sx = float(bchar->x)/float(bufferLen);
	float sy = float(bchar->y)/float(bufferLen);
	float ex = float(bchar->x+bchar->w)/float(bufferLen);
	float ey = float(bchar->y+bchar->h)/float(bufferLen);

	glPushMatrix();
		glTranslatef(bchar->dw, bchar->dh, 0);
		glBegin(GL_QUADS);
			glTexCoord2f (sx,sy);  glVertex3f(0, 0, 0);
			glTexCoord2f (ex,sy);  glVertex3f(bchar->w, 0, 0);
			glTexCoord2f (ex,ey);  glVertex3f(bchar->w, bchar->h, 0);
			glTexCoord2f (sx,ey);  glVertex3f(0, bchar->h, 0);
		glEnd();
	glPopMatrix();

	glTranslatef(bchar->adw,bchar->adh,0);

    bazinga::cache::getShaderProgram("text")->stopUse();

    return advance;
}

void text::Font::newAtlas () {
	console << LINEINFO << "creating new atlas" << outline;

	// Init buffer
	areaManagers.push_back(new AreaBuffer(bufferLen, bufferLen));

	// Create a GPU texture
	GLuint texture;

	glEnable (GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, bufferLen, bufferLen, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	textures.push_back(texture);
}

// Main code
FT_Library text::freetype;
text::Font* text::font;
text::Align text::alignment;
text::Baseline text::baseline;

void text::init() {
	// Opens the FreeType2 library
	console << LINEINFO << "initializing FreeType2...";

	if (FT_Init_FreeType(&freetype)) {
		console << " fail" << outline;
	} else {
		console << " ok" << outline;
	}

	alignment = Left;
	baseline = Alphabetic;
}

void text::deinit() {
	console << LINEINFO << "deinitializing FreeType2...";

	if (FT_Done_FreeType(freetype)) {
		console << " fail" << outline;
	} else {
		console << " ok" << outline;
	}
}

void text::setAlign (Align align) {
	alignment = align;
}

void text::setBaseline (Baseline bs) {
	baseline = bs;
}

void text::setFont (Font* ft) {
	font = ft;
}

void text::fillText (string text, int x, int y) {
	if (font == NULL) {
		console << LINEINFO << "invalid font" << outline;
		return;
	}

	const char *str = text.c_str();
	int len = text.size();
	TextMetrics metrics;

	if (alignment != Left || baseline != Alphabetic) {
		metrics = measureText(text);
	}

	switch (alignment) {
		case Center:
			x -= metrics.w/2;
		break;
		case Left:
			// Already ok
		break;
		case Right:
			x -= metrics.w;
		break;
	}

	switch (baseline) {
		case Top:
			y -= metrics.dh;
		break;
		case Middle:
			y -= metrics.dh+metrics.h/2;
		break;
		case Bottom:
			y += metrics.h-metrics.dh;
		break;
	}

	glPushMatrix();
	glTranslatef(x, y, 0);
	while (len > 0) {
		int advance = font->render(str, len);

		if (advance > 0) {
			str += advance;
			len -= advance;
		} else {
			cout << "bazinga: text: invalid utf8 string '" << text << "'" << endl;
			break;
		}
	}
	glPopMatrix();
}

text::TextMetrics text::measureText (string text) {
	if (font == NULL) {
		console << LINEINFO << "invalid font" << outline;
		return TextMetrics();
	}

	const char *str = text.c_str();
	int len = text.size();
	float charW, charH, charDH;

	TextMetrics metrics;
	metrics.w = 0;
	metrics.h = 0;
	metrics.dh = 0;

	while (len > 0) {
		int advance = font->measure(str, len, charW, charH, charDH);

		metrics.w += charW;
		metrics.h = metrics.h>charH?metrics.h:charH;
		metrics.dh = metrics.dh<charDH?metrics.dh:charDH;

		if (advance > 0) {
			str += advance;
			len -= advance;
		} else {
			cout << "bazinga: text: invalid utf8 string '" << text << "'" << endl;
			break;
		}
	}

	metrics.h -= metrics.dh;

	return metrics;
}
