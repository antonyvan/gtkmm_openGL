#pragma once
#include <gtkmm.h>
#include <glad/glad.h>  # attention here, gtkmm use glfw+ glad doesn't work, use glfw + glew instead. see files in gtkmm_openGL
#include <GLFW/glfw3.h>

enum
{
  X_AXIS,
  Y_AXIS,
  Z_AXIS,
  N_AXIS
};

class mainwindow : public Gtk::Window
{
  // Glib::RefPtr<Gtk::Application> & app_;
  Glib::RefPtr<Gtk::Builder> builder_;

  Gtk::Image *mpimage0_color, *mpimage0_depth;
  Gtk::Image *mpimage1_color, *mpimage1_depth;
  Gtk::Image *mpimage2_color, *mpimage2_depth;
  Gtk::Image *mpimage_3d;
  Gtk::Button *mpbutton_continue, *mpbutton_sequence;
  Gtk::Button *mpbutton_singleshot, *mpbutton_calibration;

  Gtk::Entry *mpinfotext;

  static std::vector<std::string> imageNames_;
  static int index_;

  void buildUI();

  // opengl
  Gtk::Frame *glframe_;
  Gtk::GLArea m_GLArea;
  GLuint m_Vao{0};
  GLuint m_Buffer{0};
  GLuint m_Program{0};
  GLuint m_Mvp{0};
  std::vector<float> m_RotationAngles;

  // void on_axis_value_change(int axis, const Glib::RefPtr<Gtk::Adjustment> &adj);
  void realize() ;
  void unrealize();
  bool render(const Glib::RefPtr<Gdk::GLContext> &context);

  // Gtk::Box *create_axis_slider_box(int axis);
  void init_buffers();
  void init_shaders(const std::string &vertex_path, const std::string &fragment_path);
  void draw_triangle();

protected:
  // signal handlers
  void button_continue_clicked();
  void button_sequence_clicked();
  void button_singleshot_clicked();
  void button_calibration_clicked();
  // bool on_key_press_event(GdkEventKey *) override;

public:
  mainwindow(BaseObjectType *, Glib::RefPtr<Gtk::Builder> &);
  virtual ~mainwindow();
};
