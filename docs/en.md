# Development guide

Windows first. Before writing flash, identify the COM port and flash size, read the entire factory flash twice and compare SHA-256 hashes. Keep backups outside the repository. Test display, touch, Wi-Fi, RTC, battery and SHT30 individually before enabling OTA. Releases must pass hardware acceptance tests.

Architecture: independent data providers, shared data model, e-paper renderer, persistent settings and power manager. Use explicit stale-data indicators. Network tests use three ICMP attempts.
