# Arduino-pohjainen peli nappuloilla, LED-valoilla ja 7-segmenttinäytöllä

## Yleiskuvaus
Tämä ohjelma on suunniteltu Arduinolle ja se toteuttaa yksinkertaisen pelin, jossa käyttäjä painaa nappuloita ja peli tarkistaa nämä painallukset satunnaisesti luodun numerosarjan kanssa. Peli antaa visuaalista palautetta LED-valoilla ja 7-segmenttinäytöllä. Pelissä hyödynnetään ajastimia ja keskeytyksiä (interrupts) nappuloiden nopeaan tunnistamiseen ja pelin aikarajojen hallintaan.

## Komponentit
Tässä projektissa käytetään seuraavia komponentteja:
- **Arduino-lauta** (esim. Arduino Uno)
- **5 painiketta**: Käyttäjä painaa näitä nappuloita pelissä.
- **4 LEDiä**: Näytetään valoja, jotka kertovat pelin tilan ja pelin edistymisen.
- **7-segmenttinäyttö**: Näytetään pelin pistemäärä ja muu peliin liittyvä tieto.
- **Timer1**: Käytetään pelin aikarajojen ja nopeuden hallintaan.
- **Shift-rekisteri**: Käytetään 7-segmenttinäytön ohjaamiseen.

## Peliin liittyvä logiikka

### Pelin aloitus
- Kun käyttäjä painaa aloitusnappia (Button 5), peli käynnistyy. Peli luo satunnaisesti numerosarjan, joka koostuu numeroista 2-5.
- Peli näyttää satunnaisen numerosarjan käyttäjälle LED-valoilla. Käyttäjän tehtävänä on painaa oikeaa nappia oikeassa järjestyksessä.

### Käyttäjän vuoro
- Käyttäjä painaa nappuloita, jotka vastaavat numeroita 2-5. Näiden nappuloiden painallukset tallennetaan `userNumbers`-taulukkoon.
- Peli tarkistaa, ovatko käyttäjän painallukset oikein verrattuna satunnaisesti luotuun `randomNumbers`-taulukkoon.

### Pisteet ja pelin nopeus
- Peli pitää kirjaa käyttäjän oikeista painalluksista. Oikean painalluksen jälkeen pelin nopeus kasvaa ja peli etenee nopeammin.
- Peli päättyy, kun käyttäjä on onnistunut toistamaan kaikki satunnaisesti luodut numerot oikein tai jos virheellinen painallus tehdään.

### Aikarajat ja nopeus
- Pelissä käytetään Timer1-ajastinta, joka määrittää kuinka nopeasti satunnaiset numerot näytetään käyttäjälle. 
- Peli nopeutuu, kun käyttäjä vastaa oikein, ja ajastin säätelee, kuinka nopeasti numerot vilkkuvat.

### Debounce (tärinänpoisto)
- Nappuloiden painallukset käsitellään käyttäen **debounce**-logiikkaa, joka varmistaa, että painallus on aito eikä johdu nappulan tärinästä.
- Tämä tapahtuu tarkistamalla, että nappula on pysyvästi painettuna tietyn ajan ennen kuin se otetaan huomioon.

## Keskeytykset (Interrupts)

### Nappuloiden tunnistus
- **Pin Change Interrupts (PCINT)**: Tämä mekanismi mahdollistaa sen, että ohjelma voi reagoida nappuloiden painalluksiin ilman, että se tarvitsee jatkuvasti tarkistaa nappuloiden tilaa pääsilmukassa. Tämä tekee koodista tehokkaamman ja vähemmän resursseja kuluttavan.
- **Interrupt Service Routine (ISR)**: Jokaiselle nappulalle on oma ISR, joka huolehtii sen käsittelystä, kun nappula painetaan.

### Ajastimen keskeytys
- **Timer1 keskeytys**: Tämä keskeytys hallitsee pelin aikarajoja. Kun ajastin ehtii tietyn rajan (OCR1A-arvo), se kutsuu ISR:ää, joka hallitsee LED-valojen näyttämistä ja pelin nopeuden säätämistä.

## Pelin tarkistaminen ja lopettaminen
- Pelin aikana peli tarkistaa käyttäjän painallusten oikeellisuuden vertaamalla käyttäjän `userNumbers`-taulukkoa pelin alkuperäiseen `randomNumbers`-taulukkoon.
- Jos painallus on väärä, peli kutsuu `stopTheGame()`-funktiota ja peli päättyy.
- Jos kaikki 100 numeron painallus on tehty oikein, peli myös päättyy ja kutsutaan `stopTheGame()`-funktiota.

## Näytön ja LED-valojen hallinta

### 7-segmenttinäytön ohjaus
- **Shift-rekisterin käyttö**: 7-segmenttinäytön ohjaamisessa käytetään shift-rekisteriä, joka muuntaa sekvenssejä ja sytyttää oikeat segmentit näytölle.
- Pisteet ja pelitiedot näytetään 7-segmenttinäytöllä, ja näyttö päivitetään pelin aikana.

### LED-valojen ohjaus
- **LEDien ohjaus**: Pelissä on neljä LEDiä, jotka syttyvät ja sammuvat näytettävän numeron mukaan. Näiden LEDien tila kertoo käyttäjälle, mikä numero on tällä hetkellä näkyvissä.

## Funktiot
Koodissa on useita funktioita, jotka huolehtivat pelin eri osa-alueista:
- **`initializeLeds()`**: Alustaa LED-pinnit.
- **`initializeDisplay()`**: Alustaa 7-segmenttinäytön ja siihen liittyvät pinnit.
- **`initButtonsAndButtonInterrupts()`**: Alustaa nappulat ja niiden keskeytykset.
- **`startTheGame()`**: Aloittaa pelin, luo satunnaiset numerot ja alustaa ajastimen.
- **`checkGame()`**: Tarkistaa, onko käyttäjän painallus oikea verrattuna satunnaisiin numeroihin.
- **`stopTheGame()`**: Pysäyttää pelin ja poistaa ajastimen käytöstä.

### Debounce-funktiot
- **`debounce()`**, **`debounce2()`**, **`debounce3()`**, **`debounce4()`**, **`debounce5()`**: Nämä funktiot käsittelevät jokaisen nappulan tärinänpoiston erikseen.
