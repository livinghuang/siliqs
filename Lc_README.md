---
title: SILIQS - LPWAN IoT PCB 板
description: 一系列用於 IoT 專案的 PCB 板，包含具備通訊功能的主板及各種周邊板。
---

# SILIQS - IoT PCB 專案

歡迎來到 SILIQS 專案！本儲存庫包含了一系列為 IoT 應用設計的 PCB 板。我們的專案包括了用於通訊的主板和數個周邊板，以支持這些主板滿足各種用戶需求。

## 主板

主板是為了通訊而設計，名稱中包含了獨特的標識符號，以 "SQxxx" 開頭，其中 "xxx" 代表板號。例如，SQ001 是我們的第一塊主板。

| 板名            | 描述                                      |
| --------------- | ---------------------------------------- |
| [SQ001S](./sq001s/Lc_readme.md) | 一個多功能的 IoT 開發平台，具備 RISC-V 的性能和兼容 Arduino。 |

## 用於感測器的周邊板

專為感測器應用而設計的周邊板，名稱以 "SQSxxxx" 開頭，其中 "xxxx" 對應到供應商和感測器晶片名稱。例如，SQS1080 是我們為 TI HDC1080 晶片設計的板，具備 SQS 介面。

| 板名            | 描述                                      |
| --------------- | ---------------------------------------- |
| [SQS001](./sqs001/Lc_readme.md) | 濕度、溫度及氣壓感測器板 |
| [SQS002](./sqs002/Lc_readme.md) | 濕度、溫度及氣壓感測器板 |
| [SQS1080](./sqs1080/Lc_readme.md) | 濕度、溫度感測器板 |
| [SQS280](./sqs280/Lc_readme.md) | 溫度及氣壓感測器板 |
| [SQS31865](./sqs31865/Lc_readme.md) | 白金 RTDs (PT100 至 PT1000) 感測器板 |
| [SQS4510](./sqs4510/Lc_readme.md) | 空氣品質 NO2 & O3 |
| [SQS6040](./sqs6040/Lc_readme.md) | 顏色感測器板 |
| [SQS6675](./sqs6675/Lc_readme.md) | 熱電偶感測器板 |
| [SQS6DS3](./sqs6ds3/Lc_readme.md) | 6 軸感測器板 |
| [SQSCD40](./sqscd40/Lc_readme.md) | CO2 氣體感測器板 |
| [SQC485](./sqc485/Lc_readme.md) | 整合 RS485 通訊 |
| [SQCuSD](./sqcusd/Lc_readme.md) | 整合微型 SD 卡 |
| [SQCNBIOT](./sqcnbiot/Lc_readme.md) | 整合 NBIOT 通訊 |
| [SQC4G](./sqc4g/Lc_readme.md) | 整合 4G 通訊 |
| [SQSH001](./sqsh001/Lc_readme.md) | 作為一個多用途外殼，適用於 SMA 天線、IPEX1 天線、鋰離子電池和 USB 連接器。 |

## 用於通訊的周邊板

專為通訊目的而設計的周邊板，名稱以 "SQCxxx" 開頭，其中 "xxx" 表示通訊方法。例如，SQC485 是我們為 RS485 通訊設計的板。

歡迎探索我們的專案及我們提供的各種板。我們歡迎貢獻和反饋！
