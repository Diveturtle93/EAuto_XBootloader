# EAuto Bootloader

Der Bootloader basiert auf dem XModem-Protokoll. Er dient dazu, die Steuergeräte
auch nach dem Einbau ins Fahrzeug zu programmieren, ohne dafür ständig einen
Debugger oder Programmer anzuschließen. Der ganze Programmiervorgang läuft über
die serielle Schnittstelle und muss aus der Application heraus gestartet werden.

## Beschreibung

Der Flash ist in zwei Bereiche aufgeteilt. Im ersten Sektor liegt der Bootloader,
der ganze restliche Flash gehört der Application. Wie die Teile zusammenspielen,
steht in der [Architektur](ARCHITECTURE.md).

| Flash | Startadresse | Endadresse | Speicher | Sektoren |
|:----- |:------------ |:---------- |:-------- |:-------- |
| Bootloader | 0x08000000 | 0x08007FFF | 32kB | 0 |
| Application | 0x08008000 | 0x081FFFFF | 2016kB | 1–11 |

Bei jedem Start des Mikrocontrollers prüft der Bootloader anhand der
Validation-Bytes, ob eine Application vorhanden ist, und startet sie direkt.
Ist keine gültige Application vorhanden, wartet er auf eine neue Firmware per
XModem, schreibt sie ab 0x08008000 und startet sie danach automatisch.

Ein Überschreiben ist erst mit dem Aufruf des Bootloaders aus der Application
möglich. Durch den Aufruf werden die Validation-Bytes zurückgesetzt. Damit ist es
dem Bootloader nicht mehr möglich, die Application zu starten, auch nicht nach
Abschalten des Steuergerätes. Das Steuergerät muss dann zwangsweise neu mit einer
Software beschrieben werden.

## Hardware

| Eigenschaft | Wert |
|:----------- |:---- |
| Mikrocontroller | STM32F767ZIT, 2 MB Flash, 512 kB RAM |
| Takt | HSI, 16 MHz |
| Platinen | Motorsteuergerät Rev 1.0, Batteriemanagement-System Rev 1.0 |

Beide Platinen haben dieselbe Pinbelegung für den Bootloader:

| Funktion | Pin |
|:-------- |:--- |
| LED rot | PB2 |
| LED grün | PB14 |
| LED blau | PB7 |
| USART2 TX | PD5 |
| USART2 RX | PD6 |

## Bauen und Flashen

Das Projekt wird mit STM32CubeIDE gebaut (GCC 11 oder neuer, das Linker-Skript
nutzt `READONLY`), die HAL stammt aus dem Paket STM32Cube FW_F7 V1.17.4. Die
Bibliotheken liegen als Submodule in `Lib/` und werden beim Klonen mitgeholt:

```sh
git clone --recursive https://github.com/Diveturtle93/EAuto_Bootloader.git
git submodule update --init          # in einem bereits geklonten Repository
```

### Bootloader bauen

Die Platine wird beim Bauen über ein Symbol ausgewählt. Dafür gibt es in der
STM32CubeIDE je Platine ein Make-Ziel:

| Make-Ziel | Symbol | Platine |
|:--------- |:------ |:------- |
| `Motor Rev 1.0` | `MOTOR=1` | Motorsteuergerät Rev 1.0 |
| `BMS Rev 1.0` | `BMS=1` | Batteriemanagement-System Rev 1.0 |

| Build-Konfiguration | Zweck | Ergebnis |
|:------------------- |:----- |:-------- |
| Debug | Entwicklung, setzt zusätzlich `EAUTO` und verwendet die Pinbelegung aus CubeMX. Nicht auf eine Platine im Fahrzeug spielen | `Debug/EAuto_XBootloader.elf` |
| Release | Build für die Platine, nur zusammen mit einem Make-Ziel | `Release/EAuto_XBootloader.elf` |

Nach dem Build zeigt die Ausgabe genau eine der Warnungen „PCB Motorsteuergeraet
ausgewaehlt“ oder „PCB Batteriemanagement-System ausgewaehlt“. Ist keine Platine
gewählt, warnt der Build, sind beide gewählt, bricht er ab. Der Bootloader muss in
32 kB passen, sonst meldet der Linker einen Überlauf von `FLASH`.

### Application für den Bootloader bauen

Eine Application, die der Bootloader starten soll, braucht drei Einstellungen:

1. **Linker-Skript:** `FLASH (rx) : ORIGIN = 0x8008000, LENGTH = 224K`. Die
   Länge ist durch den Bootloader begrenzt, siehe [Hinweise](#hinweise). Die
   letzten 8 Byte gehören den Validation-Bytes, die `.bin` muss also kleiner als
   224 kB sein.
2. **Vektortabelle:** In `system_stm32f7xx.c` `USER_VECT_TAB_ADDRESS` aktivieren
   und `VECT_TAB_OFFSET` auf `0x00008000U` setzen.
3. **Binärdatei:** Unter *MCU Post build outputs* „Convert to binary file
   (-O binary)“ aktivieren. Übertragen wird die `.bin`.

### Flashen

Der Bootloader wird einmalig mit einem ST-Link über SWD aufgespielt, mit den
Launch-Dateien `EAuto_XBootloader Debug.launch` bzw.
`EAuto_XBootloader Release.launch`. Danach werden Applications über die
serielle Schnittstelle übertragen. Die Handgriffe am Steuergerät stehen im
[Runbook](RUNBOOK.md).

## Schnittstellen

| Schnittstelle | Einstellung | Verwendung |
|:------------- |:----------- |:---------- |
| USART2 | 115200 Baud, 8N1, kein Handshake | Übertragung der Application per XModem CRC oder 1K, Ausgaben im Terminal |
| SWD | ST-Link | Aufspielen des Bootloaders, Debuggen |

Die Ausgaben im Terminal verwenden ANSI-Escape-Sequenzen für Farben, das Terminal
muss sie unterstützen (z. B. TeraTerm).

## Hinweise

- Die Flash-Adressen, mit denen der Bootloader arbeitet, stehen in
  `Core/Inc/main.h`. `FLASH_APP_END_ADDRESS` ist `0x0803FFFF` und die
  Validation-Bytes liegen bei `0x0803FFF8`. Eine Application muss deshalb trotz
  2016 kB im Linker-Skript derzeit kleiner als 224 kB sein. Details unter
  [Offene Punkte](ARCHITECTURE.md#offene-punkte).
- `main.c` bindet `BasicUart.h` ein, die Datei heißt `basicuart.h`. Unter Windows
  baut das Projekt, auf Linux/macOS oder in CI nicht.
- Fehlen beim Bauen Header aus `Lib/`, sind die Submodule nicht ausgecheckt.
- `RED_LED_Pin` nicht definiert: Release wurde ohne Make-Ziel gebaut.

## Abhängigkeiten

| Bibliothek | Tag | Zweck |
|---|---|---|
| [STM32_BootloaderXModem](https://github.com/Diveturtle93/STM32_BootloaderXModem) | – | XModem-Empfang, Flash schreiben, Sprung in die Application |
| [STM32_Basicuart](https://github.com/Diveturtle93/STM32_Basicuart) | – | Ausgaben über UART |
| [STM32_Hardfault](https://github.com/Diveturtle93/STM32_Hardfault) | – | `HardFault_Handler` mit Ausgabe der Fault-Register über UART |

Die Bibliotheken haben noch keinen Tag. Eingebunden ist jeweils der Stand des
Submoduls.

## Lizenz

Dieses Projekt steht unter der [GPL-3.0 Lizenz](LICENSE).
