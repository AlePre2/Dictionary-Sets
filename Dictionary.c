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

size_t djb33x_hash(const char *key, const size_t keylen) // Hashing function
{
    size_t hash = 5381;

    for (size_t i = 0; i < keylen; i++)
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

struct set_node *set_search(struct set_table *table, const char *key, const size_t key_len) // Find key
{
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    struct set_node *head = table->nodes[index];
    while (head)
    {
        if (head->key == key)
        {
            return head;
        }
        head = head->next;
    }
    return NULL;
}


int set_search_and_return(struct set_table *table, const char *key, const size_t key_len)
{
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    struct set_node *head = table->nodes[index];
    while (head)
    {
        if (head->key == key)
        {
            return head->value;
        }
        head = head->next;
    }
    return 0;
}

struct set_node *set_insert(struct set_table *table, const char *key, const size_t key_len, int Value)
{
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    struct set_node *head = table->nodes[index];
    
    if(set_search(table, key, hashmap_size) != NULL)
    {
        return NULL;         
    }

    if (!head)
    {
        head = malloc(sizeof(struct set_node));
        if (!head)
        {
            return NULL;
        }
        head->key = key;
        head->key_len = key_len;
        head->next = NULL;
        table->nodes[index] = head;
        head->value = Value;
        return head;
    }
    struct set_node *new_item = malloc(sizeof(struct set_node)); // Allocate memory for a new node
    if (!new_item)
    {
        return NULL;
    }
    new_item->key = key;
    new_item->key_len = key_len;
    new_item->next = NULL;
    new_item->value = Value;
    struct set_node *tail = head;

    tail->next = new_item;
    return new_item;
}



struct set_node *set_remove(struct set_table *table, const char *key, const size_t key_len) // Remove node
{
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    struct set_node *head = table->nodes[index];
    struct set_node *prev;
    while (head)
    {
        if (head->key == key)
        {
            if (head == table->nodes[index])
            {
                table->nodes[index] = head->next;
            }
            else
            {
                prev->next = head->next;
            }
            return head;
        }
        prev = head;
        head = head->next; 
    }
    return NULL;
}

int main()
{
    struct set_table *table = set_table_new(hashmap_size);
    set_insert(table, "Test1", hashmap_size, 82);
    set_insert(table, "Test2", hashmap_size, 12);
    set_insert(table, "Test3", hashmap_size, 11);
    set_insert(table, "Test4", hashmap_size, 909);
    set_insert(table, "Test5", hashmap_size, -87);

    printf("value is: %s\n", set_search(table, "Test3", hashmap_size)->key);
    printf("value is: %d\n", set_search_and_return(table, "Test2", hashmap_size));

    set_remove(table, "Test2", hashmap_size);
    printf("value is: %s\n", set_search(table, "Test2", hashmap_size)->key);
    return 0;
}
