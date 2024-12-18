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
- gewicht sensoren: https://youtu.be/LIuf2egMioA?si=A7XPyC9-2faqjMKG
- MP3 Player: https://youtu.be/PBdqgHj_AkU?si=pbIv38PhXdfLJ4m9
- Knop: https://youtu.be/VPGRqML_v0w?si=hWb6luXIO06AUFNs


---

## Toekomstige verbeteringen
- Een display toevoegen om de tijd weer te geven.
- Een mogelijkheid om verschillende wekgeluiden te kiezen.
- Een mobiele app maken om de wekker te bedienen.

---

## Dankwoord
Dank aan de begleiders vooal mo voor zijn hulp. veder dank voor de mensen die de youtube videos hebben gemaakt die ik hierboven heb vermeld want ze hebben de werk velen malen makkelijker gemaakt voor me.

---

**Veel succes met het bouwen van jouw eigen WAKY WAKY wekker!**
