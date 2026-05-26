#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

void RefreshSortedSweetsList(SortedSweetsList* list, SweetsWrapper* source, int sourceCount, int (*compar)(const void*, const void*)) {
    if (list == NULL) return;

    if (list->items != NULL) {
        free(list->items);
        list->items = NULL;
    }
    list->count = 0;

    if (source == NULL || sourceCount <= 0) return;

    int alive = 0;
    for (int i = 0; i < sourceCount; i++) {
        if (!source[i].is_deleted) {
            alive++;
        }
    }

    if (alive == 0) return;

    list->items = (Sweets**)malloc(sizeof(Sweets*) * alive);
    if (list->items == NULL) return;

    int k = 0;
    for (int i = 0; i < sourceCount; i++) {
        if (!source[i].is_deleted) {
            list->items[k++] = &(source[i].data);
        }
    }
    list->count = k;

    if (list->count > 1 && compar != NULL) {
        qsort(list->items, list->count, sizeof(Sweets*), compar);
    }
}

void RefreshSortedTypeList(SortedTypeList* list, TypeWrapper* source, int sourceCount, int (*compar)(const void*, const void*)) {
    if (list == NULL) return;

    if (list->items != NULL) {
        free(list->items);
        list->items = NULL;
    }
    list->count = 0;

    if (source == NULL || sourceCount <= 0) return;

    int alive = 0;
    for (int i = 0; i < sourceCount; i++) {
        if (!source[i].is_deleted) {
            alive++;
        }
    }

    if (alive == 0) return;

    list->items = (Type**)malloc(sizeof(Type*) * alive);
    if (list->items == NULL) return;

    int k = 0;
    for (int i = 0; i < sourceCount; i++) {
        if (!source[i].is_deleted) {
            list->items[k++] = &(source[i].data);
        }
    }
    list->count = k;

    if (list->count > 1 && compar != NULL) {
        qsort(list->items, list->count, sizeof(Type*), compar);
    }
}

void UpdateAllSortsSweets(SweetsWrapper* sweets, int s_size, SortedSweetsList* id,
    SortedSweetsList* n, SortedSweetsList* p,
    SortedSweetsList* w, SortedSweetsList* s) {

    RefreshSortedSweetsList(id, sweets, s_size, comp_sweet_id);
    RefreshSortedSweetsList(n, sweets, s_size, comp_sweet_name);
    RefreshSortedSweetsList(p, sweets, s_size, comp_sweet_price);
    RefreshSortedSweetsList(w, sweets, s_size, comp_sweet_weight);
    RefreshSortedSweetsList(s, sweets, s_size, comp_sweet_sugar);
}

void UpdateAllSortsTypes(TypeWrapper* types, int t_size,
    SortedTypeList* id, SortedTypeList* c,
    SortedTypeList* n, SortedTypeList* s_c) {

    RefreshSortedTypeList(id, types, t_size, comp_type_id);
    RefreshSortedTypeList(c, types, t_size, comp_type_code);
    RefreshSortedTypeList(n, types, t_size, comp_type_name);
    RefreshSortedTypeList(s_c, types, t_size, comp_type_count);
}