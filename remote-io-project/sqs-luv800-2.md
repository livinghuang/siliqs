---
description: 資架構
---

# 資料架構

## 專案總覽

**專案名稱**：SILIQS 子專案 — 遠端 I/O 節點（暫名：SQIO6I2O）

**簡介**：本專案提供一個「6 路數位輸入、2 路數位輸出」的遠端 I/O 節點，透過 LoRaWAN 與 ChirpStack 伺服器整合。後端可下發指令控制輸出腳位、查詢或重置輸入計數，裝置端會定期上報狀態作為 keep‑alive 與即時監測用途。

***

### Downlink 指令定義

型別：`DOWNLINK_COMMAND_t`（1 位元組 `type` + Union `data`）。建議使用固定 fPort（如 5）。

```c
typedef enum : uint8_t {
  REPORT_NONE = 0,
  REPORT_PERIODIC_ALIVE,     // periodic alive report
  REPORT_INPUT_PORT_STATUS,  // ack of read input port
  REPORT_OUTPUT_PORT_STATUS, // ack of read output port
  REPORT_COMMAND_ACK,        // ack of downlink command
} UPLINK_REPORT_t;

struct __attribute__((packed)) input_port_status {
  uint8_t port_num;  // 1..NUMBER_OF_INPUT_PORTS
  uint8_t value;     // 0=L, 1=H
  uint32_t counter;  // 計數值
};

struct __attribute__((packed)) report_input_ports_status_struct {
  uint8_t len;
  input_port_status data[NUMBER_OF_INPUT_PORTS];
};

struct __attribute__((packed)) report_output_ports_status_struct {
  uint8_t len;
  output_port_status data[NUMBER_OF_OUTPUT_PORTS];
};

struct __attribute__((packed)) lorawan_uplink_report_t {
  UPLINK_REPORT_t type; // 1 byte
  union {
    struct report_alive_struct alive; // 週期性報告（新）
    report_input_ports_status_struct  input_port_status;
    report_output_ports_status_struct output_port_status;
    report_ack_struct                 report_ack;       // 下行命令 ACK 回覆
  } data;
};
```

***

### 報文：REPORT\_PERIODIC\_ALIVE（Alive 摘要）

```c
struct __attribute__((packed)) report_alive_struct {
  uint8_t  fw_version;          // 韌體版本（自行定義編碼表）
  uint8_t  battery_level;       // 0..100 %
  uint8_t  sys_status_flags;    // 系統狀態位元（見下）
  uint32_t uptime_seconds;      // 裝置開機秒數
  uint16_t current_debounce;    // 目前 debounce 設定（ms）
  uint16_t current_keep_alive;  // 目前 keep-alive（s）
};
```

#### `sys_status_flags` 建議位元配置

| Bit | 名稱                   | 說明                      |
| --: | -------------------- | ----------------------- |
|   0 | SYS\_OK              | 一般健康（0=未知/由其他位描述, 1=OK） |
|   1 | LOW\_BAT             | 低電壓門檻觸發                 |
|   2 | TEMP\_ALERT          | 溫度告警（若有溫感）              |
|   3 | UPLINK\_BACKPRESSURE | 上行佇列壓力（節流生效）            |
|   4 | WATCH\_ACTIVE        | 輸入監看功能啟用                |
|   5 | PWM\_ACTIVE          | 任一輸出 PWM 正在執行           |
|   6 | RESERVED             | 保留                      |
|   7 | RESERVED             | 保留                      |

> 可依硬體擴充定義；Alive 報告也可附帶「最近一次錯誤碼」或計數摘要，但建議保持精簡以節省 airtime。

***

### ACK 狀態碼（更新）

```c
enum report_ack_code : uint8_t {
  REPORT_ACTION_OK = 0,
  REPORT_ACTION_OK_FOR_INPUT_DEBOUNCE_PARAMS,
  REPORT_ACTION_OK_FOR_UPLINK_RULE_PARAMS,
  REPORT_ACTION_OK_FOR_WATCH_INPUT_PORTS_PARAMS,
  REPORT_ACTION_OK_FOR_OUTPUT_PORTS,
  REPORT_ACTION_FAILED,
  REPORT_ACTION_FAILED_FOR_INVALID_PIN_NUM, // For Read/Reset failures
  REPORT_ACTION_FAILED_FOR_INPUT_DEBOUNCE_PARAMS,
  REPORT_ACTION_FAILED_FOR_UPLINK_RULE_PARAMS,
  REPORT_ACTION_FAILED_FOR_WATCH_INPUT_PORTS_PARAMS,
  REPORT_ACTION_FAILED_FOR_OUTPUT_PORTS,
};
```

對應的通用 ACK 結構維持不變：

```c
struct __attribute__((packed)) report_ack_struct {
  report_ack_code code; // 1 byte
  union {
    command_debounce_struct debounce;
    command_uplink_rule_struct uplink_rule;
    command_watch_input_port_settings_struct watch_input_port_settings;
    command_set_output_port_status_struct set_output_port_status;
  } data;
};
```

#### 上行 ACK 範例（JSON 說明）

* `type = REPORT_COMMAND_ACK`
* `data.report_ack.code = REPORT_ACTION_OK_FOR_OUTPUT_PORTS`
* `data.report_ack.data.set_output_port_status.len = 1`
* `data.report_ack.data.set_output_port_status.data[0] = { port=1, value=1, pwm_duty=50.0, pwm_period_secs=1.0 }`

> 應用伺服器可將該 ACK 與原始下行命令關聯（以 devEUI + 事件時間或自訂 request\_id）。

***

### COMMAND\_ACK 回覆格式與範例

#### 語意

* 當節點處理任何下行命令（設定/查詢/重置…）完成後，以 `REPORT_COMMAND_ACK` 回覆。
* `report_ack.code` 描述成功/失敗與作用範疇；`report_ack.data.*` 回顯**實際落地**參數（例如 PWM 實際 duty/period，或被 clamp 後的值）。

#### C++ 側封包範例（序列化 → Base64 前的 bytes）

```cpp
// 例：下行要求：設定 Uplink 規則 at_least=15s, keep_alive=300s
// 節點處理完畢後回覆：REPORT_COMMAND_ACK 並回顯實際採用值

lorawan_uplink_report_t up{};
up.type = REPORT_COMMAND_ACK;
up.data.report_ack.code = REPORT_ACTION_OK_FOR_UPLINK_RULE_PARAMS;
up.data.report_ack.data.uplink_rule.at_least = 15;   // clamp 後的值
up.data.report_ack.data.uplink_rule.keep_alive = 300; // clamp 後的值

// 序列化：請以明確 byte-writer（避免直接寫 struct 造成對齊/大小端 UB）
```

#### Python（struct.pack）序列化模板（示意）

```python
import struct, base64

REPORT_COMMAND_ACK = 4  # 對應你的列舉值
REPORT_ACTION_OK_FOR_UPLINK_RULE_PARAMS = 2

# lorawan_uplink_report_t 手動序列化（簡化示意）
# type(1B) + code(1B) + uplink_rule(4B: 2x uint16 LE)
payload = bytearray()
payload += struct.pack('<B', REPORT_COMMAND_ACK)
payload += struct.pack('<B', REPORT_ACTION_OK_FOR_UPLINK_RULE_PARAMS)
payload += struct.pack('<HH', 15, 300)  # at_least=15, keep_alive=300
b64 = base64.b64encode(payload).decode()
# 將 b64 放入 ChirpStack 上行解碼器或測試工具檢視
```

#### MQTT 側觀察（應用伺服器）

* 解析 `REPORT_COMMAND_ACK` 後：
  * 紀錄 `code` 與落地參數（方便稽核/追蹤）
  * 如為關鍵控制，將 ACK 與對應的下行命令 `request_id`（若有）建立關聯，完成閉環

> 提醒：若未來添加新的下行指令，請同步擴展 `report_ack_code` 與 `report_ack.data` 的 union 欄位，以保持語意清晰。

***

### ACK／錯誤回覆格式

> 你新增了 ACK 與錯誤語意，以下將其整理為文件規格。現行 `lorawan_uplink_report_t` 尚未納入 ACK 結構（見建議）。

#### ACK 狀態碼

```c
enum report_ack_code : uint8_t {
  REPORT_ACTION_OK = 0,
  REPORT_ACTION_OK_FOR_INPUT_DEBOUNCE_PARAMS,
  REPORT_ACTION_OK_FOR_UPLINK_RULE_PARAMS,
  REPORT_ACTION_OK_FOR_WATCH_INPUT_PORTS_PARAMS,
  REPORT_ACTION_OK_FOR_OUTPUT_PORTS,
  REPORT_ACTION_FAILED,
  REPORT_ACTION_FAILED_FOR_INPUT_DEBOUNCE_PARAMS,
  REPORT_ACTION_FAILED_FOR_UPLINK_RULE_PARAMS,
  REPORT_ACTION_FAILED_FOR_WATCH_INPUT_PORTS_PARAMS,
  REPORT_ACTION_FAILED_FOR_OUTPUT_PORTS,
};
```

#### ACK 資料結構（回顯實際落地參數）

```c
struct __attribute__((packed)) report_ack_struct {
  report_ack_code code; // 執行結果
  union {
    command_debounce_struct debounce;
    command_uplink_rule_struct uplink_rule;
    command_watch_input_port_settings_struct watch_input_port_settings;
    command_set_output_port_status_struct set_output_port_status;
  } data;
};
```

#### 建議（與相容性）

* 建議將 `report_ack_struct` 也納入 `lorawan_uplink_report_t` 的 union（例如 `report_ack`），並在 `REPORT_INPUT_PORT_STATUS/REPORT_OUTPUT_PORT_STATUS` 之外提供通用的 **指令 ACK** 型態（或沿用 `REPORT_ERROR` 搭配 `code`）。
* 若不改動現有 union，也可在 keep‑alive/狀態報文中附帶最近一次指令的 ACK 摘要（較不即時）。

***

### 預設值與錯誤碼

```c
#define REPORT_ERROR_NONE 0
#define REPORT_ERROR_INVALID_PIN_NUM 1

#define DEFAULT_KEEP_ALIVE_SEC 300 // 300 sec
#define DEFAULT_AT_LEAST_SEC 15    // 15 sec
#define DEFAULT_DEBOUNCE_MS 50     // 50 ms
```

***

### 範例 JSON

```json
{
  "devEui": "0000ec6fb9818c58",
  "confirmed": true,
  "fPort": 5,
  "data": "Ag8ALAE="
}
```

***
