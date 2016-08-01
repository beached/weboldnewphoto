// The MIT License (MIT)
//
// Copyright (c) 2014-2016 Darrell Wright
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// imagefilter.h
#pragma once

#include <boost/shared_ptr.hpp>
#include "opencvimage.h"

struct uchar3 {
	// Must remain blue, green, red to correlate with IplImage format
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	uchar3( ): blue( 0 ), green( 0 ), red( 0 ) { };
	uchar3( unsigned char R, unsigned char G, unsigned char B ):  blue( B ), green( G ), red( R ) { };
	uchar3( const uchar3 &src ): blue( src.blue ), green( src.green ), red( src.red ) { };
	uchar3 &operator=( const uchar3 &src ) {
		red = src.red;
		green = src.green;
		blue = src.blue;
		return *this;
	}
};

struct double3 {
	// Must remain blue, green, red to correlate with IplImage format
	double X;
	double Y;
	double Z;
	double3( ): X( 0.0 ), Y( 0.0 ), Z( 0.0 ) { };
	double3( double x, double y, double z ):  X( x ), Y( y ), Z( z ) { };
	double3( const double3 &src ): X( src.X ), Y( src.Y ), Z( src.Z ) { };
	double3 &operator=( const double3 &src ) {
		X = src.X;
		Y = src.Y;
		Z = src.Z;
		return *this;
	}
};

namespace daw { namespace imaging {

	class ImageFilter {
	public:
		virtual boost::shared_ptr<OpenCVImage> runfilter( boost::shared_ptr<OpenCVImage> input_image ) = 0;
	protected:
		~ImageFilter( ) { }
	};

}}

