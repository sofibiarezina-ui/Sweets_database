#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <locale.h>
#include <windows.h>
#include "functions.h"

int main(void) {
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);


	// Отдельные структуры для отсортированных представлений
	SortedSweetsList sweets_by_id = { NULL, 0, "По id" };
	SortedSweetsList sweets_by_name = { NULL, 0, "По названию" };
	SortedSweetsList sweets_by_price = { NULL, 0, "По цене" };
	SortedSweetsList sweets_by_weight = { NULL, 0, "По весу" };
	SortedSweetsList sweets_by_sugar = { NULL, 0, "По содержанию сахара" };

	SortedTypeList types_by_id = { NULL, 0, "По id" };
	SortedTypeList types_by_code = { NULL, 0, "По коду" };
	SortedTypeList types_by_name = { NULL, 0, "По названию" };
	SortedTypeList types_by_sweets_count = { NULL, 0, "По кол-ву сладостей" };

	SweetsSortContainer sweets_sorts = {
				&sweets_by_id, &sweets_by_name, &sweets_by_price, &sweets_by_weight, &sweets_by_sugar
	};
	TypesSortContainer types_sorts = {
		&types_by_id, &types_by_code, &types_by_name, &types_by_sweets_count
	};




	int choise;
	bool continue_using = true;
	bool was_formed = false;
	bool file_read = false;

	SweetsWrapper* sweets = NULL;
	TypeWrapper* types = NULL;

	int sweets_size = 0, type_size = 0, 
		sweets_capacity = 0, type_capacity = 0, 
		sweets_next_id = 0, type_next_id  = 0;

	ChristmasGift* gifts = NULL;
	int gift_count = 0;

	choise = display_menu_start();

	while (continue_using) {
		switch (choise) {
		case 1:
			if (file_read) {
				printf("Файл уже прочитан. Данное дествий невозможно выполнить снова, не прерывая вполнения программы.\n");
				printf("Выберите другую функцию.\n");
				break;
			}

			if (!complete_all_stuctures(
				&sweets, &types, &sweets_size, &type_size, &sweets_capacity, &type_capacity, &sweets_next_id, &type_next_id
			)) return 1;

			UpdateAllSortsSweets(sweets, sweets_size, &sweets_by_id, &sweets_by_name, &sweets_by_price, &sweets_by_weight, &sweets_by_sugar);
			UpdateAllSortsTypes(types, type_size, &types_by_id, &types_by_code, &types_by_name, &types_by_sweets_count);

			file_read = true;
			break;

		case 2:
			display_menu_lists(types, type_size, sweets, sweets_size, was_formed, gifts, gift_count);
			break;

		case 3:
			display_sorted_lists_menu(sweets_sorts, types_sorts, types, type_size);
			break;

		case 4:
			display_search_menu(sweets, sweets_size, types, type_size);
			break;

		case 5:
			display_add_menu(&sweets, &sweets_size, &sweets_capacity, &sweets_next_id,
				&types, &type_size, &type_capacity, &type_next_id,
				sweets_sorts, types_sorts);
			break;

		case 6:
			display_delete_menu(sweets, sweets_size, types, type_size, sweets_sorts, types_sorts);
			break;

		case 7:
			display_edit_menu(sweets, sweets_size, types, type_size, sweets_sorts, types_sorts);
			break;

		case 8:
			CreateChristmasGifts(types, type_size, sweets, sweets_size, &gifts, &gift_count);
			
			was_formed = true;
			ViewChristmasGifts(gifts, gift_count, was_formed);

			break;

		case 9:
			// just terminate
			printf("\nВы действительно хотите выйти без сохранения изменений?\n 0 - нет\n1 - да\n");
			
			int under_choice;
			scanf("%d", &under_choice);
			if (!under_choice ) {
				break;
			}

			free(sweets_by_id.items); free(sweets_by_name.items); 
			free(sweets_by_price.items);
			free(sweets_by_weight.items); free(sweets_by_sugar.items);
			
			free(types_by_id.items); free(types_by_code.items); 
			free(types_by_name.items); free(types_by_sweets_count.items);
			
			if (gifts != NULL) {
				clear_GiftList(gifts, gift_count);
			}
			if (!terminate_program(types, type_size, sweets, sweets_size)) return 1;
			continue_using = false;
			break;

		case 10:
			// with recording
			process_export_gift(gifts, gift_count, was_formed);
			if (gifts != NULL) {
				clear_GiftList(gifts, gift_count);
			}
			if (!terminate_program(types, type_size, sweets, sweets_size)) return 1;
			continue_using = false;
			break;

		}

		choise = display_menu();
	}

	return 0;
}