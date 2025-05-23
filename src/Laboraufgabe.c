
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Konstanten definiert
#define MAX_LINE_LENGTH 256 // Maximale Länge einer Zeile
#define MAX_TAGS_LENGTH 128 // Maximale Länge der Tags
#define DELIMITER "," // Trennzeichen für CSV-Datei

// Fasst mehrere zusammengehörige Datenwerte unter einem Namen zusammen
typedef struct {
    char name[50];
    int rating;
    double x_coord;
    double y_coord;
    char tags[MAX_TAGS_LENGTH];
} Sehenswuerdigkeit;

//alle Argumente werden in argv gespeichert, argc gibt die Anzahl der Argumente an 
// Dieser Teil zeigt mit welchen Argumenten das Programm gestartet wurde 
//ausgabe der Argumente in der Konsole
int main(int argc, char *argv[]) {
    printf("PLACES: %i\n", argc);
    for (int i = 0; i < argc; ++i) {
        printf("[%i]: %s\n", i, argv[i]);
    }
//Datei öffnen
    FILE *stream = fopen("../assets/places.csv", "r");
    if (stream == NULL) {
        printf("File not found\n");
        return 1;
    }
    printf("File opened\n");

//Benutzereingabe für das minimale Rating
    int min_rating;
    printf("Gib das minimale Rating für die Sehenswuerdigkeiten ein, die besucht werden sollen: ");
    scanf("%d", &min_rating);  //scanf erfasst benutzereingabe

//Datei wird Zeile für Zeile verarbeitet
    char line[MAX_LINE_LENGTH];  //Zeile wird in line gespeichert
    Sehenswuerdigkeit ort;  //ort ist die Struktur, in die jede Zeile geparsed wird
    int gefundene_orte = 0; //zählt die gefundenen Orte

    //Erste Zeile ist vermutlich der Header z. B. Name,Rating,X,Y,Tags → wird übersprungen
    fgets(line, sizeof(line), stream); 

    //liest die Zeile bis zum Ende der Datei
    while (fgets(line, sizeof(line), stream)) {
     
    //Tokenisierung (Aufteilen an Kommas)    
	char *token = strtok(line, DELIMITER); //erste Spalte:Name
        if (token == NULL) continue;     //falls kein token gefunden dann schleife fortsetzen
        strncpy(ort.name, token, sizeof(ort.name) - 1); //kopiert den Namen in das Feld ort.name
        ort.name[sizeof(ort.name) - 1] = '\0';

    //weitere Felder einlesen    
        token = strtok(NULL, DELIMITER);
        if (token == NULL) continue;
        ort.rating = atoi(token);  //atoi wandelt wert von char in int, Bewertungen

        token = strtok(NULL, DELIMITER); //holt x koordinaten
        if (token == NULL) continue;
        ort.x_coord = atof(token); //atof wandelt wert von char in double

        token = strtok(NULL, DELIMITER);
        if (token == NULL) continue;
        ort.y_coord = atof(token); //holt y koordinaten

        token = strtok(NULL, "\n");
        if (token == NULL) continue;
        strncpy(ort.tags, token, sizeof(ort.tags) - 1);
        ort.tags[sizeof(ort.tags) - 1] = '\0';

        // Prüfung auf genügendes Rating
        // Wenn das Rating größer oder gleich dem minimalen Rating ist, wird die Sehenswürdigkeit ausgegeben
        if (ort.rating >= min_rating) {
            printf("\nSehenswürdigkeit:\n");
            printf("Name: %s\n", ort.name);
            printf("Rating: %d\n", ort.rating);
            printf("X-Coordinate: %.2lf\n", ort.x_coord);
            printf("Y-Coordinate: %.2lf\n", ort.y_coord);
            printf("Tags: %s\n", ort.tags);
            gefundene_orte = 1;
        }
    }

    // Falls keine Sehenswürdigkeiten gefunden wurden, wird eine Nachricht ausgegeben
    if (!gefundene_orte) {
        printf("Keine Sehenswuerdigkeiten mit dem Rating %d gefunden.\n", min_rating);
    }

    // Datei schließen
    fclose(stream);
    return 0;
}
