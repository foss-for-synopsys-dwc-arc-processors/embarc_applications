#How to Contribute to embARC Applications#

embARC users are welcome to make contributions to embARC, including examples, source code (bug fixes and/or new packages), and documentation. This document is intended to help you contribute to embARC Applications quickly.

##Prerequisites##

The hardware we used in the development is EM Start Kit. We need to download and install the following tools: 

+ Download and install [**Development Tools**](http://embarc.org/tools.html "IDE"). 
	+ Here we use [GNU Toolchain](https://github.com/foss-for-synopsys-dwc-arc-processors/toolchain/releases "GNU Toolchain") in **windows** host.
	+ For using GNU or other tools in different host, see [Getting Started with embARC OSP](https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp/wiki/Getting-Started-with-embARC-OSP "Getting Started with embARC OSP") for reference.
	
	![GNU-make](http://i.imgur.com/wcpgoKn.png)

+ Download and install [**Digilent Adapt Software**](http://store.digilentinc.com/digilent-adept-2-download-only/ "Digilent Adapt Software"), driver for Digilent JTAG-USB cable.

	![Digilent Adapt](http://i.imgur.com/aSPUBuW.png)

+ Download and install [**zadig**](http://zadig.akeo.ie/ "zadig"), replacing a driver for **windows** host.
	+ Before you can start using OpenOCD, you have to download WinUSB driver and replace with it one of FTDI drivers for your hardware development system.
	+ See [How to Use OpenOCD on Windows](https://github.com/foss-for-synopsys-dwc-arc-processors/arc_gnu_eclipse/wiki/How-to-Use-OpenOCD-on-Windows "How to Use OpenOCD on Windows") for reference.
	+ For more information, see [toolchain Home](https://github.com/foss-for-synopsys-dwc-arc-processors/toolchain/wiki "Home").
	
	![Zadig](http://i.imgur.com/q6Q6SFX.png)

	Notices: To do that, you have to connect EMSK to PC using JTAG-USB cable. Then click on the "Options" - "List all devices", and you can find **Digilent USB Device(Interface 0/1)**. Just replacing one to WinUSB driver, it helps PC to load executable file into EMSK, and you can choose Interface 0/1.

	Make sure the driver has been install successfully, see **Diligent USB Device** in the Device Manager.

	![Device Manager](http://i.imgur.com/Amm5Lej.png)

+ Download [**embARC OSP**](https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp "embARC Software") and [**embARC Applications**](https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_applications "embARC Applications").
	+ It's better to get a [Github Account](https://github.com/ "Github account"), and fork the *embarc\_applications* to your own repository. Then develop the project based on that in your host.
	+ Placing the **embarc\_applications** under the folder **./embarc\_osp**, Rename it to applications.

+ Download and install [**Doxygen**](http://www.stack.nl/~dimitri/doxygen/download.html "Doxygen") for building embARC document.

	Goto the path: ./embarc\_osp/doc, run the **Command Line** and use the following command to make embARC document.

	![Document Folder](http://i.imgur.com/Ix9Q6wF.png) 

	![cmd-doxygen](http://i.imgur.com/gwQfcte.png)

	A few minutes later, you can open the *embARC\_Document.html* using browser.

	![embARC Document](http://i.imgur.com/wRUWgNd.png)

+ Download and install [**Tera Term**](https://zh.osdn.net/projects/ttssh2/ "Tera Term")..

##Getting Start with existing Application##

We provided some examples and applications in embARC, user can take reference from them.

+ Access to the examples: [source code of embARC examples](https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp/tree/master/example "examples")
+ Access to the applications: [source code of embARC applications](https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_applications "applications")

Here we use EMSK 2.2, EM11D for example. Goto the path: .\embarc_osp\example\baremetal\bootloader, run the Command Line and type commands for building and running.

![cmd-make](http://i.imgur.com/Kuv2dlz.png)

Run the Tera Term:

![Tera Term](http://i.imgur.com/eSYr6HN.png)

For more detail information, see [Getting Started with embARC Using GNU on Windows](https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp/wiki/Getting-Started-with-embARC-Using-GNU-on-Windows#building-and-running-your-first-example "Getting Started with embARC Using GNU on Windows") for reference. And see [Getting Started with embARC OSP](https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp/wiki/Getting-Started-with-embARC-OSP "Getting Started with embARC OSP") to learn how to get start with embARC using different toolsets or hosts.

##Creating a New Application in embARC##

See the document **Example Usage Guide** - *./embarc\_osp/doc/embARC\_Document/html/page\_example\_usage.html* in the *embarc\_osp* for more detail information. It will help you to create a **"Hello World"** application using ARC GNU quickly.

##Developing Your Application Using Git##

We recommand you to develop the application following these steps:

1) Download and install [Git](https://git-for-windows.github.io/ "Git")，and learning how to develop projects using git. See [Git Book](https://git-scm.com/book/en/v2 "Git Book") for reference.

2) You have *embarc\_osp*, then goto the folder *./embarc\_osp* and create a new folder named "applications". Running the **Git Bash** and type:

		$ git init
		$ git remote add origin [url of your embarc\_applications repository]
		$ git pull origin master

   Then, you can see there are serval applications existing in this folder, such as *aws_iot_smarthome*, *aws_iot_smarthome_multinode*, etc.

3) Create a new folder for your own application, named **helloworld**.

4) Copy **example.makefile** and **main.c.tmpl** in *./embarc\_osp/example* folder to the newly created helloworld folder.

+ Rename example.makefile to **makefile**.
+ Rename main.c.tmpl to **main.c.**
+ Change the configurations in makefile according to your hardware configuration.
+ Modify your main.c.
+ Compile this application on the command line.
+ Run or debug this application.

   See the document **Example Usage Guide** in the *embarc\_osp* for more information.

##Committing Your Work##

You can contribute your work by submitting **Pull Request** to *embarc\_applications* in github after finishing your project development. Creating a new **Pull Request**:

![new pull request](http://i.imgur.com/YrpOmXn.png)

Choose to compare across forks:

![compare across forks](http://i.imgur.com/Zl97kJV.png)

Choose compare changes:

![compare forks](http://i.imgur.com/5AgRJAL.png)

Create the new pull request:

![create pull request](http://i.imgur.com/4zpWJ7V.png)

##Copyright Notice##

Copyright (c) 2017, Synopsys, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3) Neither the name of the Synopsys, Inc., nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. NY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**For complete embARC license information, please refer to the embARC FOSS Notice** [License](file:///C:/WorkSpace/Files/embarc_osp_liteos/doc/embARC_Document/html/page_license.html "License"). 