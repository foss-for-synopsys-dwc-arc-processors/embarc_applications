# embARC Applications
This repository contains various applications based on embARC OSP, developed by the embARC team and the community (users, ARC licensees, students, hobbyists, etc).

## How to use it
Here are steps to use this repo.
- Clone or download [embARC OSP](https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp) source code to *embarc_osp* folder
    + `git clone https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp.git embarc_osp`
- Clone or download [embARC Applications](https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_applications) source code to *embarc_osp/application* folder
    + `cd embarc_osp`
    + `git clone https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_applications.git application`
- Apply required patches to embARC Open Software Platform
    + Convert the patch script to unix format using *dos2unix* command.
    + Run the patch script written by shell: `bash apply_embARC_patch.sh`
    + Refer to embARC [Documentation](https://foss-for-synopsys-dwc-arc-processors.github.io/embarc_osp/doc/embARC_Document/html/page_example_usage.html#EMBARC_APPLY_PATCHES) for detailed instructions.
- Here are a list of applications we maintain in the repo
    + [AWS IoT SmartHome Demo - Single Node](aws_iot_smarthome)
        * This application implements a demo which shows embARC Smarthome Demo based on AWS IoT Cloud.
        * This smarthome demo only implements 1 node to talk to AWS IoT Cloud.
    + [AWS IoT Smarthome Demo - Multi Node](aws_iot_smarthome_multinode)
        * This application implements a demo which shows embARC Smarthome Demo based on AWS IoT Cloud.
        * This smarthome demo implements 3 different nodes to talk to AWS IoT Cloud.
    + [iBaby Smarthome Demo - Multi Node](ibaby_smarthome_multinode)
        * This application implements a demo which shows embARC Smarthome Demo based on LwM2M protocol.
        * This smarthome demo implements 2 different nodes to talk to iBaby Smarthome Gateway.
    + [iLight Smartdevice Demo](ilight_smartdevice)
        * This application implements a demo which shows embARC Smartdevice Demo based on Bluetooth.
        * This smartdevice demo can be controlled by gestures and iOS App.
    + [OpenThread Smarthome Demo - Multi Node](ot_smarthome_multinode)
        * This application implements a demo which shows embARC Smarthome Demo based on OpenThread.
        * This smarthome demo implements 2 different nodes to talk to OpenThread Smarthome Gateway.

## How to contribute to it
We welcome embARC Applications community contributions. Please check out the [Contributing to embARC OSP and Applications](.github/CONTRIBUTING.md) for guidelines on how to proceed. Users participating in the embARC Community and its sub-projects codebases and issue trackers are expected to follow the [embARC code of conduct](https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp/blob/master/.github/CODE_OF_CONDUCT.md).
