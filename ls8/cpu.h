#ifndef _CPU_H_
#define _CPU_H_

// Holds all information about the CPU
struct cpu
{
  unsigned char *ram;
  unsigned char *registers;
  unsigned char pc;
  unsigned char ir;
  unsigned char mar;
  unsigned char mdr;
  unsigned char fl;
};

// ALU operations
enum alu_op
{
  ALU_MUL = 0b10100010,
  ALU_ADD = 0B10100000
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI 0b10000010
#define HLT 0b00000001
#define PRN 0b01000111
#define PUSH 0b01000101
#define POP 0b01000110
#define CALL 0b01010000
#define RET 0b00010001
#define ST 0b10000100
#define CPM 0b10100111
#define JMP 0b01010100
#define JEQ 0b01010101
#define JNE 0b01010110

// Function declarations

extern void cpu_load(struct cpu *cpu, int argc, char **argv);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
