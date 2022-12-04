#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>
#include <windows.h>
#include <dos.h>
#include <stddef.h>

/*
● They are unique sets with a value
● Implement them over the set structures to support any kind of value
*/
int hashmap_size = 10;

struct set_node
{
    const char *key;
    size_t key_len;
    struct set_node *next;
    int value;
};

struct set_table
{
    struct set_node **nodes;
    size_t hashmap_size;
    size_t size;
};

size_t djb33x_hash(const char *key, const size_t keylen) // funzione Hash
{
    size_t hash = 5381;

    for (size_t i = 0; i < keylen; i++) // per ogni carattere della stringa key (lunghezza keylen)
    {
        hash = ((hash << 5) + hash) ^ key[i]; 
    }
    return hash;
}


struct set_table *set_table_new(const size_t hashmap_size)
{
    struct set_table *table = malloc(sizeof(struct set_table));
    if (!table)
    {
        return NULL;
    }
    table->hashmap_size = hashmap_size;
    table->nodes = calloc(table->hashmap_size, sizeof(struct set_node *));
    if (!table->nodes)
    {
        free(table);
        return NULL;
    }
    return table;
}

struct set_node *set_search(struct set_table *table, const char *key, const size_t key_len) // cerco un nodo
{
    size_t hash = djb33x_hash(key, key_len); // calcolo la chiave hash
    size_t index = hash % table->hashmap_size; // calcolo l'indice
    struct set_node *head = table->nodes[index]; // prendo la testa della lista
    while (head) // scorro la lista
    {
        if (head->key == key) // se trovo la chiave
        {
            return head; // ritorno il nodo
        }
        head = head->next; // scorro la lista
    }
    return NULL; // se non trovo la chiave ritorno NULL
}


int set_search_and_return(struct set_table *table, const char *key, const size_t key_len) // cerco un nodo
{
    size_t hash = djb33x_hash(key, key_len); // calcolo la chiave hash
    size_t index = hash % table->hashmap_size; // calcolo l'indice
    struct set_node *head = table->nodes[index]; // prendo la testa della lista
    while (head) // scorro la lista
    {
        if (head->key == key) // se trovo la chiave
        {
            return head->value; // ritorno il nodo
        }
        head = head->next; // scorro la lista
    }
    return 0; // se non trovo la chiave ritorno NULL
}

struct set_node *set_insert(struct set_table *table, const char *key, const size_t key_len, int Value) // inserisce un elemento nella tabella
{
    size_t hash = djb33x_hash(key, key_len); // calcola l'hash della chiave
    size_t index = hash % table->hashmap_size; // calcola l'indice della tabella
    struct set_node *head = table->nodes[index]; // inizializza il nodo iniziale con il nodo dell'indice
    
    if(set_search(table, key, hashmap_size) != NULL)
    {
        return NULL;         
    }

    if (!head) // se il nodo iniziale non esiste
    {
        head = malloc(sizeof(struct set_node)); // alloca la memoria per il nuovo nodo
        if (!head) // se il nuovo nodo non esiste
        {
            return NULL; // ritorna nullo
        }
        head->key = key; // assegna la chiave al nuovo nodo
        head->key_len = key_len; // assegna la lunghezza della chiave al nuovo nodo
        head->next = NULL; // assegna il nodo successivo a NULL
        table->nodes[index] = head; // assegna il nodo dell'indice con il nodo iniziale
        head->value = Value;
        return head; // ritorna il nodo iniziale
    }
    struct set_node *new_item = malloc(sizeof(struct set_node)); // alloca la memoria per il nuovo nodo
    if (!new_item) // se il nuovo nodo non esiste
    {
        return NULL; // ritorna nullo
    }
    new_item->key = key; // assegna la chiave al nuovo nodo
    new_item->key_len = key_len; // assegna la lunghezza della chiave al nuovo nodo
    new_item->next = NULL;
    new_item->value = Value; // assegna il nodo successivo a NULL 
    struct set_node *tail = head; // inizializza il nodo finale con il nodo iniziale 

    tail->next = new_item; // assegna il nodo successivo del nodo finale con il nuovo nodo
    return new_item; // ritorna il nuovo nodo
}



struct set_node *set_remove(struct set_table *table, const char *key, const size_t key_len) // rimuovo un nodo
{
    size_t hash = djb33x_hash(key, key_len); // calcolo la chiave hash
    size_t index = hash % table->hashmap_size; // calcolo l'indice
    struct set_node *head = table->nodes[index]; // prendo la testa della lista
    struct set_node *prev; // nodo precedente
    while (head) // scorro la lista
    {
        if (head->key == key) // se trovo la chiave 
        {
            if (head == table->nodes[index]) // se è la testa della lista 
            {
                table->nodes[index] = head->next; // assegno la testa della lista al nodo successivo
            }
            else // se non è la testa della lista
            {
                prev->next = head->next; // il nodo precedente punta al nodo successivo del nodo corrente
            }
            return head; // ritorno il nodo
        }
        prev = head; // aggiorno il nodo precedente con il nodo corrente
        head = head->next; // scorro la lista 
    }
    return NULL; // se non trovo la chiave ritorno NULL
}

int main()
{
    struct set_table *table = set_table_new(hashmap_size);
    set_insert(table, "Hello", hashmap_size, 82);
    set_insert(table, "World", hashmap_size, 12);
    set_insert(table, "Ciao in Italiano", hashmap_size, 11);
    set_insert(table, "Mondo in Italiano", hashmap_size, 909);
    set_insert(table, "Sorry", hashmap_size, -87);

    printf("value is: %s\n", set_search(table, "Ciao in Italiano", hashmap_size)->key);
    printf("value is: %d\n", set_search_and_return(table, "Ciao in Italiano", hashmap_size));

    set_remove(table, "World", hashmap_size);
    printf("value is: %s\n", set_search(table, "World", hashmap_size)->key);
    return 0;
}
