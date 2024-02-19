Lazy Resident Appliances
========================

This project is a small PoC project and uses code for a TI Launchpad not included in this repository.

Starting Point
--------------

Data processing starts on the serial port COM4.  
The data on COM4 is read by *serialreader.c*, a hacky little C program.  
The data is processed by *serialreader.c* and output as a message in a specific format to *data.txt*.  
*data.txt* SHOULD only contain one message and new messages MUST overwrite old messages.

Build *serialreader.c* into an executable with your preferred method.  
This project was made and tested on a Windows machine and *serialreader.c* currently targets the Windows platform.  
Feel free to make small adjustments to make it run on your operating system.

Data Format
-----------

Messages always start with '!' and end with '$'.  
Individual components of the message are separated with ';'.  
The data type is declared with 3 characters followed by a ':'.  
Numbers are written out as string with fixed lengths: integers '1234' and floats '12.4567'.  

Start *serialreader.exe* to begin reading COM4 and output contents to *data.txt*.

Web View
--------

Use the following command to start the webserver
```
python server.py
```

Alternatively, this command can be used with additional custom parameters.
```
python -m Flask --app server run
```

Consider this to make the app visible from outside your own machine.  
As a bonus, this allows others to run python scripts on your machine which can be very fun!
```
python -m Flask --app server run --host 0.0.0.0
```

If you are being flooded with requests, you may want to adjust the interval for AJAX in *index.html*.