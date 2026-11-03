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
char* string_input(char*);
Position find_surname(Position, char[]);
int delete_element(Position, Position);


int main() {
    Position p = create_node();
    if (p == NULL) {
        return 1;
    }

    char input;

    do {
        printf("Unesi slovo:\n");
        printf("a) Dodaj element na pocetak liste\n");
        printf("b) Ispisi listu\n");
        printf("c) Dodaj element na kraj liste\n");
        printf("d) Nadi element po prezimenu\n");
        printf("e) Izbrisi element iz liste\n");
        printf("x) Izlaz\n");
        printf("SLOVO: ");

        scanf(" %c", &input);

        switch (input) {
        case 'a': {
            struct Person novaOsoba = enter_person_data();
            Position newNode = create_node();
            if (newNode != NULL)
                push_to_front(p, novaOsoba, newNode);
            break;
        }
        case 'b':
            print_list(p);
            break;
        case 'c': {
            Position node = create_node();
            if (node != NULL)
                push_to_back(p, enter_person_data(), node);
            break;
        }
        case 'd': {
            char* prezime = string_input("Unesi prezime osobe:");
            if (prezime != NULL) {
                Position node = find_surname(p, prezime);
                if (node == NULL)
                    printf("Osoba nije pronadena!\n");
                else
                    printf("Pronadena: %s %s\n", node->element.name, node->element.surname);
                free(prezime);
            }
            break;
        }
        case 'e': {
            char* prezime = string_input("Unesi prezime osobe:");
            if (prezime != NULL) {
                Position node = find_surname(p, prezime);
                if (node == NULL)
                    printf("Osoba nije pronadena!\n");
                else
                    delete_element(p, node);
                free(prezime);
            }
            break;
        }
        }
    } while (input != 'x');

    delete_list(p);
    free(p);

    return 0;
}

Position create_node() {
    struct Node* node = malloc(sizeof(struct Node));
    if (node == NULL) {
        printf("ERROR: nije alocirana memorija! \n");
        return NULL;
    }
    node->next = NULL;
    return node;
}

int push_to_front(Position p, struct Person osoba, Position node) {

    if (p == NULL || node == NULL) {
        printf("ERROR, push to front\n");
        return 1;
    }
    node->element = osoba;
    node->next = p->next;
    p->next = node;
    return 0;
}

int print_list(Position p) {

    if (p == NULL) {
        printf("ERROR, print list\n");
        return 1;
    }

    Position current = p->next;
    
    if (current == NULL) {
        printf("Lista je prazna.\n");
        return 0;
    }

    int i = 1;

    while (current != NULL) {
        printf("Osoba %d: %s %s %d\n", i++, current->element.name, 
            current->element.surname, current->element.year_of_birth);

        current = current->next;
    }
    return 0;
}

int push_to_back(Position p, struct Person osoba, Position node) {

    if (p == NULL || node == NULL) {
        printf("ERROR, push to back\n");
        return 1;
    }
    node->element = osoba;
    node->next = NULL;

    Position current = p;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = node;
    return 0;
}

Position find_surname(Position p, char prezime[]) {
    
    if (p == NULL || prezime == NULL) {
        printf("ERROR, find surname\n");
        return NULL;
    }

    Position currentNode = p->next;
    while (currentNode != NULL) {
        if (strcmp(currentNode->element.surname, prezime) == 0)
            return currentNode;
        currentNode = currentNode->next;
    }
    return NULL;

}

int delete_element(Position p, Position node) {
    
    if (p == NULL || node == NULL) {
        printf("ERROR, delete element\n");
        return 1;
    }

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
    if (p == NULL) {
        printf("ERROR, delete list\n");
        return 1;
    }

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
        printf("Greska! Pokusaj ponovo: ");
        while (getchar() != '\n');
    }

    return osoba;
}

char* string_input(char* output) {
    printf("%s ", output);
    char* string = malloc(25 * sizeof(char));
    if (string == NULL) {
        printf("ERROR, alokacija memorije\n");
        return NULL;
    }
    scanf("%24s", string);
    return string;
}
