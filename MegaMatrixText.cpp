#include "MegaMatrixText.h"
#include "Font5x7.h"


/*
  Costruttore della classe.

  La parte dopo i due punti inizializza
  l'oggetto LedControl interno.
*/
MegaMatrixText::MegaMatrixText(
  int dataPin,
  int clockPin,
  int csPin
)
  : _ledControl(dataPin, clockPin, csPin, 1),
    _scrollText(nullptr),
    _scrollTextLength(0),
    _scrollOffset(0),
    _totalScrollSteps(0),
    _scrollDelay(0),
    _lastScrollUpdate(0),
    _scrolling(false),
	_inverted(false)
{
}


/*
  Inizializza il MAX7219.
*/
void MegaMatrixText::begin()
{
  _ledControl.shutdown(0, false);
  _ledControl.setIntensity(0, 8);
  _ledControl.clearDisplay(0);
}


/*
  Spegne tutti i LED.
*/
void MegaMatrixText::clear()
{
  _ledControl.clearDisplay(0);
}


/*
  Imposta la luminosità.

  Il MAX7219 accetta valori da 0 a 15.
*/
void MegaMatrixText::setBrightness(byte brightness)
{
  if (brightness > 15)
  {
    brightness = 15;
  }

  _ledControl.setIntensity(0, brightness);
}


/*
  Restituisce l'indice del carattere nel font.
*/



/*
  Mostra un singolo carattere.
*/
void MegaMatrixText::showChar(char character)
{
  for (int displayColumn = 0;
       displayColumn < 8;
       displayColumn++)
  {
    byte columnData = B00000000;

    /*
      Il carattere occupa le colonne logiche da 1 a 5.
    */
    if (displayColumn >= 1 &&
        displayColumn <= 5)
    {
      int glyphColumn =
        displayColumn - 1;

      columnData =
        getGlyphColumn(
          character,
          glyphColumn
        );
    }

    columnData =
      applyDisplayEffects(columnData);

    _ledControl.setColumn(
      0,
      7 - displayColumn,
      columnData
    );
  }
}


/*
  Mostra un testo un carattere alla volta.
*/
void MegaMatrixText::showTextOneByOne(
  const char *text,
  unsigned long characterTime
)
{
  int position = 0;

  while (text[position] != '\0')
  {
    showChar(text[position]);

    delay(characterTime);

    position++;
  }

  clear();
}


/*
  Restituisce una singola colonna
  appartenente al testo.
*/
byte MegaMatrixText::getTextColumn(
  const char *text,
  int textLength,
  int textColumn
)
{
  const int characterWidth = 5;
  const int characterSpacing = 1;
  const int totalCharacterWidth =
    characterWidth + characterSpacing;

  /*
    Colonna precedente all'inizio del testo.
  */
  if (textColumn < 0)
  {
    return B00000000;
  }

  /*
    Calcola quale carattere contiene
    la colonna richiesta.
  */
  int characterPosition =
    textColumn / totalCharacterWidth;

  /*
    Colonna successiva alla fine del testo.
  */
  if (characterPosition >= textLength)
  {
    return B00000000;
  }

  /*
    Calcola la colonna interna
    al singolo carattere.
  */
  int columnInsideCharacter =
    textColumn % totalCharacterWidth;

  /*
    Colonna vuota tra i caratteri.
  */
  if (columnInsideCharacter >= characterWidth)
  {
    return B00000000;
  }

  char character =
  text[characterPosition];

return getGlyphColumn(
  character,
  columnInsideCharacter
);
}


/*
  Fa scorrere il testo da destra verso sinistra.
*/
void MegaMatrixText::scrollText(
  const char *text,
  unsigned long scrollDelay
)
{
  const int characterWidth = 5;
  const int characterSpacing = 1;
  const int totalCharacterWidth =
    characterWidth + characterSpacing;

  int textLength = 0;

  /*
    Calcola la lunghezza del testo.
  */
  while (text[textLength] != '\0')
  {
    textLength++;
  }

  int totalTextColumns =
    textLength * totalCharacterWidth;

  /*
    Otto colonne iniziali e otto finali
    permettono al testo di entrare e uscire
    completamente dalla matrice.
  */
  int totalScrollSteps =
    totalTextColumns + 16;

  for (
    int offset = 0;
    offset < totalScrollSteps;
    offset++
  )
  {
    for (
      int displayColumn = 0;
      displayColumn < 8;
      displayColumn++
    )
    {
      int textColumn =
        offset + displayColumn - 8;

      byte columnData = getTextColumn(
        text,
        textLength,
        textColumn
      );
	  
	  columnData =
		applyDisplayEffects(columnData);

      /*
        Correzione orizzontale
        specifica per la tua matrice.
      */
      _ledControl.setColumn(
        0,
        7 - displayColumn,
        columnData
      );
    }

    delay(scrollDelay);
  }

  clear();
}

void MegaMatrixText::startScroll(
  const char *text,
  unsigned long scrollDelay
)
{
  /*
    Se il puntatore non contiene
    un testo valido, interrompiamo.
  */
  if (text == nullptr)
  {
    stopScroll();
    return;
  }

  /*
    Memorizziamo il testo ricevuto.
  */
  _scrollText = text;

  /*
    Memorizziamo la velocità.
  */
  _scrollDelay = scrollDelay;

  /*
    Calcoliamo la lunghezza del testo.
  */
  _scrollTextLength = 0;

  while (_scrollText[_scrollTextLength] != '\0')
  {
    _scrollTextLength++;
  }

  /*
    Ogni carattere occupa:
    5 colonne + 1 spazio.
  */
  const int totalCharacterWidth = 6;

  int totalTextColumns =
    _scrollTextLength * totalCharacterWidth;

  /*
    Aggiungiamo 8 colonne prima
    e 8 colonne dopo.
  */
  _totalScrollSteps =
    totalTextColumns + 16;

  /*
    Partiamo dalla prima posizione.
  */
  _scrollOffset = 0;

  /*
    Permette al primo aggiornamento
    di essere eseguito subito.
  */
  _lastScrollUpdate =
    millis() - _scrollDelay;

  /*
    Segnaliamo che lo scorrimento è attivo.
  */
  _scrolling = true;

  clear();
}

void MegaMatrixText::update()
{
  /*
    Se non c'è uno scorrimento attivo,
    non dobbiamo fare nulla.
  */
  if (_scrolling == false)
  {
    return;
  }

  /*
    Controlliamo se è trascorso
    il tempo necessario.
  */
  if (millis() - _lastScrollUpdate < _scrollDelay)
  {
    return;
  }

  /*
    Memorizziamo il momento
    dell'aggiornamento corrente.
  */
  _lastScrollUpdate = millis();

  /*
    Disegniamo le 8 colonne visibili.
  */
  for (int displayColumn = 0;
       displayColumn < 8;
       displayColumn++)
  {
    int textColumn =
      _scrollOffset + displayColumn - 8;

    byte columnData = getTextColumn(
      _scrollText,
      _scrollTextLength,
      textColumn
    );
	
	columnData =
		applyDisplayEffects(columnData);

    /*
      Correzione per la tua matrice
      specchiata orizzontalmente.
    */
    _ledControl.setColumn(
      0,
      7 - displayColumn,
      columnData
    );
  }

  /*
    Spostiamo il testo di una colonna.
  */
  _scrollOffset++;

  /*
    Se abbiamo terminato tutti i passaggi,
    fermiamo lo scorrimento.
  */
  if (_scrollOffset >= _totalScrollSteps)
  {
    stopScroll();
  }
}

bool MegaMatrixText::isScrolling()
{
  return _scrolling;
}

void MegaMatrixText::stopScroll()
{
  _scrolling = false;
  _scrollText = nullptr;
  _scrollTextLength = 0;
  _scrollOffset = 0;
  _totalScrollSteps = 0;

  clear();
}

byte MegaMatrixText::getGlyphColumn(
  char character,
  byte column
)
{
  /*
    Ogni carattere contiene cinque colonne.
  */
  if (column >= 5)
  {
    return B00000000;
  }

  /*
    Spazio.
  */
  if (character == ' ')
  {
    return B00000000;
  }

  /*
    Lettere maiuscole.
  */
  if (character >= 'A' && character <= 'Z')
  {
    int characterIndex =
      character - 'A' + 1;

    return pgm_read_byte(
      &FONT_5X7[characterIndex][column]
    );
  }

  /*
    Lettere minuscole.
  */
  if (character >= 'a' && character <= 'z')
  {
    int characterIndex =
      character - 'a';

    return pgm_read_byte(
      &FONT_LOWERCASE_5X7[characterIndex][column]
    );
  }

  /*
    Numeri.
  */
  if (character >= '0' && character <= '9')
  {
    int characterIndex =
      character - '0' + 27;

    return pgm_read_byte(
      &FONT_5X7[characterIndex][column]
    );
  }

  /*
    Ricerca del carattere nell'elenco
    dei simboli conservato in PROGMEM.
  */
  for (int i = 0; i < FONT_SYMBOL_COUNT; i++)
  {
    char storedCharacter =
      (char)pgm_read_byte(
        &FONT_SYMBOLS[i].character
      );

    if (storedCharacter == character)
    {
      return pgm_read_byte(
        &FONT_SYMBOLS[i].columns[column]
      );
    }
  }

  /*
    Carattere non riconosciuto.
  */
  return B00000000;
}


void MegaMatrixText::showFry()
{
  clear();

  /*
    static:
    l'array viene creato una sola volta.

    PROGMEM:
    il disegno resta nella memoria Flash.
  */
  static const byte fry[8] PROGMEM =
  {
    B00011100,
    B00111110,
    B01110111,
    B01011101,
    B01010101,
    B01111111,
    B00111110,
    B00011100
  };

  for (int column = 0; column < 8; column++)
  {
    byte columnData =
      pgm_read_byte(&fry[column]);

    _ledControl.setColumn(
      0,
      7 - column,
      columnData
    );
  }
}

void MegaMatrixText::fryEasterEgg(
  unsigned long imageTime,
  unsigned long textSpeed
)
{
  /*
    Interrompiamo eventuali scritte
    non bloccanti già attive.
  */
  stopScroll();

  showFry();

  delay(imageTime);

  clear();

  scrollText(
    "SHUT UP AND TAKE MY MONEY!",
    textSpeed
  );
}


void MegaMatrixText::setInverted(bool inverted)
{
  _inverted = inverted;
}


bool MegaMatrixText::isInverted()
{
  return _inverted;
}


byte MegaMatrixText::applyDisplayEffects(
  byte columnData
)
{
  if (_inverted == true)
  {
    return (byte)~columnData;
  }

  return columnData;
}

