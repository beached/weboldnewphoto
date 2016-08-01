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

//newoldphoto.cpp

#include "weboldnewphoto.h"
#include <daw/grayscale_filter/genericrgb.h>
#include <daw/grayscale_filter/genericimage.h>
#include <daw/grayscale_filter/filterdawgs.h>
#include <daw/grayscale_filter/filterdawgscolourize.h>
#include <daw/grayscale_filter/helpers.h>
#include "nullptr.h"

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <cstdio>
#include <fstream>
#include <functional>
#include <iostream>
#include <omp.h>
#include <map>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WComboBox>
#include <Wt/WFileResource>
#include <Wt/WFileUpload>
#include <Wt/WImage>
#include <Wt/WOverlayLoadingIndicator>
#include <Wt/WPanel>
#include <Wt/WProgressBar>
#include <Wt/WPushButton>
#include <Wt/WRasterImage>
#include <Wt/WString>
#include <Wt/WTabWidget>
#include <Wt/WVirtualImage>



namespace daw { namespace imaging {
	namespace {
		template<typename T>
		auto get_repaint_formulas( ) {
			std::map<T, uint8_t> ret_map;
			ret_map["Ratio"] = 0;
			ret_map["YUV"] = 1;
			ret_map["RGB Multiply 1"] = 2;
			ret_map["RGB Addition"] = 3;
			ret_map["RGB Multiply 2"] = 4;
			ret_map["RGB Multiply 3"] = 5;

			return ret_map;
		}   


		//inline void clearWRaster( Wt::WImage* wimg, Wt::WRasterImage* wraster ) {
		void clearWRaster( Wt::WImage* wimg, Wt::WRasterImage* wraster ) {
			// WRaster is null the first time.  Clear it every other time.
			if( nullptr != wraster  ) {
				wimg->setImageLink( Wt::WLink( ) );
				//wimg->setResource( wraster );
				//wraster = nullptr;
			}
		}

		//inline Wt::WRasterImage* GenericImageToWRaster( const GenericImage<rgb3>& input_image ) {	
		Wt::WRasterImage* GenericImageToWRaster( const GenericImage<rgb3>& input_image, Wt::WObject* parent ) {
			
			auto output_image = new Wt::WRasterImage( "JPG", input_image.width( ), input_image.height( ), parent );
			
			
			nullcheck( output_image, "Could not allocate output_image in GenericImageoWRaster.  Null returned" );

			for( size_t y=0; y<input_image.height( ); ++y ) {
				for( size_t x=0; x<input_image.width( ); ++x ) {
					const rgb3 cur_val = input_image( y, x );
					output_image->setPixel( x, y, Wt::WColor( cur_val.red, cur_val.green, cur_val.blue ) );
				}
			}
			
			return output_image;
		}

		//inline void setWRaster( Wt::WImage* wimg, Wt::WRasterImage* wraster, const GenericImage<rgb3>& image ) {
		void setWRaster( Wt::WImage* wimg, Wt::WRasterImage* wraster, const GenericImage<rgb3>& image, Wt::WObject* parent ) {
			if( nullptr != wraster ) {
				parent->removeChild( wraster );
			}
			wraster = GenericImageToWRaster( image, parent );
			
			nullcheck( wraster, "wraster returned from cvImgToRaster is null" );			
			wimg->setResource( wraster );
			//wimg->setImageLink( wraster );
			wimg->refresh( );			
		}
	}

	WebOldNewPhoto::WebOldNewPhoto( const Wt::WEnvironment &env ): Wt::WApplication( env ), image_original( new GenericImage<rgb3>( 0, 0 ) ), image_grayscale( new GenericImage<rgb3>( 0, 0 ) ), wc_rasterimage_original( nullptr ) {		

		setTitle( "DAW Software Development - New Old Photo Web" );
		useStyleSheet( "weboldnewphoto.css" );
		setLoadingIndicator( new Wt::WOverlayLoadingIndicator() );

		auto wc_button_fileupload = new Wt::WPushButton( "Upload" );
		wc_fileupload = new Wt::WFileUpload( );				
		wc_fileupload->setFileTextSize( 64 );
		wc_fileupload->changed().connect( wc_fileupload, &Wt::WFileUpload::upload );
		wc_fileupload->changed().connect( wc_button_fileupload, &Wt::WPushButton::disable );
		wc_fileupload->uploaded().connect( this, &WebOldNewPhoto::imageOriginalUploaded );
		wc_fileupload->fileTooLarge().connect( this, &WebOldNewPhoto::imageOriginalTooLarge );
		wc_fileupload->setMargin( 5, Wt::Right);
// 		{	// TODO: Cannot undo without ruining upload box
// 			Wt::WProgressBar* progress = new Wt::WProgressBar( );
// 			progress->setFormat( Wt::WString::Empty );
// 			progress->setVerticalAlignment( Wt::AlignMiddle );
// 			wc_fileupload->setProgressBar( progress );
// 		}

		wc_button_fileupload->clicked().connect( wc_fileupload, &Wt::WFileUpload::upload );
		wc_button_fileupload->clicked().connect( wc_button_fileupload, &Wt::WPushButton::disable );
		
		
		wc_button_repaint = new Wt::WPushButton( "Repaint" );		
		wc_button_repaint->disable( );
		wc_button_repaint->clicked().connect( this, &WebOldNewPhoto::repaintGrayscale );

		wc_combo_validrepaintmethods = new Wt::WComboBox( );

		for( auto v: get_repaint_formulas<Wt::WString>( ) ) {
			wc_combo_validrepaintmethods->addItem( v.first );
		}
		wc_combo_validrepaintmethods->setCurrentIndex( 4 );
		

		root( )->addWidget( wc_fileupload );		
		root( )->addWidget( wc_button_fileupload );		
		root( )->addWidget( wc_combo_validrepaintmethods );
		root( )->addWidget( wc_button_repaint );
		root( )->addWidget( new Wt::WBreak( ) );
		
		wc_tabs_images = new Wt::WTabWidget( root( ) );

		wc_image_original = new Wt::WImage( Wt::WLink( "blank.jpg" ), 0 );
		wc_image_original->setStyleClass( "imgsmall" );
		wc_image_original->doubleClicked().connect( boost::bind( &WebOldNewPhoto::imageClicked, boost::ref(*this), wc_image_original ) );		
		wc_tabs_images->addTab( wc_image_original, "Original Image", Wt::WTabWidget::PreLoading );
		
		//wc_image_grayscale = new Wt::WImage( );
		//wc_image_grayscale->setStyleClass( "imgsmall" );
		//wc_image_grayscale->doubleClicked().connect( boost::bind( &WebOldNewPhoto::imageClicked, boost::ref(*this), wc_image_grayscale ) );
		//wc_tabs_images->addTab( wc_image_grayscale, "DAW Grayscale Image", Wt::WTabWidget::PreLoading );

		//wc_image_recolourized = new Wt::WImage( );
		//wc_image_recolourized->setStyleClass( "imgsmall" );
		//wc_image_recolourized->doubleClicked().connect( boost::bind( &WebOldNewPhoto::imageClicked, boost::ref(*this), wc_image_recolourized ) );
		//wc_tabs_images->addTab( wc_image_recolourized, "Colourized Image", Wt::WTabWidget::PreLoading );
	}

	void WebOldNewPhoto::repaintGrayscale( ) {
		wc_button_repaint->disable( );
		auto repaint_method = get_repaint_formulas<Wt::WString>( )[wc_combo_validrepaintmethods->currentText( )];
		
		GenericImage<rgb3> image_recolourized = FilterDAWGSColourize::filter( *image_original, *image_grayscale, repaint_method );
		clearWRaster( wc_image_recolourized, wc_rasterimage_recolourized );
		setWRaster( wc_image_recolourized, wc_rasterimage_recolourized, image_recolourized, root( ) );
		wc_button_repaint->enable( );
		wc_tabs_images->setCurrentIndex( 2 );
	}

	void WebOldNewPhoto::imageOriginalUploaded( ) {		
		try {
			*image_original = GenericImage<rgb3>::fromFile( wc_fileupload->spoolFileName( ) );
		} catch( const std::exception& ex ) {
			Wt::log( "error" ) << ex.what( );
			return;
		}
		clearWRaster( wc_image_original, wc_rasterimage_original );
		setWRaster( wc_image_original, wc_rasterimage_original, *image_original, root( ) );
		
		//*image_grayscale = FilterDAWGS::filter( *image_original );
		//clearWRaster( wc_image_grayscale, wc_rasterimage_grayscale );
		//setWRaster( wc_image_grayscale, wc_rasterimage_grayscale, *image_grayscale );		
		//wc_tabs_images->setCurrentIndex( 1 );

		//wc_button_repaint->enable( );
		//clearWRaster( wc_image_recolourized, wc_rasterimage_recolourized );
	}

	void WebOldNewPhoto::imageOriginalTooLarge( ) {
		Wt::log( "info" ) << "Image uploaded is larger than maximum allowed";
	}

	void WebOldNewPhoto::imageClicked( const Wt::WImage* const image ) {
		nullcheck( image, "image passed to imageClicked is null" );

		if( image->styleClass( ) == "imgsmall" ) {
			((Wt::WWidget*)image)->setStyleClass( "imgfull" );
		} else {
			((Wt::WWidget*)image)->setStyleClass( "imgsmall" );
		}
		refresh();
	}
} }
