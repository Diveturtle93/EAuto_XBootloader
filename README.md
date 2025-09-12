# EAuto Bootloader

Der Bootloader basiert auf dem Xmodem Protokoll. Er dient dazu um die Steuergeräte
auch nach dem Einbau ins Fahrzeug zu programmieren ohnen dafür ständig einen
Debugger oder Programmer anzuschließen. Der ganze Programmiervorgang lauft über
die serielle Schnittstelle und muss aus der Application heraus gestartet werden.

## Flash

Der Flash ist in mehrere Abschnitte für aufgeteilt. Dabei ist zu am Anfang des
Flashes der eigentliche Bereich für den Bootloader. Direkt dahinter liegt ein
Abschnitt für die Möglichkeit geteilten Code abzulegen. Als nächstes folgt der
Speicherbereich für die Konfiguration. Zum Schluss bleibt der Rest des Flashes
für die Application. Dies muss dann im Programmcode der Application mit
angegeben werden. Die folgende Tabelle gibt einen überblick über die Flash
Aufteilung:

| Flash | Startadresse | Endadresse | Speicher |
|:----- |:------------ |:---------- |:-------- |
| Bootloader | 0x08000000 | 0x08007FFF | 32kB |
| Shared Api | 0x08008000 | 0x0800FFFF | 32kB |
| Config | 0x08010000 | 0x0801FFFF | 32kB |
| Application | 0x08018000 | 0x081FFFFF | 1952kB |

### Application

Die Application wird an Speicheradresse 0x8018000 geschrieben. Danach wird die
Application automatisch gestartet. Ebenfalls wird bei jedem Start des
Mikrocontrollers geprüft ob eine Application vorhanden ist. Sollte eine Application
verfügbar sein, so wird diese direkt gestartet. Ein überschreiben ist erst aus mit
dem Aufruf des Bootloaders aus der Application möglich.

Durch den Aufruf werden die Validation-Bytes zurueckgesetzt. Damit ist dem
Bootloader nicht mehr möglich die Application zu starten. Ein erneutes Starten
der Application nach abschalten des Steuergerätes ist dann ebenfalls nicht mehr
durchführbar. Es muss dann zwangsweise das Steuergerät neu mit einer Software
beschrieben werden.

### Config

In diesem Flash-Abschnitt besteht die Möglichkeit eigene Konfigurationen zu
Speichern. Diese können dann vom Bootloader oder der Application aus aufgerufen
und genutzt werden.

### Shared API