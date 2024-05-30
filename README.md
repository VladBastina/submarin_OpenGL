# Documentație pentru Proiectul de Simulare a Unui Submarin în OpenGL

## Descriere Generală

Proiectul constă în simularea plimbării unui submarin pe suprafața și sub suprafața apei, în diferite condiții de lumină (zi și noapte). Acesta include un efect de apă realistă, un teren generat procedural și obiecte pentru pești, submarin și alte elemente. De asemenea, proiectul include efecte sonore care variază în funcție de poziția submarinului, folosind biblioteca irrKlang pentru gestionarea sunetelor. Pentru încărcarea și gestionarea obiectelor, s-a folosit biblioteca Assimp.

![Imagine de peisaj subacvatic](https://github.com/VladBastina/submarin_OpenGL/blob/main/submarin1.png)

## Componente Principale

### 1. Plimbarea Submarinului

Submarinul este controlat folosind tastele definite astfel:

- **W**: Accelerare - Submarinul se va deplasa înainte cu o viteză mai mare.
- **A**: Viraj la stânga - Submarinul va schimba direcția către stânga.
- **D**: Viraj la dreapta - Submarinul va schimba direcția către dreapta.
- **Q**: Scufundare - Submarinul va coborî în adâncime.
- **E**: Ridicare deasupra apei - Submarinul va urca la suprafață.

### 2. Efecte de Apă

Efectul de apă este implementat folosind tehnici avansate de simulare a apei, cum ar fi texturi animate, reflexii și refracții. Apa reacționează la mișcarea submarinului și la alte obiecte din scenă.

### 3. Teren Generat Procedural

Terenul subacvatic este generat procedural pentru a oferi o experiență de explorare unică și nelimitată. Se folosesc algoritmi de zgomot procedural, cum ar fi Perlin Noise, pentru a genera terenul subacvatic.

### 4. Obiecte de Pești și Submarin

Peștii și submarinul sunt modele 3D importate folosind biblioteca Assimp. Aceste obiecte sunt animate și se mișcă în mod autonom în jurul submarinului și prin peisajul subacvatic.

### 5. Efecte Sonore

Efectele sonore sunt utilizate pentru a îmbunătăți experiența de joc. Sunetele de mediu, cum ar fi zgomotul valurilor sau sunetul de plonjare, sunt generate în funcție de poziția și mișcarea submarinului, pentru a crea o atmosferă captivantă și realistă.

## Tehnologii Utilizate

- **OpenGL**: Pentru afișarea grafică și simularea mediului subacvatic.
- **GLFW**: Pentru gestionarea ferestrei și a evenimentelor de intrare.
- **Assimp**: Pentru încărcarea și gestionarea modelelor 3D.
- **irrKlang**: Pentru gestionarea efectelor sonore și a sunetelor.
- **Perlin Noise**: Pentru generarea terenului subacvatic.

![Imagine de peisaj subacvatic](https://github.com/VladBastina/submarin_OpenGL/blob/main/submarin2.png)

## Concluzie

Proiectul oferă o experiență captivantă și realistă a explorării subacvatice, oferind utilizatorului controlul asupra unui submarin într-un mediu generat procedural. Efectele grafice și sonore îmbunătățesc atmosfera și adâncesc imersiunea, oferind o experiență de joc memorabilă și plină de aventură.
