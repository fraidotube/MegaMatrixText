#include <MegaMatrixText.h>

MegaMatrixText matrix(12, 10, 11);

void setup()
{
  matrix.begin();

  /*
    In negativo si accendono molti più LED,
    quindi usiamo una luminosità moderata.
  */
  matrix.setBrightness(4);
}

void loop()
{
  /*
    Carattere normale.
  */
  matrix.setInverted(false);
  matrix.showChar('A');

  delay(1500);

  /*
    Lo stesso carattere al negativo.
  */
  matrix.setInverted(true);
  matrix.showChar('A');

  delay(1500);

  /*
    Testo normale.
  */
  matrix.setInverted(false);

  matrix.scrollText(
    "Testo normale",
    80
  );

  delay(1000);

  /*
    Testo negativo.
  */
  matrix.setInverted(true);

  matrix.scrollText(
    "Testo negativo",
    80
  );

  delay(1000);

  /*
    Ripristiniamo la modalità normale.
  */
  matrix.setInverted(false);
}