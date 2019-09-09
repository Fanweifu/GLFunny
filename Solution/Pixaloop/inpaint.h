#pragma once
#include"bitmap.h"
#include<math.h>
#include<stdlib.h>
#ifndef MIN
#  define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
#  define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif

typedef void CvArr;
typedef unsigned char uchar;

using namespace bmp;

enum MorphShapes_c
{
	CV_SHAPE_RECT = 0,
	CV_SHAPE_CROSS = 1,
	CV_SHAPE_ELLIPSE = 2,
	CV_SHAPE_CUSTOM = 100 //!< custom structuring element
};

Bitmap<uchar>* getStructuringElement(int shape, SizeI ksize, PointI anchor)
{
	int i, j;
	int r = 0, c = 0;
	double inv_r2 = 0;

	if (ksize == SizeI(1, 1))
		shape = CV_SHAPE_RECT;

	if (shape == CV_SHAPE_ELLIPSE)
	{
		r = ksize.h / 2;
		c = ksize.w / 2;
		inv_r2 = r ? 1. / ((double)r*r) : 0;
	}

	Bitmap<uchar>* elem = new Bitmap<uchar>(ksize);

	for (i = 0; i < ksize.h; i++)
	{
		uchar* ptr = elem->rowPtr(i);
		int j1 = 0, j2 = 0;

		if (shape == CV_SHAPE_RECT || (shape == CV_SHAPE_CROSS && i == anchor.y))
			j2 = ksize.w;
		else if (shape == CV_SHAPE_CROSS)
			j1 = anchor.x, j2 = j1 + 1;
		else
		{
			int dy = i - r;
			if (std::abs(dy) <= r)
			{
				int dx = (int)round(c*std::sqrt((r*r - dy*dy)*inv_r2));
				j1 = std::max(c - dx, 0);
				j2 = std::min(c + dx + 1, ksize.w);
			}
		}

		for (j = 0; j < j1; j++)
			ptr[j] = 0;
		for (; j < j2; j++)
			ptr[j] = 1;
		for (; j < ksize.w; j++)
			ptr[j] = 0;
	}

	return elem;
}

IplConvKernel *cvCreateStructuringElementEx(int cols, int rows,
	int anchorX, int anchorY,
	int shape, int *values)
{
	SizeI ksize(cols, rows);
	PointI anchor(anchorX, anchorY);

	int i, size = rows * cols;
	int element_size = sizeof(IplConvKernel) + size * sizeof(int);
	IplConvKernel *element = (IplConvKernel*)malloc(element_size + 32);

	element->nCols = cols;
	element->nRows = rows;
	element->anchorX = anchorX;
	element->anchorY = anchorY;
	element->nShiftR = shape < CV_SHAPE_ELLIPSE ? shape : CV_SHAPE_CUSTOM;
	element->values = (int*)(element + 1);

	if (shape == CV_SHAPE_CUSTOM)
	{
		for (i = 0; i < size; i++)
			element->values[i] = values[i];
	}
	else
	{
		auto elem = getStructuringElement(shape, ksize, anchor);
		for (i = 0; i < size; i++)
			element->values[i] = elem->head()[i];
		delete elem;
	}

	return element;
}

static void convertConvKernel(const IplConvKernel* src, Bitmap<uchar>& dst, PointI& anchor)
{

	anchor = PointI(src->anchorX, src->anchorY);
	dst.resize(src->nRows, src->nCols);
	int i, size = src->nRows*src->nCols;
	for (i = 0; i < size; i++)
		((uchar*)dst.head())[i] = (uchar)(src->values[i] != 0);
}

void cvDilate(const Bitmap<RgbaC>& srcarr, Bitmap<RgbaC>& dstarr, IplConvKernel* element, int iterations)
{
	PointI anchor(0,0);
	Bitmap<uchar> kernel;
	convertConvKernel(element, kernel, anchor);
	cv::dilate(srcarr, dstarr, kernel, anchor, iterations, cv::BORDER_REPLICATE);
}

typedef struct CvHeapElem
{
	float T;
	int i, j;
	struct CvHeapElem* prev;
	struct CvHeapElem* next;
}
CvHeapElem;

#define CV_MAT_ELEM(mat,row,col,pxSize) *((mat).data.ptr+(size_t)(mat).step*(row)+(pxSize)*(col))

class CvPriorityQueueFloat
{
protected:
	CvHeapElem *mem, *empty, *head, *tail;
	int num, in;

public:
	bool Init(const CvMat* f)
	{
		int i, j;
		for (i = num = 0; i < f->rows; i++)
		{
			for (j = 0; j < f->cols; j++)
				num += CV_MAT_ELEM(*f, uchar, i, j) != 0;
		}
		if (num <= 0) return false;
		mem = (CvHeapElem*)malloc((num + 2) * sizeof(CvHeapElem));
		if (mem == NULL) return false;

		head = mem;
		head->i = head->j = -1;
		head->prev = NULL;
		head->next = mem + 1;
		head->T = -FLT_MAX;
		empty = mem + 1;
		for (i = 1; i <= num; i++) {
			mem[i].prev = mem + i - 1;
			mem[i].next = mem + i + 1;
			mem[i].i = -1;
			mem[i].T = FLT_MAX;
		}
		tail = mem + i;
		tail->i = tail->j = -1;
		tail->prev = mem + i - 1;
		tail->next = NULL;
		tail->T = FLT_MAX;
		return true;
	}

	bool Add(const CvMat* f) {
		int i, j;
		for (i = 0; i < f->rows; i++) {
			for (j = 0; j < f->cols; j++) {
				if (CV_MAT_ELEM(*f, uchar, i, j) != 0) {
					if (!Push(i, j, 0)) return false;
				}
			}
		}
		return true;
	}

	bool Push(int i, int j, float T) {
		CvHeapElem *tmp = empty, *add = empty;
		if (empty == tail) return false;
		while (tmp->prev->T > T) tmp = tmp->prev;
		if (tmp != empty) {
			add->prev->next = add->next;
			add->next->prev = add->prev;
			empty = add->next;
			add->prev = tmp->prev;
			add->next = tmp;
			add->prev->next = add;
			add->next->prev = add;
		}
		else {
			empty = empty->next;
		}
		add->i = i;
		add->j = j;
		add->T = T;
		in++;
		//      printf("push i %3d  j %3d  T %12.4e  in %4d\n",i,j,T,in);
		return true;
	}

	bool Pop(int *i, int *j) {
		CvHeapElem *tmp = head->next;
		if (empty == tmp) return false;
		*i = tmp->i;
		*j = tmp->j;
		tmp->prev->next = tmp->next;
		tmp->next->prev = tmp->prev;
		tmp->prev = empty->prev;
		tmp->next = empty;
		tmp->prev->next = tmp;
		tmp->next->prev = tmp;
		empty = tmp;
		in--;
		//      printf("pop  i %3d  j %3d  T %12.4e  in %4d\n",tmp->i,tmp->j,tmp->T,in);
		return true;
	}

	bool Pop(int *i, int *j, float *T) {
		CvHeapElem *tmp = head->next;
		if (empty == tmp) return false;
		*i = tmp->i;
		*j = tmp->j;
		*T = tmp->T;
		tmp->prev->next = tmp->next;
		tmp->next->prev = tmp->prev;
		tmp->prev = empty->prev;
		tmp->next = empty;
		tmp->prev->next = tmp;
		tmp->next->prev = tmp;
		empty = tmp;
		in--;
		//      printf("pop  i %3d  j %3d  T %12.4e  in %4d\n",tmp->i,tmp->j,tmp->T,in);
		return true;
	}

	CvPriorityQueueFloat(void) {
		num = in = 0;
		mem = empty = head = tail = NULL;
	}

	~CvPriorityQueueFloat(void)
	{
		free(&mem);
	}
};

typedef struct _IplConvKernel
{
	int  nCols;
	int  nRows;
	int  anchorX;
	int  anchorY;
	int *values;
	int  nShiftR;
}
IplConvKernel;

#define KNOWN  0  //known outside narrow band
#define BAND   1  //narrow band (known)
#define INSIDE 2  //unknown
#define CHANGE 3  //servise

#define COPY_MASK_BORDER1_C1(src,dst)	for (int i=0; i<src->height(); i++) {\
											for(int j=0; j<src->width(); j++) {\
												if ((*src)(i,j)!=0)\
													(*dst)(i+1,j+1) = INSIDE;\
											}\
										}

#define SET_BORDER1_C1(image,value) {\
											int i,j,cols =image->width(),rows=image->height();\
											for(j=0; j<cols; j++) {\
												(*image)(0,j) = value;\
											}\
											for (i=1; i<rows-1; i++) {\
												(*image)(i,0) = (*image)(i,cols-1) = value;\
											}\
											for(j=0; j<cols; j++) {\
												(*image)(rows-1,j) = value;\
											}\
										}

void cvInpaint(const Bitmap<RgbaC>& inputMap, const Bitmap<uchar>& inputMask, Bitmap<RgbaC>& outMap,
	double inpaintRange, int flags)
{
	//Bitmap<uchar>* mask, *band, *f, *t, *out;
	CvPriorityQueueFloat *Heap, *Out;
	IplConvKernel *el_cross, *el_range;

	//Bitmap input_hdr, mask_hdr, output_hdr;
	//Bitmap* input_img, *inpaint_mask, *output_img;

	Bitmap<uchar>*inpaint_mask;

	int range = round(inpaintRange);
	int erows, ecols;


	range = MAX(range, 1);
	range = MIN(range, 100);

	ecols = inputMap.width() + 2;
	erows = inputMap.height() + 2;

	auto f = new Bitmap<uchar>(ecols, erows);// cvCreateMat(erows, ecols, CV_8UC1));
	auto t = new Bitmap<float>(ecols, erows);//t.reset(cvCreateMat(erows, ecols, CV_32FC1));
	auto band = new Bitmap<uchar>(ecols, erows);//band.reset(cvCreateMat(erows, ecols, CV_8UC1));
	auto mask = new Bitmap<uchar>(ecols, erows);//mask.reset(cvCreateMat(erows, ecols, CV_8UC1));
	el_cross = cvCreateStructuringElementEx(3, 3, 1, 1, CV_SHAPE_CROSS, NULL);

	outMap = inputMap;//cvCopy(input_img, output_img);
	mask->setValue(KNOWN);//cvSet(mask, cvScalar(KNOWN, 0, 0, 0));
	COPY_MASK_BORDER1_C1(inpaint_mask, mask);
	SET_BORDER1_C1(mask, 0);
	f->setValue(KNOWN);
	t->setValue(1.0e6f);

	cvDilate(mask, band, el_cross, 1);   // image with narrow band
	Heap = cv::makePtr<CvPriorityQueueFloat>();
	if (!Heap->Init(band))
		return;
	cvSub(band, mask, band, NULL);
	SET_BORDER1_C1(band, uchar, 0);
	if (!Heap->Add(band))
		return;
	cvSet(f, cvScalar(BAND, 0, 0, 0), band);
	cvSet(f, cvScalar(INSIDE, 0, 0, 0), mask);
	cvSet(t, cvScalar(0, 0, 0, 0), band);

	if (flags == cv::INPAINT_TELEA)
	{
		out.reset(cvCreateMat(erows, ecols, CV_8UC1));
		el_range.reset(cvCreateStructuringElementEx(2 * range + 1, 2 * range + 1,
			range, range, CV_SHAPE_RECT, NULL));
		cvDilate(mask, out, el_range, 1);
		cvSub(out, mask, out, NULL);
		Out = cv::makePtr<CvPriorityQueueFloat>();
		if (!Out->Init(out))
			return;
		if (!Out->Add(band))
			return;
		cvSub(out, band, out, NULL);
		SET_BORDER1_C1(out, uchar, 0);
		icvCalcFMM(out, t, Out, true);
		icvTeleaInpaintFMM(mask, t, output_img, range, Heap);
	}
	else if (flags == cv::INPAINT_NS) {
		icvNSInpaintFMM(mask, t, output_img, range, Heap);
	}
	else {
		CV_Error(cv::Error::StsBadArg, "The flags argument must be one of CV_INPAINT_TELEA or CV_INPAINT_NS");
	}
}