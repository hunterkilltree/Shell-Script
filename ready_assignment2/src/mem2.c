
#include "mem.h"
#include "stdlib.h"
#include "string.h"
#include <pthread.h>
#include <stdio.h>

static BYTE _ram[RAM_SIZE];

static struct {
	uint32_t proc;	// ID of process currently uses this page
	int index;	// Index of the page in the list of pages allocated
			// to the process.
	int next;	// The next page in the list. -1 if it is the last
			// page.
} _mem_stat [NUM_PAGES];

static pthread_mutex_t mem_lock;

void init_mem(void) {
	memset(_mem_stat, 0, sizeof(*_mem_stat) * NUM_PAGES);
	memset(_ram, 0, sizeof(BYTE) * RAM_SIZE);
	pthread_mutex_init(&mem_lock, NULL);
}

/* get offset of the virtual address */
static addr_t get_offset(addr_t addr) {
	return addr & ~((~0U) << OFFSET_LEN);
}

/* get the first layer index */
static addr_t get_first_lv(addr_t addr) {
	return addr >> (OFFSET_LEN + PAGE_LEN);
}

/* get the second layer index */
static addr_t get_second_lv(addr_t addr) {
	return (addr >> OFFSET_LEN) - (get_first_lv(addr) << PAGE_LEN);
}

/* Search for page table table from the a segment table */
static struct page_table_t * get_page_table(
		addr_t index, 	// Segment level index
		struct seg_table_t * seg_table) { // first level table

	/*
	 * TODO: Given the Segment index [index], you must go through each
	 * row of the segment table [seg_table] and check if the v_index
	 * field of the row is equal to the index
	 *
	 * */

	int i;
	for (i = 0; i < seg_table->size; i++) {
		// Enter your code here
		if (index == seg_table->table[i].v_index) 	{
			// debug
			//printf("test");
			return seg_table->table[i].pages;
		}

	}
	return NULL;

}

/* Translate virtual address to physical address. If [virtual_addr] is valid,
 * return 1 and write its physical counterpart to [physical_addr].
 * Otherwise, return 0 */
static int translate(
		addr_t virtual_addr, 	// Given virtual address
		addr_t * physical_addr, // Physical address to be returned
		struct pcb_t * proc) {  // Process uses given virtual address

	/* Offset of the virtual address */
	addr_t offset = get_offset(virtual_addr);
	/* The first layer index */
	addr_t first_lv = get_first_lv(virtual_addr);
	/* The second layer index */
	addr_t second_lv = get_second_lv(virtual_addr);

	/* Search in the first level */
	struct page_table_t * page_table = NULL;
	page_table = get_page_table(first_lv, proc->seg_table);
	if (page_table == NULL) {
		return 0;
	}
	//printf("fdsfadsfafa\n");
	int i;
	for (i = 0; i < page_table->size; i++) {
		if (page_table->table[i].v_index == second_lv) {
			/* TODO: Concatenate the offset of the virtual addess
			 * to [p_index] field of page_table->table[i] to
			 * produce the correct physiet_mem = proc->bp;
cal address and save it to
			 * [*physical_addr]  */
			//offset Concatenate page_table->table[i].p_index
			page_table->table[i].p_index =  page_table->table[i].p_index   | offset;

			return 1;
		}
	}
	return 0;
}

addr_t alloc_mem(uint32_t size, struct pcb_t * proc) {
	pthread_mutex_lock(&mem_lock);
	addr_t ret_mem = 0;
	/* TODO: Allocate [size] byte in the memory for the
	 * process [proc] and save the address of the first
	 * byte in the allocated memory region to [ret_mem].
	 * */

	// proc->code->size = size;
	// ret_mem = proc->bp;
	// printf ("%d\n",proc->code->size );

	// regs is the number of register
	// which holds the address of the first byte of the memory region to be deallocated.

	uint32_t num_pages = ((size % PAGE_SIZE) == 0) ? size / PAGE_SIZE :
		size / PAGE_SIZE + 1; // Number of pages we will use
	int mem_avail = 0; // We could allocate new memory region or not?

	/* First we must check if the amount of free memory in
	 * virtual address space and physical address space is
	 * large enough to represent the amount of required
	 * memory. If so, set 1 to [mem_avail].
	 * Hint: check [proc] bit in each page of _mem_stat
	 * to know whether this page has been used by a process.
	 * For virtual memory space, check bp (break pointer).
	 * */

	 int numberOfPagesFree = 0;
	 for (int i = 0; i < NUM_PAGES; i++) { // check Page Table in document
		 if (_mem_stat[i].proc == 0) {
			 numberOfPagesFree++;

		 }
	 }

	 if (numberOfPagesFree >= num_pages &&  num_pages <= (1 << SEGMENT_LEN) ) { // check size of memory  and virtual memory
		 mem_avail = 1;
	 }

	if (mem_avail) {
		/* We could allocate new memory region to the process */
		ret_mem = proc->bp;
		proc->bp += num_pages * PAGE_SIZE;
		/* Update status of physical pages which will be allocated
		 * to [proc] in _mem_stat. Tasks to do:
		 * 	- Update [proc], [index], and [next] field
		 * 	- Add entries to segment table page tables
		 of [proc]
		 * 	  to ensure accesses to allocated memory slot is
		 * 	  valid. */

		int seg_count = get_first_lv(ret_mem); //first layer index _ index of segment table
		int page_count = get_second_lv(ret_mem); //second  layer index _ index of _mem_stat table

		 //proc->seg_table->size = num_pages / 32; // what is 32?
		 // if (num_pages % 32 != 0) {
			//  proc->seg_table->size++;
		 // }

		 // viet cai size tren ram vo _mem_status
		 // bay gio la tu phisycal ra virtual nen ko dung translate
		 proc->seg_table->size = num_pages ; // number of rows which allocate in seg_table[]

		 // check trong segment coi no da ton tai cai page hay chua
		 struct page_table_t * page_tmp = (struct page_table_t*) malloc(sizeof(struct page_table_t));;
		 if ( get_page_table(seg_count, proc->seg_table) != NULL) {
		  	page_tmp = get_page_table(seg_count, proc->seg_table);
		 } else {

		 		 proc->seg_table->table[seg_count].pages = page_tmp; // if not start with empty segment table
	 	}

		int count = 0;
		int pre_index = 0;
		for (int i = 0; i < NUM_PAGES; i++) {

		  // check free on mem
		  if (_mem_stat[i].proc == 0) {
				_mem_stat[i].proc = proc->pid;
				_mem_stat[i].index = count; // index cuar frame trong


			 	 // update next of page
			 	 if (count > 0) {
			 		 _mem_stat[pre_index].next = i;
			 	 }
				 pre_index = i;


				 // //update page_table
				 page_tmp->table[page_count].p_index = i;
				 page_tmp->table[page_count].v_index = get_second_lv(ret_mem + count );
				 page_tmp->size++;

				 page_count++;

				 if ( (page_count != 0 && page_count % 32 == 0) || (count + 1) == num_pages) {
					 page_tmp = NULL;
					 page_tmp = (struct page_table_t*) malloc(sizeof(struct page_table_t));
					 page_count = 0;
					 seg_count++;

				 }

				 count++;
			 	 // done allocate
			 	 if (count  == num_pages) {
			 		_mem_stat[i].next = -1;
			 		free(page_tmp);
			 		break;
				}

		  }
		}





		 //_mem_stat[offsetOfPhysical].proc = proc->pid;



		 //
		 // int count = 0;
		 // int pre_index = 0;
		 //
		 // struct page_table_t * page_tmp = (struct page_table_t*) malloc(sizeof(struct page_table_t));
		 //
		 //
		 // if ( get_page_table(seg_count, proc->seg_table) != NULL) {
			//  page_tmp = get_page_table(seg_count, proc->seg_table);
		 // }
		 // else {
			//  proc->seg_table->table[seg_count].pages = page_tmp;
		 // }
		 //
		 // for (int i = 0; i < NUM_PAGES; i++) {
		 //
			//  // check free on mem
			//  if (_mem_stat[i].proc == 0) {
		 //
			// 	 // update next of page
			// 	 if (count > 0) {
			// 		 _mem_stat[pre_index].next = i;
			// 	 }
		 // 		 _mem_stat[i].proc = proc->pid;
		 // 		 _mem_stat[i].index = count;
			// 	 pre_index = i;
		 //
			// 	 // update page_table
			// 	 page_tmp->table[page_count].p_index = i;
			// 	 page_tmp->table[page_count].v_index = get_second_lv(ret_mem + i * PAGE_SIZE);
			// 	 page_tmp->size++;
			// 	 page_count++;
		 //
			// 	 if ( (page_count != 0 && page_count % 32 == 0) || (count + 1) == num_pages) {
			// 		 page_tmp = NULL;
			// 		 page_tmp = (struct page_table_t*) malloc(sizeof(struct page_table_t));
			// 		 page_count = 0;
			// 		 seg_count++;
		 //
			// 	 }
			// 	 count++;
		 //
			// 	 // done allocate
			// 	 if (count  == num_pages) {
			// 		_mem_stat[i].next = -1;
			// 		free(page_tmp);
			// 		break;
			// 	 }
			//  }
		 // }
	//
	// 	 int j;
 	// 	for (	j = 0;j < RAM_SIZE;j++) {
	//
 	// 		if (_ram[j] != 0) {
 	// 			printf("\t%05x: %02x\n", j, _ram[j]);
	// 		}
	// 	}
	}

	pthread_mutex_unlock(&mem_lock);
	return ret_mem;
}


// int free_mem(addr_t address, struct pcb_t * proc) {
// 	/*TODO: Release memory region allocated by [proc]. The first byte of
// 	 * this region is indicated by [address]. Task to do:
// 	 * 	- Set flag [proc] of physical page use by the memory block
// 	 * 	  back to zero to indicate that it is free.
// 	 * 	- Remove unused entries in segment table and page tables of
// 	 * 	  the process [proc].
// 	 * 	- Remember to use lock to protect the memory from other
// 	 * 	  processes.
// 	 */
// 	pthread_mutex_lock(&mem_lock);
//
// 	// check valid proc
// 	if (proc == NULL || proc->pid <= 0)
// 		return 0;
//
// 	int first_lv = get_first_lv(address);
// 	int second_lv = get_second_lv(address);
//
// 	struct page_table_t * page_table = NULL;
//
// 	page_table = get_page_table(first_lv, proc->seg_table);
//
// 	int p_idx = 0;
// 	int page_count = 0;
// 	int seg_count = 0;
//
// 	for (int i = 0; i < proc->seg_table->size; i++) {
// 		if (proc->seg_table->table[i].v_index = first_lv) {
// 			seg_count = i;
// 			break;
//
// 		}
// 	}
//
// 	for (int i = 0; i < page_table->size; i++) {
// 		if (page_table->table[i].v_index == second_lv) {
// 			p_idx = page_table->table[i].p_index;
// 			page_count = i;
// 			break;
// 		}
// 	}
//
// 	int i = p_idx;
// 	int count = 0;
//
// 	while(i != -1) {
// 		_mem_stat[i].proc = 0;
// 		i = _mem_stat[i].next;
// 		count++; // check
// 	}
//
// 	//free(page_table);
//
// 	pthread_mutex_unlock(&mem_lock);
//
// 	return 0;
// }


int free_mem(addr_t address, struct pcb_t * proc) {
	/*TODO: Release memory region allocated by [proc]. The first byte of
	 * this region is indicated by [address]. Task to do:
	 * 	- Set flag [proc] of physical page use by the memory block
	 * 	  back to zero to indicate that it is free.
	 * 	- Remove unused entries in segment table and page tables of
	 * 	  the process [proc].
	 * 	- Remember to use lock to protect the memory from other
	 * 	  processes.  */
	pthread_mutex_lock(&mem_lock);
	if(proc == NULL || proc->pid <= 0)
		return 0;
	int first_lv = get_first_lv(address);
	int second_lv = get_second_lv(address);
	struct page_table_t * page_table = NULL;
	page_table = get_page_table(first_lv, proc->seg_table);
	int p_idx = 0;
	int page_count = 0;
	int seg_count = 0;
	for (int i = 0; i < proc->seg_table->size; i++)
	{
		if(proc->seg_table->table[i].v_index == first_lv)
		{
			seg_count = i;
			break;
		}
	}
//	printf("page_table->size : %d \n",page_table->size );
	// printf("second_lv: %d \n",second_lv);
	// for(int i=0; i<page_table->size;i++)
	// {
	// 	printf("page_table->table[i].v_index: %d \n",page_table->table[i].v_index);
	// }
	for(int i=0; i<page_table->size;i++)
	{
		if(page_table->table[i].v_index ==second_lv)
		{
			p_idx = page_table->table[i].p_index;

			page_count = i;
//			printf("page_table->table[i].v_index : %d \n",page_table->table[i].v_index );
			break;
		}
	}
	int i = p_idx;
	int count = 0;
	while(i!=-1)
	{
		_mem_stat[i].proc = 0;
//		printf("i: %d \n",i);
		i=_mem_stat[i].next;
		count++;
	}
	int num_seg = (page_count + count) / 32;
	int num_page = (page_count + count) % 32;
	for(int i = 0;i <count ;i++)
		page_table->table[page_count + i].v_index = -1;
	if(num_seg > 0)
	{
		for(int i = 0;i <num_page ;i++)
			proc->seg_table->table[seg_count+num_seg].pages->table[i].v_index=-1;
	}
	int flag_1 = 1;
	int flag_2 = 1;
	for(int i = 0;i <proc->seg_table->table[seg_count].pages->size ;i++)
	{
		if(proc->seg_table->table[seg_count].pages->table[i].v_index!=-1)
		{
			flag_1 = 0;
			break;
		}
	}
	for(int i = 0;i <proc->seg_table->table[seg_count+num_seg].pages->size ;i++)
	{
		if(proc->seg_table->table[seg_count+num_seg].pages->table[i].v_index!=-1)
		{
			flag_2 = 0;
			break;
		}

	}
	if(flag_1 == 1)
		free(proc->seg_table->table[seg_count].pages);
	if(flag_2 == 1)
		free(proc->seg_table->table[seg_count+num_seg].pages);
	seg_count++;
	for(int i = 1;i <num_seg ;i++)
	{
		free(proc->seg_table->table[seg_count].pages);
		seg_count++;
	}

//	printf("proc->seg_table->table[0].pages->size : %d, page_table->size: %d \n ",proc->seg_table->table[0].pages->size,page_table->size);
	free(page_table);

	pthread_mutex_unlock(&mem_lock);
	return 1;
}


int read_mem(addr_t address, struct pcb_t * proc, BYTE * data) {
	addr_t physical_addr;
	if (translate(address, &physical_addr, proc)) {
		*data = _ram[physical_addr];
		return 0;
	}else{
		return 1;
	}
}

int write_mem(addr_t address, struct pcb_t * proc, BYTE data) {
	addr_t physical_addr;
	if (translate(address, &physical_addr, proc)) {
		_ram[physical_addr] = data;
		return 0;
	}else{
		return 1;
	}
}

void dump(void) {
	int i;
	for (i = 0; i < NUM_PAGES; i++) {
		if (_mem_stat[i].proc != 0) {
			printf("%03d: ", i);
			printf("%05x-%05x - PID: %02d (idx %03d, nxt: %03d)\n",
				i << OFFSET_LEN,
				((i + 1) << OFFSET_LEN) - 1,
				_mem_stat[i].proc,
				_mem_stat[i].index,
				_mem_stat[i].next
			);
			int j;
			for (	j = i << OFFSET_LEN;
				j < ((i+1) << OFFSET_LEN) - 1;
				j++) {

				if (_ram[j] != 0) {
					printf("\t%05x: %02x\n", j, _ram[j]);
				}

			}
		}
	}
}
