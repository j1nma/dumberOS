#include "tests.h"

void memoryError(int error) {
	switch (error) {
	case 1: {
		printf("Error: Value not stored correctlly on page.\n");
		break;
	}
	case 2: {
		printf("Error: Value not retained on page after free. Not critical.\n");
		break;
	}
	case 3: {
		printf("Error: Alloc didn't return the same page. Shame.\n");
		break;
	}
	default: {
		printf("Error: %d\n", error);
		break;
	}
	}
}

void testMemory() {

	printf("Allocating 1 page.\n");
	char *page = alloc(1 * sizeof(char));


	printf("Writing all a's.\n");
	for (int i = 0; i < 1024 * 4 * sizeof(char); ++i) {
		page[i] = 'a';
	}

	if (page[1024 * 4 * sizeof(char) - 1] == 'a')
		printf("On the last byte of the page, value: %c.\n", page[1024 * 4 * sizeof(char) - 1]);
	else {
		memoryError(1);
		freeP(page);
		return;
	}


	printf("Freeing page.\n");
	freeP(page);

	if (page[1024 * 4 * sizeof(char) - 1] == 'a')
		printf("The value is the same: %c. Not overwriten.\n", page[1024 * 4 * sizeof(char) - 1]);
	else {
		memoryError(2);
	}


	printf("Allocating new page, should be on the same space. (Multi-processing not contemplated)\n");
	char *page2 = alloc(1 * sizeof(char));

	printf("Writing all b's.\n");
	for (int i = 0; i < 1024 * 4 * sizeof(char); ++i) {
		page2[i] = 'b';
	}


	if (page[1024 * 4 * sizeof(char) - 1] == 'b')
		printf("The value in the first pointer should be: %c because they are in the same space.\n", page[1024 * 4 * sizeof(char) - 1]);
	else {
		memoryError(3);
	}

	printf("Freeing second page.\n");
	freeP(page2);

}