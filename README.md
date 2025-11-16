# Operation Typhoon

### Wolfenstein 3D inspired 2.5D shooter implemented using raycasting in C++.

### Controls :

- **W,A,S,D** - For movement
- **LShift** - Sprint - 2x speed
- **Mouse** - To move the camera
- **Left click** - To shoot using the weapon
- **Right Click** - To use the knife - short range with 2x damage
- **P** - To pause the game
- **R** - To restart the game when you die

### All the assets from the game are original(including the music)  and have not been taken from the internet. Excluding the font.

### Known bugs:

1. Enemies sprites might stack causing lag - will be fixed when implementing a better tracking algorithm
2. If the map is not bordered with 1's, even if the wall looks good when you "push" into it, the wall will move 1 tile -
   solved by creating a correct map(bordered with 1's) - **Not a problem**
### To implement:

(This section represents the features i want to add to the game, along side the requirements for "Tema 1", "Tema 2"
and "Tema 3")  
This does not represent the full feature list of the game, just the more important ones.

1. ~~Hp/Ammo~~
2. More maps
3. ~~Menu window to setup resolution and other small details before starting the game.~~
4. ~~Better enemy logic and more enemy types + making enemies attack, also spawning enemies based on positions given on
   the map.~~
5. ~~More enemy types - for example a boss derived from the main class - might be good for making the inheritance
   requirements.~~
6. ~~Pause menu.~~
7. ~~Sprint~~

### Things that wont be implemented:

- **Minimap** - trying to stay close to the source material, i believe that a big part of Wolfenstein 3D was the lack of
  minimap, forcing you feel lost/in a maze - also might open the possibility for future maze like levels.

### "Modding":

For basic "modding", you can go to the assets folder and replace textures and music with whatever you like. Just make
sure to keep the filenames the same.

- Cutit.png - knife texture
- Enemy1.png - alive enemy
- Enemy2.png - dead enemy
- Weapon1.png - normal weapon
- Weapon2.png - shooting weapon
- ezoteric.ogg - the music in the game.
- Close.ogg - the startup sound(when switching from the menu the game)
- Enemy1/2.png - legacy sprites.
- EnemyShort_[Idle/Attack/Dead] - sprites for the close range enemy.
- EnemyLong_[Idle/Attack/Dead] - sprites for the long range enemy.
- EnemyLongHD_[Idle/Attack/Attack2/Dead] - sprites for the long range enemy that also has a short range, high damage
  attack(Yes, HD comes from high damage).
- You can also change the map by modifying the map1 file - just make sure to have the map bordered with 1's.   
  1 is a wall and 0 is a free space -> will probably get modified in a future update to include marking enemy positions.

#### Notice the game won't start with any missing files/files named incorrectly.
### Tema 0

- [x] Nume proiect (poate fi schimbat ulterior)
- [x] Scurtă descriere a temei alese, ce v-ați propus să implementați

## Tema 1

#### Cerințe
- [x] definirea a minim **3-4 clase** folosind compunere cu clasele definite de voi; moștenirile nu se iau în considerare aici
- [x] constructori de inițializare cu parametri pentru fiecare clasă
- [x] pentru o aceeași (singură) clasă: constructor de copiere, `operator=` de copiere, destructor
<!-- - [ ] pentru o altă clasă: constructor de mutare, `operator=` de mutare, destructor -->
<!-- - [ ] pentru o altă clasă: toate cele 5 funcții membru speciale -->
- [x] `operator<<` pentru **toate** clasele pentru afișare (`std::ostream`) folosind compunere de apeluri cu `operator<<`
- [x] cât mai multe `const` (unde este cazul) și funcții `private`
- [x] implementarea a minim 3 funcții membru publice pentru funcționalități netriviale specifice temei alese, dintre care cel puțin 1-2 funcții mai complexe
  - nu doar citiri/afișări sau adăugat/șters elemente într-un/dintr-un vector
- [x] scenariu de utilizare **cu sens** a claselor definite:
  - crearea de obiecte și apelarea tuturor funcțiilor membru publice în main
  - vor fi adăugate în fișierul `tastatura.txt` DOAR exemple de date de intrare de la tastatură (dacă există); dacă aveți nevoie de date din fișiere, creați alte fișiere separat
- [x] minim 50-55% din codul propriu să fie C++, `.gitattributes` configurat corect
- [x] tag de `git`: de exemplu `v0.1`
- [x] serviciu de integrare continuă (CI) cu **toate bifele**; exemplu: GitHub Actions

## Tema 2

#### Cerințe
- [x] separarea codului din clase în `.h` (sau `.hpp`) și `.cpp`
- [x] moșteniri:
  - minim o clasă de bază și **3 clase derivate** din aceeași ierarhie
  - ierarhia trebuie să fie cu bază proprie, nu derivată dintr-o clasă predefinită
  - [] funcții virtuale (pure) apelate prin pointeri de bază din clasa care conține atributul de tip pointer de bază
    - minim o funcție virtuală va fi **specifică temei** (i.e. nu simple citiri/afișări sau preluate din biblioteci i.e. draw/update/render)
    - constructori virtuali (clone): sunt necesari, dar nu se consideră funcții specifice temei
    - afișare virtuală, interfață non-virtuală
  - [ ] apelarea constructorului din clasa de bază din constructori din derivate
  - [ ] clasă cu atribut de tip pointer la o clasă de bază cu derivate; aici apelați funcțiile virtuale prin pointer de bază, eventual prin interfața non-virtuală din bază
    - [ ] suprascris cc/op= pentru copieri/atribuiri corecte, copy and swap
    - [x] `dynamic_cast`/`std::dynamic_pointer_cast` pentru downcast cu sens
    - [x] smart pointers (recomandat, opțional)
- [ ] excepții
  - [ ] ierarhie proprie cu baza `std::exception` sau derivată din `std::exception`; minim **3** clase pentru erori specifice distincte
    - clasele de excepții trebuie să trateze categorii de erori distincte (exemplu de erori echivalente: citire fișiere cu diverse extensii)
  - [ ] utilizare cu sens: de exemplu, `throw` în constructor (sau funcție care întoarce un obiect), `try`/`catch` în `main`
  - această ierarhie va fi complet independentă de ierarhia cu funcții virtuale
- [x] funcții și atribute `static`
- [x] STL
- [x] cât mai multe `const`
- [ ] funcții *de nivel înalt*, de eliminat cât mai mulți getters/setters/funcții low-level
- [x] minim 75-80% din codul propriu să fie C++
- [ ] la sfârșit: commit separat cu adăugarea unei noi clase derivate fără a modifica restul codului, **pe lângă cele 3 derivate deja adăugate** din aceeași ierarhie
  - noua derivată nu poate fi una existentă care a fost ștearsă și adăugată din nou
  - noua derivată va fi integrată în codul existent (adică va fi folosită, nu adăugată doar ca să fie)
- [ ] tag de `git` pe commit cu **toate bifele**: de exemplu `v0.2`

## Tema 3

#### Cerințe
- [ ] 2 șabloane de proiectare (design patterns)
- [ ] o clasă șablon cu sens; minim **2 instanțieri**
  - [ ] preferabil și o funcție șablon (template) cu sens; minim 2 instanțieri
- [ ] minim 85% din codul propriu să fie C++
<!-- - [ ] o specializare pe funcție/clasă șablon -->
- [ ] tag de `git` pe commit cu **toate bifele**: de exemplu `v0.3` sau `v1.0`

## Instrucțiuni de compilare

Proiectul este configurat cu CMake.

Instrucțiuni pentru terminal:

0. Biblioteci necesare pe Linux (presupunem sistem de operare bazat pe Debian, necesită `sudo`)
```sh
bash ./scripts/install-sfml-deps.sh
```

Dacă lipsesc și alte biblioteci, ștergeți folder-ul de build de la pasul 1 și reconfigurați proiectul după ce ați instalat ce lipsea.

1. Pasul de configurare
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
# sau ./scripts/cmake.sh configure
```

Sau pe Windows cu GCC folosind Git Bash:
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja
# sau ./scripts/cmake.sh configure -g Ninja
```

La acest pas putem cere să generăm fișiere de proiect pentru diverse medii de lucru.


2. Pasul de compilare
```sh
cmake --build build --config Debug --parallel 6
# sau ./scripts/cmake.sh build
```

Cu opțiunea `parallel` specificăm numărul de fișiere compilate în paralel.


3. Pasul de instalare (opțional)
```sh
cmake --install build --config Debug --prefix install_dir
# sau ./scripts/cmake.sh install
```

Vezi și [`scripts/cmake.sh`](scripts/cmake.sh).
r
Observație: folderele `build/` și `install_dir/` sunt adăugate în fișierul `.gitignore` deoarece
conțin fișiere generate și nu ne ajută să le versionăm.

## Resources
<!-- renovate: datasource=github-tags depName=SFML/SFML versioning=loose -->
- [SFML](https://github.com/SFML/SFML/tree/3.0.1) (Zlib)

### The inspiration for the project:

- https://www.youtube.com/watch?v=LUYxLjic0Bc
- https://www.youtube.com/watch?v=OpIS1zoz6fU

### Raycasting tutorials - (helped me understand the math and logic of the rendering method)

- https://www.youtube.com/watch?v=gYRrGTC7GtA (you can check the hole series, it's amazing)
- https://lodev.org/cgtutor/raycasting.html
- https://en.wikipedia.org/wiki/Ray_casting# - Also the wikipedia page is very good for understanding the concept.

### Also the SFML documentation is very helpfull:

- https://www.sfml-dev.org/documentation/3.0.2/

### The original game can be found online and played in the browser(not sure if i can leave the link here).

### DDA

https://www.geeksforgeeks.org/computer-graphics/dda-line-generation-algorithm-computer-graphics/
https://en.wikipedia.org/wiki/Digital_differential_analyzer_(graphics_algorithm)