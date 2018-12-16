# Hvordan bruger man programmet?

Dette er et program som søger efter ting i en database afhængigt af hvilken genbrugstype du er. Derfra kommer navnet genbrugsmatcher. Programmets funktion ligger i at søge efter specifikke ting fra en liste konfigureret til den givne genbrugstype. Hvis brugeren ønsker at tilgå et produkt, bliver der yderligere mulighed for at 3) 'købe produktet', 2) 'besøg produkt siden' eller 1) 'returnere til listen'.


# Versioner:

Genbrugssammenligner0,01.c		Basic sammenligning af alle produkter, efter userens ønske
Genbrugssammenligner0,02.c		Test version
Genbrugssammenligner0,03.c		Implementering af struct
Genbrugssammenligner0,04.c		List af items og buy_item funktion implementeret
Genbrugssammenligner0,05.c		Fixes and simplifies 0,04 to work with new db and added correct time when items bought
GenbrugssammenlignerML0,10.c  		Login systemet blev implementeret og flere bugfixes og simplificeringer
GenbrugssammenlignerML0,13.c		Opdateret spørgsmål til at finde genbrugsgruppen, og små ændringer i databasen, da postnumemr nu også gemmes og filtreres efter.
GenbrugssammenlignerML0,20.c		Nyt system til at give sql commands ud fra genbrugs gruppe
GenbrugssammenlignerML0,21.c		Filter system, mangler dog stadig nogle ændringer, så man kan nulstille sine gruppe-filtre

To do in 0,21.c:
fix æ,ø,å i listen
