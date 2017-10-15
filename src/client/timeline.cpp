#include <FL/Fl_PNG_Image.H>

#include "timeline.h"

TimeLine::TimeLine(int w, int h, int font_size) : Fl_Box(0, 0, w, h / 2)
{
    this->labelcolor(fl_rgb_color(255));
    this->labelsize(font_size);
    this->label("");
    this->text = "";
}

void TimeLine::update()
{
    time_t current_time;
    tm time_buffer;
    tm *timeinfo;

    // Get current time.
    time(&current_time);
    timeinfo = localtime_r(&current_time, &time_buffer);

    if (this->last_minute != timeinfo->tm_min)
    {
        this->text = std::to_string(timeinfo->tm_hour);
        this->text += ":";
        this->text += std::to_string(timeinfo->tm_min);

        this->label(this->text.c_str());
    }

    this->last_minute = timeinfo->tm_min;
}
