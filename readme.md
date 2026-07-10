# MegaMatrixText

Arduino library for displaying characters, text and simple animations on an 8×8 LED matrix controlled by a MAX7219.

The library was developed and tested using an Elegoo Mega 2560 compatible board and the `LedControl` library.

It supports uppercase and lowercase letters, numbers, common symbols, blocking and non-blocking text scrolling, inverted display mode and font storage in Flash memory using `PROGMEM`.

## Features

* Uppercase letters `A-Z`
* Lowercase letters `a-z`
* Numbers `0-9`
* Common punctuation and symbols
* Single-character display
* Text displayed one character at a time
* Blocking text scrolling
* Non-blocking text scrolling based on `millis()`
* Normal and inverted display modes
* Adjustable brightness
* Fonts stored in Flash memory using `PROGMEM`
* Custom 8×8 bitmap support inside the library
* Designed for MAX7219-controlled 8×8 LED matrices

## Requirements

* Arduino-compatible AVR board
* MAX7219 8×8 LED matrix
* `LedControl` library

The `LedControl` library must be installed before compiling a sketch that uses MegaMatrixText.

## Hardware connections

The default examples use the following connections:

| MAX7219 pin | Arduino Mega pin |
| ----------- | ---------------: |
| DIN         |               12 |
| CLK         |               10 |
| CS / LOAD   |               11 |
| VCC         |               5V |
| GND         |              GND |

The pins can be changed when creating the `MegaMatrixText` object.

```cpp
MegaMatrixText matrix(dataPin, clockPin, csPin);
```

Example:

```cpp
MegaMatrixText matrix(12, 10, 11);
```

## Installation

### Manual installation

1. Download or clone this repository.
2. Copy the `MegaMatrixText` folder into the Arduino libraries directory.

On Windows, the directory is normally:

```text
Documents\Arduino\libraries
```

The resulting structure should be:

```text
Documents
└── Arduino
    └── libraries
        └── MegaMatrixText
            ├── MegaMatrixText.cpp
            ├── MegaMatrixText.h
            ├── Font5x7.h
            ├── library.properties
            ├── keywords.txt
            └── examples
```

3. Restart the Arduino IDE.
4. Open one of the included examples.
5. Select the correct board and serial port.
6. Compile and upload the sketch.

## Basic example

```cpp
#include <MegaMatrixText.h>

MegaMatrixText matrix(12, 10, 11);

void setup()
{
  matrix.begin();
  matrix.setBrightness(8);
}

void loop()
{
  matrix.showChar('A');
  delay(1000);

  matrix.showTextOneByOne(
    "Arduino",
    500
  );

  delay(1000);

  matrix.scrollText(
    "Ciao Utente!",
    80
  );

  delay(1000);
}
```

## Non-blocking scrolling

The non-blocking scrolling system allows the Arduino to continue executing other tasks while the text moves across the display.

Start the scrolling operation once:

```cpp
matrix.startScroll(
  "Non-blocking text",
  80
);
```

Then call `update()` continuously inside `loop()`:

```cpp
void loop()
{
  matrix.update();

  // Other code can run here.
}
```

Complete example:

```cpp
#include <MegaMatrixText.h>

MegaMatrixText matrix(12, 10, 11);

unsigned long lastBlinkTime = 0;
bool ledState = false;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  matrix.begin();
  matrix.setBrightness(5);

  matrix.startScroll(
    "Non-blocking scroll",
    80
  );
}

void loop()
{
  matrix.update();

  if (millis() - lastBlinkTime >= 500)
  {
    lastBlinkTime = millis();

    ledState = !ledState;

    digitalWrite(
      LED_BUILTIN,
      ledState
    );
  }

  if (!matrix.isScrolling())
  {
    matrix.startScroll(
      "Non-blocking scroll",
      80
    );
  }
}
```

## Inverted display mode

The inverted mode reverses every LED state:

* lit pixels become unlit;
* unlit pixels become lit.

Enable inverted mode:

```cpp
matrix.setInverted(true);
```

Return to normal mode:

```cpp
matrix.setInverted(false);
```

Example:

```cpp
matrix.setInverted(false);
matrix.showChar('A');

delay(1000);

matrix.setInverted(true);
matrix.showChar('A');
```

When inverted mode is enabled, most LEDs may remain lit. A moderate brightness value is recommended.

```cpp
matrix.setBrightness(4);
```

## Public methods

### Initialization

```cpp
void begin();
```

Initializes the MAX7219, enables the display, sets the default brightness and clears the matrix.

### Clear display

```cpp
void clear();
```

Turns off every LED on the matrix.

### Set brightness

```cpp
void setBrightness(byte brightness);
```

Sets the MAX7219 brightness.

Accepted values:

```text
0  = minimum brightness
15 = maximum brightness
```

Values greater than `15` are automatically limited to `15`.

### Show one character

```cpp
void showChar(char character);
```

Displays a single supported character.

Example:

```cpp
matrix.showChar('M');
```

### Show text one character at a time

```cpp
void showTextOneByOne(
  const char *text,
  unsigned long characterTime
);
```

Displays every character separately.

Example:

```cpp
matrix.showTextOneByOne(
  "Arduino",
  500
);
```

### Blocking text scrolling

```cpp
void scrollText(
  const char *text,
  unsigned long scrollDelay
);
```

Scrolls a text string from right to left.

This method is blocking because it uses `delay()` internally.

Example:

```cpp
matrix.scrollText(
  "Hello world!",
  80
);
```

### Start non-blocking scrolling

```cpp
void startScroll(
  const char *text,
  unsigned long scrollDelay
);
```

Starts a non-blocking scrolling operation.

The supplied text must remain valid until the scrolling operation finishes.

### Update non-blocking scrolling

```cpp
void update();
```

Advances the scrolling animation when the required time has elapsed.

This method must be called continuously inside `loop()`.

### Check scrolling state

```cpp
bool isScrolling();
```

Returns:

```text
true  = scrolling is active
false = scrolling has finished or was stopped
```

### Stop scrolling

```cpp
void stopScroll();
```

Stops the current non-blocking scrolling operation and clears the matrix.

### Enable or disable inverted mode

```cpp
void setInverted(bool inverted);
```

Example:

```cpp
matrix.setInverted(true);
```

### Read inverted mode state

```cpp
bool isInverted();
```

Returns `true` when inverted mode is enabled.

## Supported characters

The included font supports:

```text
A-Z
a-z
0-9
```

It also includes common symbols such as:

```text
. , : ; ! ? - + / \ = % ( ) [ ] ' " _ # @ & * < >
```

Unsupported characters are displayed as blank spaces.

The library currently uses single-byte ASCII characters. UTF-8 accented characters such as `è`, `à`, `ò` or `°` require dedicated handling and are not currently included.

## Font storage

The character font is stored in Flash memory using `PROGMEM`.

This reduces SRAM usage, which is important on AVR boards with limited working memory.

Font data is read using:

```cpp
pgm_read_byte()
```

## Display orientation

The library currently applies a horizontal column correction designed for the matrix used during development.

The physical column is selected using:

```cpp
7 - displayColumn
```

Some MAX7219 matrix modules may use a different physical orientation.

If text appears mirrored, rotated or upside down, the column or row mapping may need to be adapted inside the library.

## Library structure

```text
MegaMatrixText
├── MegaMatrixText.cpp
├── MegaMatrixText.h
├── Font5x7.h
├── library.properties
├── keywords.txt
├── README.md
├── LICENSE
└── examples
    ├── BasicExample
    │   └── BasicExample.ino
    └── NonBlockingScroll
        └── NonBlockingScroll.ino
```

## Compatibility

The library currently targets AVR-based Arduino boards because it uses:

```cpp
#include <avr/pgmspace.h>
```

It was developed and tested with an Elegoo Mega 2560 compatible board.

Other architectures may require changes to the Flash-memory handling.

## Current limitations

* One MAX7219 8×8 matrix is currently supported.
* Text scrolls from right to left.
* Character width is fixed at 5 pixels plus one spacing column.
* The display orientation correction is currently fixed.
* Accented and general UTF-8 characters are not currently supported.
* Blocking methods use `delay()` and temporarily prevent other sketch operations.

## Planned improvements

Possible future improvements include:

* configurable display rotation;
* configurable horizontal mirroring;
* configurable character spacing;
* support for multiple chained MAX7219 matrices;
* custom bitmap drawing;
* additional icons;
* accented characters;
* pause and resume for non-blocking scrolling;
* automatic repeating messages.

## Version

Current version:

```text
1.0.0
```

## Author

Fraido

## License

This project is released under the MIT License.

See the `LICENSE` file for details.
