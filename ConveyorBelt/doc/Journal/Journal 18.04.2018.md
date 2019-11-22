# Protokoll 18.04.2018

## Anwesende:
- Luis
- Johannes
- Malte

## Neue Absprachen:
- Coding Style siehe Wiki
- Kommentare werden in Englisch geschrieben
- Funktionsnamen werden in Englisch gehalten
- Nach Möglichkeit das Single Responsibility Principle Pattern einhalten

- ISR ist start gekoppelt an den Controller, jedoch nicht mit HAL gekoppelt
- Dispatcher als Broker zwischen Logiken der einzelne Hardware Komponenten und dem Controller, der Dispatcher dient auch als Glied zwischen Controller und Anwendungslogik
- Timer kommuniziert auch mit Dispatcher
	- Es muss geklärt werden inwiefern der Timer Daten liefert
