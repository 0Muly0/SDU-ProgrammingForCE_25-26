// =========================================================================
// File: main.cpp
// Purpose: Program entry point for PicoBricks sensor application
// Author: Tommy Nielsen
// Date: 2025-10-25
// =========================================================================

#include "pico/stdlib.h"                     // Pico SDK base functions
#include "MyApp.h"                           // Application class
#include "dbop.h"                            // Debug macros (optional)

int main() {
    stdio_init_all();                        // Initialize USB serial and stdio

    MyApp app;                               // Create application object
    app.run();                               // Start main application loop

    return 0;                                // Program never reaches this point
}

// PlantUML
/*
@startuml
title Exercise 8A–8D – PicoBricks Middleware Architecture

skinparam classAttributeIconSize 0
skinparam shadowing false
skinparam linetype ortho
skinparam dpi 150
skinparam packageStyle rectangle
skinparam classFontSize 13

' ------------------------------------------------------------
' Packages to group conceptual layers (top-down architecture)
' ------------------------------------------------------------
package "Application Layer" #DDFFDD {
    class MyApp {
        +MyApp()
        ---
        "Handles sensor reading and console output"
    }
}

package "Middleware Layer (PicoBricks-specific)" #FFF3CC {
    class KeyPress {
        +isPressed() : bool
    }
    class Pot {
        +readPercent() : float
    }
    class LightSensorLDR {
        +readLux() : float
    }
    class DieTemp {
        +readCelsius() : float
        +readFahrenheit() : float
    }
}

package "Hardware Abstraction Layer (HAL)" #CCE5FF {
    class Button {
        -_pin : uint
        -_edge : uint
        +hasEvent() : bool
        +isPressed() : bool
    }

    class ADC {
        -_pin : uint
        -_channel : uint
        +read() : int
        +readAvg(avgVal:int) : int
    }
}

' ------------------------------------------------------------
' Relationships between classes
' ------------------------------------------------------------
MyApp --> KeyPress : uses
MyApp --> Pot : uses
MyApp --> LightSensorLDR : uses
MyApp --> DieTemp : uses

KeyPress --|> Button
Pot --|> ADC
LightSensorLDR --|> ADC
DieTemp --|> ADC

' ------------------------------------------------------------
' Notes (all syntactically valid)
' ------------------------------------------------------------
'note top of Button
'Generic Button class (HAL layer)
'end note

'note bottom of ADC
'Generic ADC interface (HAL layer)
'end note

'note left of KeyPress
'PicoBricks push button (GPIO10)
'end note

'note right of Pot
'Potentiometer (GPIO26 → ADC0)
'end note

'note right of LightSensorLDR
'Light sensor (GPIO27 → ADC1)
'end note

'note right of DieTemp
'Internal temperature sensor (GPIO29 → ADC4)
'end note

' ------------------------------------------------------------
' Free-standing note in the middle of the diagram
' ------------------------------------------------------------
'note as MW_NOTE
'Middleware classes abstract hardware details from the application
'end note

'MyApp .. MW_NOTE
'MW_NOTE .. ADC

' ------------------------------------------------------------
' Conceptual view with corrected order
' ------------------------------------------------------------
package "Conceptual View" {
    note "Application Layer (MyApp)" as APP_note
    note "Middleware Layer (PicoBricks)" as MW_note2
    note "Hardware Abstraction Layer (HAL)" as HAL_note
}

APP_note -[hidden]-> MW_note2
MW_note2 -[hidden]-> HAL_note

@enduml
*/