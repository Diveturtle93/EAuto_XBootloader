# Changelog

Alle nennenswerten Änderungen an diesem Projekt werden hier dokumentiert.
Format nach [Keep a Changelog](https://keepachangelog.com/de/1.1.0/),
Versionierung nach [Semantic Versioning](https://semver.org/lang/de/).

## [Unveröffentlicht]

### Hinzugefügt
- Bootloader für den STM32F767ZIT, der eine neue Application per XModem über USART2 mit 115200 Baud empfängt und in den Flash schreibt.
- Beim Start prüft der Bootloader die Validation-Bytes und springt direkt in eine vorhandene Application.
- Mit „a“ oder „A“ im Terminal springt der Bootloader in eine vorhandene, gültige Application. Nach einem Fehler wartet er auf einen neuen Versuch.
- Willkommensbildschirm im Terminal mit Versionsnummer.
- Flash-Aufteilung: Bootloader in Sektor 0 (32 kB), Application ab 0x08008000 im restlichen Flash.
- Pinbelegung der Status-LEDs für Motorsteuergerät und BMS in Revision 1.0, ausgewählt über das Build-Symbol `MOTOR` bzw. `BMS`. Ist keine oder sind beide Platinen gewählt, warnt bzw. bricht der Build ab.
- Build-Konfigurationen Debug (Entwicklungsboard) und Release (Platine) mit passenden Launch-Dateien.
- Versionsnummer des Bootloaders in `version.h`.
- Bibliotheken STM32_BootloaderXModem, STM32_Basicuart und STM32_Hardfault als Submodule.
- README mit Flash-Aufteilung, Hardware, Bauen und Flashen, Einstellungen für die Application, Schnittstellen, Hinweisen und Abhängigkeiten, Lizenz GPL-3.0.
- Architekturbeschreibung in `ARCHITECTURE.md` mit Komponenten, Datenfluss bei Start und Update, Speicher und Ressourcen, offenen Punkten, Nebenläufigkeit und Erweiterung.
- Runbook in `RUNBOOK.md` für Erstprogrammierung, Firmware-Update per XModem, Wiederherstellung und Aktualisieren des Bootloaders, je mit Prüfung und Fehlerbildern.
