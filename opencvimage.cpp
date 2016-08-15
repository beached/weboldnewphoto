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

//opencvimage.cpp

#include <iostream>
#include <opencv/highgui.h>
#include <stdexcept>
#include <string>

#include <daw/daw_exception.h>
#include "opencvimage.h"


namespace daw { namespace imaging {
	OpenCVImage::OpenCVImage( int width, int height, int depth, int channels ):
			_image( cvCreateImage( cvSize( width, height ), depth, channels ) ) {

		daw::exception::daw_throw_on_null( _image, "OpenCVImage::OpenCVImag - Could not create IplImage" );
		if( _image->nChannels != 3 ) {
			static std::string const msg = "OpenCVImage::OpenCVImag - Uploaded image must be in RGB format";
			std::cerr << msg << std::endl;
			throw std::runtime_error( msg );
		}
	}

	OpenCVImage::OpenCVImage( std::string const & filename, int iscolor ):_image( cvLoadImage( filename.c_str( ), iscolor ) ) {
		daw::exception::daw_throw_on_null( _image, "OpenCVImage::OpenCVImag - Could not create IplImage" );
		if( _image->nChannels != 3 ) {
			static std::string const msg = "OpenCVImage::OpenCVImag - Uploaded image must be in RGB format";
			std::cerr << msg << std::endl;
			throw std::runtime_error( msg );
		}
	}

	OpenCVImage::OpenCVImage( const OpenCVImage &src ): 
			_image( nullptr ) {

		cvCopy( src._image, _image, nullptr );
	}

	OpenCVImage::operator IplImage*( ) {
		return _image;
	}

	IplImage* OpenCVImage::get( ) {
		return _image;
	}

	OpenCVImage::~OpenCVImage( ) {
		if( nullptr != _image ) {
			cvReleaseImage( &_image );
			_image = nullptr;
		}
	}

	OpenCVImage &OpenCVImage::operator=( OpenCVImage const & src ) {
		if( nullptr != _image ) {
			cvReleaseImage( &_image );
			_image = nullptr;
		}
		cvCopy( src._image, _image, nullptr );
		return *this;
	}
}}
