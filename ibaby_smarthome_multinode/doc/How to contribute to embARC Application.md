# How to Contribute to embARC Applications

embARC users are welcome to make contributions to embARC, including examples, source code (bug fixes and/or new packages), and documentation. This document is intended to help you contribute to embARC Applications quickly.

* [Prerequisites](#prerequisites)
* [Getting Start with Existing Application](#get_start)
* [Creating a New Application in embARC](#new_app)
* [Developing Your Application Using Git](#development)
* [Committing Your Work](#commit)
* [Copyright Notice](#copyright)

## <a name="prerequisites"/> Prerequisites

The hardware we used during development is **EM Starter Kit(aka EMSK)**. We need to download and install the following tools:
	+ Download and install [**Development Tools**][30]. 
	+ Here we use [GNU Toolchain][31] in **windows** host.
	+ For using GNU or other tools in different host, see [Getting Started with embARC OSP][32] for reference.
	
	![gnu_make][0]

+ Download and install [**Digilent Adapt Software**][33], driver for Digilent JTAG-USB cable.

	![digilent_adapt][1]

+ Download and install [**Zadig**][34], replacing a driver for **Windows** host.
	+ Before you can start using OpenOCD, you have to download WinUSB driver and replace with it one of FTDI drivers for your hardware development system.
	+ See [How to Use OpenOCD on Windows][35] for reference.
	+ For more information, see [ARC GNU Toolchain Wiki Home][36].
	
	![zadig][2]

	Notices: To do that, you have to connect EMSK to PC using JTAG-USB cable. Then click on the *Options* - *List all devices*, and you can find **Digilent USB Device(Interface 0/1)**. Just replacing **Digilent USB Device(Interface 0)** to WinUSB driver, it helps PC to load executable file into EMSK.

	You can see **Digilent USB Device** in the Device Manager if the driver has been installed successfully.

	![device_manager][3]

+ Get [**embARC OSP**][37] and [**embARC Applications**][38] source code.
	+ Get a [Github Account][39]. Download and install [Git][40]，and you have to learn how to develop projects using git. See [Git Book][41] for reference.
	+ Download the [**embARC OSP**][37] and rename it to *embarc\_osp*.
	+ Fork the *embarc\_applications* to your own repo. You will develop the project based on that in your host.
	
	![fork_app_repo][4]

	+ Goto the folder `./embarc_osp`. Run the **Git Bash** and type the following commands to clone **embARC Applications** source code to `./embarc_osp/application` folder:
		
		    cd embarc_osp
			git clone https://github.com/foss-for-synopsys-dwc-arc-processors
			embarc_applications.git application

	Then, you can see there are serval applications existing in this folder, such as *aws\_iot\_smarthome*, *aws\_iot\_smarthome\_multinode*, etc.

	+ See [embARC Applications README][42] for reference.

+ Download and install [**Doxygen**][43] for building embARC document.

	Goto the path: `./embarc_osp/doc`, run the **Command Line** and use the following commands to make embARC document.

	![document_folder][5]

	![doxygen_make][6]

	A few minutes later, you can open the *embARC\_Document.html* using browser.

	![embarc_document][7]

+ Download and install [**Tera Term**][44].

## <a name="get_start"/> Getting Start with Existing Application

We provided some examples and applications in embARC, user can take reference from them.

+ Access to the examples: [source code of embARC examples][45]
+ Access to the applications: [source code of embARC applications][38]

Here we use EMSK 2.2, EM11D for example. Goto the path: `.\embarc_osp\example\baremetal\bootloader`, run the Command Line and type commands for building and running.

![cmd_make][8]

Run the Tera Term:

![tera_term][9]

For more detail information, see [Getting Started with embARC Using GNU on Windows][46] for reference. And see [Getting Started with embARC OSP][47] to learn how to get start with embARC using different toolsets or hosts.

## <a name="new_app"/> Creating a New Application in embARC

See the document **Example Usage Guide** - *./embarc\_osp/doc/embARC\_Document/html/page\_example\_usage.html* in the *embarc\_osp* for more detail information. It will help you to create a **Hello World** application using ARC GNU quickly.

![example_usage_guide][10]

## <a name="development"/> Developing Your Application Using Git

1) Make sure you have installed [Git][40]，and know how to use it.

2) Goto the folder `./embarc_osp/application`, Create a new folder for your own application, like **helloworld**.

3) Copy **example.makefile** and **main.c.tmpl** in `./embarc_osp/example` folder to the newly created helloworld folder.

+ Rename example.makefile to **makefile**.
+ Rename main.c.tmpl to **main.c.**
+ Change the configurations in makefile according to your hardware configuration.
+ Modify your main.c.
+ Compile this application on the command line.
+ Run or debug this application.

   See the document **Example Usage Guide** in the *embarc\_osp* for more information.

Notices: 

+ All C source code must adhere to [**Linux Kernel Coding Style**][48]. The source-code files in embARC are documented with C-style code comments and Doxygen-based comments. A Doxygen comment example is shown below:

		/**
		 * \defgroup    EMBARC_APP_TMPL         embARC Template Example
		 * \ingroup     EMBARC_APPS_TOTAL
		 * \ingroup     EMBARC_APPS_BOARD_EMSK
		 * \ingroup     EMBARC_APPS_BAREMETAL
		 * \brief       embARC Example for template
		  
		 * \details
		 * ### Extra Required Tools
		  
		 * ### Extra Required Peripherals
		  
		 * ### Design Concept
		  
		 * ### Usage Manual
		  
		 * ### Extra Comments
		  
		 */
		/**
		 * \file
		 * \ingroup     EMBARC_APP_TMPL
		 * \brief       main source of template example
		 */
		/**
		 * \addtogroup  EMBARC_APP_TMPL
		 * @{
		 */
		/* embARC HAL */
		#include "embARC.h"
		#include "embARC_debug.h"
		/** main entry */
		int main(void)
		{
		        EMBARC_PRINTF("Hello embARC from template example\r\n");
		        return E_SYS;
		}
		/** @} */

+ See [Contributing to embARC OSP][49] for more information about Coding-Style, Document-Style and Git Commit Message Guide, etc.

## <a name="commit"/> Committing Your Work

You can contribute your work by submitting **Pull Request** to *embarc\_applications* in github after finishing your project development. Creating a new **Pull Request**:

![new_pull_request][11]

Choose to compare across forks:

![compare_across_forks][12]

Choose compare changes:

![compare_changes][13]

Create the new pull request:

![create_pull_request][14]

## <a name="copyright"/> Copyright Notice

Copyright (c) 2017, Synopsys, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3) Neither the name of the Synopsys, Inc., nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. NY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**For complete embARC license information, please refer to the embARC FOSS Notice** [License][50]. 

[0]: ../img/gnu_make.png               "gnu_make"
[1]: ../img/digilent_adapt.png         "digilent_adapt"
[2]: ../img/zadig.png                  "zadig"
[3]: ../img/device_manager.png         "device_manager"
[4]: ../img/fork_app_repo.png          "fork_app_repo"
[5]: ../img/document_folder.png        "document_folder"
[6]: ../img/doxygen_make.png           "doxygen_make"
[7]: ../img/embarc_document.png        "embarc_document"
[8]: ../img/cmd_make.png               "cmd_make"
[9]: ../img/tera_term.png              "tera_term"
[10]: ../img/example_usage_guide.png   "example_usage_guide"
[11]: ../img/new_pull_request.png      "new_pull_request"
[12]: ../img/compare_across_forks.png  "compare_across_forks"
[13]: ../img/compare_changes.png       "compare_changes"
[14]: ../img/create_pull_request.png   "create_pull_request"

[30]: http://embarc.org/tools.html
	"Development Tools"
[31]: https://github.com/foss-for-synopsys-dwc-arc-processors/toolchain/releases 
	"GNU Toolchain"
[32]: https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp/wiki/Getting-Started-with-embARC-OSP 
	"Getting Started with embARC OSP"
[33]: http://store.digilentinc.com/digilent-adept-2-download-only/ 
	"Digilent Adapt Software"     
[34]: http://zadig.akeo.ie/ 
	"zadig"
[35]: https://github.com/foss-for-synopsys-dwc-arc-processors/arc_gnu_eclipse/wiki/How-to-Use-OpenOCD-on-Windows 
	"How to Use OpenOCD on Windows"
[36]: https://github.com/foss-for-synopsys-dwc-arc-processors/toolchain/wiki
	"ARC GNU Toolchain Wiki Home"
[37]: https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp
	"embARC OSP"
[38]: https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_applications
	"embARC Applications"
[39]: https://github.com/
	"Github Account"
[40]: https://git-for-windows.github.io/
	"Git"
[41]: https://git-scm.com/book/en/v2
	"Git Book"
[42]: https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_applications/blob/master/README.md
	"embARC Applications README"
[43]: http://www.stack.nl/~dimitri/doxygen/download.html
	"Doxygen"
[44]: https://zh.osdn.net/projects/ttssh2/
	"Tera Term"
[45]: https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp/tree/master/example
	"source code of embARC examples"
[46]: https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp/wiki/Getting-Started-with-embARC-Using-GNU-on-Windows#building-and-running-your-first-example
	"Getting Started with embARC Using GNU on Windows"
[47]: https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp/wiki/Getting-Started-with-embARC-OSP
	"Getting Started with embARC OSP"
[48]: https://github.com/torvalds/linux/blob/master/Documentation/process/coding-style.rst     
	"Linux Kernel Coding Style"
[49]: https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp/blob/master/.github/CONTRIBUTING.md 
	"Contributing to embARC OSP"
[50]: http://embarc.org/embarc_osp/doc/embARC_Document/html/page_license.html
	"License"
