
#include "cpu.h"
#include "mem.h"

static int calc(struct pcb_t * proc) {
	return ((unsigned long)proc & 0UL);
}

static int alloc(struct pcb_t * proc, uint32_t size, uint32_t reg_index) {

	// uint32_t pid;	// PID
	// uint32_t priority;
	// struct code_seg_t * code;	// Code segment
	// addr_t regs[10]; // Registers, store address of allocated regions
	// uint32_t pc; // Program pointer, point to the next instruction
	// struct seg_table_t * seg_table; // Page table
	// uint32_t bp;	// Break pointer
	// printf("\nPCB of proc: \n");
	// printf("PID: %d\n", proc->pid);
	// printf("priority: %d\n", proc->priority);
	// printf("Code size: %d\n", proc->code->size);
	//
	// printf("regs: %d\n", proc->regs[reg_index]);
	//
	// printf("Program Counter: %d\n", proc->pc);
	// printf("Segment size : %d\n", proc->seg_table->size);
	//
	// printf("Break pointer: %d\n", proc->bp);



	addr_t addr = alloc_mem(size, proc);
	if (addr == 0) {
		return 1;
	}else{
		proc->regs[reg_index] = addr;
		return 0;
	}
}

static int free_data(struct pcb_t * proc, uint32_t reg_index) {
	return free_mem(proc->regs[reg_index], proc);
}

static int read(
		struct pcb_t * proc, // Process executing the instruction
		uint32_t source, // Index of source register
		uint32_t offset, // Source address = [source] + [offset]
		uint32_t destination) { // Index of destination register

	BYTE data;
	if (read_mem(proc->regs[source] + offset, proc,	&data)) {
		proc->regs[destination] = data;
		return 0;
	}else{
		return 1;
	}
}

static int write(
		struct pcb_t * proc, // Process executing the instruction
		BYTE data, // Data to be wrttien into memory
		uint32_t destination, // Index of destination register
		uint32_t offset) { 	// Destination address =
					// [destination] + [offset]
	return write_mem(proc->regs[destination] + offset, proc, data);
}

int run(struct pcb_t * proc) {
	/* Check if Program Counter point to the proper instruction */
	if (proc->pc >= proc->code->size) {
		return 1;
	}
	// 
	// printf("\nPCB of proc: \n");
	// printf("PID: %d\n", proc->pid);
	// printf("priority: %d\n", proc->priority);
	// printf("Code size: %d\n", proc->code->size);
	//
	// //printf("regs: %d\n", proc->regs[reg_index]);
	//
	// printf("Program Counter: %d\n", proc->pc);
	// printf("Segment size : %d\n", proc->seg_table->size);
	//
	// for (int i = 0; i < (1 << SEGMENT_LEN); i ++) {
	// 		printf("%d : Virtual addr from segment table : %05x\n", i, proc->seg_table->table[i].v_index);
	// 		//printf("%d : Size of each page_table in segment : %d\n", i, proc->seg_table->table[i]->*pages->size);
	// 		// for (int j = 0; j < (1 << PAGE_LEN); j ++) {
	// 		// 		printf("%d : Virtual addr from page_table_t : %05x\n", j, proc->seg_table->table[j]);
	// 		// 		printf("%d : phisycal addr from page_table_t : %05x\n", j, proc->seg_table->table[j].p_index);
	// 		// 		printf("%d : Virtual addr from page_table_t : %05x\n", j, proc->seg_table->table[j].v_index);
	// 		// }
	// }
	// printf("Break pointer: %d\n", proc->bp);


	struct inst_t ins = proc->code->text[proc->pc];
	proc->pc++;
	int stat = 1;



	switch (ins.opcode) {
	case CALC:
		stat = calc(proc);
		break;
	case ALLOC:
		stat = alloc(proc, ins.arg_0, ins.arg_1);
		break;
	case FREE:
		stat = free_data(proc, ins.arg_0);
		break;
	case READ:
		stat = read(proc, ins.arg_0, ins.arg_1, ins.arg_2);
		break;
	case WRITE:
		stat = write(proc, ins.arg_0, ins.arg_1, ins.arg_2);
		break;
	default:
		stat = 1;
	}
	return stat;

}
