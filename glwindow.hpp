#pragma once
#include <GL/glew.h>
// #include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <gtkmm.h>

class glviewwindow : public Gtk::Window
{
public:
    glviewwindow();

    Gtk::Box m_VBox{Gtk::Orientation::ORIENTATION_VERTICAL, false};
    Gtk::Box m_Controls{Gtk::Orientation::ORIENTATION_VERTICAL, false};

    Gtk::GLArea m_GLArea;

    void buildUI();

    // opengl
    GLuint m_Vao{0};
    GLuint m_Buffer{0};
    GLuint m_Program{0};
    GLuint m_Mvp{0};
    std::vector<float> m_RotationAngles;

    void on_axis_value_change(int axis, const Glib::RefPtr<Gtk::Adjustment> &adj);
    void realize();
    void unrealize();
    bool render(const Glib::RefPtr<Gdk::GLContext> &context);
    
    Gtk::Box *create_axis_slider_box(int axis);
    void init_buffers();
    void init_shaders(const std::string &vertex_path, const std::string &fragment_path);
    void draw_triangle();
};