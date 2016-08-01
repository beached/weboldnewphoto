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


#include <boost/scoped_array.hpp>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <Wt/WApplication>

#include "channelcombiner.h"
#include <daw/grayscale_filter/genericimage.h>
#include "largepixeldata.h"

namespace daw { namespace imaging {
	namespace {
		float too_gs_small( const float &R, const float &G, const float &B ) {
			return 0.299f*R + 0.587f*G + 0.114f*B;
		}

		float round( const float &value ) {
			return floor( value + 0.5f );
		}

		const int max( const uchar3 lpd ) {
			int ret = lpd.red;
			if( lpd.green > ret ) {
				ret = lpd.green;
			}
			if( lpd.blue > ret ) {
				ret = lpd.blue;
			}
			return ret;
		}

		const int max3( const int &value1, const int &value2, const int &value3 ) {
			int maxval = value1;
			if( value2 > maxval ) {
				maxval = value2;
			}
			if( value3 > maxval ) {
				maxval = value3;
			}
			return maxval;
		}

		const int max3( const LargePixelData &pd ) {
			return max3( pd.R, pd.G, pd.B );
		}

		const int min3( const int &value1, const int &value2, const int &value3 ) {
			int maxval = value1;
			if( value2 < maxval ) {
				maxval = value2;
			}
			if( value3 < maxval ) {
				maxval = value3;
			}
			return maxval;
		}

		const int min3( const LargePixelData &pd ) {
			return min3( pd.R, pd.G, pd.B );
		}

		void clampvalue( int &value, const int min, const int max ) {
			if( value < min ) {
				Wt::log( "info" ) << "Had to clamp value from from " << value << " to " << min;
				value = min;
			} else if( value > max ) {
				Wt::log( "info" ) << "Had to clamp value from from " << value << " to " << max;
				value = max;
			}
		}

		void clampvalue( LargePixelData &value, const int min, const int max ) {
			clampvalue( value.R, min, max );
			clampvalue( value.G, min, max );
			clampvalue( value.B, min, max );
		}

		const float colform( const uchar3 c, const float R, const float G, const float B ) {
			return R*(float)c.red + G*(float)c.green + B*(float)c.blue;
		}
	}
	GenericImage ChannelCombiner::runfilter( const GenericImage& image_y, const GenericImage& image_u, const GenericImage& image_v ) const {
		if( !((image_y.size( ) == image_v.size( )) && (image_y.size( ) == image_v.size( ))) ) {
			const std::string msg = "Images are not the same size in channel filter";
			Wt::log( "error" ) << msg;
			throw std::runtime_error( msg );
		}

		boost::scoped_array<LargePixelData> output_lpdimg( new LargePixelData[image_y.size( )] );	//output_lpdimg
		for( size_t n=0; n<image_y.size( ); ++n ) {
			const float Y = (float)image_y[n].red;
			const float U = (float)image_u[n].red;
			const float V = (float)image_v[n].red;			
			output_lpdimg[n] = LargePixelData( (int)(Y + 1.14f*V), (int)(Y - 0.395f*U - 0.581f*V), (int)(Y + 2.032f*U) );
		}

		LargePixelData pd_min( std::numeric_limits<int>::max( ), std::numeric_limits<int>::max( ), std::numeric_limits<int>::max( ) );
		LargePixelData pd_max( std::numeric_limits<int>::min( ), std::numeric_limits<int>::min( ), std::numeric_limits<int>::min( ) );

		// Do not parallelize without accounting for shared data
		for( size_t n=0; n<image_y.size( ); ++n ) {
			LargePixelData::min( output_lpdimg[n], pd_min );
			LargePixelData::max( output_lpdimg[n], pd_max );
		}

		const int max_all = max3( pd_max );
		const int min_all = min3( pd_min );
		const float range_all = (float)(max_all - min_all);

		GenericImage output_image( image_y.width( ), image_y.height( ) );

		//#pragma omp parallel for
		for( int n=0; n<(int)image_y.size( ); ++n ) {
			LargePixelData cur_value;
			cur_value.R = (int)((float)(output_lpdimg[n].R-pd_min.R)*(255.0f/range_all));
			cur_value.G = (int)((float)(output_lpdimg[n].G-pd_min.G)*(255.0f/range_all));
			cur_value.B = (int)((float)(output_lpdimg[n].B-pd_min.B)*(255.0f/range_all));
			clampvalue( cur_value, 0, 255 );
			output_image[n] = uchar3( (unsigned char)cur_value.R, (unsigned char)cur_value.G, (unsigned char)cur_value.B );

		}


		return output_image;
	}
}}
