AtCore - KDE Print Service for 3DPrinters [![Build Status](https://travis-ci.org/TeamWho/AtCore.svg?branch=master)](https://travis-ci.org/TeamWho/AtCore)
----

AtCore is a API to manage the serial connection between the computer and 3D Printers.

> **Note:**

> - It's currently in developing!
> - Join us on [IRC] #kde-atelier channel.

This project is under LGPL V3, for more information read LICENSE.

###Testing AtCore###

Testing is simple and will help us a lot. 

- 	<i class="icon-terminal" aria-hidden="true"></i>**Clone**.
		
	`$ git clone https://github.com/KDE/atcore.git`

- 	<i class="icon-terminal" aria-hidden="true"></i>**Build**.

	`$ cd atcore
	 $ mkdir build
	 $ cd build
	 $ cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TEST_GUI=ON ..
	 $ make
	 `
- <i class="icon-wrench" aria-hidden="true"></i> **Connect your printer**.

- 	<i class="icon-terminal" aria-hidden="true"></i>**Open**.

	`$ ./testclient/AtCoreTest`

- 	<i class="icon-lightbulb" aria-hidden="true"></i>**Test**.

	Try the buttons on GUI to test the communication
	
-	<i class="icon-print" aria-hidden="true"></i> **Print**!

	Print File <i class="icon-right-dir" aria-hidden="true"></i> Select a *.gcode <i class="icon-right-dir" aria-hidden="true"></i> Ok
> **Note:**
> To test printing attempt to Print test/sample.gcode, it may not process more then one command. 

Copy your log before disconnecting from the printer and send it to the team !
> **Note:**
> For your test results to be helpful they must include the full log output. 

[IRC]: https://webchat.freenode.net/