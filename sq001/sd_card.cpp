#include "global.h"

bool sdInitialized = false;
bool sdd_setting();
bool sd_write_success = false;
void sd_init(void)
{
	extern RadioEvents_t RadioEvents;
	Radio.Init(&RadioEvents);
	Radio.Sleep();
	SPI.end();

	extern RadioEvents_t RadioEvents;
	Radio.Init(&RadioEvents);
	Radio.Sleep();
	SPI.end();

	pinMode(RADIO_DIO_1, ANALOG);
	pinMode(RADIO_NSS, ANALOG);
	pinMode(RADIO_RESET, ANALOG);
	pinMode(RADIO_BUSY, ANALOG);
	pinMode(LORA_CLK, ANALOG);
	pinMode(LORA_MISO, ANALOG);
	pinMode(LORA_MOSI, ANALOG);

	pinMode(pVext, OUTPUT);
	digitalWrite(pVext, HIGH);
	pinMode(RADIO_RESET, OUTPUT);
	digitalWrite(RADIO_RESET, LOW);
	delay(10);
	sdInitialized = sdd_setting();

	if (sdInitialized)
	{
		Serial.println("SD卡初始化成功");
	}
	else
	{
		Serial.println("SD卡初始化失敗");
	}
}

void sd_test()
{
	// 主要迴圈程式碼放在這裡
	File file = SD.open("/test.txt");
	if (file)
	{
		Serial.println("Successfully opened file:");
		while (file.available())
		{
			Serial.write(file.read());
		}
		file.close();
	}
	else
	{
		Serial.println("Failed to open file");
	}

	File writeFile = SD.open("/test.txt", FILE_WRITE);
	if (writeFile)
	{
		sd_write_success = true;
		String data = "Hello, SD card ^_^";
		writeFile.println(data);
		writeFile.close();
		Serial.println("Successfully wrote to file");
	}
	else
	{
		Serial.println("Failed to write to file");
	}

	delay(500);
}

bool sdd_setting()
{
	SPI.begin(pSCK, pMISO, pMOSI, pCS);

	if (!SD.begin(pCS, SPI, 1000000))
	{
		return false;
	}
	return true;
}
