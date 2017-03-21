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

## How to contribute to it
We encourage you to contribute to embARC! Please check out the [Contributing to embARC](.github/CONTRIBUTING.md) for guidelines about how to proceed.
