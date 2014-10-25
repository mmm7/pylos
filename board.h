#ifndef PYLOS_BOARD_H__
#define PYLOS_BOARD_H__

#include <cstdlib>
#include <iostream>

#define INF 65000 * 30000

#define ASSERT(x) do { \
  if (!(x)) { \
    std::cout << "FAIL[" << __LINE__ << "]: " << #x << std::endl; \
    exit(7); \
  } \
} while(0)

//#define ASSERT(x) (void*)0;

class Board {
 public:
  Board() : b((unsigned long long)BLACK << 62), remaining{15,15} {
  }
  inline void Move(int pos, int mark) {
    ASSERT(mark > 0);
    ASSERT(mark < 0b100);
    const unsigned long long shifted_mark =
        ((unsigned long long)mark) << (pos * 2);
    b |= shifted_mark;

    b &= ~(0b11L << 62);
    b |= ((unsigned long long)mark) << 62;
    --remaining[mark-1];
  }
  inline void Clear(int pos) {
    ++remaining[Get(pos)-1];
    const unsigned long long shifted_negated_mark =
        ~(((unsigned long long)0b11L) << (pos * 2));
    b &= shifted_negated_mark;
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
    const unsigned long long &mask = under_mask[pos];
    return ((b | (b >> 1)) & mask) == mask;
  }
  /*
   * Return true iff the position is not blocked, it is allowed to remove ball.
   */
  inline bool NotBlocked(int pos) const {
    return ((b | (b >> 1)) & over_mask[pos]) == 0L;
  }
  /*
   * Return true iff pos is a valid move position.
   */
  inline bool ValidMovePos(int pos) const {
    return (Get(pos) == EMPTY) && FullUnder(pos);
  }
  /*
   * Return true iff pos is a valid take position.
   */
  inline bool ValidTakePos(int pos) const {
    return (Get(pos) != EMPTY) && NotBlocked(pos);
  }
  /*
   * Return true iff mark has a quartet at pos.
   */
  inline bool HasQuartet(int pos, int mark) {
    unsigned long long state = b;
    if (mark == BLACK) state >>= 1;
    unsigned long long *masks = quartet_mask[pos];
    int i = 0;

    while(1) {
      unsigned long long mask = masks[i];
      if (mask == 0L) return false;
      if ((state & mask) == mask) return true;
    }
  }

  static const int EMPTY = 0;
  static const int WHITE = 1;
  static const int BLACK = 2;

  unsigned long long Hash() const {
    return b;
  }

  int Score() const {
    switch(Get(Pos(3,0,0))) {
      case WHITE: return INF;
      case BLACK: return -INF;
    }
    int w = Remaining(WHITE);
    int b = Remaining(BLACK);
    return (w-b) * 100 * (200-w+b);
  }

  int Remaining(int mark) const {
    ASSERT(mark == 1 || mark == 2);
    return remaining[mark-1];
  }

  // For testing.
  unsigned long long Raw() const {
    return b & ~(0b11L << 62);
  }

 private:
  // The board representation. 30 2-bit fields.
  unsigned long long b;
  char remaining[2];
  // layer, x, y -> index
  static int pmap[4][4][4];
  // Mask with fields that are under this.
  static unsigned long long under_mask[30];
  // Mask with fields that are over this.
  static unsigned long long over_mask[30];
  // For each position:
  // A list of up to 7 masks that are its quartets.
  static unsigned long long quartet_mask[30][7];
};

#endif
