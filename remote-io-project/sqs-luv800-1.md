---
description: 實作硬體
---

# SQIO-6I2O 智慧遠端 I/O 模組簡介

## SQIO-6I2O 智慧遠端 I/O 模組簡介



SQIO-6I2O 是一款為工業與物聯網應用設計的智慧遠端 I/O 模組，整合 LoRa 通訊、RL78/G12 控制核心、隔離式數位輸入與輸出，可在惡劣環境下穩定運行，支援多種電源輸入並具備 USB 介面方便韌體開發與調試。

***

### 電路模組構成

#### 1. AUTO-Boot 模組

支援自動進入編程模式，透過 USB-UART 的 DTR/RTS 信號控制 MCU 的 RESET 與 BOOT，方便開發階段快速燒錄韌體。

#### 2. Extend Power 模組

提供外部電源擴充控制電路，可由 MCU GPIO 控制 MOSFET 開關，實現外接模組（如感測器或執行器）供電的自動管理。

#### 3. Type-C USB 介面

整合 USB Type-C 輸入電源 與 USB-to-UART 通訊：

* 支援 USB 與外部電源雙供電（8V–30V 寬範圍 DC 輸入）。
* 具備 降壓轉換器，將 VP 輸出穩壓至 3.3 V 系統電源。
* 內建 ESD 保護與電流保護元件，確保長期穩定運作。

#### 4. UART-to-USB 轉換

採用 CH340 系列 USB-UART 晶片，與 RL78/G12 MCU 串接，便於 PC 端監控、除錯與參數設定。

#### 5. LORA CT62 通訊模組

搭載 Semtech SX1262 LoRa 收發晶片，支援 Sub-GHz 頻段（例如 923 MHz），提供長距離低功耗通訊能力，可直接連接至 LoRaWAN 網路（如 ChirpStack）。

#### 6. RL78/G12 微控制器核心

Renesas RL78/G12 MCU 作為主控單元，負責：

* GPIO 輸入輸出控制與邏輯判斷
* UART 通訊與 LoRa 指令橋接
* 去抖動、計數器、PWM 產生等邏輯功能
* LoRaWAN 協議堆疊處理

#### 7. Optocouple DRIVER 輸入/輸出模組

* 6 路光耦隔離輸入：採用 EL817 光耦，支援 24 V 工控訊號，具備抗干擾能力。
* 2 路 MOSFET 輸出驅動：支援最大數安培負載，可直接驅動繼電器、LED 指示或小型電磁閥。
* 光耦與 MOSFET 之間電氣隔離，保障系統安全。

***

### 主要特色

| 項目      | 規格                                 |
| ------- | ---------------------------------- |
| MCU     | Renesas RL78/G12                   |
| LoRa 模組 | SX1262 (CT62 footprint)            |
| 輸入端     | 6 路光耦隔離輸入 (24 V compatible)        |
| 輸出端     | 2 路 MOSFET 開集輸出                    |
| 電源輸入    | 8–30 V DC，USB Type-C 或外部電源         |
| 通訊介面    | LoRa / USB-UART                    |
| 功能特性    | Debounce、Input Counter、PWM 輸出、遠端控制 |
| 應用領域    | 工業監控、智慧建築、環境感測、遠端繼電控制              |

***

### 系統運作概要

1.  上報 (Uplink)

    MCU 週期性上報輸入狀態與設備健康資訊至 LoRaWAN 伺服器。
2.  下發 (Downlink)

    伺服器可遠端控制輸出腳位、修改上報間隔、設定去抖時間或 PWM。



## 實機



## 架構圖

<figure><img src="../.gitbook/assets/截圖 2025-11-10 上午11.43.05.png" alt=""><figcaption></figcaption></figure>

## 原理圖

<figure><img src="../.gitbook/assets/截圖 2025-11-10 上午11.45.17.png" alt=""><figcaption></figcaption></figure>



***

