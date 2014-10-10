#ifndef PYLOS_BOARD_H__
#define PYLOS_BOARD_H__

#include <cstdlib>
#include <iostream>

#define ASSERT(x) do { \
  if (!(x)) { \
    std::cout << "FAIL[" << __LINE__ << "]: " << #x << std::endl; \
    exit(7); \
  } \
} while(0)

//#define ASSERT(x) (void*)0;

class Board {
 public:
  Board() : b(0) {}
  inline bool Move(int pos, int mark) {
    ASSERT(mark > 0);
    ASSERT(mark < 0b100);
    const unsigned long long shifted_mark = ((unsigned long long)mark) << (pos * 2);
    b |= shifted_mark;
  }
  static inline int Pos(int layer, int y, int x) {
    ASSERT(layer >= 0);
    ASSERT(layer < 4);
    ASSERT(y >= 0);
    ASSERT(x >= 0);
    ASSERT(x <= 4-layer);
    ASSERT(y <= 4-layer);
    return pmap[layer][y][x];
  }
  // Get the mark.
  inline int Get(int pos) const {
    return (b >> (2 * pos)) & 0b11L;
  }
  /*
   * Return true iff all four positions below pos are filled.
   * Returns true on bottom row.
   */
  inline bool FullUnder(int pos) const {
    ASSERT(pos >= 0);
    ASSERT(pos < 30);
    const long long &mask = under_mask[pos];
    return ((b | (b >> 1)) & mask) == mask;
  }
  inline bool NotBlocked(int pos) const {
    return ((b | (b >> 1)) & over_mask[pos]) == 0L;
  }
  inline bool ValidMovePos(int pos) const {
    return (Get(pos) == EMPTY) && FullUnder(pos);
  }
  inline bool ValidTakePos(int pos) const {
    return (Get(pos) != EMPTY) && NotBlocked(pos);
  }
  static const int EMPTY = 0;
  static const int WHITE = 1;
  static const int BLACK = 2;

  // For testing.
  unsigned long long Raw() const {
    return b;
  }

 private:
  unsigned long long b;
  static int pmap[4][4][4];
  static unsigned long long under_mask[30];
  static unsigned long long over_mask[30];
  static unsigned long long quartet_mask[30][7];
};

#endif
