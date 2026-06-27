# moist-home
home automation

## services

- Mosquitto on `1883` / `8883`
- InfluxDB on `8086`
- Grafana on `3000`

## quick start

Bring the stack up:

```bash
docker compose up -d
```

Grafana defaults:

- user: `admin`
- password: `change-me-now`

InfluxDB defaults:

- user: `admin`
- password: `change-me-now`
- org: `moist-home`
- bucket: `mqtt`

Telegraf subscribes to:

- `home/+/telemetry`
- `home/+/state`

Send JSON like this to test ingestion:

```bash
mosquitto_pub -h localhost -p 1883 -t home/kitchen/telemetry -m '{"temperature":22.4,"humidity":48}'
```
