#include <MegaMatrixText.h>

/*
  Collegamenti MAX7219:

  DIN -> pin 12
  CLK -> pin 10
  CS  -> pin 11
*/
MegaMatrixText matrix(12, 10, 11);

void setup()
{
  /*
    Inizializza il display.
  */
  matrix.begin();

  /*
    Luminosità da 0 a 15.
  */
  matrix.setBrightness(6);
}

void loop()
{
  /*
    Mostra un singolo carattere.
  */
  matrix.showChar('A');

  delay(1000);

  /*
    Mostra un testo un carattere alla volta.
  */
  matrix.showTextOneByOne(
    "Arduino",
    500
  );

  delay(1000);

  /*
    Fa scorrere un testo da destra a sinistra.

    Questa funzione è bloccante:
    Arduino rimane occupato finché
    lo scorrimento non termina.
  */
  matrix.scrollText(
    "Ciao Fraido!",
    80
  );

  delay(1000);

  /*
    Attiva la modalità negativa.
  */
  matrix.setInverted(true);

  matrix.scrollText(
    "Testo negativo",
    80
  );

  /*
    Ripristina la modalità normale.
  */
  matrix.setInverted(false);

  delay(1500);
}