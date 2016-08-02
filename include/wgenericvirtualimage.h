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

#pragma once

#include <Wt/WVirtualImage>
#include <cstdint>

#include <daw/grayscale_filter/genericimage.h>

namespace Wt {
	class WRasterImage;
}

namespace daw { namespace imaging {

	class WGenericVirtualImage: public Wt::WVirtualImage {
	public:
		WGenericVirtualImage( int width, int height, GenericImage<rgb3> const & generic_image = GenericImage<rgb3>( 0, 0 ), Wt::WContainerWidget *parent = nullptr );
		void genericImage( GenericImage<rgb3> const & src );
		GenericImage<rgb3> genericImage( ) const;

		WGenericVirtualImage( ) = default;
		WGenericVirtualImage( WGenericVirtualImage const & ) = default;
		WGenericVirtualImage( WGenericVirtualImage && ) = default;
		WGenericVirtualImage & operator=( WGenericVirtualImage const & ) = default;
		WGenericVirtualImage & operator=( WGenericVirtualImage && ) = default;
		virtual ~WGenericVirtualImage( );
	private:
		GenericImage<rgb3> input_image;
		virtual Wt::WResource* render( int64_t x, int64_t y, int w, int h );
	};
}}

