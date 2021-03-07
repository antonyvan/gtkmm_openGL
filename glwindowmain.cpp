#include "glwindow.hpp"
#include "gtkmm.h"

int main(int argc, char* argv[])
{
  auto app = Gtk::Application::create(argc, argv,"org.gtkmm.examples.base");
  glviewwindow window;
  return app->run(window);
}