#include <cstdio>
#include <vector>

#include "board.h"

typedef std::vector<unsigned long long> longv;

unsigned long long under[30];
unsigned long long over[30];

void Add(int pos_over, int pos_under) {
  unsigned long long valueover = 0L;
  over[pos_under] |= 1L << (2 * pos_over);
  under[pos_over] |= 1L << (2 * pos_under);
}

void PrintLong(unsigned long long q) {
  if (q != 0L) {
    printf ("  0b");
    for (int i = 0; i < 64; ++i) {
      printf("%d", int(q >> 63) & 1);
      q <<= 1;
    }
  } else {
    printf("  0");
  }
  printf (",\n");
}

void Output() {
  printf("{  // Under\n");
  for (int i = 0; i < 30; ++i) {
    PrintLong(under[i]);
  }
  printf("},\n");

  printf("{  // Over\n");
  for (int i = 0; i < 30; ++i) {
    PrintLong(over[i]);
  }
  printf("},\n");
}

int main(int argc, char **argv) {
  for (int l = 1; l < 4; ++l) {
    for (int i = 0; i < 4-l; ++i) {
      for (int j = 0; j < 4-l; ++j) {
        Add(Board::Pos(l,i,j), Board::Pos(l-1,i,j));
        Add(Board::Pos(l,i,j), Board::Pos(l-1,i+1,j));
        Add(Board::Pos(l,i,j), Board::Pos(l-1,i,j+1));
        Add(Board::Pos(l,i,j), Board::Pos(l-1,i+1,j+1));
      }
    }
  }
  Output();
}
