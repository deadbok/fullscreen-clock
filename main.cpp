#include <time.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.h>
#include <FL/Fl_Box.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>

#define PORT 8888

char time_str[6];

void update_time(void *ui_element)
{
        time_t current_time;
        tm time_buffer;
        tm *timeinfo;

        time(&current_time);
        timeinfo = localtime_r(&current_time, &time_buffer);

        Fl_Box* box = static_cast<Fl_Box*>(ui_element);

        snprintf(time_str, 5, "%d:%d", timeinfo->tm_hour, timeinfo->tm_min);
        time_str[5] = '\0';

        printf("%d:%d\n", timeinfo->tm_hour, timeinfo->tm_min);
        printf("%s\n", time_str);

        box->label(time_str);


        Fl::repeat_timeout(10.0, update_time, ui_element);
}

int main()
{
        Fl_Window *win = new Fl_Window(Fl::w(), Fl::h(), "Fullscreen clock");
        win->color(fl_rgb_color(0));
        win->fullscreen();

        Fl_Box *time_disp = new Fl_Box(0, 0, Fl::w(), Fl::h() / 1.5);
        time_disp->color(fl_rgb_color(150));
        time_disp->labelcolor(fl_rgb_color(255));
        time_disp->labelsize(int(Fl::h() / 2));
        time_disp->label("00:00");

        Fl::add_timeout(0.1, update_time, (void*)time_disp);

        win->show();
        return(Fl::run());
}
