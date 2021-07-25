# 分散式無人機飛安控管與空品監測系統
因為無人機技術突破且價格逐漸親民，無人機使用率逐年攀升，勢必需要一套完善的管理系統，管理空域中之無人機避免其相撞或是闖入禁飛區，本團隊開發分散式無人機飛安控管與空品監測系統可以有效率得進行無人機監管且可以利用無人機可懸停及起降方便的特性收集空氣品質數據並追蹤空氣汙染源頭。

* [簡介](#簡介)
	* [功能](#功能)
	* [系統架構](#系統架構)
	* [APP UI](#app-ui)
    * [Web UI](#web-ui)
* [軟硬體](#軟硬體)
	* [硬體](#硬體需求)
	* [軟體](#軟體需求)
* [使用手冊](#使用手冊)
	* [無人機空間環境感測模組](#無人機空間環境感測模組)
	* [監控塔台及局部環境感測模組](#監控塔台及局部環境感測模組)
    * [空品平台](#空品平台)
    * [手機應用程式(安卓)](#手機應用程式)

## 簡介
    分散式無人機飛安控管與空品監測系統

### 功能
- 無人機立體空間環境數據收集
- 監管塔臺局部低空環境數據收集
- 無人機限禁航區飛航監管
- 無人機飛航風險預測
- 無人機空氣品質數據收集
- 空氣品質監控可視化平臺

![avatar](https://raw.githubusercontent.com/qingxuan-yang/google_map_icon/main/1.png)

### 系統架構
![avatar](https://raw.githubusercontent.com/qingxuan-yang/google_map_icon/main/2.png)

### APP UI
- 即時飛航廣播APP-主畫面:<br/>
![avatar](https://raw.githubusercontent.com/qingxuan-yang/google_map_icon/main/3.png)
- 即時飛航廣播APP-無人機風險評估:<br/>
![avatar](https://raw.githubusercontent.com/qingxuan-yang/google_map_icon/main/4.png)<br/>
1.連接MQTT 代理人<br/>
2-1. 輸入最大風速<br/>
2-2. 輸入衛星訊號強度<br/>
3-1. 送出輸入數值至風險評估模型<br/>
3-2. 按下顯示按鈕，並顯示經運算後之風險等級<br/>
3-3. 切換頁面至無人機飛航監管畫面<br/>

### Web UI
![avatar](https://raw.githubusercontent.com/qingxuan-yang/google_map_icon/main/5.png)

## 軟硬體
### 硬體需求
無人機空間環境感測模組:
- 無人機搭載空間環境感測模組，收集3D立體空間中之環境數據
- Arduino UNO x1
- 4G模組(SIM7600CE) x1
- 溫溼度感測器(SHT31) x1
- PM2.5感測器(SPS30) x1
- 壓力感測器(BMP280) x1<br/>
![avatar](https://raw.githubusercontent.com/qingxuan-yang/google_map_icon/main/6.png)<br/>

監控平臺兼地面局部環境感測站:
- 風向儀(Aprs天氣感測模組) x1
- 風速計(Aprs天氣感測模組) x1
- 雨量計(Aprs天氣感測模組) x1
- Himax WE -I x1
- Arduino Due x1
- Wifi模組(ESP-01) x1
- 天氣模組監控板(Aprs天氣感測模組) x1
- 溫溼度感測器(AM2320) x1
- 大氣壓力感測器(BMP280) x1
- PM2.5感測器(SPS30) x1
- 行動電源 x1<br/>
![avatar](https://raw.githubusercontent.com/qingxuan-yang/google_map_icon/main/7.png)<br/>
![avatar](https://raw.githubusercontent.com/qingxuan-yang/google_map_icon/main/8.png)

### 軟體需求
- Google TensorFlow
- Google TensorFlow Lite for Microcontroller (TinyML)
- Sketch
- C++
- React
- Nodejs
- Google map API

## 使用手冊

### 無人機空間環境感測模組
感測器與Arduino通訊及接線方式如下表格
|  溫溼度感測器(SHT31) |            Arduino(Uno)                             |
| ------------------- | ----------------------------------------------------|
|  VCC                |              5V                                     |
| GND                 |              GND                                    |
|  SDA                |              A4                                     |
|  SCL                |              A5                                     |

|  PM2.5感測器(SPS30) |            Arduino(Uno)                              |
| ------------------- | ----------------------------------------------------|
|  VDD                |              5V                                     |
|  SEL                |              GND                                    |
|  GND                |              GND                                    |
|  SDA                |              SDA                                    |
|  SCL                |              SCL                                    |

|  大氣壓力感測器(BMP280)  |            Arduino(Uno)                             |
| ----------------------- | ----------------------------------------------------|
|  VCC                    |              3.3V                                   |
|  GND                    |              GND                                    |
|  SDA                    |              PIN11                                  |
|  SCL                    |              PIN13                                  |
|  CSB                    |              PIN10                                  |
|  SDO                    |              PIN12                                  |

### 監控塔台及局部環境感測模組
將ESP-01連接Arduino Due，上傳程式至Arduino Due，透過ESP-01接收Wifi訊號，使用MQTT通訊協議與無人機端進行溝通，為了讓Arduino Due 能與Himax WE-I互相通訊，以I2C作為通訊方式，在Himax WE-I的部份藉由Tera Term把程式(內含風險評估模型及無人機監管演算法)燒入，當通訊連接完成後，Arduino Due即可利用MQTT訂閱來Broker中的APP發布訊息及無人機空間環境感測模組:的發布訊息，進入Himax中運算，分別輸出風險等級、飛行指示及風速等級，提供無人機操作者在操作上的參考。

感測器與天氣監控模組通訊及接線方式如下表格
|  感測器                   |            天氣監控模組                             |
| -------------------------|----------------------------------------------------|
|  風速計、風向儀            |             風向儀接口                             |
|  雨量計                    |            雨量計接口                             |
| VCC (AM2320溫溼度感測器)   |     3.3V (溫溼度感測器接口)                        |
| SDA (AM2320溫溼度感測器)   |     SDA (溫溼度感測器接口)                         |
| GND (AM2320溫溼度感測器)   |     GND (溫溼度感測器接口)                         |
| SCL (AM2320溫溼度感測器)   |     SCL (溫溼度感測器接口)                         |
| VCC (BMP280大氣壓力感測器) |     3.3V (大氣壓力感測器接口)                      |
| GND (BMP280大氣壓力感測器) |     GND (大氣壓力感測器接口)                       |
| SCL (BMP280大氣壓力感測器) |     SCL (大氣壓力感測器接口)                       |
| SDA (BMP280大氣壓力感測器) |     SDA (大氣壓力感測器接口)                       |

天氣監控模組與Arduino Due通訊及接線方式如下表格
|  天氣監控模組        |            Arduino(Due)                             |
| ------------------- | ----------------------------------------------------|
|  5V                 |              5V                                     |
|  RX                 |              TX2-16                                 |
|  TX                 |              TX2-17                                 |
|  GND                |              GND                                    |

SPS30 PM2.5感測器與Arduino Due通訊及接線方式如下表格
|  PM2.5感測器(SPS30) |            Arduino(Due)                              |
| ------------------- | ----------------------------------------------------|
|  VDD                |              5V                                     |
|  SEL                |              GND                                    |
|  GND                |              GND                                    |
|  SDA                |              SDA                                    |
|  SCL                |              SCL                                    |

ESP-01與Arduino Due通訊及接線方式如下表格
|  ESP-01             |            Arduino(Due)                             |
| ------------------- | ----------------------------------------------------|
|  VCC                |              5V                                     |
|  CHPD               |              5V                                     |
|  RX                 |              TX2-14                                 |
|  TX                 |              RX2-15                                 |
|  GND                |              GND                                    |

Himax WE-I 與Arduino Due通訊及接線方式如下表格
|  Himax WE-I         |            Arduino(Due)                             |
| ------------------- | ----------------------------------------------------|
|  SCL                |              SCL-21                                 |
|  SDA                |              SDA-20                                 |

### 空品平台
1. 申辦Google map API放入.html api中
2. 於本機terminal輸入npm install及npm start
3. 系統將自動導入React UI
4. 於本機terminal輸入node.index即可使用空品監測平台

### 手機應用程式
1. 本系統採用APP Inventor進行APP開發，檔案類型為aia檔，可以在APP Inventor開發網站中導入檔案，內含畫面編排及Block程式碼。針對使用的部分，本團隊提供apk檔，只要將檔案傳至手機，即可安裝使用，由於APP Inventor開發的APP只支援Android系統的手機，蘋果系列的手機無法直接安裝使用。
2. 將SIM卡安裝進4G模組，並上傳程式至Arduino，Arduino會開始透過4G模組使用MQTT通訊協議與塔台進行溝通。</br>
