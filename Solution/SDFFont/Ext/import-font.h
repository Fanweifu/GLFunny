
#pragma once

#include <cstdlib>
#include "../core/Shape.h"

namespace msdfgen {

class FreetypeHandle;
class FontHandle;

struct GlyphInfo
{
   
    /// Glyph's width in pixels.
    float width;

    /// Glyph's height in pixels.
    float height;

    /// Glyph's left offset in pixels
    float offset_x;

    /// Glyph's top offset in pixels.
    ///
    /// @remark This is the distance from the baseline to the top-most glyph
    ///   scan line, upwards y coordinates being positive.
    float offset_y;

    /// For horizontal text layouts, this is the unscaled horizontal
    /// distance in pixels used to increment the pen position when the
    /// glyph is drawn as part of a string of text.
    float advance_x;

    /// For vertical text layouts, this is the unscaled vertical distance
    /// in pixels used to increment the pen position when the glyph is
    /// drawn as part of a string of text.
    float advance_y;

};

/// Initializes the FreeType library
FreetypeHandle * initializeFreetype();
/// Deinitializes the FreeType library
void deinitializeFreetype(FreetypeHandle *library);
/// Loads a font file and returns its handle
FontHandle * loadFont(FreetypeHandle *library, const char *filename);
/// Unloads a font file
void destroyFont(FontHandle *font);
/// Returns the size of one EM in the font's coordinate system
bool getFontScale(double &output, FontHandle *font);
/// Returns the width of space and tab
bool getFontWhitespaceWidth(double &spaceAdvance, double &tabAdvance, FontHandle *font);
/// Loads the shape prototype of a glyph from font file
bool loadGlyph(Shape &output, FontHandle *font, int unicode, double *advance = NULL);
/// Returns the kerning distance adjustment between two specific glyphs.
bool getKerning(double &output, FontHandle *font, int unicode1, int unicode2);

int fontSDF(const char *file, uint16_t unicode, const char *outfile, int width, int height, GlyphInfo& info , Vector2& basePoint, float* &dataptr);

}
