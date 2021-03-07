#include "mainwindow.hpp"
#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

static const GLfloat vertex_data[] = {
  0.f,   0.5f,   0.f, 1.f,
  0.5f, -0.366f, 0.f, 1.f,
 -0.5f, -0.366f, 0.f, 1.f,
};


int mainwindow::index_ = 0;
std::vector<std::string> mainwindow::imageNames_ = {
    "img/donut.png",
    "img/fractal_image.png",
    "img/cat-face-close-view-115011.jpeg"};

mainwindow::mainwindow(BaseObjectType *cobject,
                       Glib::RefPtr<Gtk::Builder> &builder) : Gtk::Window{cobject}, builder_{builder},
                                                              mpimage0_color{nullptr}, mpimage0_depth{nullptr},
                                                              mpimage1_color{nullptr}, mpimage1_depth{nullptr},
                                                              mpimage2_color{nullptr}, mpimage2_depth{nullptr},
                                                              mpbutton_continue{nullptr}, mpbutton_sequence{nullptr},
                                                              mpbutton_singleshot{nullptr}, mpbutton_calibration{nullptr},
                                                              mpinfotext{nullptr}
{
    set_title("Example 6");
    set_default_size(300, 400); // Set default size, width and height, in pixels.
    set_border_width(10);

    // m_GLArea.set_expand(true);
   

    buildUI();
    add_events(Gdk::KEY_PRESS_MASK);
    show_all_children();
}

mainwindow::~mainwindow()
{
}

void mainwindow::buildUI()
{
    builder_->get_widget("button_continue", mpbutton_continue);
    builder_->get_widget("button_sequence", mpbutton_sequence);
    builder_->get_widget("button_singleshot", mpbutton_singleshot);
    builder_->get_widget("button_calibration", mpbutton_calibration);

    builder_->get_widget("image0_color", mpimage0_color);
    builder_->get_widget("image0_depth", mpimage0_depth);
    builder_->get_widget("image1_color", mpimage1_color);
    builder_->get_widget("image1_depth", mpimage1_depth);
    builder_->get_widget("image2_color", mpimage2_color);
    builder_->get_widget("image2_depth", mpimage2_depth);
    // builder_->get_widget("image_3d", mpimage_3d);
    builder_->get_widget("infotext", mpinfotext);
    builder_->get_widget("glframe1", glframe_ );

    if (mpbutton_continue == nullptr || mpbutton_sequence == nullptr || mpbutton_singleshot == nullptr || mpbutton_calibration == nullptr || mpinfotext == nullptr)
    {
        g_warning("unable to extract window sub-components");
        return;
    } // if

    // mpimage_3d->set(imageNames_.at(1));
    // mpimage0_depth->set(imageNames_.at(0));
    m_GLArea.set_auto_render();
    m_GLArea.set_hexpand();
    m_GLArea.set_vexpand();
    m_GLArea.set_halign(Gtk::ALIGN_FILL);
    m_GLArea.set_valign(Gtk::ALIGN_FILL);
    m_GLArea.set_size_request(200, 200);
    m_GLArea.set_auto_render(true);
    std::cout << "set gl area"<< std::endl;
    add(m_GLArea);
    // Connect gl area signals
    m_GLArea.signal_realize().connect(sigc::mem_fun(*this, &mainwindow::realize));
    // Important that the unrealize signal calls our handler to clean up
    // GL resources _before_ the default unrealize handler is called (the "false")
    m_GLArea.signal_unrealize().connect(sigc::mem_fun(*this, &mainwindow::unrealize), false);
    m_GLArea.signal_render().connect(sigc::mem_fun(*this, &mainwindow::render), false);
    m_GLArea.show();


    mpbutton_continue->signal_clicked().connect(sigc::mem_fun(*this, &mainwindow::button_continue_clicked));
    mpbutton_sequence->signal_clicked().connect(sigc::mem_fun(*this, &mainwindow::button_sequence_clicked));
    mpbutton_singleshot->signal_clicked().connect(sigc::mem_fun(*this, &mainwindow::button_singleshot_clicked));
    mpbutton_calibration->signal_clicked().connect(sigc::mem_fun(*this, &mainwindow::button_calibration_clicked));
} // mainwindow::buildUI

void mainwindow::button_continue_clicked()
{
    std::cout << "continue run" << std::endl;
    mpinfotext->set_text("continue");
}

void mainwindow::button_sequence_clicked()
{
    std::string s = mpinfotext->get_text();
    std::cout << "sequence run " << s << std::endl;
    mpinfotext->set_text("sequence");
}

void mainwindow::button_singleshot_clicked()
{
    std::string s = mpinfotext->get_text();
    std::cout << "single shot " << s << std::endl;
    mpinfotext->set_text("singleshot");
}
void mainwindow::button_calibration_clicked()
{
    std::string s = mpinfotext->get_text();
    std::cout << "calibration " << s << std::endl;
    mpinfotext->set_text("calibration");
}

// openGL 
void mainwindow::init_buffers()
{
  glGenVertexArrays(1, &m_Vao);
  glBindVertexArray(m_Vao);

  glGenBuffers(1, &m_Buffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static GLuint create_shader(int type, const char *src)
{
  auto shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    int log_len;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);

    std::string log_space(log_len+1, ' ');
    glGetShaderInfoLog(shader, log_len, nullptr, (GLchar*)log_space.c_str());

    std::cerr << "Compile failure in " <<
      (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<
      " shader: " << log_space << std::endl;

    glDeleteShader(shader);

    return 0;
  }

  return shader;
}
void mainwindow::init_shaders(const std::string& vertex_path, const std::string& fragment_path)
{
  auto vshader_bytes = Gio::Resource::lookup_data_global(vertex_path);
  if(!vshader_bytes)
  {
    std::cerr << "Failed fetching vertex shader resource" << std::endl;
    m_Program = 0;
    return;
  }
  gsize vshader_size {vshader_bytes->get_size()};
  auto vertex = create_shader(GL_VERTEX_SHADER,
                         (const char*)vshader_bytes->get_data(vshader_size));

  if(vertex == 0)
  {
    m_Program = 0;
    return;
  }

  auto fshader_bytes = Gio::Resource::lookup_data_global(fragment_path);
  if(!fshader_bytes)
  {
    std::cerr << "Failed fetching fragment shader resource" << std::endl;
    glDeleteShader(vertex);
    m_Program = 0;
    return;
  }
  gsize fshader_size {fshader_bytes->get_size()};
  auto fragment = create_shader(GL_FRAGMENT_SHADER,
                           (const char*)fshader_bytes->get_data(fshader_size));

  if(fragment == 0)
  {
    glDeleteShader(vertex);
    m_Program = 0;
    return;
  }

  m_Program = glCreateProgram();
  glAttachShader(m_Program, vertex);
  glAttachShader(m_Program, fragment);

  glLinkProgram(m_Program);

  int status;
  glGetProgramiv(m_Program, GL_LINK_STATUS, &status);
  if(status == GL_FALSE)
  {
    int log_len;
    glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &log_len);

    string log_space(log_len+1, ' ');
    glGetProgramInfoLog(m_Program, log_len, nullptr, (GLchar*)log_space.c_str());

    std::cerr << "Linking failure: " << log_space << std::endl;

    glDeleteProgram(m_Program);
    m_Program = 0;
  }
  else
  {
    /* Get the location of the "mvp" uniform */
    // m_Mvp = glGetUniformLocation(m_Program, "mvp");

    // glDetachShader(m_Program, vertex);
    // glDetachShader(m_Program, fragment);
  }
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

static void compute_mvp(float *res,
                        float phi,
                        float theta,
                        float psi)
{
  float x       {phi * ((float)G_PI / 180.f)};
  float y       {theta * ((float)G_PI / 180.f)};
  float z       {psi * ((float)G_PI / 180.f)};
  float c1      {cosf (x)};
  float s1      {sinf (x)};
  float c2      {cosf (y)};
  float s2      {sinf (y)};
  float c3      {cosf (z)};
  float s3      {sinf (z)};
  float c3c2    {c3 * c2};
  float s3c1    {s3 * c1};
  float c3s2s1  {c3 * s2 * s1};
  float s3s1    {s3 * s1};
  float c3s2c1  {c3 * s2 * c1};
  float s3c2    {s3 * c2};
  float c3c1    {c3 * c1};
  float s3s2s1  {s3 * s2 * s1};
  float c3s1    {c3 * s1};
  float s3s2c1  {s3 * s2 * c1};
  float c2s1    {c2 * s1};
  float c2c1    {c2 * c1};

  /* apply all three rotations using the three matrices:
   *
   * ⎡  c3 s3 0 ⎤ ⎡ c2  0 -s2 ⎤ ⎡ 1   0  0 ⎤
   * ⎢ -s3 c3 0 ⎥ ⎢  0  1   0 ⎥ ⎢ 0  c1 s1 ⎥
   * ⎣   0  0 1 ⎦ ⎣ s2  0  c2 ⎦ ⎣ 0 -s1 c1 ⎦
   */
  res[0] = c3c2;  res[4] = s3c1 + c3s2s1;  res[8] = s3s1 - c3s2c1; res[12] = 0.f;
  res[1] = -s3c2; res[5] = c3c1 - s3s2s1;  res[9] = c3s1 + s3s2c1; res[13] = 0.f;
  res[2] = s2;    res[6] = -c2s1;         res[10] = c2c1;          res[14] = 0.f;
  res[3] = 0.f;   res[7] = 0.f;           res[11] = 0.f;           res[15] = 1.f;
}


void mainwindow::realize()
{
  m_GLArea.make_current();
  try
  {
    m_GLArea.throw_if_error();
    if (!glfwInit())
        return;

    // create buffer, glbindbuffer, glbufferdata 
    init_buffers();

    // const bool use_es = m_GLArea.get_context()->get_use_es();
    const bool use_es = true;
    const std::string vertex_path = use_es ? "/glarea/glarea-gles.vs.glsl" : "/glarea/glarea-gl.vs.glsl";
    const std::string fragment_path = use_es ? "/glarea/glarea-gles.fs.glsl" : "/glarea/glarea-gl.fs.glsl";
    init_shaders(vertex_path, fragment_path);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

  }
  catch(const Gdk::GLError& gle)
  {
    std::cerr << "An error occured making the context current during realize:" << std::endl;
    std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
  }

}

void mainwindow::unrealize()
{
//   m_GLArea.make_current();
//   try
//   {
//     m_GLArea.throw_if_error();

//     // Delete buffers and program
//     glDeleteBuffers(1, &m_Buffer);
//     glDeleteProgram(m_Program);
//   }
//   catch(const Gdk::GLError& gle)
//   {
//     std::cerr << "An error occured making the context current during unrealize" << std::endl;
//     std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
//   }
}

bool mainwindow::render(const Glib::RefPtr<Gdk::GLContext>& /* context */)
{
  try
  {
    m_GLArea.throw_if_error();
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_triangle();

    glFlush();

    return true;
  }
  catch(const Gdk::GLError& gle)
  {
    std::cerr << "An error occurred in the render callback of the GLArea" << std::endl;
    std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
    return false;
  }
}


void mainwindow::draw_triangle()
{
  float mvp[16];

  compute_mvp(mvp,
              m_RotationAngles[X_AXIS],
              m_RotationAngles[Y_AXIS],
              m_RotationAngles[Z_AXIS]);

  glUseProgram(m_Program);

  glUniformMatrix4fv(m_Mvp, 1, GL_FALSE, &mvp[0]);

  glBindBuffer(GL_ARRAY_BUFFER, m_Buffer);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(0);
}

// bool mainwindow::on_key_press_event(GdkEventKey *keyEvent)
// {
//     static int numTimesReturnPressed = 0;
//     if (keyEvent->keyval == GDK_KEY_Return)
//     {
//         numTimesReturnPressed++;
//         string s = valueField_->get_text();
//         cout << "User entered: " << s << endl;
//         label_->set_text(s);
//         valueField_->set_text(std::to_string(numTimesReturnPressed));
//         return true;
//     } // if

//     // return true to indicate handled; otherwise call the base class
//     return Gtk::Window::on_key_press_event(keyEvent);
// } // mainwindow::on_key_press_event
