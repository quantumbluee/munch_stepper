# Munch Stepper Motor Control

Submission for Munch's Technical Challenge. A Zephyr RTOS app for controlling a stepper motor using the NUCLEO-F767ZI micrcontroller and TMC2208 stepper driver. The application moves a stepper motor through four specific angles (30°, 45°, 90°, 180°) with configurable microstepping.


## Wiring Diagram

### NUCLEO-F767ZI to TMC2208 Connections
```
NUCLEO Pin    →    TMC2208 Pin    Function
------------------------------------------------
D3 (PB3)      →    STEP           Step pulse signal
D4 (PB5)      →    DIR            Direction control
D5 (PB4)      →    EN             Enable (optional)
3.3V          →    VIO            Logic voltage 
GND           →    GND            Common ground
```

### Power Supply to TMC2208
```
Power Supply  →    TMC2208 Pin    Function
------------------------------------------------
+ (12-24V)    →    VM             Motor power
-             →    GND            Ground (shared with NUCLEO)
```

### TMC2208 to Stepper Motor  
```Motor Wire    →    TMC2208 Pin
---------------------------------
BLK (A+)      →    A1  (not A2!)
GRN (A-)      →    A2
RED (B+)      →    B1  (not A2!)
BLU (B-)      →    B2
```