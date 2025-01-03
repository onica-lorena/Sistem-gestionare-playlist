#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLAYLISTS 100
#define MAX_MELODIES 100

struct Melodie {
    char nume_melodie[30];
    char nume_artist[30];
};

struct Playlist {
    char nume[30];
    int nr_melodii;
    double rating;
    struct Melodie melodii[MAX_MELODIES];
};

// Funcție pentru a elimina extensia din numele fișierului
void eliminaExtensie(char *nume_fisier) {
    char *punct = strrchr(nume_fisier, '.');
    if (punct != NULL) {
        *punct = '\0';
    }
}

// Functie pentru a actualiza numerul de melodii dupa adaugarea/stergerea unei melodii
void actualizeazaNumarMelodii(const char *playlisturi_file_name, const char *playlist_file_name, int delta) {
    FILE *playlisturi_file = fopen(playlisturi_file_name, "r");
    if (playlisturi_file == NULL) {
        perror("Eroare la deschiderea fisierului de playlisturi");
        return;
    }

    FILE *temp_file = fopen("temp_playlisturile.txt", "w");
    if (temp_file == NULL) {
        perror("Eroare la deschiderea fisierului temporar");
        fclose(playlisturi_file);
        return;
    }

    char playlist_nume[30];
    int nr_melodii;
    double rating;
    int playlist_gasit = 0;

    // Eliminăm extensia din numele playlist-ului căutat
    char playlist_file_name_no_ext[30];
    strcpy(playlist_file_name_no_ext, playlist_file_name);
    eliminaExtensie(playlist_file_name_no_ext);

    while (fscanf(playlisturi_file, "%s %d %lf", playlist_nume, &nr_melodii, &rating) == 3) {
        // Eliminăm extensia din numele playlist-ului curent pentru a compara fără extensie
        char current_playlist_no_ext[30];
        strcpy(current_playlist_no_ext, playlist_nume);
        eliminaExtensie(current_playlist_no_ext);

        // Verificăm dacă numele playlist-ului curent (fără extensie) coincide cu numele playlist-ului căutat (fără extensie)
        if (strcmp(current_playlist_no_ext, playlist_file_name_no_ext) == 0) {
            nr_melodii += delta;
            playlist_gasit = 1;
        }
        // Scriem datele în fișierul temporar
        fprintf(temp_file, "%s %d %.2f\n", playlist_nume, nr_melodii, rating);
    }

    fclose(playlisturi_file);
    fclose(temp_file);

    if (!playlist_gasit) {
        printf("Playlist-ul \"%s\" nu a fost gasit in fisierul de playlisturi.\n", playlist_file_name);
    } else {
        // Ștergem fișierul original și redenumim fișierul temporar pentru a lua locul acestuia
        if (remove(playlisturi_file_name) != 0) {
            perror("Eroare la stergerea fisierului original");
        }

        if (rename("temp_playlisturile.txt", playlisturi_file_name) != 0) {
            perror("Eroare la redenumirea fisierului temporar");
        }
    }
}


void adaugaMelodie(const char *playlist_file_name, const char *playlisturi_file_name) {
    FILE *playlist_file = fopen(playlist_file_name, "a");
    if (playlist_file == NULL) {
        perror("Eroare la deschiderea fisierului de playlist");
        return;
    }

    struct Melodie melodie;
    printf("\033[1;34mIntroduceti detaliile melodiei:\033[0m\n");
    printf("\033[1;36mNume melodie: \033[0m");
    scanf(" %[^\n]", melodie.nume_melodie);
    printf("\033[1;36mNume artist: \033[0m");
    scanf(" %[^\n]", melodie.nume_artist);

    fprintf(playlist_file, "\n%s\n%s", melodie.nume_melodie, melodie.nume_artist);

    fclose(playlist_file);

    actualizeazaNumarMelodii(playlisturi_file_name, playlist_file_name, 1);
}



void stergeMelodie(const char *playlist_file_name, const char *playlisturi_file_name) {
    FILE *file = fopen(playlist_file_name, "r");
    if (file == NULL) {
        perror("Failed to open playlist file for reading");
        return;
    }

    int index = 1, selected;
    char line[100];
    printf("Melodii disponibile pentru stergere:\n");
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] != '\n') {
            line[strcspn(line, "\n")] = 0;
            printf("%d. %s\n", index, line);
            fgets(line, sizeof(line), file);
            index++;
        }
    }
    fclose(file);

    printf("Introduceti numarul melodiei pe care doriti sa o stergeti: ");
    scanf("%d", &selected);
    if (selected < 1 || selected >= index) {
        printf("Numarul melodiei este invalid!\n");
        return;
    }

    file = fopen(playlist_file_name, "r");
    FILE *temp_file = fopen("temp_playlist.txt", "w");
    if (temp_file == NULL) {
        perror("Failed to open temporary file for writing");
        fclose(file);
        return;
    }

    int current = 1;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (current != selected) {
            fputs(line, temp_file);
            fgets(line, sizeof(line), file);
            fputs(line, temp_file);
        } else {
            fgets(line, sizeof(line), file);
        }
        current++;
    }

    fclose(file);
    fclose(temp_file);

    remove(playlist_file_name);
    rename("temp_playlist.txt", playlist_file_name);

    actualizeazaNumarMelodii(playlisturi_file_name, playlist_file_name, -1);
}


void vizualizarePlaylisturi(const char *playlisturi_file_name) {
    FILE *file = fopen(playlisturi_file_name, "r");
    if (file == NULL) {
        perror("Eroare la deschiderea fisierului playlisturilor");
        return;
    }

    struct Playlist playlisturi[MAX_PLAYLISTS];
    int nr_playlisturi = 0;

    // Citim playlisturile și le încărcăm în array
    char playlist_nume[30];
    int nr_melodii;
    double rating;
    while (fscanf(file, "%s %d %lf", playlist_nume, &nr_melodii, &rating) == 3 && nr_playlisturi < MAX_PLAYLISTS) {
        strcpy(playlisturi[nr_playlisturi].nume, playlist_nume);
        playlisturi[nr_playlisturi].nr_melodii = nr_melodii;
        playlisturi[nr_playlisturi].rating = rating;
        nr_playlisturi++;
    }

    fclose(file);

    // Sortăm playlisturile în ordine descrescătoare după rating
    for (int i = 0; i < nr_playlisturi - 1; i++) {
        for (int j = i + 1; j < nr_playlisturi; j++) {
            if (playlisturi[i].rating < playlisturi[j].rating) {
                struct Playlist temp = playlisturi[i];
                playlisturi[i] = playlisturi[j];
                playlisturi[j] = temp;
            }
        }
    }

    // Afișăm playlisturile sortate
    for (int i = 0; i < nr_playlisturi; i++) {
        printf("\033[1;32m%s (%d melodii, Rating: %.2f):\033[0m\n", playlisturi[i].nume, playlisturi[i].nr_melodii, playlisturi[i].rating);
        char playlist_file_name[40]; // Alocăm spațiu pentru numele fișierului playlist-ului
        sprintf(playlist_file_name, "%s.txt", playlisturi[i].nume); // Construim numele fișierului playlist-ului

        // Afisam melodiile din playlist
        FILE *playlist_file = fopen(playlist_file_name, "r");
        if (playlist_file == NULL) {
            perror("Eroare la deschiderea fisierului de playlist");
            return;
        }

        char buffer[100]; // Alocăm un buffer pentru citirea fiecărei linii
        for (int j = 0; j < playlisturi[i].nr_melodii; j++) {
            fgets(buffer, sizeof(buffer), playlist_file);
            buffer[strcspn(buffer, "\n")] = 0; // Eliminăm caracterul newline, dacă există

            printf("\033[1;36mMelodie: %s\033[0m\n", buffer);
            fgets(buffer, sizeof(buffer), playlist_file);
            buffer[strcspn(buffer, "\n")] = 0; // Eliminăm caracterul newline, dacă există

            printf("      \033[1;36mArtist: %s\033[0m\n", buffer);
        }

        fclose(playlist_file);

        printf("\033[1;93m1. Adauga melodie\033[0m\n");
        printf("\033[1;93m2. Sterge melodie\033[0m\n");
        printf("\033[1;93m3. Revenire\033[0m\n");

        int optiune;
        printf("\033[1;31mIntroduceti optiunea dorita:\033[0m ");
        scanf("%d", &optiune);

        switch (optiune) {
            case 1:
                adaugaMelodie(playlist_file_name, playlisturi_file_name);
                break;
            case 2:
                stergeMelodie(playlist_file_name, playlisturi_file_name);
                break;
            case 3:
                break;
            default:
                printf("Optiune invalida!\n");
        }
    }
}


void adaugaPlaylist(struct Playlist playlisturi[], int *nr_playlisturi) {
    if (*nr_playlisturi >= MAX_PLAYLISTS) {
        printf("Numarul maxim de playlist-uri a fost atins!\n");
        return;
    }

    struct Playlist nou_playlist;

    printf("\033[1;90mIntrodu numele playlist-ului: \033[0m");
    scanf(" %[^\n]", nou_playlist.nume);

    printf("\033[1;32mIntrodu numarul de melodii pentru noul playlist: \033[0m");
    scanf("%d", &nou_playlist.nr_melodii);

    do {
        printf("\033[1;93mIntrodu rating-ul pentru noul playlist (intre 0 si 5): \033[0m");
        scanf("%lf", &nou_playlist.rating);

        if (nou_playlist.rating < 0 || nou_playlist.rating > 5) {
            printf("Rating-ul trebuie sa fie intre 0 si 5!\n");
        }
    } while (nou_playlist.rating < 0 || nou_playlist.rating > 5);

    FILE *file = fopen("playlisturi.txt", "a");
    if (file == NULL) {
        perror("Eroare la deschiderea fisierului pentru adaugarea unui playlist");
        return;
    }
    fprintf(file, "%s %d %.2f\n", nou_playlist.nume, nou_playlist.nr_melodii, nou_playlist.rating);
    fclose(file);

    char nume_fisier[50];
    strcpy(nume_fisier, nou_playlist.nume);
    strcat(nume_fisier, ".txt");

    file = fopen(nume_fisier, "w");
    if (file == NULL) {
        perror("Eroare la crearea fisierului pentru noul playlist");
        return;
    }

    for (int i = 0; i < nou_playlist.nr_melodii; i++) {
        printf("\033[1;31mIntroduceti detaliile melodiei %d:\033[0m\n", i + 1);
        printf("\033[1;36mNume melodie: \033[0m");
        scanf(" %[^\n]", nou_playlist.melodii[i].nume_melodie);
        printf("\033[1;36mNume artist: \033[0m");
        scanf(" %[^\n]", nou_playlist.melodii[i].nume_artist);
        fprintf(file, "%s\n%s\n", nou_playlist.melodii[i].nume_melodie, nou_playlist.melodii[i].nume_artist);
    }

    fclose(file);

    playlisturi[(*nr_playlisturi)++] = nou_playlist;

    printf("Playlist-ul a fost adaugat cu succes!\n");
}

void stergePlaylist(struct Playlist playlisturi[], int *nr_playlisturi) {
    if (*nr_playlisturi == 0) {
        printf("Nu exista playlist-uri de sters!\n");
        return;
    }

    printf("\033[1;34mPlaylist-uri disponibile pentru stergere:\033[0m\n");
    for (int i = 0; i < *nr_playlisturi; i++) {
        printf("\033[1;36m%d. %s (Melodii: %d, Rating: %.2f)\033[0m\n", i + 1, playlisturi[i].nume, playlisturi[i].nr_melodii, playlisturi[i].rating);
    }

    int index_stergere;
    printf("\033[1;31mIntroduceti numarul playlist-ului pe care doriti sa-l stergeti: \033[0m");
    scanf("%d", &index_stergere);

    if (index_stergere < 1 || index_stergere > *nr_playlisturi) {
        printf("Indexul introdus nu este valid!\n");
        return;
    }

    char nume_fisier[50];
    sprintf(nume_fisier, "%s.txt", playlisturi[index_stergere - 1].nume);
    if (remove(nume_fisier) != 0) {
        perror("Eroare la stergerea fisierului playlist-ului");
        return;
    }

    // Elimină playlist-ul din array-ul de playlist-uri
    for (int i = index_stergere - 1; i < *nr_playlisturi - 1; i++) {
        playlisturi[i] = playlisturi[i + 1];
    }
    (*nr_playlisturi)--;

    // Actualizează fișierul playlisturi.txt
    FILE *file = fopen("playlisturi.txt", "w");
    if (file == NULL) {
        perror("Eroare la deschiderea fisierului playlisturi.txt");
        return;
    }

    for (int i = 0; i < *nr_playlisturi; i++) {
        fprintf(file, "%s %d %.2f\n", playlisturi[i].nume, playlisturi[i].nr_melodii, playlisturi[i].rating);
    }

    fclose(file);

    printf("\033[1;34mPlaylist-ul a fost sters cu succes!\033[0m\n");
}


int main() {
    struct Playlist playlisturi[MAX_PLAYLISTS];
    int nr_playlisturi = 0;

    FILE *playlist_file = fopen("playlisturi.txt", "r");
    if (playlist_file == NULL) {
        perror("Eroare la deschiderea fisierului de playlist-uri");
        return 1;
    }

    // Citirea playlist-urilor din fisier și încărcarea lor în array
    char playlist_nume[30];
    int nr_melodii;
    double rating;
    while (fscanf(playlist_file, "%s %d %lf", playlist_nume, &nr_melodii, &rating) == 3 && nr_playlisturi < MAX_PLAYLISTS) {
        strcpy(playlisturi[nr_playlisturi].nume, playlist_nume);
        playlisturi[nr_playlisturi].nr_melodii = nr_melodii;
        playlisturi[nr_playlisturi].rating = rating;
        nr_playlisturi++;
    }

    fclose(playlist_file);

    while (1) {
        system("cls");
        printf("\033[1;95m-= Sistem gestionare playlist-uri =-\033[0m\n");
        printf("\033[1;33m1. Vizualizare playlist-uri\033[0m\n");
        printf("\033[1;33m2. Adaugare playlist\033[0m\n");
        printf("\033[1;33m3. Stergere playlist\033[0m\n");
        printf("\033[1;33m4. Iesire\033[0m\n");

        int choice = 0;
        while (choice < 1 || choice > 4) {
            printf("\033[1;31mIntrodu optiunea dorita:\033[0m ");
            scanf("%d", &choice);
        }
        system("cls");

        if (choice == 1) {
            printf("\033[1;95m-= Vizualizare playlist-uri =-\033[0m\n");
            vizualizarePlaylisturi("playlisturi.txt");
            system("pause");
        } else if (choice == 2) {
            printf("\033[1;95m-= Adaugare playlist =-\033[0m\n");
            adaugaPlaylist(playlisturi, &nr_playlisturi);
            system("pause");
        } else if (choice == 3) {
            printf("\033[1;95m-= Stergere playlist =-\033[0m\n");
            stergePlaylist(playlisturi, &nr_playlisturi);
            system("pause");
        } else if (choice == 4) {
            return 0;
        }
    }

    return 0;
}
