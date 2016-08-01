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

#include <daw/grayscale_filter/genericimage.h>
#include <daw/grayscale_filter/genericrgb.h>
#include <daw/grayscale_filter/filterdawgs.h>
#include <daw/grayscale_filter/filterdawgscolourize.h>

#include <boost/shared_ptr.hpp>
#include <Wt/WApplication>

namespace daw { namespace imaging {
	class WebOldNewPhoto: public Wt::WApplication {
	public:
		WebOldNewPhoto( const Wt::WEnvironment &env );
	//public:
		Wt::WFileUpload* wc_fileupload;
		Wt::WImage* wc_image_original;
		Wt::WImage* wc_image_grayscale;
		Wt::WImage* wc_image_recolourized;
		Wt::WRasterImage* wc_rasterimage_original;
		Wt::WRasterImage* wc_rasterimage_grayscale;
		Wt::WRasterImage* wc_rasterimage_recolourized;
		Wt::WTabWidget*	wc_tabs_images;
		Wt::WPushButton* wc_button_repaint;
		Wt::WComboBox* wc_combo_validrepaintmethods;

		boost::shared_ptr<GenericImage<rgb3>> image_original;
		boost::shared_ptr<GenericImage<rgb3>> image_grayscale;

		//WebOldNewPhoto( const WebOldNewPhoto &src );
		void newUploadCtrl( Wt::WContainerWidget* const parent );
		void imageOriginalUploaded( );
		void imageOriginalTooLarge( );
		void imageClicked( const Wt::WImage* const image );
		void repaintGrayscale( );
		//WebOldNewPhoto &operator=( const WebOldNewPhoto &src );
	};
} }

