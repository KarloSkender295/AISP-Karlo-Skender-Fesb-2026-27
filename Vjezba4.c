#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Term {
    float koef;
    float exp;
};

typedef struct Node* Position;

struct Node {
    struct Term element;
    Position next;
};

Position create_node();
int add_to_start(Position, struct Term, Position);
int add_to_end(Position, struct Term, Position);
int print_list(Position);
int delete_list(Position);
int delete_node(Position, Position);
int add_after_node(Position, int, struct Term, Position);
int add_before_node(Position, int, struct Term, Position);
Position read_list();
int add_sorted_node(Position, struct Term, Position);
int polynomial_sort(Position*);
int polynomial_clean(Position*);
Position join_lists(Position*, int);
Position multiply(Position*, int);

int main() {
    int n = 0;

    do
    {
        printf("Unesi broj polinoma: ");
        scanf_s(" %d", &n);
    } while (n <= 0);


    Position* headers = malloc(sizeof(Position) * n);

    if (headers == NULL) {
        printf("ERROR: alokacija memorije headers\n");
        return 1;
    }

    for (int i = 0; i < n; i++)
    {
        printf("%d ", i + 1);
        headers[i] = read_list();
        if (headers[i] == NULL)
            i--;
    }

    char input = '0';
    do {
        printf("\nUnesi znak:\n+) Zbrajanje polinoma\n*) Mnozenje polinoma\nx) Izlaz\n");
        printf("Odabir: ");
        scanf(" %c", &input);

        switch (input) {
        case '+': {
            Position joinedListHeader = join_lists(headers, n);
            if (joinedListHeader != NULL) {
                polynomial_clean(&joinedListHeader);
                printf("Rezultat zbrajanja je: ");
                print_list(joinedListHeader);
                delete_list(joinedListHeader);
                free(joinedListHeader);
            }
            printf("\n");
            break;
        }
        case '*': {
            Position outputHeader = multiply(headers, n);
            if (outputHeader != NULL) {
                printf("Rezultat mnozenja je: ");
                print_list(outputHeader);
                delete_list(outputHeader);
                free(outputHeader);
            }
            printf("\n");
            break;
        }
        case 'x':
            break;
        default:
            printf("Nepoznata radnja!\n");
            break;
        }
    } while (input != 'x');

    for (int i = 0; i < n; i++)
    {
        if (headers[i] != NULL) {
            delete_list(headers[i]);
            free(headers[i]);
        }
    }
    free(headers);
    return 0;
}


Position create_node() {
    Position node = malloc(sizeof(struct Node));
    if (node == NULL) {
        printf("ERROR, alokacija cvora\n");
        return NULL;
    }
    node->next = NULL;
    return node;
}

int add_to_start(Position p, struct Term term, Position node) {
    if (p == NULL || node == NULL) {
        printf("ERROR, add to start\n");
        return 1;
    }
    node->element = term;
    node->next = p->next;
    p->next = node;
    return 0;
}

int add_to_end(Position p, struct Term term, Position node) {
    if (p == NULL || node == NULL) {
        printf("ERROR, add to end\n");
        return 1;
    }
    node->element = term;
    node->next = NULL;

    Position current = p;
    while (current->next != NULL)
        current = current->next;

    current->next = node;
    return 0;
}

int print_list(Position p) {
    if (p == NULL) {
        printf("ERROR, print list\n");
        return 1;
    }

    Position current = p->next;
    int i = 1;
    if (current == NULL) {
        printf("Lista je prazna.\n");
        return 2;
    }

    while (current != NULL) {
        if (current->element.exp == 0)
            printf("%.2f ", current->element.koef);
        else
            printf("%.2fx^%.2f ", current->element.koef, current->element.exp);
        if (current->next != NULL)
            printf("+ ");
        current = current->next;
    }
    return 0;
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

int delete_node(Position p, Position node) {
    if (p == NULL || node == NULL) {
        printf("ERROR, delete node\n");
        return 1;
    }

    Position currentNode = p;
    while (currentNode->next != NULL) {
        if (currentNode->next == node) {
            currentNode->next = node->next;
            free(node);
            printf("Element izbrisan iz liste\n");
            return 0;
        }
        currentNode = currentNode->next;
    }

    printf("term nije pronaden\n");
    return 1;
}

int add_after_node(Position p, int targetIndex, struct Term term, Position newNode) {
    if (p == NULL || newNode == NULL) {
        printf("ERROR, add after node\n");
        return 1;
    }

    Position pointer = p->next;
    int i = 0;
    while (pointer != NULL) {
        if (i == targetIndex) {
            newNode->element = term;
            newNode->next = pointer->next;
            pointer->next = newNode;
            printf("Clan dodan nakon elementa %d\n", targetIndex);
            return 0;
        }
        pointer = pointer->next;
        i++;
    }

    printf("Indeks nije pronaden!\n");
    free(newNode);
    return 1;
}

int add_before_node(Position p, int targetIndex, struct Term term, Position newNode) {
    if (p == NULL || newNode == NULL) {
        printf("ERROR, add before node\n");
        return 1;
    }

    Position pointer = p;
    int i = 0;
    while (pointer->next != NULL) {
        if (i == targetIndex) {
            newNode->element = term;
            newNode->next = pointer->next;
            pointer->next = newNode;
            printf("Clan dodan prije elementa %d!\n", targetIndex);
            return 0;
        }
        pointer = pointer->next;
        i++;
    }

    printf("Indeks nije pronaden!\n");
    free(newNode);
    return 1;
}

Position read_list() {

    FILE* file = NULL;
    char fileName[40] = "";
    printf("Unesi ime datoteke: ");
    scanf("%39s", fileName);

    fopen_s(&file, fileName, "r");

    if (file == NULL) {
        printf("ERROR, otvaranje datoteke\n");
        return NULL;
    }


    Position header = create_node();
    if (header == NULL) {
        printf("ERROR, read list alokacija");
        fclose(file);
        return NULL;
    }
       

    
    struct Term term;
    while (fscanf(file, "%f %f", &term.koef, &term.exp) == 2)
    {
        Position newNode = create_node();
        if (newNode == NULL) {
            printf("ERROR: alokacija memorije u read list\n");
            fclose(file);
            delete_list(header);
            free(header);
            return NULL;
        }

        add_sorted_node(header, term, newNode);
    }

    fclose(file);

    return header;
}

int add_sorted_node(Position p, struct Term term, Position newNode) {
    if (p == NULL || newNode == NULL)
    {
        printf("ERROR, ucitavanje liste");
        return 1;
    }

    Position current = p;
    while (current->next != NULL) {
        if (term.exp >= current->next->element.exp)
            break;
        current = current->next;
    }

    newNode->element = term;
    newNode->next = current->next;
    current->next = newNode;

    return 0;
}

int polynomial_sort(Position* p) {
    if (p == NULL || *p == NULL)
    {
        printf("ERROR, ucitavanje liste");
        return 1;
    }
    if ((*p)->next == NULL) {
        printf("Lista je prazna!");
        return 0;
    }

    Position newHeader = create_node();
    if (newHeader == NULL)
        return 2;

    Position current = (*p)->next;
    Position newNode = NULL;
    while (current != NULL)
    {
        newNode = create_node();
        if (newNode == NULL)
            return 2;
        if (add_sorted_node(newHeader, current->element, newNode) != 0) {
            printf("ERROR, dodavanje u listu");
            return 3;
        }
        current = current->next;
    }
    delete_list(*p);
    *p = newHeader;
    return 0;
}

int polynomial_clean(Position* p) {
    if (p == NULL || *p == NULL)
    {
        printf("ERROR, ucitavanje liste");
        return 1;
    }
    if ((*p)->next == NULL) {
        printf("Lista je prazna!");
        return 0;
    }
    polynomial_sort(p);

    Position newHeader = create_node();
    if (newHeader == NULL)
        return 2;

    Position current = (*p)->next;

    Position newNode = NULL;

    struct Term temp = { current->element.koef, current->element.exp };

    while (current != NULL) {
        float sumKoef = current->element.koef;
        float currentExp = current->element.exp;

        while (current->next != NULL && current->next->element.exp == currentExp) {
            sumKoef += current->next->element.koef;
            current = current->next;
        }

        if (sumKoef != 0) {
            newNode = create_node();
            struct Term t = { sumKoef, currentExp };
            add_sorted_node(newHeader, t, newNode);
        }
        current = current->next;
    }

    delete_list(*p);
    *p = newHeader;
    return 0;
}

Position join_lists(Position* p, int arr_length) {
    if (p == NULL)
    {
        printf("ERROR, ucitavanje niza");
        return NULL;
    }
    Position newHeader = create_node();
    if (newHeader == NULL) {
        printf("ERROR, join lists");
        return NULL;
    }
    Position newNode;
    Position current = NULL;
    for (int i = 0; i < arr_length; i++)
    {
        if (p[i] == NULL) {
            printf("ERROR, join lists funkcija");
            free(newHeader);
            return NULL;
        }
        current = p[i]->next;
        while (current != NULL)
        {
            newNode = create_node();
            if (newNode == NULL) {
                printf("ERROR, alokacija cvora join lists");
                free(newHeader);
                return NULL;
            }
            add_to_start(newHeader, current->element, newNode);
            current = current->next;
        }
    }
    return newHeader;
}

Position multiply(Position* p, int arr_length) {
    if (p == NULL)
    {
        printf("ERROR, ucitavanje niza, multiply\n");
        return NULL;
    }
    if (arr_length < 2) {
        printf("arr len manji od 2\n");
        return NULL;
    }
    Position newHeader = create_node();
    if (newHeader == NULL) {
        printf("ERROR, multiply");
        return NULL;
    }

    Position newNode = NULL;
    Position currI = NULL;
    Position currJ = NULL;

    struct Term temp = { 0, 0 };

    for (int i = 1; i < arr_length; i++)
    {
        currI = p[0]->next;
        while (currI != NULL) {
            currJ = p[i]->next;
            while (currJ != NULL) {
                newNode = create_node();
                if (newNode == NULL) {
                    printf("ERROR, alokacija cvora multiply");
                    delete_list(newHeader);
                    free(newHeader);
                    return NULL;
                }
                temp.koef = currI->element.koef * currJ->element.koef;
                temp.exp = currI->element.exp + currJ->element.exp;
                add_to_start(newHeader, temp, newNode);
                currJ = currJ->next;
            }
            currI = currI->next;
        }
    }

    if (polynomial_clean(&newHeader) != 0)
        return NULL;
    return newHeader;
}
