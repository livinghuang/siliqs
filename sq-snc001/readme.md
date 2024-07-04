# SQ-SNC001: Weather Station Data Collection and Recorder

![SQ-SNC001](./sq_snc001.png?raw=true)

## Overview

The SQ-SNC001 is a multifunction weather station data collection device. It can collect sensor data via RS485/Modbus, record it on a local SD card, and forward it to a web cloud platform. It supports multiple communication methods, including WiFi and NB-IoT.

## Key Features

- Collect sensor data via RS485/Modbus
- Publish data to MQTT server via WiFi or NB-IoT
- Powered by a solar panel (+18V)
- Internal battery charger function for lead-acid batteries

## Application Scenarios

- Weather Stations
- Agriculture
- Building Environmental Monitoring
- General Environmental Monitoring

## Technical Specifications

- **CPU:** Dual-core Arm Cortex-M0+ processor, flexible clock running up to 133 MHz, 264kB on-chip SRAM
- **Power Source Interface:** Solar charger power source (+18V~30V)
- **Battery Port:** Lead-acid battery (+12V) port x2
- **Communication Port:** RS485/Modbus port with power driving (+12V) x1
- **Connectivity:** NB-IoT, WiFi
- **Storage:** SD Card (32GB)
- **GPS:** Reserved

## Usage Guide

### Connection Guide

#### Solar Panel

Connect the solar panel to the power source interface.

![Solar Panel](./solar_power.png?raw=true)

#### Lead-Acid Battery

Install your lead-acid battery in the main battery port near the solar power interface.

![Lead-Acid Battery](./lead_acid.png?raw=true)

#### 12V Power Output

The 12V power output is reserved for future usage. Leave it open if not needed.

![External Power Output](./external_power_output.png?raw=true)

#### RS485 Communication Port

The SQ-SNC001 supports two Modbus/RS485 communication ports. By default, use Port 1 labeled A1, B1, and leave Port 2 open.

##### Port 1

By default, connect the SQS-LUV800 to this port.

![Modbus Port 1](./modbus_port1.png?raw=true)

##### Port 2

Reserved for future use.

![Modbus Port 2](./modbus_port2.png?raw=true)

### Setup

#### Connection for Weather Sensor via RS485 Outside Cable Bus 

![Wiring Diagram](./wires_weather_sensor.png?raw=true)

#### Connection for Solar Panel via Outside Cable Bus 

![Wiring Diagram](./wires_solar_panel.png?raw=true)

#### Connection for Lead-Acid Battery via Inside Cable Bus 

![Wiring Diagram](./wires_acid_lead.png?raw=true)

#### SD Card Slot  

![Wiring Diagram](./wires_sd_card.png?raw=true)

#### SIM Card Slot  

![Wiring Diagram](./wires_sim_card.png?raw=true)

### Initial Setup of Your System

The SQ-SNC001 uses an SD card file to set up the system. Initially, the SD card is empty. You must add a file named `setup.json` using an editor on your computer (e.g., Notepad on Windows, TextEdit on Mac). The file `sensor_data.csv` will be created and updated by the SQ-SNC001. You only need to copy it to your computer. When initializing a new record, delete the `sensor_data.csv` file, leaving only `setup.json` on the SD card.

![SD Card Files Diagram](./sd_card_files.png?raw=true)

#### How to Set Up Publish Mode

The SQ-SNC001 supports WiFi and NB-IoT modes to publish data, but they cannot work simultaneously. Users can configure the system using a `setup.json` file.

Use the following JSON format to set up the SQ-SNC001:

```json
{
  "mode": "NBIOT",  // NBIOT or WIFI only, default: "NBIOT"
  "device_id": "0",  // 0 ~ 255, default: "0"
  "WIFI_SSID": "your-wifi-ssid",  // Input your WiFi SSID for internet connection, disabled in NBIOT mode
  "WIFI_PASSWORD": "your-wifi-password"  // Input your WiFi password for internet connection, disabled in NBIOT mode
}
```

#### How to Read Local Records

Read the SD card on your computer. It should contain a file named `sensor_data.csv`. The data may appear as shown below.

![Sensor Data Diagram](./sensor_data.png?raw=true)

## Additional Resources

(Include links or references to additional resources, manuals, or support documents.)

# ===

# SQ-SNC001: 天氣站數據收集與記錄器

![SQ-SNC001](./sq_snc001.png?raw=true)

## 概述

SQ-SNC001 是一款多功能天氣站數據收集設備。它可以通過 RS485/Modbus 收集傳感器數據，將其記錄在本地 SD 卡上，並轉發到雲端平台。它支持多種通信方式，包括 WiFi 和 NB-IoT。

## 主要功能

- 通過 RS485/Modbus 收集傳感器數據
- 通過 WiFi 或 NB-IoT 發佈數據到 MQTT 服務器
- 由太陽能板供電（+18V）
- 內置鉛酸電池充電功能

## 應用場景

- 天氣站
- 農業
- 建築環境監測
- 一般環境監測

## 技術規格

- **CPU:** 雙核 Arm Cortex-M0+ 處理器，靈活時鐘最高可達 133 MHz，264kB 片上 SRAM
- **電源接口:** 太陽能充電電源接口（+18V~30V）
- **電池端口:** 鉛酸電池端口（+12V）x2
- **通信端口:** 帶電源驅動的 RS485/Modbus 端口（+12V）x1
- **連接性:** NB-IoT，WiFi
- **存儲:** SD 卡（32GB）
- **GPS:** 預留

## 使用指南

### 連接指南

#### 太陽能板

將太陽能板連接到電源接口。

![Solar Panel](./solar_power.png?raw=true)

#### 鉛酸電池

將鉛酸電池安裝在靠近太陽能電源接口的主電池端口。

![Lead-Acid Battery](./lead_acid.png?raw=true)

#### 12V 電源輸出

12V 電源輸出預留用於未來使用。如果不需要，請保持開放狀態。

![External Power Output](./external_power_output.png?raw=true)

#### RS485 通信端口

SQ-SNC001 支持兩個 Modbus/RS485 通信端口。默認情況下，使用標有 A1、B1 的端口 1，並保持端口 2 開放。

##### 端口 1

默認情況下，將 SQS-LUV800 連接到此端口。

![Modbus Port 1](./modbus_port1.png?raw=true)

##### 端口 2

預留用於未來使用。

![Modbus Port 2](./modbus_port2.png?raw=true)

### 設置

#### 通過 RS485 外部線纜總線連接天氣傳感器

![Wiring Diagram](./wires_weather_sensor.png?raw=true)

#### 通過外部線纜總線連接太陽能板

![Wiring Diagram](./wires_solar_panel.png?raw=true)

#### 通過內部線纜總線連接鉛酸電池

![Wiring Diagram](./wires_acid_lead.png?raw=true)

#### SD 卡插槽

![Wiring Diagram](./wires_sd_card.png?raw=true)

#### SIM 卡插槽

![Wiring Diagram](./wires_sim_card.png?raw=true)

### 初始設置您的系統

SQ-SNC001 使用 SD 卡文件設置系統。初始情況下，SD 卡是空的。您需要使用電腦上的編輯器（例如 Windows 系統上的 Notepad，Mac 系統上的 TextEdit）添加一個名為 `setup.json` 的文件。文件 `sensor_data.csv` 將由 SQ-SNC001 創建和更新。您只需要將其複製到您的電腦中。當初始化新記錄時，刪除 `sensor_data.csv` 文件，只保留 `setup.json` 文件在 SD 卡上。

![SD Card Files Diagram](./sd_card_files.png?raw=true)

#### 如何設置發佈模式

SQ-SNC001 支持 WiFi 和 NB-IoT 模式發佈數據，但它們不能同時工作。用戶可以使用 `setup.json` 文件來配置系統。

使用以下 JSON 格式設置 SQ-SNC001：

```json
{
  "mode": "NBIOT",  // 僅支持 NBIOT 或 WIFI，默認值為 "NBIOT"
  "device_id": "0",  // 0 ~ 255，默認值為 "0"
  "WIFI_SSID": "your-wifi-ssid",  // 輸入您的 WiFi SSID 以連接到互聯網，在 NBIOT 模式下禁用
  "WIFI_PASSWORD": "your-wifi-password"  // 輸入您的 WiFi 密碼以連接到互聯網，在 NBIOT 模式下禁用
}
```

#### 如何讀取本地記錄

在電腦上讀取 SD 卡。應該有一個名為 `sensor_data.csv` 的文件。數據顯示如下。

![Sensor Data Diagram](./sensor_data.png?raw=true)

## 其他資源

（包括其他資源、手冊或支持文檔的鏈接或參考資料。）