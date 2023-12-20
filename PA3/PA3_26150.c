#include <stdio.h>
#include <stdlib.h>

#define BACKING_STORE_SIZE 128
#define PHYSICAL_MEMORY_SIZE 256
#define FRAME_SIZE 32
#define PAGE_SIZE 32
#define PAGE_TABLE_SIZE 4

typedef struct {
	int frame_number;
} PageTableEntry;

/*
void printDataInBackingStore(unsigned char* backing_store, int size) {
	printf("Data in pdata.bin:\n");
	for (int i = 0; i < size; i++) {
		if (i % FRAME_SIZE == 0) {
			printf("Page %d\n", i / FRAME_SIZE);
		}
		printf("%d\t", backing_store[i]);
		if ((i + 1) % FRAME_SIZE == 0) {
			printf("\n");
		}
	}
	printf("\n");
}
*/

void printDataInBackingStore(char* backing_store, int size) {
	printf("Data in pdata.bin:\n");
	for (int i = 0; i < size; i++) {
		if (i % FRAME_SIZE == 0) {
			printf("Page %d\n", i / FRAME_SIZE);
		}
		printf("%d\t", backing_store[i]);
		if ((i + 1) % FRAME_SIZE == 0) {
			printf("\n");
		}
	}
	printf("\n");
}

int main() {
	// Open files.
	FILE* la_file = fopen("la.txt", "r");
	FILE* pdata_file = fopen("pdata.bin", "rb");
	
	// calculate the size of file.
	fseek(pdata_file, 0, SEEK_END);
	int file_size = ftell(pdata_file);
	fseek(pdata_file, 0, SEEK_SET);

	// Create memory arrays
	char backing_store[BACKING_STORE_SIZE];
	unsigned char physical_memory[PHYSICAL_MEMORY_SIZE];

	// Read pdata.bin to backing store.
	fread(backing_store, sizeof(char), BACKING_STORE_SIZE, pdata_file);
	fclose(pdata_file);

	// Print the content in pdata.bin.
	printDataInBackingStore(backing_store, file_size);

	// Initialize page table, here we assign initial value as -1.
	// indicating that the page is not loaded into memory yet.
	PageTableEntry page_table[PAGE_TABLE_SIZE];
	for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
		page_table[i].frame_number = -1;
	}

	// Read logical addresses one by one from la.txt.
	int logical_address;
	while (fscanf(la_file, "%d", &logical_address) != EOF) {
		// Extract page number and offset from logical address.
		int page_number = (logical_address >> 5) & 0x07;
		int offset = logical_address & 0x1F;

		// Check if page is loaded in physical memory.
		if (page_table[page_number].frame_number == -1) {
			// Page fault, load page into a free frame.
			// Allocate frame number starting from 0.
			int frame_number = page_number; 
			page_table[page_number].frame_number = frame_number;

			// Copy page from backing store to physical memory.
			int start_address = frame_number * PAGE_SIZE;
			for (int i = 0; i < PAGE_SIZE; i++) {
				physical_memory[start_address + i] = backing_store[page_number * PAGE_SIZE + i];
			}
		}

		// Generate physical address and access data
		int frame_number = page_table[page_number].frame_number;
		int physical_address = ((3-frame_number) * FRAME_SIZE) + offset;
		//int physical_address = (frame_number << 5) | offset;
		//unsigned char data = physical_memory[physical_address];
		//unsigned char data = physical_memory[frame_number * FRAME_SIZE + offset];
		char data = backing_store[frame_number * PAGE_SIZE + offset];

		// Print logical address, physical address, and data
		printf("Page no: %d\t\toffset: %d\n", page_number, offset);
		printf("Virtual Address: %d\t\tPhysical Address: %d\tValue: %d\n\n", logical_address, physical_address, data);
	}

	// Close files
	fclose(la_file);

	return 0;
}


