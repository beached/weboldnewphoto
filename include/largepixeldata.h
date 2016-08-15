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

#include <cstdint>

namespace daw { namespace imaging {
	struct LargePixelData {
		int32_t R;
		int32_t G;
		int32_t B;

		LargePixelData( int32_t red = 0, int32_t green = 0, int32_t blue = 0 );
		LargePixelData( LargePixelData const & ) = default;
		LargePixelData & operator=( LargePixelData const & ) = default;
		LargePixelData( LargePixelData && ) = default;
		LargePixelData & operator=( LargePixelData && ) = default;
		~LargePixelData( );
		static void min( LargePixelData const & value, LargePixelData & cur_min );
		static void max( LargePixelData const & value, LargePixelData & cur_max );
	};
}}

