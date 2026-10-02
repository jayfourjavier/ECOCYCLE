# EcoCycle Firebase Data Contract

This project writes machine activity and batch records to Firebase Realtime Database using fixed paths. The mobile app and dashboard should read from these exact values instead of using random push IDs.

## 1. Activity state map

The machine activity value is stored in `/activity` as a numeric integer.

| Value | Meaning |
| --- | --- |
| 0 | Idle |
| 1 | Grinding |
| 2 | Adding Soil |
| 3 | Mixing |
| 4 | Getting Sensor Data |
| 5 | Dispensing |
| 6 | Waiting to Remove Container |

This is the value map the app should read and interpret.

Example:

```cpp
Database.get<int>(aClient, "/activity");
```

Flutter example:

```dart
final activity = snapshot.child('/activity').value as int? ?? 0;

final labels = {
  0: 'Idle',
  1: 'Grinding',
  2: 'Adding Soil',
  3: 'Mixing',
  4: 'Getting Sensor Data',
  5: 'Dispensing',
  6: 'Waiting to Remove Container',
};

final label = labels[activity] ?? 'Unknown';
```

---

## 2. Recommended Firebase paths

```cpp
const String kActivityPath = "/activity";
const String kRecordsPath = "/records";
```

The app can also read the human-readable status in the same pattern if needed:

```cpp
const String kMachineStatePath = "/machine/state";
const String kAppStatusPath = "/machine/app_status";
```

---

## 3. Batch record data format

Each batch is stored under `/records/EC-XXX` as a CSV string.

CSV format:

```text
batch number,timestamp,temperature,humidity
```

Example:

```text
EC-004,1790904651,29.4,58.0
```

This means:

- `EC-004` = batch number
- `1790904651` = Unix timestamp in seconds
- `29.4` = temperature in °C
- `58.0` = moisture in %

The app should read the value as a string and then split by comma.

Flutter example:

```dart
final recordValue = snapshot.child('EC-004').value as String? ?? '';
final parts = recordValue.split(',');

if (parts.length >= 4) {
  final batchNumber = parts[0];
  final timestamp = int.tryParse(parts[1]) ?? 0;
  final temperature = double.tryParse(parts[2]) ?? 0.0;
  final humidity = double.tryParse(parts[3]) ?? 0.0;
}
```

---

## 4. Fetching records from Flutter

### Read the activity value

```dart
final activitySnap = await FirebaseDatabase.instance.ref('/activity').get();
final activityValue = activitySnap.value as int? ?? 0;
```

### Read all records

```dart
final recordsSnap = await FirebaseDatabase.instance.ref('/records').get();
final recordsMap = recordsSnap.value as Map<dynamic, dynamic>? ?? {};

recordsMap.forEach((key, value) {
  final csv = value.toString();
  final parts = csv.split(',');

  if (parts.length >= 4) {
    final batch = parts[0];
    final timestamp = int.tryParse(parts[1]) ?? 0;
    final temp = double.tryParse(parts[2]) ?? 0.0;
    final humidity = double.tryParse(parts[3]) ?? 0.0;

    print('batch=$batch timestamp=$timestamp temp=$temp humidity=$humidity');
  }
});
```

### Read a single batch record

```dart
final recordSnap = await FirebaseDatabase.instance.ref('/records/EC-004').get();
final csv = recordSnap.value?.toString() ?? '';
print(csv);
```

---

## 5. Activity and batch usage in the app

Use the activity value to show current machine state in the UI, and use `/records/EC-XXX` to show historical batch data.

Example:

```dart
final activity = snapshot.child('/activity').value as int? ?? 0;
final description = {
  0: 'Idle',
  1: 'Grinding',
  2: 'Adding Soil',
  3: 'Mixing',
  4: 'Getting Sensor Data',
  5: 'Dispensing',
  6: 'Waiting to Remove Container',
}[activity] ?? 'Unknown';
```

For records, parse the CSV string and display the values in a list or detail screen.

---

## 6. Notes

- Use fixed Firebase paths, not random push IDs.
- Store numeric activity as an integer.
- Store each batch as a single CSV string under `/records/EC-XXX`.
- Keep the batch number padded to three digits, for example `EC-001`, `EC-010`, `EC-020`.
