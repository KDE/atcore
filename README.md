AtCore - KDE Print Service for 3DPrinters [![Build Status](https://travis-ci.org/TeamWho/AtCore.svg?branch=master)](https://travis-ci.org/TeamWho/AtCore)
----------------------------------------------------

AtCore is a API to manage the serial connection between the computer and the 3DPrinter.

>> It's currently in developing! <<

This project is under LGPL V3, for more information read LICENSE

Join us on irc we can be found in #kde-atelier on freenode.

###Testing AtCore###

Testing is simple and will help us alot. Build the Atcore program run it and then connect to your printer. 

Send the command M115 while its running this will report the firmware your printer is using. 

Try the buttons on GUI report on their working or not.

To Test Printing attempt to Print test/sample.gcode, it may not process more then one command. 

Copy your log before disconnecting from the printer and send it to the team.

For your test results to be helpful they must include the full log output and the results of the M115 Command. 
