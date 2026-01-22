# Dostępne kontrolki WinAPI (przykład)

Prosta aplikacja okienkowa Windows (`.exe`) pokazująca przykładowe standardowe kontrolki WinAPI / common controls:

- checkboxy, radio buttony
- pole edycji (single-line i multiline)
- combobox
- suwak (trackbar/slider)
- progressbar
- listbox
- listview (tryb raportu)
- treeview
- SysLink, DateTimePicker

Kontrolki są standardowe (bez niestandardowego rysowania), mają kolejność TAB i etykiety z mnemotechnikami (`ALT+litera`), dzięki czemu są czytelne dla czytników ekranu.

## Budowanie `.exe` w WSL/Linux (Docker)

Wymagania: działający Docker.

```bash
./scripts/build-win-docker.sh
```

Wynik: `build/DostepneKontrolki.exe` (bez dodatkowych DLL z MinGW — runtime jest linkowany statycznie).

## Budowanie na Linux/WSL (MinGW-w64 lokalnie)

Jeśli masz zainstalowane `mingw-w64` (kompilator `x86_64-w64-mingw32-g++` i `windres`):

```bash
./scripts/build-win-mingw.sh
```

## Budowanie na Windows (MSVC/Visual Studio)

1. Otwórz „x64 Native Tools Command Prompt for VS”.
2. Z katalogu projektu uruchom:

```bat
rc /nologo /fo build\\app.res resources\\app.rc
cl /nologo /EHsc /std:c++17 /O2 /DUNICODE /D_UNICODE src\\main.cpp build\\app.res user32.lib gdi32.lib comctl32.lib uxtheme.lib /link /SUBSYSTEM:WINDOWS /OUT:build\\DostepneKontrolki.exe
```
