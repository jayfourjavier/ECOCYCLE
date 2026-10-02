# EcoCycle Machine Operation Guide

This project controls a composting / soil mixing machine with LCD display output, sensor monitoring, and Firebase status reporting. The machine follows a defined sequence of operations and updates both the local display and the mobile app status.

## 1. User Interaction / Hopper Conditions

### Loam Soil Hopper

- If loam soil level is OK:
  - LCD: `LOAM SOIL: OK`
  - App status: `LOAM SOIL: OK`
- If loam soil level is below the acceptable limit:
  - LCD: `WARNING: ADD LOAM SOIL`
  - App status: `WARNING: ADD LOAM SOIL`

### Food Waste Hopper

- If food waste level is empty:
  - LCD: `WAITING`
  - App status: `WAITING`
- If food waste level is OK:
  - LCD: `FOOD WASTE: OK`
  - App status: `FOOD WASTE: OK`

## 2. Soil Addition

When the machine is adding soil to the mixing chamber:

- LCD: `ADDING SOIL: CURRENT WEIGHT / TARGET WEIGHT`
- Example: `90 / 950 GRAMS`
- App status: `ADDING SOIL`

This is used to add a target amount of soil into the chamber:

- `TARGET_SOIL_GRAMS = 950`
- The system continues until the current weight reaches the target weight.

## 3. Food Waste Processing Sequence

When the food waste level reaches a defined threshold:

1. The machine waits for the drainage time before grinding.
2. During waiting, the LCD displays:
   - `DRAINING. TIME REMAINING: (XXXs)`
3. Before starting the grinder:
   - `TURNING ON GRINDER`
4. During grinding:
   - `GRINDING: FOOD WASTE CURRENT WT / TARGET WT`
   - Example: `GRINDING: 230 / 500 GRAMS`
5. After grinding:
   - `TURNING OFF GRINDER`

Then the machine starts the mixing cycle for the configured mixing time.

## 4. Mixing Cycle

### Before mixing

- LCD: `TURNING ON MIXER`
- App status: `MIXING`

### During mixing

- LCD: `MIXING: TIME REMAINING: (XXX)s`
- App status: `MIXING`

### After mixing

- LCD: `TURNING OFF MIXER`

## 5. Blade Positioning and Sensor Probe Operation

The machine positions the mixing blades to accommodate the sensor probes using a Hall effect sensor.

- LCD: `POSITIONING MIXING BLADES`
- App status: `POSITIONING BLADES`

Then the machine lowers the sensor probe:

- LCD: `LOWERING SENSOR PROBE`
- App status: `LOWERING PROBE`

## 6. Sensor Reading Sequence

The sensor probe gets the temperature and moisture of the mixture.

### Temperature

- LCD: `GETTING TEMPERATURE`
- App status: `READING TEMPERATURE`
- Final print: `TEMPERATURE: XX.X C`

### Moisture

- LCD: `GETTING MOISTURE`
- App status: `READING MOISTURE`
- Final print: `MOISTURE: XX %`

## 7. Batch Record Creation

After sensor values are captured, the machine assigns a batch number and saves the record.

Example output:

```text
BATCH #: 104
DATE: 2026-10-02
TIME: 14:35:21
TEMPERATURE: 29.4 C
MOISTURE: 58 %
```

Then the machine opens the mixer outlet / discharge opening.

- LCD: `OPENING MIXER`
- App status: `OPENING MIXER`

## 8. Dispensing / Output Cycle

Before dispensing:

- LCD: `TURNING ON MIXER`
- App status: `DISPENSING`

During dispensing:

- LCD: `MIXING: TIME REMAINING: (XXX)s`
- App status: `DISPENSING`

After dispensing:

- LCD: `TURNING OFF MIXER`

Finally, the machine waits for the user to remove the container.

- LCD: `WAITING FOR CONTAINER REMOVAL`
- App status: `WAITING TO REMOVE CONTAINER`

---

# Machine State Table

| Machine State | LCD PRINT | MOBILE APP STATUS PRINT |
| --- | --- | --- |
| 0 = Idle | `IDLE` | `IDLE` |
| 1 = Grinding | `TURNING ON GRINDER` / `GRINDING: FOOD WASTE CURRENT WT / TARGET WT` / `TURNING OFF GRINDER` | `GRINDING` |
| 2 = Adding Soil | `ADDING SOIL: CURRENT WEIGHT / TARGET WEIGHT` | `ADDING SOIL` |
| 3 = Mixing | `TURNING ON MIXER` / `MIXING: TIME REMAINING: (XXX)s` / `TURNING OFF MIXER` | `MIXING` |
| 4 = Getting Sensor Data | `GETTING TEMPERATURE` / `GETTING MOISTURE` / `TEMPERATURE: XX.X C` / `MOISTURE: XX %` | `READING SENSORS` |
| 5 = Dispensing | `OPENING MIXER` / `TURNING ON MIXER` / `MIXING: TIME REMAINING: (XXX)s` / `TURNING OFF MIXER` | `DISPENSING` |
| 6 = Waiting to Remove Container | `WAITING FOR CONTAINER REMOVAL` | `WAITING TO REMOVE CONTAINER` |

---

# Firebase Status Writing

These states are intended to be written to Firebase for the mobile app and dashboard.

## Recommended Firebase paths

```cpp
const String kMachineStatePath   = "/machine/state";
const String kLcdPrintPath       = "/machine/lcd_print";
const String kAppStatusPath      = "/machine/app_status";
const String kBatchPath          = "/machine/batch";
const String kTemperaturePath    = "/machine/temperature";
const String kMoisturePath       = "/machine/moisture";
const String kLastUpdatedPath    = "/machine/last_updated";
```

## Example Firebase write code

```cpp
void updateFirebaseStatus(int state, const String &lcdText, const String &appText,
                          float temperature, float moisture, int batchNumber)
{
  FirebaseWriter firebase(
      FIREBASE_WEB_API_KEY,
      FIREBASE_DATABASE_URL,
      FIREBASE_USER_EMAIL,
      FIREBASE_USER_PASSWORD,
      3);

  firebase.begin();

  firebase.writeInt("/machine/state", state);
  firebase.writeString("/machine/lcd_print", lcdText);
  firebase.writeString("/machine/app_status", appText);
  firebase.writeFloat("/machine/temperature", temperature);
  firebase.writeFloat("/machine/moisture", moisture);
  firebase.writeInt("/machine/batch", batchNumber);
}
```

If the project uses the FirebaseClient API directly, the equivalent pattern is:

```cpp
Database.set<int>(aClient, "/machine/state", state);
Database.set<String>(aClient, "/machine/lcd_print", lcdText);
Database.set<String>(aClient, "/machine/app_status", appText);
Database.set<float>(aClient, "/machine/temperature", temperature);
Database.set<float>(aClient, "/machine/moisture", moisture);
Database.set<int>(aClient, "/machine/batch", batchNumber);
```

> Use exact fixed-path writes for app status, not random push IDs. This keeps the app status synchronized and easy to read on the dashboard.

---

# Example Flow

```text
0 -> IDLE
1 -> GRINDING
2 -> ADDING SOIL
3 -> MIXING
4 -> GETTING SENSOR DATA
5 -> DISPENSING
6 -> WAITING TO REMOVE CONTAINER
```

The machine should keep updating the app with the latest status message and the corresponding machine state value so the mobile app reflects the current process in real time.
