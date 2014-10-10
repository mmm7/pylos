#include <cstdio>
#include <vector>

#include "board.h"

typedef std::vector<unsigned long long> longv;

void Add(std::vector<longv> *m, const std::vector<int> &p) {
  unsigned long long value = 0L;
  for(auto i : p) {
    value |= 1L << (2 * i);
  }
  for(auto i : p) {
    (*m)[i].push_back(value);
  }
}

void Output(const std::vector<longv> &m) {
  int pos_index = 0;
  for (auto pos : m) {
    printf("{  // %d\n", pos_index++);
    for (int p = 0; p < 7; ++p) {
      unsigned long long q = pos[p];
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
    printf("},\n");
  }
}

int main(int argc, char **argv) {
  std::vector<std::vector<unsigned long long>> m;
  for (int i = 0; i < 30; ++i) {
    m.push_back(std::vector<unsigned long long>());
  }
  // =================================================================== 4x4
  for (int i = 0; i < 4; ++i) {
    std::vector<int> p;
    for (int j = 0; j < 4; ++j) {
      p.push_back(Board::Pos(0,i,j));
    }
    Add(&m, p);
  }
  for (int i = 0; i < 4; ++i) {
    std::vector<int> p;
    for (int j = 0; j < 4; ++j) {
      p.push_back(Board::Pos(0,j,i));
    }
    Add(&m, p);
  }
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      std::vector<int> p;
      p.push_back(Board::Pos(0,i,j));
      p.push_back(Board::Pos(0,i+1,j));
      p.push_back(Board::Pos(0,i,j+1));
      p.push_back(Board::Pos(0,i+1,j+1));
      Add(&m, p);
    }
  }
  // =================================================================== 3x3
  for (int i = 0; i < 3; ++i) {
    std::vector<int> p;
    for (int j = 0; j < 3; ++j) {
      p.push_back(Board::Pos(1,i,j));
    }
    Add(&m, p);
  }
  for (int i = 0; i < 3; ++i) {
    std::vector<int> p;
    for (int j = 0; j < 3; ++j) {
      p.push_back(Board::Pos(1,j,i));
    }
    Add(&m, p);
  }
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      std::vector<int> p;
      p.push_back(Board::Pos(1,i,j));
      p.push_back(Board::Pos(1,i+1,j));
      p.push_back(Board::Pos(1,i,j+1));
      p.push_back(Board::Pos(1,i+1,j+1));
      Add(&m, p);
    }
  }
  // =================================================================== 2x2
  std::vector<int> p;
  p.push_back(Board::Pos(2,0,0));
  p.push_back(Board::Pos(2,1,0));
  p.push_back(Board::Pos(2,0,1));
  p.push_back(Board::Pos(2,1,1));
  Add(&m, p);

  // ========================================== Add 0s.

  for (int i = 0; i < 30; ++i) {
    for (int j = 0; j < 7; ++j) {
      m[i].push_back(0L);
    }
  }
  Output(m);
}
