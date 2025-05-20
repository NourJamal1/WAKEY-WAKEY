# WAKY WAKY

### Gemaakt door: Nour

---

## Over het project
**WAKY WAKY** is een slim wekker-systeem dat ervoor zorgt dat je niet makkelijk terug in slaap valt. Het systeem gebruikt gewichtssensoren om te detecteren of je echt uit bed bent, en een MP3-speler om een wekgeluid af te spelen. Het idee is simpel: ga je terug in bed liggen, dan gaat de wekker opnieuw af!

---

## Functionaliteiten
- **MP3-speler**: Speelt een liedje af vanaf een SD-kaart om je wakker te maken.  
- **Gewichtssensoren**: Detecteren of je uit bed bent gestapt of weer bent gaan liggen.  
- **Knop**: Hiermee kun je het alarm stoppen als je daadwerkelijk uit bed bent.  

---

## Organisatie van de code

De hoofdcode van het project heet **`WEKKER_SAMENSTELLING.ino`**. Dit is het bestand dat je moet **runnen om de wekker te laten werken**. Hierin zijn alle onderdelen samengevoegd tot één geheel.

De code is logisch opgebouwd in blokken. Elk blok behandelt een specifiek onderdeel van het systeem:

- **Regel 2–4**: Bibliotheken voor gewichtssensor en MP3-speler.  
- **Regel 7–15**: Declaraties van alle functies die in de code gebruikt worden.  
- **Regel 18–25**: Algemene variabelen zoals de tijd en de status van de alarmen.  
- **Regel 28–39**: Instellingen van de gebruikte pinnen op de Arduino.  
- **Regel 42–62**: Variabelen voor de sensor, MP3-speler en knop, inclusief instellingen zoals volume en debounce-tijden.  
- **Regel 65–78**: Timerinstellingen voor het starten van het alarm en een tweede controle.  
- **Regel 81–85**: De `setup()` functie initialiseert de sensor, de MP3-speler en de knop.  
- **Regel 88–120**: De `loop()` functie, waar de hoofdlogica van het project plaatsvindt:

### Wat gebeurt er in de `loop()`?  
- **Regel 90**: Eerst wordt alles geüpdatet (tijd, gewicht, knopstatus).  
- **Regel 92–95**: Als de eerste timer nog niet is gestart, wordt gekeken of die gestart kan worden (door op de knop te drukken).  
- **Regel 97–99**: Als de ingestelde tijd van timer één voorbij is, wordt deze uitgeschakeld.  
- **Regel 101–104**: Als je nog in bed ligt en de eerste timer is voorbij, wordt het alarm afgespeeld.  
- **Regel 106–109**: Als je opstaat (gewicht < 300g) en op de knop drukt, of het geluid is klaar, dan stopt het alarm en wordt de tweede timer gestart.  
- **Regel 111–114**: Als je tijdens de tweede timer weer in bed gaat liggen, speelt het alarm opnieuw af.  
- **Regel 116–118**: Als de tweede timer is afgelopen, wordt die uitgeschakeld.  
- **Regel 119**: Seriële uitvoer voor debugging (verschil in tijd).

- **Regel 123–einde**: De rest van de code bestaat uit losse functies, waaronder:
  - Instellen van de gewichtssensor
  - Uitlezen van het gewicht
  - Afspelen van het MP3-bestand
  - Detectie van knopstatus
  - Starten van timers
  - Update-functie die alles bijhoudt

---

## Bestandsstructuur

Het project bestaat uit:

- **`WEKKER_SAMENSTELLING.ino`**  
  Dit is de hoofdcode. Hier worden alle onderdelen gecombineerd tot een compleet werkend systeem. Deze code moet je gebruiken om de wekker daadwerkelijk te laten werken.

- **Map: `wekker onderdelen`**  
  Deze map bevat aparte bestanden met **testcode per onderdeel**. Hiermee kun je elk onderdeel afzonderlijk testen, begrijpen en kalibreren voordat je alles samenvoegt. Denk aan testcode voor de gewichtssensor, de MP3-speler en de knop.  
  Sommige van deze bestanden zijn gebaseerd op YouTube-tutorials. Dit was vooral handig bij het vinden van de juiste **calibration value** en **tare offset** voor de gewichtssensor, zodat deze goed op 0 kg start — ook als er al iets op ligt.

---

## Installatie en gebruik

### Benodigdheden:
- Een **Arduino Uno**
- Hardwarecomponenten:
  - **MP3-speler module**
  - **Gewichtssensoren**
  - **Drukknop**

### Stappen:
1. Sluit alle onderdelen aan op de Arduino Uno. De juiste pinnen vind je bovenaan in de `WEKKER_SAMENSTELLING`-code. Verderop in dit document zie je schema's per onderdeel. Onderaan staan video’s die de aansluitingen stap voor stap uitleggen.
2. Installeer de benodigde bibliotheken:
   - **Extern**: 
     - [`mp3tf16p`](#) *(https://dev.azure.com/overlording/The%20Last%20Outpost%20Workshop/_git/MP3-TF-16P)*
   - **Via Arduino IDE**:
     - `DFRobotDFPlayerMini` (voor de MP3-speler)
     - `HX711_ADC` (voor de gewichtssensor)
3. Open het bestand **`WEKKER_SAMENSTELLING.ino`** in de Arduino IDE en upload het naar de Arduino.
4. Bekijk de afbeeldingen in de volgende sectie om je bedrading te controleren.

---

## Hardware-aansluitingen

Hieronder zie je hoe je elk onderdeel moet aansluiten op de Arduino Uno:

1. **MP3-speler module**  
   ![mp3 player](mp3player.png)

2. **Gewichtssensoren**  
   ![Gewicht sensor](gewichtsensor.png)

3. **Drukknop**  
   ![Knop circuit](circuit_button.png)

---

## Extra uitleg en referenties

Leer meer over de onderdelen via deze handige YouTube-video's:  

- **Gewichtssensoren**  
  [![Gewicht sensoren video](https://img.youtube.com/vi/LIuf2egMioA/0.jpg)](https://youtu.be/LIuf2egMioA?si=A7XPyC9-2faqjMKG)

- **MP3 Player**  
  [![MP3 Player video](https://img.youtube.com/vi/PBdqgHj_AkU/0.jpg)](https://youtu.be/PBdqgHj_AkU?si=pbIv38PhXdfLJ4m9)

- **Knop**  
  [![Knop video](https://img.youtube.com/vi/VPGRqML_v0w/0.jpg)](https://youtu.be/VPGRqML_v0w?si=hWb6luXIO06AUFNs)

---

## Demonstratievideo

Bekijk hier hoe het hele systeem werkt in de praktijk:

[![Demonstratievideo](https://img.youtube.com/vi/mI2D8F5TRIw/0.jpg)](https://youtu.be/mI2D8F5TRIw)

---

## Toekomstige verbeteringen

- Een display toevoegen om de tijd te tonen.  
- De mogelijkheid om meerdere wekgeluiden te kiezen.  
- Een mobiele app bouwen om de wekker te bedienen.

---

## Dankwoord

Dank aan mijn begeleiders, vooral Mo, voor de begeleiding. Ook dank aan de makers van de YouTube-video’s die ik heb gebruikt – zij hebben het veel makkelijker gemaakt.

---

## Bronvermelding

- **Gewichtssensoren**:  
  [YouTube-video](https://youtu.be/LIuf2egMioA?si=A7XPyC9-2faqjMKG),  
  [HX711_ADC GitHub](https://github.com/olkal/HX711_ADC),  
  [Schema’s](https://circuitjournal.com/50kg-load-cells-with-HX711)

- **MP3 Player**:  
  [YouTube-video](https://youtu.be/PBdqgHj_AkU?si=pbIv38PhXdfLJ4m9),  
  [Library GitHub](https://dev.azure.com/overlording/The%20Last%20Outpost%20Workshop/_git/MP3-TF-16P)

- **Knop**:  
  [YouTube-video](https://youtu.be/VPGRqML_v0w?si=hWb6luXIO06AUFNs)

---

**Veel succes met het bouwen van je eigen WAKY WAKY wekker!**
