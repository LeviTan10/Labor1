#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Konstante Definitionen 
#define MAX_LINE_LENGTH 256
#define MAX_TAGS_LENGTH 128
#define MAX_SEHENSW 1000
#define DELIMITER ","

//  Datenstruktur für Sehenswürdigkeit 
typedef struct {
    char name[50];
    int rating;
    double x_coord;
    double y_coord;
    char tags[MAX_TAGS_LENGTH];
} Sehenswuerdigkeit;

//  Globale Liste von Sehenswürdigkeiten 
Sehenswuerdigkeit orte[MAX_SEHENSW];
int anzahl_orte = 0;

//  Zwei Orte tauschen (für Quicksort) 
void swap(Sehenswuerdigkeit *a, Sehenswuerdigkeit *b) {
    Sehenswuerdigkeit temp = *a;
    *a = *b;
    *b = temp;
}

// Partitionierung für Quicksort nach Rating 
int partition(Sehenswuerdigkeit arr[], int low, int high) {
    int pivot = arr[high].rating;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j].rating <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

//  Quicksort-Aufruf 
void quicksort(Sehenswuerdigkeit arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

//  Einzelne Sehenswürdigkeit ausgeben 
void ausgabe(Sehenswuerdigkeit *s) {
    printf("\nName: %s\n", s->name);
    printf("Rating: %d\n", s->rating);
    printf("Koordinaten: (%.2lf, %.2lf)\n", s->x_coord, s->y_coord);
    printf("Tags: %s\n", s->tags);
}

// Entfernung berechnen
// Hier quadrieren wir Differenzen
double distanz_quadrat(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return dx * dx + dy * dy;
}

//  CSV-Datei laden 
void lade_csv(const char *filename) {
    FILE *stream = fopen(filename, "r");
    if (!stream) {
        printf("Fehler: Datei konnte nicht geöffnet werden.\n");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    fgets(line, sizeof(line), stream); // Header überspringen

    while (fgets(line, sizeof(line), stream)) {
        Sehenswuerdigkeit ort;

        char *token = strtok(line, DELIMITER);
        if (!token) continue;
        strncpy(ort.name, token, sizeof(ort.name) - 1);

        token = strtok(NULL, DELIMITER);
        if (!token) continue;
        ort.rating = atoi(token);

        token = strtok(NULL, DELIMITER);
        if (!token) continue;
        ort.x_coord = atof(token);

        token = strtok(NULL, DELIMITER);
        if (!token) continue;
        ort.y_coord = atof(token);

        token = strtok(NULL, "\n");
        if (!token) continue;
        strncpy(ort.tags, token, sizeof(ort.tags) - 1);

        if (anzahl_orte < MAX_SEHENSW) {
            orte[anzahl_orte++] = ort;
        }
    }

    fclose(stream);
}

//  Orte nach Bewertung filtern 
void filter_rating(char op, int wert) {
    int gefunden = 0;

    for (int i = 0; i < anzahl_orte; i++) {
        if ((op == '>' && orte[i].rating > wert) ||
            (op == '<' && orte[i].rating < wert) ||
            (op == '=' && orte[i].rating == wert)) {
            ausgabe(&orte[i]);
            gefunden = 1;
        }
    }

    if (!gefunden) {
        printf("Keine Orte mit Bedingung gefunden.\n");
    }
}

//  Die n nächsten Orte zu Koordinate (x, y) finden 
void finde_naechste(double x, double y, int n) {
    double dists[MAX_SEHENSW];
    int verwendet[MAX_SEHENSW] = {0}; // Array zur Markierung verwendeter Orte

    for (int i = 0; i < anzahl_orte; i++) {
        dists[i] = distanz_quadrat(x, y, orte[i].x_coord, orte[i].y_coord);
    }

    for (int k = 0; k < n; k++) {
        int minIndex = -1;
        double minDist = 1e10;

        for (int j = 0; j < anzahl_orte; j++) {
            if (!verwendet[j] && dists[j] < minDist) {
                minDist = dists[j];
                minIndex = j;
            }
        }

        if (minIndex != -1) {
            ausgabe(&orte[minIndex]);
            verwendet[minIndex] = 1;
        }
    }
}

//  Hauptfunktion mit Menü 
int main() {
    lade_csv("../assets/places.csv");  // Pfad ggf. anpassen
    quicksort(orte, 0, anzahl_orte - 1); // Nach Bewertung sortieren

    int auswahl;

    while (1) {
        printf("\n--- Menü ---\n");
        printf("1) Orte mit Rating > x\n");
        printf("2) Orte mit Rating < x\n");
        printf("3) Orte mit Rating = x\n");
        printf("4) Nächste n Orte zu Koordinaten\n");
        printf("5) Beenden\n");
        printf("Auswahl: ");
        scanf("%d", &auswahl);

        if (auswahl >= 1 && auswahl <= 3) {
            int wert;
            printf("Geben Sie das Rating ein: ");
            scanf("%d", &wert);
            if (auswahl == 1) filter_rating('>', wert);
            if (auswahl == 2) filter_rating('<', wert);
            if (auswahl == 3) filter_rating('=', wert);
        } else if (auswahl == 4) {
            double x, y;
            int n;
            printf("Standort eingeben (x y): ");
            scanf("%lf %lf", &x, &y);
            printf("Anzahl nächster Orte: ");
            scanf("%d", &n);
            finde_naechste(x, y, n);
        } else if (auswahl == 5) {
            break; // Programm beenden
        } else {
            printf("Ungültige Eingabe.\n");
        }
    }

    return 0;
}


