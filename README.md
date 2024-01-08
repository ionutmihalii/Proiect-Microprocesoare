# Proiect-Microprocesoare

## Descriere

Proiectul propus implică dezvoltarea unui sistem care utilizează un microcontroller pentru monitorizarea unui senzor de rotație și controlul LED-urilor. Interfața grafică pentru acest proiect este realizată în Python, în timp ce manipularea senzorului de rotație se face prin intermediul modulelor C din folderul Keil.

## Structura Director

- **docs**: Conține documentația, imagini și fișierele utile pentru proiect.
- **gui**: Conține codul sursă al interfeței grafice dezvoltate în Python.
- **src**: Conține codul sursă Keil pentru manipularea senzorului de rotație.

## Ghid de Inițializare

### Pasul 1: Configurarea Componentelor Hardware

1. Consultați documentația tehnică din folderul `docs` pentru specificațiile pinilor și porturilor.
2. Realizați conexiunile corespunzătoare pe breadboard pentru senzorul de rotație și LED-uri.
3. Configurați microcontrollerul conform specificațiilor, asigurându-vă că pinii și porturile sunt setate corespunzător.

### Pasul 2: Conectarea și Configurarea Microcontrollerului

1. Conectați microcontrollerul la portul USB al laptopului.
2. Deschideți proiectul Keil, importați fișierele sursă din `src` și efectuați build-ul.
3. Descărcați firmware-ul pe microcontroller folosind Keil.

### Pasul 3: Inițializarea Interfeței Grafice Python
1. Instalați bibliotecile necesare folosind pip:
```bash
pip install PySide6 pyserial pyqtgraph
```
2. Navigați în directorul gui și executați scriptul Python pentru a deschide interfața grafică.
3. În interfața grafică, monitorizați barplot-ul în timp real și utilizați butonul pentru controlul LED-urilor on-board.
