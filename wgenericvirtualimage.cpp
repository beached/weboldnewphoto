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


#include <Wt/WResource>
#include <Wt/WRasterImage>
#include <Wt/WVirtualImage>

#include <daw/grayscale_filter/genericimage.h>
#include "wgenericvirtualimage.h"
#include "whelpers.h"

namespace daw { namespace imaging {
	WGenericVirtualImage::WGenericVirtualImage( const int width, const int height, GenericImage<rgb3> const & generic_image, Wt::WContainerWidget *parent ): Wt::WVirtualImage( width, height, generic_image.width( ), generic_image.height( ), 256, parent ), input_image( generic_image ) {
		enableDragging( );
		redrawAll( );		
	}

	Wt::WResource* WGenericVirtualImage::render( int64_t x, int64_t y, int w, int h ) {
		GenericImage<rgb3> output_image( w, h );
		auto x32 = static_cast<int32_t>(x);
		auto y32 = static_cast<int32_t>(y);

		for( auto row=y32; row < y32+h; ++row ) {
			for( int col=x32; col < x32+w; ++col ) {
				if( row < 0 || col < 0 || row >= (int)input_image.height( ) || col >= (int)input_image.width( ) ) {
					output_image( row - y32, col - x32 ) = rgb3( 0, 0, 0 );
				} else {
					output_image( row - y32, col - x32 ) = input_image( row, col );
				}
			}
		}
		return genericImageToWRaster( output_image );
	}

	void WGenericVirtualImage::genericImage( GenericImage<rgb3> const & src ) {
		input_image = src;
		resize( src.width( ), src.height( ) );
		scrollTo( 0, 0 );		
	}

	GenericImage<rgb3> WGenericVirtualImage::genericImage( ) const {
		return input_image;
	}
}}
