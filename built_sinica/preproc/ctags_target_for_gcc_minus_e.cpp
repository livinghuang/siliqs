# 1 "/Users/living/code/github/siliqs/sq-snc001/mqtt_test_in_ht62/mqtt_test_in_ht62.ino"
# 2 "/Users/living/code/github/siliqs/sq-snc001/mqtt_test_in_ht62/mqtt_test_in_ht62.ino" 2

// base64 Base64; // If the library requires an object instantiation.

// WiFi credentials
const char *ssid = "LIVING_TCL"; // Replace with your WiFi SSID
const char *password = "asdfghjklzzz"; // Replace with your WiFi password

// MQTT Broker settings
const char *mqtt_broker = "13.210.141.86";
const char *mqtt_username = "siliq"; // If needed
const char *mqtt_password = "siliq1234"; // If needed
const char *mqtt_topic = "test_v2/topic";
char encoded_data[255];

WiFiClient espClient;
PubSubClient client(espClient);
WiFiMulti WiFiMulti;
void generate_sensor_data(uint8_t sensor_id, char *encoded_data);
void send_data(char *data, int len)
{
  char encodedString[255];

  Base64.encode(encodedString, data, len);
  printHex((byte *)data, len);
  // Publish the encoded data to MQTT
  // Publish the data
  if (client.connected())
  {
    client.publish(mqtt_topic, encodedString);
    Serial0.println(encodedString);
  }
  else
  {
    Serial0.println("MQTT not connected, cannot send data");
  }
}

void wifi_begin_setup()
{
  // Set WiFi to station mode and disconnect from an AP if it was previously connected.
  WiFi.mode(WIFI_MODE_STA);
  WiFi.disconnect();
  delay(100);

  Serial0.println("Setup done");

  Serial0.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  WiFiMulti.addAP(ssid, password);

  Serial0.println();
  Serial0.println();
  Serial0.print("Waiting for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED)
  {
    Serial0.print(".");
    delay(500);
  }

  Serial0.println("");
  Serial0.println("WiFi connected");
  Serial0.println("IP address: ");
  Serial0.println(WiFi.localIP());

  delay(500);
}
time_t nowSecs;
void setup()
{
  Serial0.begin(115200);

  wifi_begin_setup();
  // Initialize NTP
  configTime(0, 0, "pool.ntp.org", "time.nist.gov"); // Timezone is UTC by default
  Serial0.println("Waiting for NTP time sync");
  nowSecs = time(nullptr);
  while (nowSecs < 8 * 3600 * 2)
  { // Wait for a valid time
    delay(500);
    nowSecs = time(nullptr);
  }

  Serial0.println("Time synced");
  // Connect to MQTT Broker
  client.setServer(mqtt_broker, 1883);
  client.setCallback(mqtt_callback);

  while (!client.connected())
  {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial0.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password))
    {
      Serial0.println("Public emqx mqtt broker connected");
    }
    else
    {
      Serial0.print("failed with state ");
      Serial0.print(client.state());
      delay(2000);
    }
  }
}

void loop()
{
  generate_fixed_sensor_data(12345, encoded_data);
  // Publish the encoded data to MQTT
  // Publish the data
  if (client.connected())
  {
    client.publish(mqtt_topic, encoded_data);
  }
  else
  {
    Serial0.println("MQTT not connected, cannot send data");
  }
  delay(1000);
}
void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
  Serial0.print("Message arrived in topic: ");
  Serial0.println(topic);

  Serial0.print("Message:");
  for (int i = 0; i < length; i++)
  {
    Serial0.print((char)payload[i]);
  }
  Serial0.println();
  Serial0.println("-----------------------");
}

void generate_fixed_sensor_data(uint32_t sensor_id, char *encoded_data)
{
  nowSecs = time(nullptr);
  struct station_data_v2 data;
  data.station_type = 1;
  data.station_id = sensor_id;
  data.timestamp = nowSecs;
  uint8_t mac[] = {
      0xAA,
      0xAA,
      0xAA,
      0xAA,
      0xAA,
      0xAA,
      0xAA,
      0xAA,
      0xAA,
      0xAA,
      0xAA,
      0xAA,
      0xAA,
      0xAA,
      0xAA,
      0xAA};
  memcpy(data.station_mac, mac, sizeof(mac));
  data.battery_voltage = 3.7;
  memset(data.station_reserved, 0, sizeof(data.station_reserved));
  struct louver_data_struct_v2 louver = {
      {0xAA,
       0xAA,
       0xAA,
       0xAA,
       0xAA,
       0xAA},
      {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
      50,
      80,
      450,
      25.0,
      26.0,
      60,
      1000.0};

  uint8_t louver_test_buffer[] = {0x50, 0xE3, 0xB6, 0x6E, 0x9E, 0x9C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0x08, 0xC1, 0x02, 0x40, 0x9D, 0xEF, 0x41, 0x20, 0xC8, 0xF5, 0x41, 0x8F, 0x59, 0xC3, 0x47};
  memcpy(&data.louver, &louver_test_buffer, sizeof(louver));
  Serial0.printf("sizeof(louver):%d\n", sizeof(louver));

  struct wind_data_struct_v2 wind = {5, 180};
  memcpy(&data.wind, &wind, sizeof(wind));
  Serial0.printf("sizeof(wind):%d\n", sizeof(wind));

  struct gnss_data_struct_v2 gnss = {30.0, 120.0, 100.0, {0, 0, 0, 0}};
  memcpy(&data.gnss, &gnss, sizeof(gnss));
  Serial0.printf("sizeof(gnss):%d\n", sizeof(gnss));

  Serial0.printf("sizeof(data):%d\n", sizeof(data));
  Base64.encode(encoded_data, (char *)&data, sizeof(data));

  Serial0.println(encoded_data);
}
