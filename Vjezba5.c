#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct Node* Position;

struct Node {
    int element;
    Position next;
};

Position create_node();
int add_to_start(Position, int, Position);
int add_to_end(Position, int, Position);
int print_list(Position);
int delete_list(Position);
int delete_node(Position, Position);
int add_after_node(Position, int, int, Position);
int add_before_node(Position, int, int, Position);
int add_sorted_node(Position, int, Position);
int polynomial_sort(Position*);
int already_in_list(Position, int);
Position unija(Position*);
Position presjek(Position*);
Position read_sorted_list();
void cleanup(Position heads[], int);


int main()
{
    Position heads[2];

    for (int i = 0; i < 2; i++)
    {
        printf("%d ", i + 1);
        heads[i] = read_sorted_list();
        if (heads[i] == NULL)
            i--;
    }

    char unos = 's';
    do
    {
        printf("\nUnesi znak:\n+) unija\n*) presjek\nx) Izlaz\n");
        printf("Odabir: ");
        scanf(" %c", &unos);


        Position newHead = NULL;

        switch (unos)
        {
        case '+': {
            newHead = unija(heads);
            if (newHead == NULL) {
                printf("ERROR: unija nije uspila!\n");
                cleanup(heads, 2);
                return 1;
            }

            printf("Nova lista: ");
            print_list(newHead);
            delete_list(newHead);
            free(newHead);
            break;
        }
        case '*': {
            newHead = presjek(heads);
            if (newHead == NULL) {
                printf("ERROR: presjek nije uspia!\n");
                cleanup(heads, 2);
                return 1;
            }

            printf("Nova lista: ");
            print_list(newHead);
            delete_list(newHead);
            free(newHead);
            break;
        }
        case 'x':
            break;
        default:
            printf("Nepoznata radnja!\n");
            break;
        }

    } while (unos != 'x');

    for (int i = 0; i < 2; i++)
    {
        if (heads[i] != NULL) {
            delete_list(heads[i]);
            free(heads[i]);
        }
    }
    return 0;
}

Position create_node() {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        printf("ERROR, nije alociran cvor\n");
        return NULL;
    }
    node->next = NULL;
    return node;
}

int add_to_start(Position p, int num, Position node) {
    if (p == NULL || node == NULL) {
        printf("ERROR, add to start\n");
        return 1;
    }
    node->element = num;
    node->next = p->next;
    p->next = node;
    return 0;
}

int add_to_end(Position p, int num, Position node) {
    if (p == NULL || node == NULL) {
        printf("ERROR, add to end\n");
        return 1;
    }
    node->element = num;
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
    if (current == NULL) {
        printf("Lista je prazna.\n");
        return 2;
    }

    while (current != NULL) {
        printf("%d ", current->element);
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
            printf("Element izbrisan iz liste!\n");
            return 0;
        }
        currentNode = currentNode->next;
    }
    printf("Element nije pronaden!");
    return 1;
}

int add_after_node(Position p, int targetIndex, int num, Position newNode) {
    if (p == NULL || newNode == NULL) {
        printf("ERROR, add after node\n");
        return 1;
    }

    Position pointer = p->next;
    int i = 1;
    while (pointer != NULL) {
        if (i == targetIndex) {
            newNode->element = num;
            newNode->next = pointer->next;
            pointer->next = newNode;
            printf("Clan dodan nakon elementa s %d!\n", targetIndex);
            return 0;
        }
        pointer = pointer->next;
        i++;
    }

    printf("Indeks nije pronaden!\n");
    free(newNode);
    return 1;
}

int add_before_node(Position p, int targetIndex, int num, Position newNode) {
    if (p == NULL || newNode == NULL) {
        printf("ERROR, add before node\n");
        return 1;
    }

    Position pointer = p;
    int i = 1;
    while (pointer->next != NULL) {
        if (i == targetIndex) {
            newNode->element = num;
            newNode->next = pointer->next;
            pointer->next = newNode;
            printf("Clan dodan prije elementa s %d!\n", targetIndex);
            return 0;
        }
        pointer = pointer->next;
        i++;
    }

    printf("Indeks nije pronaden!\n");
    free(newNode);
    return 1;
}

int add_sorted_node(Position p, int num, Position newNode) {
    if (p == NULL || newNode == NULL)
    {
        printf("ERROR, ucitavanje liste u add sorted node");
        return 1;
    }

    Position current = p;
    while (current->next != NULL) {
        if (num >= current->next->element)
            break;
        current = current->next;
    }

    newNode->element = num;
    newNode->next = current->next;
    current->next = newNode;

    return 0;
}

int polynomial_sort(Position* p) {
    if (p == NULL || *p == NULL)
    {
        printf("ERROR, ucitavanje liste u polynomial sort");
        return 1;
    }
    if ((*p)->next == NULL) {
        printf("Lista je prazna!");
        return 0;
    }

    Position newp = create_node();
    if (newp == NULL)
        return 2;

    Position current = (*p)->next;
    Position newNode = NULL;
    while (current != NULL)
    {
        newNode = create_node();
        if (newNode == NULL)
            return 2;
        if (add_sorted_node(newp, current->element, newNode) != 0) {
            printf("ERROR, polynomial sort");
            return 3;
        }
        current = current->next;
    }
    delete_list(*p);
    *p = newp;
    return 0;
}

int already_in_list(Position p, int unos) {
    if (p == NULL)
    {
        printf("ERROR, ucitavanje liste u already in list");
        return -1;
    }

    Position current = p->next;

    while (current != NULL)
    {
        if (current->element == unos) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

Position unija(Position* p) {
    if (p == NULL)
    {
        printf("ERROR, unija 1");
        return NULL;
    }
    Position newp = create_node();
    if (newp == NULL) {
        printf("ERROR, unija 2");
        return NULL;
    }

    for (int i = 0; i < 2; i++) {
        Position current = p[i]->next;
        while (current != NULL) {
            if (already_in_list(newp, current->element) == 0) {
                Position newNode = create_node();
                if (newNode == NULL) {
                    printf("ERROR: alokacija cvora u uniji\n");
                    delete_list(newp);
                    free(newp);
                    return NULL;
                }
                add_sorted_node(newp, current->element, newNode);
            }
            current = current->next;
        }
    }
    return newp;
}

Position presjek(Position* p) {
    if (p == NULL || p[0] == NULL || p[1] == NULL) {
        printf("ERROR, presjek\n");
        return NULL;
    }

    Position newp = create_node();
    if (newp == NULL) {
        printf("ERROR, alokacija cvora u funkciji presjek\n");
        return NULL;
    }

    Position current = p[1]->next;
    while (current != NULL) {
        if (already_in_list(p[0], current->element) == 1 &&
            already_in_list(newp, current->element) == 0) {

            Position newNode = create_node();
            if (newNode == NULL) {
                printf("ERROR: alokacija cvora u presjek\n");
                delete_list(newp);
                free(newp);
                return NULL;
            }
            add_sorted_node(newp, current->element, newNode);
        }
        current = current->next;
    }

    return newp;
}


Position read_sorted_list() {
    FILE* file = NULL;
    printf("Unesi ime datoteke:\n ");
    char filename[32] = "";
    scanf(" %31s", filename);

    fopen_s(&file, filename, "r");
    if (file == NULL) {
        printf("Greska, datoteka nije otvorena!\n");
        return NULL;
    }


    Position p = create_node();
    if (p == NULL)
    {
        fclose(file);
        return NULL;
    }


    int num;
    Position node = NULL;
    while (fscanf(file, "%d", &num) == 1)
    {
        node = create_node();
        if (node == NULL) {
            fclose(file);
            delete_list(p);
            free(p);
            return NULL;
        }
        add_sorted_node(p, num, node);
    }
    fclose(file);

    return p;
}

void cleanup(Position heads[], int count) {
    for (int i = 0; i < count; i++) {
        if (heads[i] != NULL) {
            delete_list(heads[i]);
            free(heads[i]);
            heads[i] = NULL;
        }
    }
}
