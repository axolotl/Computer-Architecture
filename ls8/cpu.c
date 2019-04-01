#include "cpu.h"

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
      // From print8.ls8
      0b10000010, // LDI R0,8
      0b00000000,
      0b00001000,
      0b01000111, // PRN R0
      0b00000000,
      0b00000001 // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++)
  {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    // TODO
    break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    char *current = cpu->pc;
    // 2. Figure out how many operands this next instruction requires
    switch (*current)
    {
    case LDI:
      unsigned char reg = current + 1;
      int val = current + 2;
      cpu->registers[reg] = val;
      cpu->pc = current + 3;
      break;

    case PRN:
      unsigned char reg = current + 1;
      int val = registers[reg];
      printf("%d\n", val);
      cpu->pc = current + 2;
      break;

    case HLT:
      running = 0;
      current++;
      break;

    default:
      printf("Unrecognized instruction\n");
      exit(1);
      break;
    }

    // 3. Get the appropriate value(s) of the operands following this instruction
    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    // 6. Move the PC to the next instruction.
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  struct cpu *cpu = malloc(sizeof(cpu));
  cpu->registers = malloc(sizeof(char *) * 8);
  cpu->ram = malloc(sizeof(char *) * 256);
  cpu->pc = cpu->ram;
  return cpu;
}
