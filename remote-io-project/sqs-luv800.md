---
description: 專案名稱：SILIQS 子專案 — 遠端 I/O 節點（SQIO6I2O）
---

# 專案總覽

## 簡介：

本專案是一個完整的 遠端 I/O 控制與監測系統，由三個部分組成：

1. 節點端（Node Side）
   * 採用 SQIO-6I2O 智慧 I/O 模組，內含 6 路光耦隔離輸入與 2 路 MOSFET 輸出。
   * 由 RL78/G12 控制核心與 LoRa CT62 模組構成，具備 Debounce、Counter、PWM、Keep-alive 等功能。
   * 透過 LoRaWAN 與網關通信，定期上報輸入狀態與系統資訊，並接收來自伺服器的遠端控制命令。
2. 伺服器端（ChirpStack Server）
   * 以 ChirpStack 為中心的 LoRaWAN 網路伺服器，負責裝置註冊、下行命令轉發與上行訊息解碼。
   * 伺服器內部使用 Encoder / Decoder 腳本（在 Application 層）解析節點端上行的報文結構（lorawan\_uplink\_report\_t），並將下行指令（lorawan\_downlink\_command\_t）編碼為 Base64 傳輸。
   * 可透過 MQTT 介面將裝置資料同步至應用層或外部系統。
3. 客戶端（Client Side）
   * 透過 MQTT Broker（如 Mosquitto）與 ChirpStack 進行資料交換。
   * 負責：
     * 訂閱上行主題 application/\<appID>/device/\<devEUI>/event/up 接收節點上報資料；
     * 發布下行主題 application/\<appID>/device/\<devEUI>/command/down 傳送控制指令。
   * 可作為資料採集與控制前端，整合至 Python、Node-RED 或 Web Dashboard 系統，用於即時監控與參數設定。

***

整體架構關係圖（邏輯層）

```
 ┌─────────────────────────────┐
 │       Client Side           │
 │  (MQTT Subscriber/Publisher)│
 │ ─────────────────────────── │
 │  • 收集節點上報資料          │
 │  • 發送控制指令至伺服器      │
 └────────────┬────────────────┘
              │ MQTT
              ▼
 ┌─────────────────────────────┐
 │       ChirpStack Server     │
 │ ─────────────────────────── │
 │  • LoRaWAN Network / App 層 │
 │  • Encoder / Decoder         │
 │  • 與 MQTT Broker 溝通       │
 └────────────┬────────────────┘
              │ LoRaWAN
              ▼
 ┌─────────────────────────────┐
 │         Node (SQIO-6I2O)    │
 │ ─────────────────────────── │
 │  • RL78/G12 + SX1262        │
 │  • 6DI + 2DO + PWM 控制     │
 │  • Keep-alive + Debounce    │
 │  • LoRaWAN Uplink/Downlink  │
 └─────────────────────────────┘
```

***

### 系統資料流程（Data Flow Overview）

整個 SILIQS 遠端 I/O 節點系統 的核心運作可分為三個主要資料流：

#### 1. 上行資料（Uplink Data）

節點端（SQIO-6I2O）會依照設定的規則定期或事件觸發上報資料，主要類型如下：

* REPORT\_PERIODIC\_ALIVE：週期性 keep-alive 資訊，包括電壓、韌體版本、運行時間等。
* REPORT\_INPUT\_PORT\_STATUS：當輸入腳位狀態變化（或伺服器要求即時回報）時上傳。
* REPORT\_OUTPUT\_PORT\_STATUS：輸出腳位狀態變化後的回報。
* REPORT\_COMMAND\_ACK：執行下行命令後的回覆（成功/失敗與實際參數）。

上報資料經由 LoRaWAN 網路傳送至 Gateway → ChirpStack Network Server → Application Server，

最終以 MQTT JSON 格式發佈至：

```
application/<appID>/device/<devEUI>/event/up
```

客戶端即可透過訂閱此主題獲得節點最新狀態。

***

#### 2. 下行資料（Downlink Command）

伺服器端或客戶端可對節點發送控制命令，例如：

* 設定輸入去抖時間（COMMAND\_SET\_INPUT\_DEBOUNCE）
* 修改上報規則（COMMAND\_SET\_UPLINK\_RULE）
* 控制輸出腳位高低電位或 PWM（COMMAND\_SET\_OUTPUT\_PORTS）

命令會先經 ChirpStack Application Server 編碼（Base64），再發佈至：

```
application/<appID>/device/<devEUI>/command/down
```

節點接收並解析後，執行對應動作，然後透過 REPORT\_COMMAND\_ACK 回覆結果。

***

#### 3. 雙向回饋（Command Feedback Loop）

為確保命令閉環可靠，節點端所有設定指令（Set、Reset、Watch）都必須：

1. 執行後回覆 REPORT\_COMMAND\_ACK。
2. 回覆內容包含 report\_ack\_code 狀態與落地參數。
3. 若執行失敗（如無效腳位或超出範圍），會附上對應錯誤碼。

客戶端可根據此回覆確認：

* 指令是否成功執行；
* 實際生效參數與預期是否一致；
* 節點當前運行狀態。

***

### 開發分工與架構建議

| 模組                | 主要內容                                                       | 開發語言 / 環境                        | 負責層面               |
| ----------------- | ---------------------------------------------------------- | -------------------------------- | ------------------ |
| 節點端 (Node)        | SQIO-6I2O 韌體，含 LoRaWAN Stack、I/O 控制、PWM、報文封包               | C / RL78-CS+ / Arduino           | 訊號輸入輸出邏輯、封包結構定義    |
| 伺服器端 (ChirpStack) | LoRaWAN Network + Application Server，包含 Encoder/Decoder 腳本 | JavaScript (Decoder) / TOML (設定) | 封包轉換、裝置管理、MQTT 接口  |
| 客戶端 (Client)      | MQTT 資料處理、命令發布、資料入庫                                        | Python / Node-RED / Go           | 與伺服器互通、視覺化監控、指令自動化 |



***

### 系統特點總結

* 模組化架構：節點、伺服器、客戶端相互解耦，方便獨立開發與測試。
* 低功耗設計：LoRa 通訊、週期上報與 Debounce 降耗控制。
* 高可靠性：支援指令 ACK 與狀態回饋閉環。
* 擴展性：支援多節點、多應用場景（PLC 介面、遠端繼電控制、感測器整合）。

***

***
