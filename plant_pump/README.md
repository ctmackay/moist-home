## Build and Run

From the project root:

```bash
mkdir -p build
cd build
cmake ..
make
```

Run the program:

```bash
./plant_controller
```

Example output:

```text
Connected to MQTT broker
Published status: moisture=14.000000, pump_on=false
Published status: moisture=13.000000, pump_on=false
Published status: moisture=12.000000, pump_on=false
Pump ON
Published status: moisture=9.000000, pump_on=true
```

To rebuild after changing the code:

```bash
cd build
make
./plant_controller
```
