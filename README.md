# Sistem Gestionare Playlist-uri

„Sistem gestionare playlist-uri” este o aplicație de consolă scrisă în limbajul C, care permite utilizatorilor să creeze, să gestioneze și să vizualizeze playlist-uri muzicale. Funcționalitățile includ adăugarea și ștergerea melodiilor din playlist-uri, sortarea playlist-urilor în funcție de rating și ștergerea playlist-urilor nedorite. Aplicația este simplă și interactivă, fiind destinată înțelegerii conceptelor de programare procedurală, manipularea fișierelor și gestionarea memoriei dinamice.

## Funcționalități
1. Creare de playlist-uri cu melodii personalizate.
2. Adăugare și ștergere melodii dintr-un playlist.
3. Vizualizare playlist-uri sortate descrescător după rating.
4. Ștergere playlist-uri.

## Structura proiectului
- `main.c`: Codul sursă principal al aplicației.
- `playlisturi.txt`: Fișier care conține detalii despre toate playlisturile.
- `playlist1.txt`, `playlist2.txt`, `playlist3.txt`: Exemple de fișiere pentru playlisturi individuale.

## Tehnologii utilizate
- **Limbaj**: C
- **Consolă**: Aplicație rulată din terminal

## Cum să rulezi aplicația
1. **Folosind terminalul**:
   - Clonează repository-ul:
     ```bash
     git clone https://github.com/onica-lorena/Sistem-gestionare-playlist.git
     ```
   - Compilează codul:
     ```bash
     gcc main.c -o Sistem-gestionare-playlist
     ```
   - Rulează aplicația:
     ```bash
     ./Sistem-gestionare-playlist
     ```

2. **Folosind un IDE**:
   - **Code::Blocks**:
     - Deschide Code::Blocks.
     - Creează un proiect nou și adaugă fișierul `main.c`.
     - Click pe **Build and Run**.
   - **Dev-C++**:
     - Deschide Dev-C++.
     - Încarcă fișierul `main.c` și click pe **Execute > Compile and Run**.
   - **Visual Studio**:
     - Creează un nou proiect de tip **Console Application**.
     - Adaugă fișierul `main.c` și compilează aplicația.

## Exemple de utilizare

### 1. Vizualizare Playlist-uri
- **Comanda în meniul aplicației**: `1`
- Aplicația afișează toate playlisturile ordonate descrescător după rating, împreună cu melodiile incluse.

### 2. Adăugare Playlist
- **Comanda în meniul aplicației**: `2`
- Se cere introducerea numelui playlistului, numărul de melodii și ratingul acestuia.

### 3. Ștergere Playlist
- **Comanda în meniul aplicației**: `3`
- Se afișează toate playlisturile disponibile, iar utilizatorul poate selecta unul pentru a-l șterge.

## Autori
- **Onica Lorena-Andreea**  
- [Profil GitHub](https://github.com/onica-lorena)


