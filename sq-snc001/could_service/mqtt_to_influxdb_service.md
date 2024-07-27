

```
sudo nano /etc/systemd/system/mqtt_to_influx.service
```

```mqtt_to_influx.service
[Unit]
Description=MQTT to InfluxDB
After=network.target

[Service]
Type=simple
User=siliq
ExecStart=/usr/bin/python3 /python/mqtt_to_influx.py
Restart=on-failure

[Install]
WantedBy=multi-user.target
```

```
sudo nano /etc/systemd/system/simulate_node.service
```

```mqtt_to_influx.service
[Unit]
Description=MQTT to InfluxDB
After=network.target

[Service]
Type=simple
User=siliq
ExecStart=/usr/bin/python3 /python/simulate_node.py
Restart=on-failure

[Install]
WantedBy=multi-user.target
```

```
sudo nano /etc/systemd/system/mqtt_to_influx_v2.service
```

```mqtt_to_influx.service
[Unit]
Description=MQTT to InfluxDB
After=network.target

[Service]
Type=simple
User=siliq
ExecStart=/usr/bin/python3 /python/mqtt_to_influx_v2.py
Restart=on-failure

[Install]
WantedBy=multi-user.target
```