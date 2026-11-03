
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Person {
    char name[25];
    char surname[25];
    int year_of_birth;
};

typedef struct Node* Position;

struct Node {
    struct Person element;
    Position next;
};

Position create_node();
int push_to_front(Position, struct Person, Position);
int push_to_back(Position, struct Person, Position);
int print_list(Position);
int delete_list(Position);
struct Person enter_person_data();
char* string_input(const char*);
Position find_surname(Position, char[]);
int delete_element(Position, Position);
int add_after_node(Position, int, struct Person, Position);
int add_before_node(Position, int, struct Person, Position);
int write_list_to_file(Position, char[]);
Position read_list_from_file(char[]);

int main() {
    Position p = create_node();

    if (p == NULL)
        return 1;

    char input;

    do {
        printf("\nUnesi slovo:\n");
        printf("a) Dodaj element na pocetak liste\n");
        printf("b) Ispisi listu\n");
        printf("c) Dodaj element na kraj liste\n");
        printf("d) Nadi element po prezimenu\n");
        printf("e) Izbrisi element iz liste\n");
        printf("f) Dodaj novi element iza nekog elementa\n");
        printf("g) Dodaj novi element prije nekog elementa\n");
        printf("h) Upisi listu u datoteku\n");
        printf("i) Procitaj listu iz datoteke\n");
        printf("x) Izlaz\n");
        printf("SLOVO: ");

        if (scanf(" %c", &input) != 1)
            break;

        switch (input) {

        case 'a': {
            struct Person novaOsoba = enter_person_data();

            Position newNode = create_node();

            if (newNode == NULL) {
                delete_list(p);
                free(p);
                return 1;
            }

            push_to_front(p, novaOsoba, newNode);
            break;
        }

        case 'b':
            print_list(p);
            break;

        case 'c': {
            struct Person novaOsoba = enter_person_data();

            Position node = create_node();

            if (node == NULL) {
                delete_list(p);
                free(p);
                return 1;
            }

            push_to_back(p, novaOsoba, node);
            break;
        }

        case 'd': {
            char* prezime = string_input("Unesi prezime osobe:");

            if (prezime == NULL)
                break;

            Position node = find_surname(p, prezime);

            if (node == NULL)
                printf("Osoba nije pronadena!\n");
            else
                printf("Pronadena: %s %s %d\n",
                    node->element.name,
                    node->element.surname,
                    node->element.year_of_birth);

            free(prezime);
            break;
        }

        case 'e': {
            char* prezime = string_input("Unesi prezime osobe za brisanje:");

            if (prezime == NULL)
                break;

            Position node = find_surname(p, prezime);

            if (node == NULL)
                printf("Osoba nije pronadena!\n");
            else
                delete_element(p, node);

            free(prezime);
            break;
        }

        case 'f': {
            int index;

            printf("Unesi indeks: ");

            if (scanf("%d", &index) != 1) {
                printf("Neispravan unos!\n");
                while (getchar() != '\n' && !feof(stdin));
                break;
            }

            Position node = create_node();

            if (node == NULL)
                break;

            struct Person osoba = enter_person_data();

            add_after_node(p, index, osoba, node);
            break;
        }

        case 'g': {
            int index;

            printf("Unesi indeks: ");

            if (scanf("%d", &index) != 1) {
                printf("Neispravan unos!\n");
                while (getchar() != '\n' && !feof(stdin));
                break;
            }

            Position node = create_node();

            if (node == NULL)
                break;

            struct Person osoba = enter_person_data();

            add_before_node(p, index, osoba, node);
            break;
        }

        case 'h': {
            char* filename = string_input("Unesi naziv datoteke:");

            if (filename == NULL)
                break;

            if (write_list_to_file(p, filename) != 0)
                printf("Greska pri upisu u datoteku!\n");

            free(filename);
            break;
        }

        case 'i': {
            char* filename = string_input("Unesi naziv datoteke:");

            if (filename == NULL)
                break;

            Position readP = read_list_from_file(filename);

            if (readP == NULL) {
                printf("Greska pri citanju liste iz datoteke!\n");
            }
            else {
                delete_list(p);
                free(p);
                p = readP;
            }

            free(filename);
            break;
        }

        case 'x':
            break;

        default:
            printf("Nepoznata opcija!\n");
            break;
        }

    } while (input != 'x');

    delete_list(p);
    free(p);

    return 0;
}

Position create_node() {
    Position node = (Position)malloc(sizeof(struct Node));

    if (node == NULL) {
        printf("ERROR: nije alocirana memorija!\n");
        return NULL;
    }

    node->next = NULL;

    return node;
}

int push_to_front(Position p, struct Person osoba, Position node) {
    if (p == NULL || node == NULL)
        return 1;

    node->element = osoba;
    node->next = p->next;
    p->next = node;

    return 0;
}

int push_to_back(Position p, struct Person osoba, Position node) {
    if (p == NULL || node == NULL)
        return 1;

    node->element = osoba;
    node->next = NULL;

    Position current = p;

    while (current->next != NULL)
        current = current->next;

    current->next = node;

    return 0;
}

int print_list(Position p) {
    if (p == NULL)
        return 1;

    Position current = p->next;

    if (current == NULL) {
        printf("Lista je prazna.\n");
        return 0;
    }

    int i = 1;

    while (current != NULL) {
        printf("Osoba %d: %s %s %d\n",
            i++,
            current->element.name,
            current->element.surname,
            current->element.year_of_birth);

        current = current->next;
    }

    return 0;
}

Position find_surname(Position p, char prezime[]) {
    if (p == NULL || prezime == NULL)
        return NULL;

    Position currentNode = p->next;

    while (currentNode != NULL) {
        if (strcmp(currentNode->element.surname, prezime) == 0)
            return currentNode;

        currentNode = currentNode->next;
    }

    return NULL;
}

int delete_element(Position p, Position node) {
    if (p == NULL || node == NULL)
        return 1;

    Position currentNode = p;

    while (currentNode->next != NULL) {
        if (currentNode->next == node) {
            currentNode->next = node->next;
            free(node);
            printf("Element izbrisan!\n");
            return 0;
        }

        currentNode = currentNode->next;
    }

    printf("Osoba nije pronadena!\n");

    return 1;
}

int delete_list(Position p) {
    if (p == NULL)
        return 1;

    Position current = p->next;

    while (current != NULL) {
        Position next = current->next;
        free(current);
        current = next;
    }

    p->next = NULL;

    return 0;
}

struct Person enter_person_data() {
    struct Person osoba;

    printf("Unesi ime osobe: ");
    scanf("%24s", osoba.name);

    printf("Unesi prezime osobe: ");
    scanf("%24s", osoba.surname);

    printf("Unesi godinu rodenja: ");

    while (scanf("%d", &osoba.year_of_birth) != 1) {
        if (feof(stdin)) {
            osoba.year_of_birth = 0;
            break;
        }

        printf("Greska! Pokusaj ponovo: ");

        while (getchar() != '\n' && !feof(stdin));
    }

    return osoba;
}

char* string_input(const char* output) {
    printf("%s ", output);

    char* string = (char*)malloc(256 * sizeof(char));

    if (string == NULL) {
        printf("ERROR: alokacija memorije!\n");
        return NULL;
    }

    if (scanf("%255s", string) != 1) {
        free(string);
        return NULL;
    }

    return string;
}

int add_after_node(Position p, int targetIndex, struct Person osoba, Position newNode) {
    if (p == NULL || newNode == NULL || targetIndex < 1) {
        free(newNode);
        return 1;
    }

    Position pointer = p->next;
    int i = 1;

    while (pointer != NULL) {
        if (i == targetIndex) {
            newNode->element = osoba;
            newNode->next = pointer->next;
            pointer->next = newNode;

            printf("Osoba uspjesno dodana nakon elementa s indeksom %d!\n", targetIndex);

            return 0;
        }

        pointer = pointer->next;
        i++;
    }

    printf("Indeks nije pronaden!\n");
    free(newNode);

    return 1;
}

int add_before_node(Position p, int targetIndex, struct Person osoba, Position newNode) {
    if (p == NULL || newNode == NULL || targetIndex < 1) {
        free(newNode);
        return 1;
    }

    Position pointer = p;
    int i = 1;

    while (pointer->next != NULL) {
        if (i == targetIndex) {
            newNode->element = osoba;
            newNode->next = pointer->next;
            pointer->next = newNode;

            printf("Osoba uspjesno dodana prije elementa s indeksom %d!\n", targetIndex);

            return 0;
        }

        pointer = pointer->next;
        i++;
    }

    printf("Indeks nije pronaden!\n");
    free(newNode);

    return 1;
}

int write_list_to_file(Position p, char filename[]) {
    if (p == NULL)
        return 1;

    FILE* fp = fopen(filename, "w");

    if (fp == NULL) {
        printf("ERROR: datoteka nije otvorena!\n");
        return 1;
    }

    Position current = p->next;
    int i = 1;

    while (current != NULL) {
        if (fprintf(fp, "Osoba %d: %s %s %d\n",
            i++,
            current->element.name,
            current->element.surname,
            current->element.year_of_birth) < 0) {

            fclose(fp);
            return 1;
        }

        current = current->next;
    }

    if (fclose(fp) != 0)
        return 1;

    printf("Uspjesno upisivanje u datoteku!\n");

    return 0;
}

Position read_list_from_file(char filename[]) {
    FILE* fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Greska: datoteka nije otvorena!\n");
        return NULL;
    }

    Position p = create_node();

    if (p == NULL) {
        fclose(fp);
        return NULL;
    }

    int id;
    struct Person osoba;
    int result;

    while ((result = fscanf(fp, "Osoba %d: %24s %24s %d",
        &id,
        osoba.name,
        osoba.surname,
        &osoba.year_of_birth)) == 4) {

        Position node = create_node();

        if (node == NULL) {
            fclose(fp);
            delete_list(p);
            free(p);
            return NULL;
        }

        if (push_to_back(p, osoba, node) != 0) {
            free(node);
            fclose(fp);
            delete_list(p);
            free(p);
            return NULL;
        }
    }

    if (ferror(fp) || result != EOF) {
        printf("Greska: neispravan format datoteke!\n");
        fclose(fp);
        delete_list(p);
        free(p);
        return NULL;
    }

    fclose(fp);

    printf("Uspjesno ucitana lista iz datoteke!\n");

    return p;
}
