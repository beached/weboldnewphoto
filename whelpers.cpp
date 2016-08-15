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

#include <Wt/WColor>
#include <Wt/WRasterImage>

#include <daw/grayscale_filter/genericimage.h>

#include "itypes.h"
#include <daw/daw_exception.h>
#include "whelpers.h"

namespace daw { namespace imaging {

	Wt::WRasterImage* genericImageToWRaster( GenericImage<rgb3> const & input_image ) {

		auto output_image = new Wt::WRasterImage( "jpeg", input_image.width( ), input_image.height( ) );
		daw::exception::daw_throw_on_null( output_image, "Could not allocate output_image in genericImageToWRaster.  Null returned" );
	
		for( size_t y=0; y<input_image.height( ); ++y ) {
			for( size_t x=0; x<input_image.width( ); ++x ) {
				auto cur_val = input_image( y, x );
				output_image->setPixel( x, y, Wt::WColor( cur_val.red, cur_val.green, cur_val.blue ) );
			}
		}
		return output_image;
	}

}}
