#ifndef TIMELINE_H_
#define TIMELINE_H_

#include <time.h>

#include <string>

#include <FL/Fl_Box.H>

#include "json.hpp"

class TimeLine : public Fl_Box
{
  private:
    time_t last_minute;
    std::string text;

  public:
    TimeLine(int w, int h, int font_size);
    void update();
};

#endif
