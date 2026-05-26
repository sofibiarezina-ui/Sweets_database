#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

bool load_binary_sweets(SweetsWrapper** sweets, int* size, int* capacity, int* next_id) {

    FILE* fp = fopen("sweets.bin", "rb"); 
    if (fp == NULL) {
        printf("Ошибка: файл sweets.bin не найден.\n");
        return false;
    }

    //Чтение информации о размере
    int count_in_file;
    if (fread(&count_in_file, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return false;
    }

    *size = count_in_file;
    *capacity = (int)(count_in_file * 1.1) + 1;

    *sweets = (SweetsWrapper*)malloc(sizeof(SweetsWrapper) * (*capacity));
    if (*sweets == NULL) {
        fclose(fp);
        return false;
    }

    //Чтение данных из файла во временный массив структур Sweets
    // (в памяти необходимы SweetsWrapper)
    Sweets temp;
    int max_id = count_in_file;

    for (int i = 0; i < count_in_file; i++) {

        // Копируем данные в обертку
        fread((*sweets)[i].data.sweet_name, sizeof(char), 100, fp);
        fread(&((*sweets)[i].data.type_code), sizeof(int), 1, fp);
        fread(&((*sweets)[i].data.price), sizeof(double), 1, fp);
        fread(&((*sweets)[i].data.weight), sizeof(double), 1, fp);
        fread(&((*sweets)[i].data.amount_of_sugar), sizeof(double), 1, fp);

        (*sweets)[i].data.id = i + 1;
        (*sweets)[i].is_deleted = false;

    }

    // Глобальный счетчик ID для новых записей
    *next_id = max_id + 1;

    fclose(fp);
    return true;
}


bool load_binary_types(TypeWrapper** types, int* size, int* capacity, int* next_id) {

    FILE* fp = fopen("types.bin", "rb");
    if (fp == NULL) {
        printf("Ошибка: файл types.bin не найден.\n");
        return false;
    }

    //Чтение информации о размере
    int count_in_file;
    if (fread(&count_in_file, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return false;
    }

    *size = count_in_file;
    *capacity = (int)(count_in_file * 1.1) + 1;

    *types = (TypeWrapper*)malloc(sizeof(TypeWrapper) * (*capacity));
    if (*types == NULL) {
        fclose(fp);
        return false;
    }

    //Чтение данных из файла во временный массив структур Sweets
    // (в памяти необходимы SweetsWrapper)
    Type temp;
    int max_id = count_in_file;

    for (int i = 0; i < count_in_file; i++) {
        // Обнуляем временную структуру, чтобы не было мусора в полях, которые не читаем
        memset(&temp, 0, sizeof(Type));

        // Копируем данные в обертку
        fread(&((*types)[i].data.code), sizeof(int), 1, fp);
        fread(&((*types)[i].data.type_name), sizeof(char), 100, fp);

        (*types)[i].data.id = i + 1;
        (*types)[i].is_deleted = false;

    }

    // Глобальный счетчик ID для новых записей
    *next_id = max_id + 1;

    fclose(fp);
    return true;
}

void link_sweets_to_types(TypeWrapper* types, int typeCount, SweetsWrapper* sweets, int sweetCount) {
    for (int i = 0; i < typeCount; i++) {
        // Пропускаем удаленные типы
        if (types[i].is_deleted) continue;

        int current_code = types[i].data.code;
        int found = 0;

        // сколько сладостей относится к этому типу
        for (int j = 0; j < sweetCount; j++) {
            if (!sweets[j].is_deleted && sweets[j].data.type_code == current_code) {
                found++;
            }
        }

        // Количество в структуре
        types[i].data.sweets_count = found;

        if (found > 0) {
            types[i].data.sweets_of_type = (Sweets**)malloc(sizeof(Sweets*) * found);

            int k = 0;
            // Записываем АДРЕСА "чистых" сладостей в массив типа
            for (int j = 0; j < sweetCount; j++) {
                if (!sweets[j].is_deleted && sweets[j].data.type_code == current_code) {
                    types[i].data.sweets_of_type[k] = &(sweets[j].data);
                    k++;
                }
            }
        }
        else {
            types[i].data.sweets_of_type = NULL;
        }
    }
    printf("Связывание данных завершено успешно.\n");
}


bool complete_all_stuctures(
    SweetsWrapper** sweets, TypeWrapper** types, 
    int *sweets_size, int *type_size, 
    int *sweets_capacity, int *type_capacity, 
    int *sweets_next_id, int *type_next_id) {

    if (!load_binary_sweets(sweets, sweets_size, sweets_capacity, sweets_next_id)) {
        printf("Ошибка при загрузке сладостей\n");
        return false;
    }

    if (!load_binary_types(types, type_size, type_capacity, type_next_id)) {
        printf("Ошибка при загрузке типов\n");
        return false;
    }

    link_sweets_to_types(*types, *type_size, *sweets, *sweets_size);
    return true;
}