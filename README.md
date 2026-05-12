# Edge-AI-Temperature-Predictor-on-STM32
A microcontroller that doesn't just read temperature — it predicts it.

---

## What is this?

Most embedded systems just collect sensor data and send it to a cloud for processing. This project does things differently — the STM32F4 microcontroller reads temperature from a DHT11 sensor, **and** runs a small AI model on-chip to predict the next temperature, in real time.

---

## How it works

Every 2 seconds, the board does this in order:

1. **Predict** — The AI model guesses what the temperature will be
2. **Read** — The DHT11 sensor measures the actual temperature
3. **Correct** — The model compares its guess to reality and adjusts itself
4. **Send** — The result is sent to your PC over USB/UART

After just a few cycles, the model learns the pattern and starts predicting with higher accuracy.

---

## Files

| File | What it does |
|------|-------------|
| `dht11.c / dht11.h` | Reads temperature and humidity from the DHT11 sensor |
| `ssm.c / ssm.h` | The AI model — predicts temperature and tracks its own accuracy |
| `main.c` | Ties everything together and runs the main loop |
| `visual.py` | Python script that plots live graphs on your PC |

---

## What you need

**Hardware**
- STM32F4 board (example - Nucleo-F446RE)
- DHT11 temperature sensor
- Jumper wires

**Software**
- STM32CubeIDE
- Python 3 with `pyserial` and `matplotlib`

---

## Wiring

| DHT11 Pin | STM32 Pin |
|-----------|-----------|
| VCC       | 3.3V      |
| GND       | GND       |
| DATA      | PA1       |

---

## Setup

**Flash the board**
1. Open the project in STM32CubeIDE
2. Build and flash to your board

**Run the visualizer**
1. Install dependencies:
   ```
   pip install pyserial matplotlib
   ```
2. Open `visual.py` and change the `SERIAL_PORT` to match your COM port
3. Run it:
   ```
   python visual.py
   ```

You should see two live charts — actual vs predicted temperature, and model accuracy over time.

---

## Sample Output

```
Cycle:0  | Temp:26.40 | Hum:65.30 | Pred:25.00 | Err:1.40  | Acc:30.0% | MAE:1.400
Cycle:1  | Temp:26.30 | Hum:66.90 | Pred:26.41 | Err:-0.11 | Acc:94.3% | MAE:0.757
Cycle:3  | Temp:26.40 | Hum:66.40 | Pred:26.41 | Err:-0.01 | Acc:99.4% | MAE:0.404
```

The model starts rough and quickly settles into near-perfect accuracy within a few cycles.

---

## The AI Model (SSM)

The model is called a **State Space Model**. It tracks two things:
- The current temperature
- The trend

In each cycle, it uses these to make a prediction, then corrects itself based on the real reading. It also keeps a running average of its past errors so you can see how well it's doing over time.

It's simple, lightweight, and runs comfortably on a microcontroller with very limited memory and no floating point unit required.

---
