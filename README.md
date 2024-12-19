
# WAKY WAKY

### Gemaakt door: Nour

---

## Over het project
**WAKY WAKY** is een slim wekker-systeem dat ervoor zorgt dat je niet eenvoudig terug in slaap kunt vallen. Het systeem maakt gebruik van gewichtssensoren om te detecteren of je daadwerkelijk uit bed bent en een MP3-speler om een liedjes af te spelen. Het idee is simpel: ga je weer in bed liggen, dan gaat de wekker opnieuw af!

---

## Functionaliteiten
- **MP3-player**: Speelt een liedje af uit een sd kaart af dat je wakker maakt.
- **Gewichtsensoren**: Detecteren of je daadwerkelijk uit bed bent gestapt, of als je weer in bed ligt.
- **Knop**: Hiermee kun je de wekker stoppen als je uit bed bent.

---

## Installatie en gebruik
### Benodigdheden:
- Een **Arduino Uno**.
- De hardwarecomponenten die in het project zijn opgenomen:
  - **MP3-speler module.**
  - **Gewichtsensoren.**
  - **Drukknop.**

### Stappen:
1. Sluit alle hardwareonderdelen aan op de Arduino Uno. De exacte pinnen die je moet gebruiken staan bovenaan de code vermeld. In de fotos verder hier onder kan je de schemas per onderdeel vinden. Ook helemaal onderaan de video vin je youtube videos die je elke onderdeel en zijn aansluiting laat zien.
2. Download de benodigde bibliotheken:
   - **Extern**: 
     - [`mp3tf16p`](#) (downloadlink toevoegen).
   - **Via Arduino IDE**:
     - `DFRobotDFPlayerMini` (voor de MP3-speler).
     - `HX711_ADC` (voor de gewichtsensoren).
3. Upload de code naar de Arduino via de Arduino IDE.
4. Bekijk de afbeeldingen in de sectie "Hardware-aansluitingen" om te controleren of je alles correct hebt aangesloten.

---

## Hardware-aansluitingen
Hier vind je afbeeldingen die je helpen bij het aansluiten van elk onderdeel op de Arduino Uno:

1. **MP3-speler module:**  
   ![mp3 player](mp3player.png)

2. **Gewichtsensoren:**  
     ![Gewicht sensor](gewichtsensor.png)

3. **Drukknop:**  
   ![Knop circuit](circuit_button.png) 

---

## Extra uitleg en referenties
Om meer te leren over de hardwareonderdelen en hun werking, bekijk de volgende YouTube-video's:  
- **Gewicht sensoren:**  
  [![Gewicht sensoren video](https://img.youtube.com/vi/LIuf2egMioA/0.jpg)](https://youtu.be/LIuf2egMioA?si=A7XPyC9-2faqjMKG)

- **MP3 Player:**  
  [![MP3 Player video](https://img.youtube.com/vi/PBdqgHj_AkU/0.jpg)](https://youtu.be/PBdqgHj_AkU?si=pbIv38PhXdfLJ4m9)

- **Knop:**  
  [![Knop video](https://img.youtube.com/vi/VPGRqML_v0w/0.jpg)](https://youtu.be/VPGRqML_v0w?si=hWb6luXIO06AUFNs)

---

## Demonstratievideo
Bekijk hieronder de demonstratievideo van het volledige systeem:

[![Demonstratievideo](https://img.youtube.com/vi/mI2D8F5TRIw/0.jpg)](https://youtu.be/mI2D8F5TRIw)

---

## Toekomstige verbeteringen
- Een display toevoegen om de tijd weer te geven.
- Een mogelijkheid om verschillende wekgeluiden te kiezen.
- Een mobiele app maken om de wekker te bedienen.

---

## Dankwoord
Dank aan de begeleiders, vooral Mo, voor zijn hulp. Verder dank aan de mensen die de YouTube-video's hebben gemaakt die ik hierboven heb vermeld, want ze hebben het werk vele malen makkelijker gemaakt voor me.

---

## Bronvermelding
Hier zijn de bronnen die zijn gebruikt voor dit project:

- **Gewicht sensoren**: [Gewicht sensoren video](https://youtu.be/LIuf2egMioA?si=A7XPyC9-2faqjMKG), de [HX711_ADC GitHub-pagina](https://github.com/olkal/HX711_ADC), en de [diagrammen link](https://circuitjournal.com/50kg-load-cells-with-HX711).
- **MP3 Player**: [MP3 Player video](https://youtu.be/PBdqgHj_AkU?si=pbIv38PhXdfLJ4m9) en de [library GitHub-pagina](https://dev.azure.com/overlording/The%20Last%20Outpost%20Workshop/_git/MP3-TF-16P).
- **Knop**: [Knop video](https://youtu.be/VPGRqML_v0w?si=hWb6luXIO06AUFNs).

---

**Veel succes met het bouwen van jouw eigen WAKY WAKY wekker!**
