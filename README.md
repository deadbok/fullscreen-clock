# fullscreen-clock

Fullscreen clock using the FLTK toolkit.

 * Clock (use NTP on the system).
 * Open Weather Map integration.

# Dependencies

## Client

 * X11
 * FLTK
 * json-c
 * nlohmann-json (JSON for Modern C++: https://github.com/nlohmann/json)
 * libcurl

## Server

 * Flask
 * Flask-restplus
 * Request

# Design

## Front end (client)

The front end is a C++ application using the FLTK ui toolkit and curl. The UI
shows time on the top half of the window, and two lines of custom messages on
the lower half.

    |-----------------|
    |                 |
    |      22:00      |
    |                 |
    |-----------------|
    | Message line 1  |
    |-----------------|
    | Message line 2  |
    |-----------------|

The time field uses the system time. The messages fields retrieved from the
server using a RESTful interface.

## Server

The server is a Python application using the flask framework. The server is
extendable through plugins, and can recieve message and icon URLs using a
RESTful interface.

# Lightweight FreeBSD installation

Install git for cloning this repository:

    $ pkg install git

Optionally install nano if you do not know how to quit vi:

    $ pkg install nano

Install build and runtime dependencies

    $ pkg install xorg-minimal fltk json-c pkgconf urwfonts xset

## Building the clock

Get the source from github:

    git clone https://github.com/deadbok/fullscreen-clock

Go there:

    cd fullscreen-clock

Build it:

    make

The output should look something like this:

    $ make
    make -C src -f BSDmakefile all
    c++ -I/usr/local/include -I/usr/local/include/freetype2 -O2 -pipe -fstack-protector -isystem /usr/local/include -fno-strict-aliasing -isystem /usr/local/include -fvisibility-inlines-hidden -I/usr/local/include -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_THREAD_SAFE -D_REENTRANT -I/usr/local/include -I/usr/local/include/json-c  -Wall -I. -I..  -std=c++11 -c app.cpp -o  app.o
    c++ -I/usr/local/include -I/usr/local/include/freetype2 -O2 -pipe -fstack-protector -isystem /usr/local/include -fno-strict-aliasing -isystem /usr/local/include -fvisibility-inlines-hidden -I/usr/local/include -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_THREAD_SAFE -D_REENTRANT -I/usr/local/include -I/usr/local/include/json-c  -Wall -I. -I..  -std=c++11 -c config.cpp -o  config.o
    c++ -I/usr/local/include -I/usr/local/include/freetype2 -O2 -pipe -fstack-protector -isystem /usr/local/include -fno-strict-aliasing -isystem /usr/local/include -fvisibility-inlines-hidden -I/usr/local/include -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_THREAD_SAFE -D_REENTRANT -I/usr/local/include -I/usr/local/include/json-c  -Wall -I. -I..  -std=c++11 -c main.cpp -o  main.o
    c++ -I/usr/local/include -I/usr/local/include/freetype2 -O2 -pipe -fstack-protector -isystem /usr/local/include -fno-strict-aliasing -isystem /usr/local/include -fvisibility-inlines-hidden -I/usr/local/include -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_THREAD_SAFE -D_REENTRANT -I/usr/local/include -I/usr/local/include/json-c  -Wall -I. -I..  -std=c++11 -c owm.cpp -o  owm.o
    c++ app.o config.o main.o owm.o -L/usr/local/lib -lm -Wl,-rpath,/usr/local/lib -fstack-protector -lfltk_images -lpng -lz -ljpeg -lfltk -lXcursor -lXfixes -lXext -lXft -lfontconfig -lXinerama -lpthread -lm -lX11 -L/usr/local/lib -lcurl -L/usr/local/lib -ljson-c  -o ../clock
    echo "Compiling for FreeBSD done."
    Compiling for FreeBSD done.

Create the `.clockrc` configuration file in your home directory. An easy way is
copying the included example and editing it afterwards.

    $ cp clockrc.example ~/.clockrc
    $ nano ~/.clockrc

## EeePC 701 specifics

Install the xorg driver fot the Intel 915GM graphics:

    pkg install xf86-video-intel

## Kiosk mode

To have the system boot into X and start the clock automatically, X is told to
start the clock on start. The default shell is told to start X on login. Lastly
auto login is enabled for the desired user.

### X

Edit the `sh` startup script:

    nano ~/.shrc

and add the following at the end:

    XPID=`/usr/bin/pgrep xinit`
    if [ -z "$XPID" ]
    then
      startx
      logout
    fi

This will start X, on login, if not running. To make the default X session
start the clock create a file `~/.xinitrc` with this content:

    #!/bin/sh

    xset dpms 0 0 0 &
    #xset -dpms &
    exec ./fullscreen-clock/clock

Where `./fullscreen-clock/clock` is the path to the clock executable.
The xset lines disables screensaver blanking and power saving in X, but all is
not well yet edit `/etc/rc.conf`:

    nano /etc/rc.conf

and add the following lines:

    saver="NO"
    blanktime="NO"

### Auto login

Edit `/etc/gettytab`:

    nano /etc/gettytab

and add the following lines:

    # Autologin console as user "user"
    A|Al|Autologin console:\
        :ht:np:sp#115200:al=user

Replace `user` in the last line with the desired user.

Edit `/etc/ttys`:

    nano /etc/ttys

Replace the line:

    ttyv0   "/usr/libexec/getty Pc"         xterm   on  secure

With the line:

    ttyv0   "/usr/libexec/getty Al"         xterm   on  secure

### Boot splash

*This has not worked, and is left as a puzzle to the reader.*

/etc/rc.conf
    beastie_disable="YES"



/boot/loader.conf
vesa_load="YES"
splash_bmp_load="YES"
bitmap_load="YES"
bitmap_name="/boot/splash.bmp"

boot_mute="YES"

"-mnq" to /boot.config.
