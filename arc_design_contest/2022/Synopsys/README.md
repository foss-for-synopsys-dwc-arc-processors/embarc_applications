## 基于ARC EM9D处理器的AIoT电子系统设计

### 基于ARC IoT DK平台开发指南

#### ARC IoT DK简介
![ARC IoT Development Kit](https://github.com/foss-for-synopsys-dwc-arc-processors/foss-for-synopsys-dwc-arc-processors.github.io/raw/master/images/arc_iot_dev_kit2.png)
ARC IoT Development Kit（IoTDK）开发板是基于ARC EM9D处理器的低功耗高性能MCU, 内部振荡器频率高达144MHz. 同时采用超低功耗工艺，以满足物联网应用所需的严苛的功耗与性能要求。IoTDK集成了多个通用外设几口、车载低功耗蓝牙模块、9轴传感器，可应用于可穿戴设备、家庭自动化等智能物联网场景。

IoTDK可通过可用的Arduino、mikroBus和Pmod连接器进一步扩展。

IoTDK的介绍和主要特性，请参考[这里](https://github.com/foss-for-synopsys-dwc-arc-processors/ARC-Development-Systems-Forum/wiki/ARC-Development-Systems-Forum-Wiki-Home#arc-iot-development-kit-1).

#### 赛前准备
选择IoTDK开发平台的同学，请事先务必：
1. 选择软件开发平台，IoTDK已广泛支持于各物联网操作系统软件开发平台，如embARC OSP、 Zephyr、RT-Thread、FreeRTOS、uCOS、TencentOS Tiny、Alios things等，比赛中均可以使用和参考[这些平台](https://embarc.org/projects/iot-projects/ )。推荐从以下平台中选择
    - [embARC Open Software Platform (OSP)](https://embarc.org/project/embarc-open-software-platform-osp/)
        - OSP的releases版本请从[这里](https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp/releases)下载
        - OSP的文档，请参考[这里](https://foss-for-synopsys-dwc-arc-processors.github.io/embarc_osp/doc/build/html/index.html)
    - [Zephyr](https://github.com/zephyrproject-rtos/zephyr)
        - Zephyr的releases版本请从[这里](https://github.com/zephyrproject-rtos/zephyr/releases)下载
        - Zephyr的文档，请参考[这里](https://docs.zephyrproject.org/latest/index.html)
2. 下载[embARC机器学习库](https://github.com/foss-for-synopsys-dwc-arc-processors/tensorflow/tree/arc_iotdk/tensorflow/lite/micro/kernels/arc_mli)
    - 机器学习模型部署说明，请参考[这里](https://github.com/foss-for-synopsys-dwc-arc-processors/tensorflow/tree/arc_iotdk/tensorflow/lite/micro/tools/make/targets/arc#arc-iot-development-kit-arc-iot-dk)
    - IoTDK机器学习模型部署实例，请参考[这里](https://github.com/foss-for-synopsys-dwc-arc-processors/tensorflow/tree/arc_iotdk/tensorflow/lite/micro/examples )
3. 下载工具链
    - GUN工具链， [下载链接](https://github.com/foss-for-synopsys-dwc-arc-processors/toolchain/releases )
    - MetaWare工具链， [下载链接](https://eval.synopsys.com/Home/MetaWareLite )

#### 参考资料
- ARC处理器内核硬件特性详细说明，可参考MetaWare软件安装目录下相关文档,`MetaWare/arc/docs/pdf/hardware/arc_em`,`MetaWare/arc/docs/pdf/dsp`。
- ARC Machine Learning Inference（MLI1.1）软件库。机器学习模型可以使用TFLM（已内嵌MLI，不需要单独下载和使用MLI源码）部署，也可以直接调用MLI1.1 API实现
    - 软件库，[下载链接](https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_mli/releases)
    - 应用实例，请参考[这里](https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_mli/tree/mli_dev/examples/tutorial_emnist_tensorflow)
    - 说明文档，请参考[这里](https://foss-for-synopsys-dwc-arc-processors.github.io/embarc_mli/doc/build/html/index.html )
- [集成MLI链接库的embARC OSP软件平台](https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp/tree/embarc_mli )，支持GUN工具链。
### ARC AIoT DK开发指南
![ARC AIoT Development Kit](https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_applications/tree/master/arc_design_contest/2022/Synopsys/img/AIoT.PNG)