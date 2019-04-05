#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

unsigned char cpu_ram_read(struct cpu *cpu, int index)
{
  return cpu->ram[index];
}

void cpu_ram_write(struct cpu *cpu, int index, unsigned char val)
{
  cpu->ram[index] = val;
}

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, int argc, char **argv)
{
  FILE *fp;
  char line[1048];

  if (argc != 2)
  {
    printf("please specific a file\n");
    return 1;
  }

  fp = fopen(argv[1], "r");

  if (fp == NULL)
  {
    printf("error reading file: %s\n", argv[1]);
    return 2;
  }

  int address = 0;

  while (fgets(line, 1048, fp) != NULL)
  {
    char *endptr;
    unsigned char val = strtoul(line, &endptr, 2);

    if (line != endptr)
    {
      cpu_ram_write(cpu, address++, val);
    }
  }
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
    break;

  case ALU_ADD:
    cpu->registers[regA] = cpu->registers[regA] + cpu->registers[regB];
    break;

  case ALU_CMP:
    // flags: 00000LGE
    if (cpu->registers[regA] == cpu->registers[regB])
    {
      cpu->fl = 0b00000001;
    }
    else if (cpu->registers[regA] < cpu->registers[regB])
    {
      cpu->fl = 0b00000100;
    }
    else if (cpu->registers[regA] > cpu->registers[regB])
    {
      cpu->fl = 0b00000010;
    }

    break;

  default:
    printf("Unrecognized ALU instruction: %d\n", op);
    exit(1);
    break;
  }

  // TODO: implement more ALU ops
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running)
  {
    // 1. Get the value of the current instruction (in address PC).
    unsigned char command = cpu->ram[cpu->pc];
    int is_alu = (command & 0b00100000) > 0 ? 1 : 0;                // bit mask to check for alu ops
    int increment_pc_normally = (command & 0b00010000) > 0 ? 0 : 1; // bit mask to check if op sets pc manually

    // 2. Figure out how many operands this next instruction requires
    int num_operands = command >> 6;

    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char operand_1;
    unsigned char operand_2;

    if (num_operands > 0)
    {
      operand_1 = cpu_ram_read(cpu, cpu->pc + 1);
    }
    if (num_operands > 1)
    {
      operand_2 = cpu_ram_read(cpu, cpu->pc + 2);
    }

    // 4. check if op is alu, if yes call alu
    if (is_alu)
    {
      alu(cpu, command, operand_1, operand_2);
    }

    // else switch() over it to decide on a course of action.
    else
    {
      switch (command)
      {
      // 5. Do whatever the instruction should do according to the spec.
      case LDI:
        cpu->registers[operand_1] = operand_2;
        break;

      case PRN:
        printf("%d\n", cpu->registers[operand_1]);
        break;

      case HLT:
        running = 0;
        break;

      case PUSH:
        cpu->registers[7]--;
        cpu->registers[operand_1];
        cpu_ram_write(cpu, cpu->registers[7], cpu->registers[operand_1]);
        break;

      case POP:
        cpu->registers[operand_1] = cpu_ram_read(cpu, cpu->registers[7]);
        cpu->registers[7]++;
        break;

      case CALL:
        // push next instruction to stack
        // set pc to where call is pointing

        // push to stack
        cpu->registers[7]--;
        cpu_ram_write(cpu, cpu->registers[7], cpu->pc + 2);

        // set pc
        cpu->pc = cpu->registers[operand_1];

        break;

      case RET:
        // pop from stack and set pc to that var
        cpu->pc = cpu_ram_read(cpu, cpu->registers[7]);
        cpu->registers[7]++;

        break;

      case ST:
        cpu_ram_write(cpu, cpu->registers[operand_1], cpu->registers[operand_2]);
        break;

      case JMP:
        cpu->pc = cpu->registers[operand_1];
        break;

      case JEQ:
        if (cpu->fl & 0b00000001 > 0)
        {
          cpu->pc = cpu->registers[operand_1];
        }
        else
        {
          increment_pc_normally = 1;
        }

        break;

      case JNE:
        if (cpu->fl != 1)
        {
          cpu->pc = cpu->registers[operand_1];
        }
        else
        {
          increment_pc_normally = 1;
        }

        break;

      default:
        printf("Unrecognized instruction: %d\n", command);
        exit(1);
        break;
      }
    }

    // 6. Move the PC to the next instruction.
    if (increment_pc_normally)
    {
      cpu->pc += num_operands + 1;
    }
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // initialize the PC and other special registers
  cpu->ram = calloc(256, sizeof(unsigned char));
  cpu->registers = calloc(8, sizeof(unsigned char));
  cpu->registers[7] = 0xF3; // stack pointer
  cpu->pc = 0;
  cpu->fl = 0;
  return cpu;
}
