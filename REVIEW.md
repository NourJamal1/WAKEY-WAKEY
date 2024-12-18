

# Code Review

hieronder vindt u de twee code revieuws voor de wekker.

## Reviewer 1: Wassim

### Feedback:
1. **Sterke punten:**
   - Wassim vond dat de code over het algemeen goed gedocumenteerd is. De comments maken duidelijk wat elk gedeelte van de code doet.
   - De structuur van de code is begrijpelijk, en het project is goed te navigeren.

2. **Verbeterpunten:**
   - **Variabele naamgeving:**  
     De variabele `reading` wordt gebruikt in de code, maar Wassim merkte op dat deze naam niet logisch is in de context waarin deze voorkomt. Hij stelde voor om een specifiekere naam te kiezen die beter aangeeft wat deze variabele vertegenwoordigt.
   - **Gecommentarieerde debugging-middelen:**  
     Er zijn stukken code in comments die voor debugging worden gebruikt. Wassim stelde voor om in de comments expliciet te vermelden dat deze stukken voor debugging zijn bedoeld. Dit maakt het duidelijker voor toekomstige ontwikkelaars die de code lezen.

---

## Reviewer 2: Tygo

### Feedback:
1. **Sterke punten:**
   - Tygo gaf aan dat de code over het algemeen netjes en goed georganiseerd is.
   - Hij merkte op dat er een duidelijke structuur zit in hoe functies worden opgeroepen in de `loop()`.

2. **Verbeterpunten:**
   - **Inclusies bovenaan plaatsen:**  
     Tygo gaf aan dat hij gewend is om alle `#include` statements helemaal bovenaan de code te zetten. Dit maakt het makkelijker om in één oogopslag te zien welke bibliotheken worden gebruikt.
   - **Optimalisatie van functie-aanroepen:**  
     De functie `update` wordt meerdere keren aangeroepen in de `loop`. Volgens Tygo kan dit slimmer worden gedaan, bijvoorbeeld door de logica van deze functie te optimaliseren of door redundante aanroepen te vermijden. Dit kan zowel de leesbaarheid verbeteren als de prestaties verhogen.

---

## Reflectie

De feedback van beide reviewers heeft mij inzichten gegeven in hoe ik mijn code kan verbeteren. Hoewel ik geen grote veranderingen meer mag aanbrengen aan mijn project, kan ik op basis van deze reviewpunten reflecteren en aangeven hoe ik mijn code met meer tijd zou verbeteren.

1. **Variabele naamgeving:**  
   - Ik zou de naam `reading` veranderen in een meer beschrijvende naam zoals `sensorReading`.

2. **Gecommentarieerde debugging-middelen:**  
   - Voor debugging-gerelateerde comments zou ik een gestandaardiseerd label zoals `// DEBUG:` gebruiken, zodat duidelijk is dat deze code tijdelijk is of voor een specifiek doel wordt gebruikt.

3. **Inclusies bovenaan plaatsen:**  
   - Ik zou mijn includes laten hoe ze zijn. volgensmij staan ze duidelijk genoeg en in onder de juiste groepen verdeeld. ik snap wel dat Tygo het een beetje verwarend vond.

4. **Optimalisatie van `update`:**  
   - Ik zou de functie `update` minder vaak aanroepen in de  `loop()` want het is niet zo hard nodig. dat heb ik al aangepast.


# Uitwerken van de reviews
- ik zal de de volgende punten verbeteren in mijn code:
1. Variabele naamgeving.
2. Gecommentarieerde debugging-middelen.
3. Optimalisatie van `update`.
