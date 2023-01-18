/**
 * Project 1
 * Assembler code fragment for LC-2K
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Every LC2K file will contain less than 1000 lines of assembly.
#define MAXLINELENGTH 1000

typedef struct node {
    const char* label;
    uint32_t address;
    struct node* next;
} node_t;

node_t* empty_list();
void insert_label(node_t* head, const char* label, uint32_t address);
uint32_t find_label(node_t* head, const char* label);
uint32_t resolve_address(node_t* head, const char* addr);
int readAndParse(FILE *, char *, char *, char *, char *, char *);
static inline int isNumber(const char *);
uint8_t parse_register(const char* reg);
int32_t encodeInstruction(uint32_t addr, node_t* head, char *, char *, char *, char *, char *);

int main(int argc, char **argv) {
  char *inFileString, *outFileString;
  FILE *inFilePtr, *outFilePtr;
  char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH],
      arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];

  if (argc != 3) {
    printf("error: usage: %s <assembly-code-file> <machine-code-file>\n",
           argv[0]);
    exit(1);
  }

  inFileString = argv[1];
  outFileString = argv[2];

  inFilePtr = fopen(inFileString, "r");
  if (inFilePtr == NULL) {
    printf("error in opening %s\n", inFileString);
    exit(1);
  }
  outFilePtr = fopen(outFileString, "w");
  if (outFilePtr == NULL) {
    printf("error in opening %s\n", outFileString);
    exit(1);
  }

  // Parse labels
  node_t* label_list = empty_list();
  for (uint32_t addr = 0; readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2); ++addr) {
    if (strlen(label) == 0) {
      continue;
    }

    insert_label(label_list, label, addr);
  }

  /* this is how to rewind the file ptr so that you start reading from the
      beginning of the file */
  rewind(inFilePtr);

  // parse opcodes
  for (uint32_t addr = 0; readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2); ++addr) {
    int32_t instruction = encodeInstruction(addr, label_list, label, opcode, arg0, arg1, arg2);

    char outLine[16];
    snprintf(outLine, 16, "%d\n", instruction);
    fputs(outLine, outFilePtr);
  }

  return (0);
}

/*
 * NOTE: The code defined below is not to be modifed as it is implimented
 * correctly.
 */

/*
 * Read and parse a line of the assembly-language file.  Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0,
                 char *arg1, char *arg2) {
  char line[MAXLINELENGTH];
  char *ptr = line;

  /* delete prior values */
  label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

  /* read the line from the assembly-language file */
  if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL) {
    /* reached end of file */
    return (0);
  }

  /* check for line too long */
  if (strlen(line) == MAXLINELENGTH - 1) {
    printf("error: line too long\n");
    exit(1);
  }

  // Treat a blank line as end of file.
  // Arguably, we could just ignore and continue, but that could
  // get messy in terms of label line numbers etc.
  char whitespace[4] = {'\t', '\n', '\r', ' '};
  int nonempty_line = 0;
  for (size_t line_idx = 0; line_idx < strlen(line); ++line_idx) {
    int line_char_is_whitespace = 0;
    for (int whitespace_idx = 0; whitespace_idx < 4; ++whitespace_idx) {
      if (line[line_idx] == whitespace[whitespace_idx]) {
        ++line_char_is_whitespace;
        break;
      }
    }
    if (!line_char_is_whitespace) {
      ++nonempty_line;
      break;
    }
  }
  if (nonempty_line == 0) {
    return 0;
  }

  /* is there a label? */
  ptr = line;
  if (sscanf(ptr, "%[^\t\n ]", label)) {
    /* successfully read label; advance pointer over the label */
    ptr += strlen(label);
  }

  /*
   * Parse the rest of the line.  Would be nice to have real regular
   * expressions, but scanf will suffice.
   */
  sscanf(ptr,
         "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r "
         "]%*[\t\n\r ]%[^\t\n\r ]",
         opcode, arg0, arg1, arg2);

  return (1);
}

node_t* empty_list() {
    node_t* res = malloc(sizeof(node_t));

    char* label = malloc(1);
    label[0] = '\0';
    res->label = label;
    res->address = -1;
    res->next = NULL;
    return res;
}

void insert_label(node_t* head, const char* label, uint32_t address) {
    while (head->next != NULL) {
      if (!strcmp(label, head->label)) {
        puts("Error: duplicate label\n");
        exit(1);
      }
      head = head->next;
    }

    node_t* node = head->next = malloc(sizeof(node_t));
    node->label = strdup(label);
    node->address = address;
    node->next = NULL;
}

uint32_t find_label(node_t* head, const char* label) {
    while (head != NULL && strcmp(head->label, label)) {
      head = head->next;
    }

    if (head == NULL) {
      puts("Error: undefined label\n");
      exit(1);
    }

    return head->address;
}

uint32_t resolve_address(node_t* head, const char* addr) {
    if (strlen(addr) == 0) {
      puts("Error: expected address, found empty string \n");
      exit(1);
    }

    if (isNumber(addr)) {
      return atoi(addr);
    }

    return find_label(head, addr);
}

static inline int isNumber(const char *string) {
  int num;
  char c;
  return ((sscanf(string, "%d%c", &num, &c)) == 1);
}

uint8_t parse_register(const char* reg) {
  if (!isNumber(reg)) {
    puts("Error: register not a number\n");
    exit(1);
  }
  uint32_t res = atoi(reg);

  if (res > 7 || res < 0) {
    puts("Error: register number out of bounds\n");
    exit(1);
  }

  return res;
}

int32_t encodeInstruction(uint32_t addr, node_t* label_list, char *label, char *opcode, char *arg0, char *arg1, char *arg2) {
    // O-type
    // no arguments
    if (!strcmp(opcode, "halt")) {
      return 0b110 << 22;
    }

    if (!strcmp(opcode, "noop")) {
      return 0b111 << 22;
    }

    // Fill
    // arg0 -> immediate or label
    if (!strcmp(opcode, ".fill")) {
      return resolve_address(label_list, arg0);
    }

    // J-type
    // arg0 -> reg (A)
    // arg1 -> reg (B)
    uint8_t regA = parse_register(arg0);
    uint8_t regB = parse_register(arg1);
    if (!strcmp(opcode, "jalr")) {
      return (0b101 << 22) |
        (regA << 19) |
        (regB << 16);
    }

    // R-type
    // arg0 -> reg (A)
    // arg1 -> reg (B)
    // arg2 -> reg (dest)
    if (!strcmp(opcode, "add")) {
      uint8_t regDest = parse_register(arg2);
      return (0b000 << 22) |
        (regA << 19) |
        (regB << 16) |
        regDest;
    }

    if (!strcmp(opcode, "nor")) {
      uint8_t regDest = parse_register(arg2);
      return (0b001 << 22) |
        (regA << 19) |
        (regB << 16) |
        regDest;
    }

    // I-type
    // arg0 -> reg (A)
    // arg1 -> reg (B)
    // arg2 -> address or label
    int16_t arg2Resolved = resolve_address(label_list, arg2);
    if (arg2Resolved >= pow(2, 15) || arg2Resolved < -pow(2, 15)) {
      puts("Error: out of bounds offset\n");
      exit(1);
    }
    if (!strcmp(opcode, "lw")) {
      return (0b010 << 22) |
        (regA << 19) |
        (regB << 16) |
        arg2Resolved;
    }

    if (!strcmp(opcode, "sw")) {
      return (0b011 << 22) |
        (regA << 19) |
        (regB << 16) |
        arg2Resolved;
    } 

    if (!strcmp(opcode, "beq")) {
      if (isNumber(arg2)) {
        arg2Resolved = atoi(arg2);
      } else {
        arg2Resolved -= addr + 1;
        if (arg2Resolved >= pow(2, 15) || arg2Resolved < -pow(2, 15)) {
          puts("Error: out of bounds offset\n");
          exit(1);
        }
      }

      return (0b100 << 22) |
        (regA << 19) |
        (regB << 16) |
        ((uint32_t) (uint16_t) arg2Resolved);
    }

    exit(1);
}