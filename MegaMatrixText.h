#ifndef MEGA_MATRIX_TEXT_H
#define MEGA_MATRIX_TEXT_H

#include <Arduino.h>
#include <LedControl.h>

class MegaMatrixText
{
  public:

    /*
      Costruttore.

      Riceve i pin:
      DIN, CLK e CS.
    */
    MegaMatrixText(
      int dataPin,
      int clockPin,
      int csPin
    );

    /*
      Inizializza la matrice.
    */
    void begin();

    /*
      Spegne tutti i LED.
    */
    void clear();

    /*
      Imposta la luminosità da 0 a 15.
    */
    void setBrightness(byte brightness);

    /*
      Mostra un singolo carattere.
    */
    void showChar(char character);

    /*
      Mostra un testo un carattere alla volta.
    */
    void showTextOneByOne(
      const char *text,
      unsigned long characterTime
    );

    /*
      Fa scorrere un testo da destra a sinistra.
    */
    void scrollText(
      const char *text,
      unsigned long scrollDelay
    );
    /*
	Avvia uno scorrimento non bloccante.
	*/
	void startScroll(
	  const char *text,
	  unsigned long scrollDelay
	);
	
	/*
	Aggiorna lo scorrimento.
	
	Deve essere richiamata continuamente
	dentro loop().
	*/
	void update();
	
	/*
	Restituisce true se lo scorrimento
	è ancora attivo.
	*/
	bool isScrolling();
	
	/*
	Ferma lo scorrimento e pulisce
	la matrice.	
	*/
	void stopScroll();
	/*
	Mostra un piccolo Fry stilizzato.
	*/
	void showFry();
	
	/*
	Mostra l'easter egg completo.
	*/
	void fryEasterEgg(
	unsigned long imageTime = 1500,
	unsigned long textSpeed = 80
	);
		
	/*
	Attiva o disattiva la visualizzazione negativa.
	*/
	void setInverted(bool inverted);
	
	/*
	Restituisce true se la modalità negativa è attiva.
	*/
	bool isInverted();

  private:

    /*
      Oggetto LedControl interno.

      È privato perché sarà utilizzato
      soltanto dalla libreria.
    */
    LedControl _ledControl;

 

    /*
      Restituisce una specifica colonna
      del testo completo.
    */
    byte getTextColumn(
      const char *text,
      int textLength,
      int textColumn
    );
    /*
	Testo attualmente in scorrimento.
	*/
	const char *_scrollText;
	
	/*
	Lunghezza del testo.
	*/
	int _scrollTextLength;
	
	/*
	Posizione corrente dello scorrimento.
	*/
	int _scrollOffset;
	
	/*	
	Numero totale di passaggi necessari.
	*/
	int _totalScrollSteps;
	
	/*
	Tempo tra uno spostamento e il successivo.
	*/
	unsigned long _scrollDelay;
	
	/*
	Momento dell'ultimo aggiornamento.
	*/
	unsigned long _lastScrollUpdate;
	
	/*
	Indica se lo scorrimento è attivo.
	*/
	bool _scrolling;
	
	/*
	Restituisce una colonna del disegno
	corrispondente a un carattere.
	*/
	byte getGlyphColumn(
	char character,
	byte column
	);
    
    /*
	Stato della modalità negativa.
	*/
	bool _inverted;
	
	/*
	Applica eventuali effetti al byte
	prima di inviarlo alla matrice.
	*/
	byte applyDisplayEffects(byte columnData);
    
    
    
    
    
    
    
    
    
    
    
    
};

#endif
