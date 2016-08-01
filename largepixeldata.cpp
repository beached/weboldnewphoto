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

//largepixeldata.cpp

#include "largepixeldata.h"

namespace daw { namespace imaging {
	LargePixelData::LargePixelData(const int red, const int green, const int blue): R( red ), G( green ), B( blue ) { }
	LargePixelData::LargePixelData( ): R( 0 ), G( 0 ), B( 0 ) { }

	void LargePixelData::min( const LargePixelData &value, LargePixelData &cur_min ) {
		if ( value.R < cur_min.R ) {
			cur_min.R = value.R;
		}
		if ( value.G < cur_min.G ) {
			cur_min.G = value.G;
		}
		if ( value.B < cur_min.B ) {
			cur_min.B = value.B;
		}
	}

	void LargePixelData::max( const LargePixelData &value, LargePixelData &cur_max ) {
		if ( value.R > cur_max.R ) {
			cur_max.R = value.R;
		}
		if ( value.G > cur_max.G ) {
			cur_max.G = value.G;
		}
		if ( value.B > cur_max.B ) {
			cur_max.B = value.B;
		}
	}

	LargePixelData::LargePixelData( const LargePixelData &src ):R( src.R ), G( src.G ), B( src.B ) { };

	LargePixelData &LargePixelData::operator=( const LargePixelData &src ) {
		R = src.R;
		G = src.G;
		B = src.B;
		return *this;
	}

}}
