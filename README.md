# EAuto Bootloader

Der Bootloader basiert auf dem Xmodem Protokoll. Er dient dazu um die Steuergeräte
auch nach dem Einbau ins Fahrzeug zu programmieren ohnen dafür ständig einen
Debugger oder Programmer anzuschließen.

Eine Application wird an Speicheradresse 0x8008000 geschrieben. Danach wird die
Application automatisch gestartet. Ebenfalls wird bei jedem Start des
Mikrocontrollers geprüft ob eine Application vorhanden ist. Sollte eine Application
verfügbar sein, so wird diese direkt gestartet. Ein überschreiben ist erst mit
einer entsprechenden Tastenkombination am Steuergerät möglich.