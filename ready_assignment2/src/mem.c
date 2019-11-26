
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

	int i;
	for (i = 0; i < page_table->size; i++) {
		if (page_table->table[i].v_index == second_lv) {
			/* TODO: Concatenate the offset of the virtual addess
			 * to [p_index] field of page_table->table[i] to
			 * produce the correct physical address and save it to
			 * [*physical_addr]  */
			 *physical_addr =  (page_table->table[i].p_index  << OFFSET_LEN )| offset ;

			return 1;
		}
	}
	return 0;
}
//
// addr_t alloc_mem(uint32_t size, struct pcb_t * proc) {
// 	pthread_mutex_lock(&mem_lock);
// 	addr_t ret_mem = 0;
// 	/* TODO: Allocate [size] byte in the memory for the
// 	 * process [proc] and save the address of the first
// 	 * byte in the allocated memory region to [ret_mem].
// 	 * */
//
// 	uint32_t num_pages = ((size % PAGE_SIZE) == 0) ? size / PAGE_SIZE :
// 		size / PAGE_SIZE + 1; // Number of pages we will use
// 	int mem_avail = 0; // We could allocate new memory region or not?
//
// 	/* First we must check if the amount of free memory in
// 	 * virtual address space and physical address space is
// 	 * large enough to represent the amount of required
// 	 * memory. If so, set 1 to [mem_avail].
// 	 * Hint: check [proc] bit in each page of _mem_stat
// 	 * to know whether this page has been used by a process.
// 	 * For virtual memory space, check bp (break pointer).
// 	 * */
//
// 	 	 int numberOfPagesFree = 0;
// 	 	 for (int i = 0; i < NUM_PAGES; i++) { // check disk in document
// 	 		 if (_mem_stat[i].proc == 0) {
// 	 			 numberOfPagesFree++;
//
// 	 		 }
// 	 	 }
//
// 		 uint32_t MaxBreakPoint = proc->bp + NUM_PAGES * PAGE_SIZE;
// 		 uint32_t temp = proc->bp + num_pages * PAGE_SIZE;
// 	 	 if (numberOfPagesFree >= num_pages && MaxBreakPoint >= temp ) { // is disk ready and virtual <= MaxVirtualAdress ready
// 			 		mem_avail = 1;
// 	 	 }
//
// 	 	if (mem_avail) {
// 	 		/* We could allocate new memory region to the process */
// 	 		ret_mem = proc->bp;
// 	 		proc->bp += num_pages * PAGE_SIZE;
// 	 		/* Update status of physical pages which will be allocated
// 	 		 * to [proc] in _mem_stat. Tasks to do:
// 	 		 * 	- Update [proc], [index], and [next] field
// 	 		 * 	- Add entries to segment table page tables
// 	 		 of [proc]
// 	 		 * 	  to ensure accesses to allocated memory slot is
// 	 		 * 	  valid. */
// 			 int sizeNeedForEachSeg = size;
//
// 			 if (sizeNeedForEachSeg % 1024 == 0) {
// 				 sizeNeedForEachSeg = sizeNeedForEachSeg / 1024;
// 			 } else {
// 				 sizeNeedForEachSeg= (int)(sizeNeedForEachSeg / 1024) + 1;
// 			 }
// 			 //printf("%d\n", sizeNeedForEachSeg); // euqal num_pages
// 			 struct page_table_t * page_tmp= (struct page_table_t*)malloc(sizeNeedForEachSeg * sizeof(struct page_table_t));
// 			 // addr_t indexOfFirstLayerFrom = get_first_lv(ret_mem);
// 			 // addr_t indexOfFirstLayerTo = get_first_lv(proc->bp);
// 			 //printf("From %05x to %05x\n", indexOfFirstLayerFrom, indexOfFirstLayerTo);
//
//
//
// 			 // for (addr_t i = indexOfFirstLayerFrom; i < indexOfFirstLayerTo; i++) {
// 				//  proc->seg_table->table[i].pages = page_tmp;
// 				//  proc->seg_table->table[i].v_index = i;
// 				//  proc->seg_table->size++;
// 			 // }
//
//
// 				if(proc->seg_table->size == 0)
// 		 		{
// 		 			proc->seg_table->table[0].pages = page_tmp;
// 		 			proc->seg_table->table[0].v_index = get_first_lv(ret_mem);
// 		 			proc->seg_table->size++;
// 		 		}
// 			 		int seg_count = proc->seg_table->size;
// 					//printf(" %d \n ", proc->seg_table->size);
//
// 			 		int pages_count = proc->seg_table->table[seg_count - 1].pages->size;
// 			 //		printf("pages_count : %d \n",pages_count);
// 			 		// if(pages_count == 32)
// 			 		// {
// 			 		// 	proc->seg_table->size++;
// 			 		// 	seg_count++;
// 			 		// 	proc->seg_table->table[seg_count-1].v_index = get_first_lv(ret_mem);
// 			 		// 	pages_count=0;
// 			 		// }
// 			 		int count = 0;
// 			 		int pre_index = 0;
// 			 		page_tmp = proc->seg_table->table[seg_count - 1].pages;
// 			 		for(int i=0; i<NUM_PAGES; i++)
// 			 		{
// 			 			// Nếu page free
// 			 			if(_mem_stat[i].proc == 0)
// 			 			{
// 			 				//Update [next] của page
// 			 				//count > 0 gán giá trị next của page trước đó bằng page hiện tại
// 			 				if(count>0)
// 			 				{
// 			 					_mem_stat[pre_index].next = i;
// 			 				}
// 			 				//Update [proc] và [index] của page trong _mem_stat
// 			 				_mem_stat[i].proc = proc->pid;
// 			 				_mem_stat[i].index = count;
// 			 				pre_index = i;
// 			 				//Update page_table
// 			 				page_tmp->table[pages_count].p_index = i;
// 			 				page_tmp->table[pages_count].v_index = get_second_lv(ret_mem + count * PAGE_SIZE);
// 			 //				printf("page_tmp->table[pages_count].v_index : %d,pages_count :%d \n",get_second_lv(ret_mem + count * PAGE_SIZE),pages_count);
// 			 				page_tmp->size++;
// 			 				pages_count++;
// 			 				if(pages_count !=0 && pages_count % 32 == 0)
// 			 				{
// 			 //					proc->seg_table->table[seg_count].pages = page_tmp;
// 			 					proc->seg_table->table[seg_count].v_index = get_first_lv(ret_mem + (count + 1) * PAGE_SIZE);
// 			 					page_tmp = NULL;
// 			 					page_tmp = (struct page_table_t*)malloc(sizeNeedForEachSeg * sizeof(struct page_table_t));
// 			 					page_tmp = proc->seg_table->table[seg_count].pages;
// 			 					proc->seg_table->size++;
// 			 					seg_count++;
// 			 					pages_count = 0;
// 			 					page_tmp->size=0;
// 			 				}
// 			 				count++;
// 			 				// Gán hết thì dừng
// 			 				if(count == num_pages)
// 			 				{
// 			 					_mem_stat[i].next = -1;
// 							//	free(page_tmp);
// 			 					break;
//
// 			 				}
// 			 			}
// 			 		}
//
// //
// // 	 		// int seg_count = get_first_lv(ret_mem); //first layer index _ index of segment table
// // 	 		// int page_count = get_second_lv(ret_mem); //second  layer index _ index of _mem_stat table
// //
// // 	 		 //proc->seg_table->size = num_pages / 32; // what is 32?
// // 	 		 // if (num_pages % 32 != 0) {
// // 	 			//  proc->seg_table->size++;
// // 	 		 // }
// //
// // 	 		 // viet cai size tren ram vo _mem_status
// // 	 		 // bay gio la tu phisycal ra virtual nen ko dung translate
// // 	 		 //proc->seg_table->size = num_pages ; // number of rows which allocate in seg_table[]
// //
// // 	 		 // check trong segment coi no da ton tai cai page hay chua
// // 	 		//  struct page_table_t * page_tmp = (struct page_table_t*) malloc(sizeof(struct page_table_t));;
// // 	 		//  if ( get_page_table(seg_count, proc->seg_table) != NULL) {
// // 	 		//   	page_tmp = get_page_table(seg_count, proc->seg_table);
// // 	 		//  } else {
// // 			//
// // 	 		//  		 proc->seg_table->table[seg_count].pages = page_tmp; // if not start with empty segment table
// // 	 	 	// }
// //
// // 			struct page_table_t * page_tmp= (struct page_table_t*)malloc(sizeof(struct page_table_t));
// // 		if(proc->seg_table->size == 0)
// // 		{
// // 			proc->seg_table->table[0].pages = page_tmp;
// // 			proc->seg_table->table[0].v_index = get_first_lv(ret_mem);
// // 			proc->seg_table->size++;
// // //			printf("proc->seg_table->table[0].pages->size : %d, page_tmp->size: %d \n ",proc->seg_table->table[0].pages->size,page_tmp->size);
// // 		}
// //
// // 			int seg_count = proc->seg_table->size;
// // 			int pages_count = proc->seg_table->table[seg_count-1].pages->size;
// //
// // 	 		int count = 0;
// // 	 		int pre_index = 0;
// // 			page_tmp = proc->seg_table->table[seg_count-1].pages;
// // 	 		for (int i = 0; i < NUM_PAGES; i++) {
// //
// // 	 		  // check free on mem
// // 	 		  if (_mem_stat[i].proc == 0) {
// // 	 				_mem_stat[i].proc = proc->pid;
// // 	 				_mem_stat[i].index = count; // index cuar frame trong
// //
// //
// // 	 			 	 // update next of page
// // 	 			 	 if (count > 0) {
// // 	 			 		 _mem_stat[pre_index].next = i;
// // 	 			 	 }
// // 	 				 pre_index = i;
// //
// //
// // 	 				 // //update page_table
// // 	 				 page_tmp->table[pages_count].p_index = i; // to get the physical on ram
// // 	 				 page_tmp->table[pages_count].v_index = get_second_lv(ret_mem + count * PAGE_SIZE);
// // 	 				 page_tmp->size++;
// // 					 //printf("fsdaf: %d   --- %d\n" , page_tmp->table[page_count].v_index, page_tmp->table[page_count].p_index);
// //
// // 	 				 pages_count++;
// //
// // 					 if ( (pages_count != 0 && pages_count % 32 == 0) || (count + 1) == num_pages) {
// // 						 page_tmp = NULL;
// // 						 page_tmp = (struct page_table_t*) malloc(sizeof(struct page_table_t));
// // 						 pages_count = 0;
// // 						 seg_count++;
// //
// // 					 }
// //
// // 	 				 // if ( (page_count != 0 && page_count % 32 == 0) || (count + 1) == num_pages) {
// // 	 					//  page_tmp = NULL;
// // 	 					//  page_tmp = (struct page_table_t*) malloc(sizeof(struct page_table_t));
// // 	 					//  page_count = 0;
// // 	 					//  seg_count++;
// // 					 //
// // 	 				 // }
// // 	 				 count++;
// // 	 			 	 // done allocate
// // 	 			 	 if (count  == num_pages) {
// // 	 			 		_mem_stat[i].next = -1;
// // 	 			 		free(page_tmp);
// // 	 			 		break;
// // 	 				}
// //
// // 	 		  }
// // 	 		}
// 	 	}
//
// 	 	pthread_mutex_unlock(&mem_lock);
// 	 	return ret_mem;
// 	 }
//

addr_t alloc_mem(uint32_t size, struct pcb_t * proc) {
	pthread_mutex_lock(&mem_lock);
	addr_t ret_mem = 0;
	/* TODO: Allocate [size] byte in the memory for the
	 * process [proc] and save the address of the first
	 * byte in the allocated memory region to [ret_mem].
	 * */

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


		// uint32_t MaxBreakPoint = proc->bp + NUM_PAGES * PAGE_SIZE;
		//  uint32_t temp = proc->bp + num_pages * PAGE_SIZE;
		 if (numberOfPagesFree >= num_pages && num_pages < (1 << SEGMENT_LEN) ) { // check size of memory  and virtual memory
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

 			 // uint32_t sizeNeedForEachSeg = size;
			 //
 			 // if (sizeNeedForEachSeg % 1024 == 0) {
 				//  sizeNeedForEachSeg = sizeNeedForEachSeg / 1024;
 			 // } else {
 				//  sizeNeedForEachSeg= (uint32_t)(sizeNeedForEachSeg / 1024) ;
 			 // }
 			 //printf("%d\n", sizeNeedForEachSeg); // euqal num_pages
 			 struct page_table_t * page_tmp= (struct page_table_t*)malloc( sizeof(struct page_table_t));
 			 // int indexOfFirstLayerFrom = get_first_lv(ret_mem);
 			 // int indexOfFirstLayerTo = get_first_lv(proc->bp);
 			 // printf("From %05x to %05x\n", indexOfFirstLayerFrom, indexOfFirstLayerTo);
			 //
			 // printf("Size : %02x\n", size);
			 //
			 // for (int i = indexOfFirstLayerFrom; i < indexOfFirstLayerTo; i++) {
				//  proc->seg_table->table[i].pages = page_tmp;
				//  proc->seg_table->table[i].v_index = get_first_lv(ret_mem + i * PAGE_SIZE);
				//  proc->seg_table->size++;
			 // }

				if(proc->seg_table->size == 0) {
					proc->seg_table->table[0].pages = page_tmp;
					page_tmp->table[0].p_index = 0;
					proc->seg_table->table[0].v_index = get_first_lv(ret_mem );
					proc->seg_table->size++;
				}
					int count = 0;
			 		int pre_index = 0;

					// vi size = 5 thi co the accesses 0 - 4
			 		page_tmp = proc->seg_table->table[proc->seg_table->size - 1].pages;

			 		for(int i = 0; i < NUM_PAGES; i++)
			 		{

			 			if(_mem_stat[i].proc == 0) {
				 			if( count > 0 )
				 				{
				 					_mem_stat[pre_index].next = i;
				 				}
			 			_mem_stat[i].proc = proc->pid;
			 			_mem_stat[i].index = count;
			 				pre_index = i;

							page_tmp->table[page_tmp->size].p_index = i; // bang ben trong
			 				page_tmp->table[page_tmp->size].v_index = get_second_lv(ret_mem +  (addr_t)count * PAGE_SIZE); // ben ben trong ung voi 1 Segment
			 				page_tmp->size++;


							// het 1 page qua seg moi
			 				if (	page_tmp->size != 0 && 	page_tmp->size % 32 == 0)
			 				{

			 					proc->seg_table->table[	proc->seg_table->size].v_index = get_first_lv(ret_mem + (addr_t)count  * PAGE_SIZE);
			 					page_tmp = proc->seg_table->table[	proc->seg_table->size].pages;

			 					proc->seg_table->size++;

			 				}
			 				count++;

							// done _mem_stat
			 				if(count == num_pages)
			 				{
			 					_mem_stat[i].next = -1;

			 					break;

			 				}
			 			}
			 		}
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
// 	 * 	  processes.  */
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

	if ( page_table != NULL) {
			int p_idx = 0;


			// tim trong page_table cua address do
			for(int i = 0; i < page_table->size;i++)
			{
				if(page_table->table[i].v_index == second_lv)
				{
					p_idx = page_table->table[i].p_index;
					break;
				}
			}

			// printf("Before free: \n");
			//
			// int i;
			// for (i = 0; i < NUM_PAGES; i++) {
			// 	if (_mem_stat[i].proc != 0) {
			// 		printf("%03d: ", i);
			// 		printf("%05x-%05x - PID: %02d (idx %03d, nxt: %03d)\n",
			// 			i << OFFSET_LEN,
			// 			((i + 1) << OFFSET_LEN) - 1,
			// 			_mem_stat[i].proc,
			// 			_mem_stat[i].index,
			// 			_mem_stat[i].next
			// 		);
			//
			// 	}
			// }
			int i = p_idx;
			//i = p_idx;
			int count = 0;
			while(i != -1)
			{
				//printf("Free:%d,  Next: %d , Next id: %d \n", i, _mem_stat[i].next, i + 1);
				_mem_stat[i].proc = 0;
				i = _mem_stat[i].next;
				count++;

				// int j;
				// // printf("%d\n", i << OFFSET_LEN);
				// // printf("%d\n", (i+ 1) << OFFSET_LEN);
				//
				// for (	j = i << OFFSET_LEN; j < ((i+1) << OFFSET_LEN) - 1; j++) {
				//
				// 	if (_ram[j] != 0) {
				// 		printf("\t%05x: %02x\n", j, _ram[j]);
				// 	}
				// }

			}
			// printf("After free: \n");
			//
			// for (i = 0; i < NUM_PAGES; i++) {
			// 	if (_mem_stat[i].proc != 0) {
			// 		printf("%03d: ", i);
			// 		printf("%05x-%05x - PID: %02d (idx %03d, nxt: %03d)\n",
			// 			i << OFFSET_LEN,
			// 			((i + 1) << OFFSET_LEN) - 1,
			// 			_mem_stat[i].proc,
			// 			_mem_stat[i].index,
			// 			_mem_stat[i].next
			// 		);
			//
			// 	}
			// }

		}

	pthread_mutex_unlock(&mem_lock);
	return 0;
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
		//printf("%d\n", data);
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
			// printf("%d\n", i << OFFSET_LEN);
			// printf("%d\n", (i+ 1) << OFFSET_LEN);

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
