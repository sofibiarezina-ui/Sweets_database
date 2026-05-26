#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <string.h>
#include "functions.h"

//==================================================================Sweets

// По id
int comp_sweet_id(const void* a, const void* b) {
    const Sweets* s1 = *(const Sweets**)a;
    const Sweets* s2 = *(const Sweets**)b;
    if (s1 == NULL) return 1;
    if (s2 == NULL) return -1;
    return (s1->id > s2->id) - (s1->id < s2->id);
}

// По названию (А-Я)
int comp_sweet_name(const void* a, const void* b) {
    const Sweets* s1 = *(const Sweets**)a;
    const Sweets* s2 = *(const Sweets**)b;
    if (s1 == NULL || s1->sweet_name == NULL) return 1;
    if (s2 == NULL || s2->sweet_name == NULL) return -1;
    return strcmp(s1->sweet_name, s2->sweet_name);
}

// По цене (возрастание)
int comp_sweet_price(const void* a, const void* b) {
    const Sweets* s1 = *(const Sweets**)a;
    const Sweets* s2 = *(const Sweets**)b;
    if (s1 == NULL) return 1;
    if (s2 == NULL) return -1;
    return (s1->price > s2->price) - (s1->price < s2->price);
}

// По весу (убывание — от тяжелых к легким)
int comp_sweet_weight(const void* a, const void* b) {
    const Sweets* s1 = *(const Sweets**)a;
    const Sweets* s2 = *(const Sweets**)b;
    if (s1 == NULL) return 1;
    if (s2 == NULL) return -1;
    return (s1->weight < s2->weight) - (s1->weight > s2->weight);
}

// По содержанию сахара (возрастание)
int comp_sweet_sugar(const void* a, const void* b) {
    const Sweets* s1 = *(const Sweets**)a;
    const Sweets* s2 = *(const Sweets**)b;
    if (s1 == NULL) return 1;
    if (s2 == NULL) return -1;
    return (s1->amount_of_sugar > s2->amount_of_sugar) - (s1->amount_of_sugar < s2->amount_of_sugar);
}


//==================================================================Types

// По id
int comp_type_id(const void* a, const void* b) {
    const Type* t1 = *(const Type**)a;
    const Type* t2 = *(const Type**)b;
    if (t1 == NULL) return 1;
    if (t2 == NULL) return -1;
    return (t1->id > t2->id) - (t1->id < t2->id);
}

// По названию типа (А-Я)
int comp_type_name(const void* a, const void* b) {
    const Type* t1 = *(const Type**)a;
    const Type* t2 = *(const Type**)b;
    if (t1 == NULL || t1->type_name == NULL) return 1;
    if (t2 == NULL || t2->type_name == NULL) return -1;
    return strcmp(t1->type_name, t2->type_name);
}

// По коду типа (по возрастанию)
int comp_type_code(const void* a, const void* b) {
    const Type* t1 = *(const Type**)a;
    const Type* t2 = *(const Type**)b;
    if (t1 == NULL) return 1;
    if (t2 == NULL) return -1;
    return (t1->code > t2->code) - (t1->code < t2->code);
}

// По количеству сладостей в типе (убывание)
int comp_type_count(const void* a, const void* b) {
    const Type* t1 = *(const Type**)a;
    const Type* t2 = *(const Type**)b;
    if (t1 == NULL) return 1;
    if (t2 == NULL) return -1;
    return (t2->sweets_count - t1->sweets_count); 
}




