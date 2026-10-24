# Runbook EAuto Bootloader

Handgriffe am Steuergerät: Bootloader aufspielen, Firmware aktualisieren und ein
Steuergerät ohne lauffähige Software wiederherstellen. Wie der Bootloader arbeitet,
steht in der [Architektur](ARCHITECTURE.md), wie er gebaut wird, in der
[README](README.md#bauen-und-flashen).

| Einstellung | Wert |
|:----------- |:---- |
| Serielle Schnittstelle | USART2, PD5 = TX, PD6 = RX, 3,3 V Pegel |
| Terminal | 115200 Baud, 8 Datenbits, keine Parität, 1 Stoppbit, Flusssteuerung keine |
| Protokoll | XModem CRC oder XModem 1K |
| Firmware | `.bin`-Datei der Application, kleiner als 224 kB |

## Bootloader erstmalig programmieren

### Voraussetzungen
- ST-Link an SWD des Steuergeräts
- STM32CubeIDE oder STM32CubeProgrammer
- `Release/EAuto_XBootloader.elf`, gebaut mit dem Make-Ziel der Platine
- USB-UART-Adapter und TeraTerm für die Prüfung

### Schritte
1. ST-Link anschließen und den gesamten Flash löschen:
   `STM32_Programmer_CLI -c port=SWD mode=UR -e all`
2. Bootloader schreiben:
   `STM32_Programmer_CLI -c port=SWD mode=UR -w Release/EAuto_XBootloader.elf -v -rst`
   oder in STM32CubeIDE die Launch-Konfiguration `EAuto_XBootloader Release` starten.
3. USB-UART-Adapter an PD5/PD6 und GND anschließen, TX des Adapters an RX des
   Controllers und umgekehrt.
4. TeraTerm verbinden, *Setup → Serial port* wie in der Tabelle oben.
5. Reset drücken.

### Prüfung
- Willkommensbildschirm mit „EAuto Bootloader“ und Versionsnummer.
- Alle drei LEDs leuchten.
- „Bitte sende eine neue Binaerdatei mit Xmodemprotokoll …“, danach etwa jede
  Sekunde ein `C`.

### Fehlerbilder

| Symptom | Ursache | Abhilfe |
|---------|---------|---------|
| ST-Link verbindet nicht | Reset-Strategie falsch oder Read-out Protection aktiv | Mit *connect under reset* verbinden, RDP auf Level 0 setzen |
| Keine Ausgabe im Terminal | Falscher COM-Port, TX/RX vertauscht oder Bootloader nicht geschrieben | Verkabelung und Port prüfen, Schritt 2 wiederholen |
| Unleserliche Zeichen | Falsche Baudrate | 115200 einstellen |
| „Application vorhanden.“ statt `C` | Flash vor dem Schreiben nicht gelöscht, alte Validation-Bytes | Schritt 1 und 2 wiederholen |

## Firmware aktualisieren

### Voraussetzungen
- USB-UART-Adapter an PD5/PD6 und GND, TeraTerm verbunden wie in der Tabelle oben
- `.bin`-Datei der Application, kleiner als 224 kB. Keine `.hex` oder `.elf`.

### Schritte
1. Steuergerät in den Bootloader bringen:
   - Mit lauffähiger Application: Update in der Application auslösen. Ab hier ist das
     Steuergerät bis zum erfolgreichen Upload ohne lauffähige Software.
   - Ohne gültige Application (leerer Flash, abgebrochenes Update): Reset drücken.
2. Warten, bis im Terminal `C` erscheint.
3. *File → Transfer → XMODEM → Send*, die `.bin` wählen, Option **CRC** oder **1K**,
   nicht **Checksum**.
4. Warten, bis der Dialog sich schließt. Bei 115200 Baud dauert das etwa 10 s pro
   100 kB. Nicht abbrechen.

### Prüfung
- Im Terminal steht „Firmware geupdated!“ und „Springe zu Benutzer Application...“.
- Die Application zeigt ihr erwartetes Verhalten.
- Reset drücken: Der Bootloader gibt „Application vorhanden.“ aus und startet sie
  direkt.

### Fehlerbilder

| Symptom | Ursache | Abhilfe |
|---------|---------|---------|
| Kein `C`, nur „Application vorhanden.“ | Gültige Application vorhanden | Update in der Application auslösen oder [wiederherstellen](#steuergerät-ohne-lauffähige-software-wiederherstellen) |
| Übertragung bricht nach etwa 32 kB ab | Flash hinter dem ersten App-Sektor nicht gelöscht | [Wiederherstellen](#steuergerät-ohne-lauffähige-software-wiederherstellen), dann erneut übertragen |
| Übertragung bricht kurz vor Ende ab | `.bin` ist 224 kB oder größer | Nicht am Gerät lösbar, Entwickler informieren |
| „Fehler... bitte erneut versuchen.“ | Option **Checksum** gewählt oder Störungen auf der Leitung | Option **CRC** wählen, Kabel prüfen, ab Schritt 2 wiederholen |
| Nach „Springe zu Benutzer Application...“ passiert nichts oder HardFault-Ausgabe | Application nicht für 0x08008000 gebaut | Entwickler informieren, siehe [README](README.md#application-für-den-bootloader-bauen) |
| Nach Reset springt der Bootloader, aber nichts läuft | Validation-Bytes stehen noch, Application gelöscht | [Wiederherstellen](#steuergerät-ohne-lauffähige-software-wiederherstellen) |
| HardFault-Ausgabe mit Registern | Fehler in Bootloader oder Application | `CFSR`, `HFSR`, `BFAR` und `PC` notieren und an den Entwickler geben |

## Steuergerät ohne lauffähige Software wiederherstellen

### Voraussetzungen
- ST-Link an SWD des Steuergeräts, STM32CubeProgrammer
- `Release/EAuto_XBootloader.elf` und die `.bin` der Application

### Schritte
1. Gesamten Flash löschen: `STM32_Programmer_CLI -c port=SWD mode=UR -e all`
2. Bootloader schreiben wie in
   [Bootloader erstmalig programmieren](#bootloader-erstmalig-programmieren), Schritt 2.
3. Application übertragen wie in [Firmware aktualisieren](#firmware-aktualisieren)
   ab Schritt 2.

### Prüfung
- Wie in [Firmware aktualisieren](#firmware-aktualisieren).

### Fehlerbilder

| Symptom | Ursache | Abhilfe |
|---------|---------|---------|
| ST-Link verbindet nicht | Read-out Protection aktiv oder SWD nicht erreichbar | RDP auf Level 0 setzen, Verbindung prüfen. Hilft das nicht, Entwickler informieren |
| Nach Schritt 2 keine Ausgabe | Siehe [Bootloader erstmalig programmieren](#fehlerbilder) | Dort beschriebene Abhilfe |

## Bootloader aktualisieren, Application behalten

### Voraussetzungen
- ST-Link an SWD des Steuergeräts, STM32CubeProgrammer
- Neue `Release/EAuto_XBootloader.elf`

### Schritte
1. Nur Sektor 0 löschen und schreiben:
   `STM32_Programmer_CLI -c port=SWD mode=UR -e 0 -w Release/EAuto_XBootloader.elf -v -rst`

### Prüfung
- Willkommensbildschirm mit der neuen Versionsnummer, danach „Application vorhanden.“
  und Start der Application.

### Fehlerbilder

| Symptom | Ursache | Abhilfe |
|---------|---------|---------|
| Bootloader wartet auf Upload statt zu springen | Mehr als Sektor 0 gelöscht | Application übertragen wie in [Firmware aktualisieren](#firmware-aktualisieren) |
