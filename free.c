#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "functions.h"

void clear_type_links(TypeWrapper* types, int typeCount) {
    if (types == NULL) return;

    for (int i = 0; i < typeCount; i++) {
        // Если для данного типа выделялся массив указателей на сладости
        if (types[i].data.sweets_of_type != NULL) {
            free(types[i].data.sweets_of_type);
            types[i].data.sweets_of_type = NULL; 
        }
        types[i].data.sweets_count = 0;
    }
}


bool terminate_program(TypeWrapper** types, int* typeCount, SweetsWrapper** sweets, int* sweetCount) {
    // 1. Сначала очищаем внутренние динамические связи в типах
    if (*types != NULL) {
        clear_type_links(*types, *typeCount);
    }

    // 2. Освобождаем основной массив типов
    if (*types != NULL) {
        free(*types);
        *types = NULL;
    }

    // 3. Освобождаем основной массив сладостей
    if (*sweets != NULL) {
        free(*sweets);
        *sweets = NULL;
    }

    // 4. Сбрасываем счетчики
    *typeCount = 0;
    *sweetCount = 0;

    printf("Ресурсы успешно очищены.\n");
    return true;
}

void clear_GiftList(ChristmasGift* gifts, int giftCount) {
    if (gifts == NULL) return;
    for (int i = 0; i < giftCount; i++) {
        GiftNode* current = gifts[i].head;
        while (current != NULL) {
            GiftNode* next = current->next;
            free(current);
            current = next;
        }
    }
    free(gifts);
}