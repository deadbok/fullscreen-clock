
## API endpoints

 * `/api/line<0/1>`: (GET)
    * Get a line that is to be displayed by the client.
    * JSON:
            
            {
                'text': <string>,
                'icon': <string>
            }

        * **text**: Text to display
        * **icon**: URL to an icon to display
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


## Install server on FreeBSD 11.1

As root:

    pkg install py27-Flask py27-flask-restplus py27-pip
    
as the user runnng the server:

    pip install --user requests
    

    
    