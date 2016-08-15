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

//autolevelfilter.cpp

#include <boost/scoped_array.hpp>
#include <iostream>
#include <limits>
#include <omp.h>
#include <stdexcept>
#include <string>
#include <vector>
#include <Wt/WApplication>

#include "autolevelfilter.h"
#include "itypes.h"
#include <daw/daw_exception.h>

namespace daw { namespace imaging {
	
	GenericImage<rgb3> AutoLevelFilter::runfilter( GenericImage<rgb3> const & input_image ) const {

		rgb3 min_vals( std::numeric_limits<unsigned char>::max(), std::numeric_limits<unsigned char>::max(), std::numeric_limits<unsigned char>::max() );
		rgb3 max_vals( std::numeric_limits<unsigned char>::min(), std::numeric_limits<unsigned char>::min(), std::numeric_limits<unsigned char>::min() );

		for( size_t n=0; n<input_image.size( ); ++n ) {
			const rgb3 cur_val = input_image[n];
			if( cur_val.red < min_vals.red ) {
				min_vals.red = cur_val.red;
			}
			if( cur_val.green < min_vals.green ) {
				min_vals.green = cur_val.green;
			}
			if( cur_val.blue < min_vals.blue ) {
				min_vals.blue = cur_val.blue;
			}
			if( cur_val.red > max_vals.red ) {
				max_vals.red = cur_val.red;
			}
			if( cur_val.green > max_vals.green ) {
				max_vals.green = cur_val.green;
			}
			if( cur_val.blue > max_vals.blue ) {
				max_vals.blue = cur_val.blue;
			}		
		}

		rgb3 range_vals( max_vals.red-min_vals.red, max_vals.green-min_vals.green, max_vals.blue-min_vals.blue );
		boost::scoped_array<float> val_mul( new float[3] );
		daw::exception::daw_throw_on_null( val_mul.get( ), "AutoLevelFilter::::runfilter could not allocate float[3] array. Null returned" );

		if( range_vals.red > 0 ) {
			val_mul[0] = 255.0f/(float)range_vals.red;
		} else {
			val_mul[0] = 0.0f;
		}
		if( range_vals.green > 0 ) {
			val_mul[1] = 255.0f/(float)range_vals.green;
		} else {
			val_mul[1] = 0.0f;
		}
		if( range_vals.blue > 0 ) {
			val_mul[2] = 255.0f/(float)range_vals.blue;
		} else {
			val_mul[2] = 0.0f;
		}

		GenericImage<rgb3> output_image( input_image.width( ), input_image.height( ) );
		if( input_image.size( ) != output_image.size( ) ) {
			static std::string const msg = "Input and Output image in autolevel filter are not the same.  This should never happen.";
			Wt::log( "error" ) << msg;
			throw std::runtime_error( msg );
		}

		//TODO: Reenable OMP #pragma omp parallel for
		for( size_t n=0; n<input_image.size( ); ++n ) {
			output_image[n] = rgb3( 0, 0, 0 );
			if( range_vals.red > 0 ) {
				output_image[n].red = (unsigned char)(((float)(input_image[n].red - min_vals.red))*val_mul[0]);
			}
			if( range_vals.green > 0 ) {
				output_image[n].green = (unsigned char)(((float)(input_image[n].green - min_vals.green))*val_mul[0]);
			}
			if( range_vals.blue > 0 ) {
				output_image[n].blue = (unsigned char)(((float)(input_image[n].blue - min_vals.blue))*val_mul[0]);
			}
		}
		return output_image;
	}

}}
