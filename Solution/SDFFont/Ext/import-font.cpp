#include "import-font.h"
#include "freetype\freetype.h"
#include "msdfgen.h"
#include "Core\Bitmap.h"
#include "save-png.h"
#include <cstdlib>
#include <queue>

//#include <ft2build.h>
//#include FT_FREETYPE_H
//#include FT_OUTLINE_H
//
//#ifdef _WIN32
//    #pragma comment(lib, "freetype.lib")
//#endif

namespace msdfgen {
#define REQUIRE(cond) { if (!(cond)) return false; }

    class FreetypeHandle {
        friend FreetypeHandle * initializeFreetype();
        friend void deinitializeFreetype(FreetypeHandle *library);
        friend FontHandle * loadFont(FreetypeHandle *library, const char *filename);

        FT_Library library;
    };

    class FontHandle {
        friend FontHandle * loadFont(FreetypeHandle *library, const char *filename);
        friend void destroyFont(FontHandle *font);
        friend bool getFontScale(double &output, FontHandle *font);
        friend bool getFontWhitespaceWidth(double &spaceAdvance, double &tabAdvance, FontHandle *font);
        friend bool loadGlyph(Shape &output, FontHandle *font, int unicode, double *advance);
        friend bool getKerning(double &output, FontHandle *font, int unicode1, int unicode2);

        FT_Face face;
    };

    struct FtContext {
        Point2 position;
        Shape *shape;
        Contour *contour;
    };

    static Point2 ftPoint2(const FT_Vector &vector) {
        return Point2(vector.x / 64., vector.y / 64.);
    }

    static int ftMoveTo(const FT_Vector *to, void *user) {
        FtContext *context = reinterpret_cast<FtContext *>(user);
        context->contour = &context->shape->addContour();
        context->position = ftPoint2(*to);
        return 0;
    }

    static int ftLineTo(const FT_Vector *to, void *user) {
        FtContext *context = reinterpret_cast<FtContext *>(user);
        context->contour->addEdge(new LinearSegment(context->position, ftPoint2(*to)));
        context->position = ftPoint2(*to);
        return 0;
    }

    static int ftConicTo(const FT_Vector *control, const FT_Vector *to, void *user) {
        FtContext *context = reinterpret_cast<FtContext *>(user);
        context->contour->addEdge(new QuadraticSegment(context->position, ftPoint2(*control), ftPoint2(*to)));
        context->position = ftPoint2(*to);
        return 0;
    }

    static int ftCubicTo(const FT_Vector *control1, const FT_Vector *control2, const FT_Vector *to, void *user) {
        FtContext *context = reinterpret_cast<FtContext *>(user);
        context->contour->addEdge(new CubicSegment(context->position, ftPoint2(*control1), ftPoint2(*control2), ftPoint2(*to)));
        context->position = ftPoint2(*to);
        return 0;
    }

    FreetypeHandle * initializeFreetype() {
        FreetypeHandle *handle = new FreetypeHandle;
        FT_Error error = FT_Init_FreeType(&handle->library);
        if (error) {
            delete handle;
            return NULL;
        }
        return handle;
    }

    void deinitializeFreetype(FreetypeHandle *library) {
        FT_Done_FreeType(library->library);
        delete library;
    }

    FontHandle * loadFont(FreetypeHandle *library, const char *filename) {
        if (!library)
            return NULL;
        FontHandle *handle = new FontHandle;
        FT_Error error = FT_New_Face(library->library, filename, 0, &handle->face);
        if (error) {
            delete handle;
            return NULL;
        }
        return handle;
    }

    void destroyFont(FontHandle *font) {
        FT_Done_Face(font->face);
        delete font;
    }

    bool getFontScale(double &output, FontHandle *font) {
        output = font->face->units_per_EM / 64.;
        return true;
    }

    bool getFontWhitespaceWidth(double &spaceAdvance, double &tabAdvance, FontHandle *font) {
        FT_Error error = FT_Load_Char(font->face, ' ', FT_LOAD_NO_SCALE);
        if (error)
            return false;
        spaceAdvance = font->face->glyph->advance.x / 64.;
        error = FT_Load_Char(font->face, '\t', FT_LOAD_NO_SCALE);
        if (error)
            return false;
        tabAdvance = font->face->glyph->advance.x / 64.;
        return true;
    }

    bool loadGlyph(Shape &output, FontHandle *font, int unicode, double *advance) {
        if (!font)
            return false;
        FT_Error error = FT_Load_Char(font->face, unicode, FT_LOAD_NO_SCALE);
        if (error)
            return false;
        output.contours.clear();
        output.inverseYAxis = false;
        if (advance)
            *advance = font->face->glyph->advance.x / 64.;

        FtContext context = {};
        context.shape = &output;
        FT_Outline_Funcs ftFunctions;
        ftFunctions.move_to = &ftMoveTo;
        ftFunctions.line_to = &ftLineTo;
        ftFunctions.conic_to = &ftConicTo;
        ftFunctions.cubic_to = &ftCubicTo;
        ftFunctions.shift = 0;
        ftFunctions.delta = 0;
        error = FT_Outline_Decompose(&font->face->glyph->outline, &ftFunctions, &context);
        if (error)
            return false;
        return true;
    }

    bool getKerning(double &output, FontHandle *font, int unicode1, int unicode2) {
        FT_Vector kerning;
        if (FT_Get_Kerning(font->face, FT_Get_Char_Index(font->face, unicode1), FT_Get_Char_Index(font->face, unicode2), FT_KERNING_UNSCALED, &kerning)) {
            output = 0;
            return false;
        }
        output = kerning.x / 64.;
        return true;
    }

#define ABORT(msg) { puts(msg); return 1; }
#define LARGE_VALUE 1e240

    int fontSDF(const char *file, uint16_t unicode, const char *outfile, int width, int height, GlyphInfo& info,Vector2& basePoint, float* &dataptr)
    {
        // Load input
        Vector2 translate;
        double glyphAdvance = 0;
        double pxRange = 2;
        Shape shape;
        FT_Error err;
        FT_Library lib;
        FT_Face face;

        if (FT_Init_FreeType(&lib))  return -1;
        if (FT_New_Face(lib, file, 0, &face)) return -1;

        
        if (FT_Load_Char(face, unicode, FT_LOAD_NO_SCALE)) return -1;
        
        shape.contours.clear();
        shape.inverseYAxis = false;
        

        FtContext context = {};
        context.shape = &shape;
        FT_Outline_Funcs ftFunctions;
        ftFunctions.move_to = &ftMoveTo;
        ftFunctions.line_to = &ftLineTo;
        ftFunctions.conic_to = &ftConicTo;
        ftFunctions.cubic_to = &ftCubicTo;
        ftFunctions.shift = 0;
        ftFunctions.delta = 0;
        if(FT_Outline_Decompose(&face->glyph->outline, &ftFunctions, &context)) return -1;
       


        //if (!unicode)
        //    ABORT("No character specified! Use -font <file.ttf/otf> <character code>. Character code can be a number (65, 0x41), or a character in apostrophes ('A').");
        //FreetypeHandle *ft = initializeFreetype();
        //if (!ft) return -1;
        //FontHandle *font = loadFont(ft, file);
        //if (!font) {
        //    deinitializeFreetype(ft);
        //    ABORT("Failed to load font file.");
        //}
        //if (!loadGlyph(shape, font, unicode, &glyphAdvance)) {
        //    destroyFont(font);
        //    deinitializeFreetype(ft);
        //    ABORT("Failed to load glyph from font file.");
        //}
        //destroyFont(font);
        //deinitializeFreetype(ft);

        // Validate and normalize shape
        if (!shape.validate())
            ABORT("The geometry of the loaded shape is invalid.");
        //shape.normalize();

        Vector2 scale(width/16.0f);

     
        auto meters = face->glyph->metrics;

        const float scl = 1 / 64.0f;
   

        // Auto-frame

        double l = LARGE_VALUE, b = LARGE_VALUE, r = -LARGE_VALUE, t = -LARGE_VALUE;
        shape.bounds(l, b, r, t);

        Vector2 frame(width, height);

        //frame -= 2 * pxRange;

        if (l >= r || b >= t)
            l = 0, b = 0, r = 1, t = 1;
        if (frame.x <= 0 || frame.y <= 0)
            ABORT("Cannot fit the specified pixel range.");
        Vector2 dims(r - l, t - b);

        translate = .5*(frame / scale - dims) - Vector2(l, b);

       /* if (dims.x*frame.y < dims.y*frame.x) {
            translate.set(.5*(frame.x / frame.y*dims.y - dims.x) - l, -b);
            scale = frame.y / dims.y;
        }
        else {
            translate.set(-l, .5*(frame.y / frame.x*dims.x - dims.y) - b);
            scale = frame.x / dims.x;
        }*/

        //translate += pxRange / scale;

        // Compute output
        Bitmap<float> *sdf = new Bitmap<float>(width, height);;
        Bitmap<FloatRGB> msdf;
        generateSDF(*sdf, shape, dims.x , scale, translate);
        

        info.width = scale.x*(meters.width*scl);
        info.height = scale.y*(meters.height*scl);
        info.advance_x = scale.x*(meters.horiAdvance*scl);
        info.offset_x = scale.x*(meters.horiBearingX*scl);
        info.offset_y = scale.y*(meters.horiBearingY*scl);
        info.advance_y = scale.y*(meters.vertAdvance*scl);

       

        basePoint = (Vector2(l-meters.horiBearingX*scl , meters.horiBearingY*scl) + translate)*scale;
        Vector2 lefttop = (Vector2(l, b) + translate) *scale;
        dataptr = (float*)sdf->data();

        return savePng(*sdf, outfile);
    }
   
}

