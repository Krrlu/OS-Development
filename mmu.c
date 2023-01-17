#include "mmu.h"
#include "print.h"
#define ALIGNMENT 32 //must be mutiple of 2
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))
#define HEAP 0x0100000

/**
*   Memory management unit
*	Divide memory into blocks, size of block is aligned to macro ALIGNMENT. 
*	Each block has a 16 bits header. First bit in header indicate whether the block is used of not
*	1 for used and 0 for free
*	The rest 15 bits store the size of the block
*/

void* malloc(uint32_t size) {

	char* heap = (char*) HEAP; // start adress of heap
	int blocks = ((uint32_t)ALIGN(size + 4) / ALIGNMENT);//number of blocks we need
	char* p1 = heap;
	char* p2 = p1;
	// we use two pointer

	while (p1) {
		blocks = ((uint32_t)ALIGN(size + 4) / ALIGNMENT);
		// find first block that is not used
		while ((*((uint32_t*)p1) & 1) == 1) {
			p1 += *(uint32_t*) p1 - 1; // set lowest bit to zero, and we obtain the offset to next chunk
		}
		p2 = p1;
		// how many free blocks there are
		while (((*((uint32_t*)p2) & 1) == 0) && blocks > 0) {
			p2 += ALIGNMENT;
			blocks--;
		}

		if (!blocks) break; // we found enough blocks
		p1 = p2;
	}
	if (blocks){
		print_string("error, allocating memory");
		return NULL;
	} // not enough memory to allocate 

	*(uint32_t*) p1 |= 1; // set first bit 1
	*(uint32_t*) p1 |= (uint32_t)ALIGN(size + 4); // store the size of chunk in rest 31 bits

	return p1 + 4; 
}

void free(char* p) {
	if (p == NULL) return; //Null pointer

	p -= 4; // p point to begin of the header


	if (((*p & 1) == 0)) {
		print_string("error, free memory that is not allocated");
	}// if memory is not used

	uint32_t blocks = (*(uint32_t*)p) / ALIGNMENT;
	// use for loop to clean every possible header in this chunk
	for (int i = 0; i < blocks; i++) {
		*(uint32_t*)p &= 0;
		p += ALIGNMENT;
	}
}
