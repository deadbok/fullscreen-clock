# Introduction

This is the server part of the full screen clock. It is a flask application
controlled by a web configuration interface (TBD) and a REST interface.

The server has the following tasks.

 * Queue messages for the client.
 * Repeat messages.
 * Control repeat interval.
 * Add messages by running plug-ins.
 * Add messages using the REST interface.
 * Scaling and conversion of online graphics to the desired resolution.
 * Configure the clock using a web interface. (TBD)

# Interfaces

## API endpoints

 * `/api/line<0/1>`: (GET)
    * Get a line that is to be displayed by the client.
    * JSON:

            {
                'text': <string>,
                'icon': <string>
                'seconds': <int>
            }

        * **text**: Text to display
        * **icon**: URL to an icon to display
        * **seconds**: Second to display the message.
 * `/api/message`: (PUT)
    * Add a message to the queue of waiting messages for the client.
    * JSON:

            {
                'text': <string>,
                'icon': <string>,
                'repeat': <int>,
                'interval_sec': <int>
                'display_sec': <int>
            }

        * **text**: Text of the message to queue
        * **icon**: URL of icon to enque (empty if unused)
        * **repeat**: Number times to repeat the message (-1 means forever)
        * **interval**: Minimum number of seconds between message repeats
        * **display**: Minimum number of seconds the messaage is visible
