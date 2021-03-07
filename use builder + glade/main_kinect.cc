#include <gtkmm.h>
#include "mainwindow.hpp"
#include <iostream>

int main( int argc, char * argv[] ) {
	try {
	        auto app = Gtk::Application::create( argc, argv, "gtkmm.tutorial.example6" ); // *** leaks
		auto builder = Gtk::Builder::create_from_file( "kinect2.glade" );

		mainwindow *window_ = nullptr;

		// Builds MyWindow, derived from Gtk::Window. Automatically gets the
		// BaseObjectType and Builder reference passed in, so only need to
		// add the app parameter.
		builder->get_widget_derived( "window", window_);
		if ( window_ == nullptr ) {
		  g_warning("unable to extract window"); 
		  return -1;
		} // if

		app->run( *window_ );
		delete window_;
		
	}  catch( const Glib::FileError & ex ) {
		std::cerr << "FileError: " << ex.what() << std::endl;
		return 1;
	} catch( const Glib::MarkupError & ex ) {
		std::cerr << "MarkupError: " << ex.what() << std::endl;
		return 1;
	} catch( const Gtk::BuilderError & ex ) {
		std::cerr << "BuilderError: " << ex.what() << std::endl;
		return 1;
    } // catch
    
    return 0;
} // main
	
