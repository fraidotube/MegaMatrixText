#include <MegaMatrixText.h>

/*
  Collegamenti MAX7219:

  DIN -> pin 12
  CLK -> pin 10
  CS  -> pin 11
*/
MegaMatrixText matrix(12, 10, 11);


/*
  Variabili utilizzate per far lampeggiare
  il LED integrato senza utilizzare delay().
*/
unsigned long lastBlinkTime = 0;
bool ledState = false;


void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  matrix.begin();
  matrix.setBrightness(6);

  /*
    Avvia lo scorrimento non bloccante.
  */
  matrix.startScroll(
    "MegaMatrixText non-blocking scroll",
    80
  );
}


void loop()
{
  /*
    Deve essere richiamato continuamente.

    La libreria controlla autonomamente
    quando deve spostare il testo.
  */
  matrix.update();


  /*
    Contemporaneamente facciamo lampeggiare
    il LED integrato ogni 500 millisecondi.
  */
  if (millis() - lastBlinkTime >= 500)
  {
    lastBlinkTime = millis();

    ledState = !ledState;

    digitalWrite(
      LED_BUILTIN,
      ledState
    );
  }


  /*
    Quando lo scorrimento termina,
    lo riavviamo.
  */
  if (matrix.isScrolling() == false)
  {
    matrix.startScroll(
      "MegaMatrixText non-blocking scroll",
      80
    );
  }
}