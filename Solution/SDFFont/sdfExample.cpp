#include"sdfExample.h"

#include"msdfgen.h"
#include "freetype\freetype.h"
#include "Core\Shape.h"
#include "Ext\save-png.h"

using namespace msdfgen;

#define ABORT(msg) { puts(msg); return 1; }
#define LARGE_VALUE 1e240

bool initLibary(FT_Library & lib)
{
    FT_Error error = FT_Init_FreeType(&lib);
    if (error) {
        delete lib;
        return false;
    }
    return true;
}

void releaseLibary(FT_Library & lib)
{
    FT_Done_FreeType(lib);
    delete lib;
}

bool loadFaceTTF(FT_Library lib, const char * filename, FT_Face &face)
{
    return   !FT_New_Face(lib, filename, 0, &face);
}

void destroyFace(FT_Face & face)
{
    FT_Done_Face(face);
    delete face;
}

bool loadChar(FT_Face face, uint16_t unicode)
{
    return !FT_Load_Char(face, unicode, FT_LOAD_NO_SCALE);
}

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

bool generateShape(FT_GlyphSlot glyph, Shape & shp)
{
    shp.contours.clear();
    shp.inverseYAxis = false;

    FtContext context = {};
    context.shape = &shp;
    FT_Outline_Funcs ftFunctions;
    ftFunctions.move_to = &ftMoveTo;
    ftFunctions.line_to = &ftLineTo;
    ftFunctions.conic_to = &ftConicTo;
    ftFunctions.cubic_to = &ftCubicTo;
    ftFunctions.shift = 0;
    ftFunctions.delta = 0;
    FT_Outline_Decompose(&glyph->outline, &ftFunctions, &context);
    return shp.validate();
}

int loadFont(const char* input, int unicode, const char* output, int width, int height) {
    FT_Error err;
    FT_Library lib;
    FT_Face face;
    double pxPadding = 2;

    initLibary(lib);
    loadFaceTTF(lib, input, face);
    loadChar(face, unicode);

    auto meters = face->glyph->metrics;

    Shape shape;

    bool gen = generateShape(face->glyph, shape);
    shape.normalize();

    //if (!loadGlyph(shape, font, unicode, &glyphAdvance)) {
    //    destroyFont(font);
    //    deinitializeFreetype(ft);
    //    ABORT("Failed to load glyph from font file.");
    //}
    //destroyFont(font);
    //deinitializeFreetype(ft);
    //if (!shape.validate())
    //    ABORT("The geometry of the loaded shape is invalid.");

    Vector2 scale(1);

    //获取包罗盒

    double l = LARGE_VALUE, b = LARGE_VALUE, r = LARGE_VALUE, t = LARGE_VALUE;
    shape.bounds(l, b, r, t);

    //填充长宽 pxPadding 字体区域间距
    Vector2 frame(width, height);
    frame -= 2 * pxPadding;

    Vector2 dims(r - l, t - b);//矢量包罗盒
    Vector2 translate;

    //确定比例缩放 （字体长宽同比例） 位移
    if (dims.x*frame.y < dims.y*frame.x) {
        //frame宽高比 较高。
        translate.set(.5*(frame.x / frame.y*dims.y - dims.x) - l, -b);
        scale = frame.y / dims.y;
    }
    else {
        translate.set(-l, .5*(frame.y / frame.x*dims.x - dims.y) - b);
        scale = frame.x / dims.x;
    }

    translate += pxPadding / scale;

    //先移动后缩放
    Bitmap<float> sdf(width, height);
    generateSDF(sdf, shape, dims.x, scale, translate);

    savePng(sdf, "fontsdf.png");

    return 0;
}