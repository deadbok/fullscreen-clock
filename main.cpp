#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Display.H>

int main()
{
   Fl_Window *win = new Fl_Window(Fl::h(), Fl::w(), "Fullscreen clock");
   
   win->fullscreen();
   
   Fl_Text_Display *time_disp = new Fl_Text_Display(0, 0, Fl::w(), Fl::h() / 3);
   Fl_Text_Buffer *time_buff = new Fl_Text_Buffer();
   time_disp->buffer(time_buff);
   time_buff->text("00:00");
   
   win->show();
   return(Fl::run());
}

