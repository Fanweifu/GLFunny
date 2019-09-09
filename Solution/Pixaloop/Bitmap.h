#pragma once
#include<assert.h>
#include<stdio.h>

namespace bmp {

	typedef unsigned int uint;
	typedef unsigned char uchar;

	template <typename T>
	struct Rgb {
		T R;
		T G;
		T B;
	};
	template <typename T>
	struct Rgba {
		T R;
		T G;
		T B;
		T A;
	};

	typedef Rgba<float> RgbaF;
	typedef Rgba<uchar> RgbaC;

	template <typename T>
	struct Size {
		T w;
		T h;
		Size(T _w, T _h) :w(_w), h(_h) {}

		operator==(Size right) {
			return right.w == w&&right.h == h;
		}
	};
	typedef Size<int> SizeI;

	template <typename T>
	struct Point {
		T x;
		T y;
		Point(T _x, T _y) :x(_x), y(_y) {}
	};
	typedef Point<int> PointI;
	/// A 2D image bitmap with N channels of type T. Pixel memory is managed by the class.
	template <typename T>
	class Bitmap {

	public:

		void resize(uint rows, uint cols) {
			if (cols == w && rows == h) return;
			release();
			if (rows*cols > 0) pixels = new T[cols*rows];
			w = cols; h = rows;
		}

		void copy(const T* source, uint rows, uint cols) {
			assert(source != nullptr);
			resize(rows, cols);
			memcpy((void*)this->head(), source, w*h * sizeof(T));
		}

		Bitmap() : pixels(NULL), w(0), h(0) { }

		Bitmap(uint width, uint height) {
			resize(height, width);
		}

		Bitmap(SizeI size):Bitmap(size.w,size.h) {

		}

		Bitmap<T> & operator=(const Bitmap<T> &orig) {
			copy(orig.head(), orig.height(), orig.width());
		}

		Bitmap(const Bitmap<T> &orig) {
			*this = orig;
		}

		~Bitmap() {
			release();
		}


		uint width() const { return w; }
		uint height() const { return h; }

		T& operator()(uint rowi, uint coli) {
			assert(coli < w && rowi < h);
			return *(pixels + (rowi*w + coli));
		}

		T* rowPtr(uint i) {
			assert(i < h);
			return pixels + i*w;
		}

		const T* head() const { return pixels; }

		void setValue(const T& v) {
			for (uint i = 0; i < h; i++)
				for (uint j = 0; j < w; j++)
					*(this)(i, j) = v;
		}

	protected:
		void release() {
			if (pixels) {
				delete[] pixels;
				pixels = nullptr;
			}
			w = h = 0;
		}

	private:
		T *pixels;
		uint w, h;
	};
}


