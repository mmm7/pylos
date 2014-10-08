#include "board.h"

#include "gtest/gtest.h"

TEST(BoardStatic, Pmap) {
  for (int l = 0; l < 4; ++l) {
    for (int y = 0; y < 4-l; y++) {
      for (int x = 0; x < 4-l; x++) {
        EXPECT_LE(0, Board::Pos(l, y, x));
      }
    }
  }
}

TEST(BoardStatic, BitWidth) {
  EXPECT_GT(4, Board::WHITE);
  EXPECT_GT(4, Board::BLACK);
  EXPECT_NE(0, Board::WHITE & 1);
  EXPECT_NE(0, Board::BLACK & 1);
  EXPECT_EQ(0, Board::WHITE & 0b10);
  EXPECT_NE(0, Board::BLACK & 0b10);
}

TEST(BoardStatic, Binary) {
  EXPECT_EQ(3, 0b11);
}

TEST(Board, FullUnder_Random) {
  Board b;
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      EXPECT_FALSE(b.FullUnder(Board::Pos(0, y, x)));
    }
  }
  b.Move(Board::Pos(0,0,0), Board::WHITE);
  b.Move(Board::Pos(0,0,1), Board::WHITE);
  b.Move(Board::Pos(0,0,2), Board::WHITE);
  b.Move(Board::Pos(0,0,3), Board::WHITE);
  // (0,1,0) missing
  b.Move(Board::Pos(0,1,1), Board::WHITE);
  b.Move(Board::Pos(0,1,2), Board::WHITE);
  b.Move(Board::Pos(0,1,3), Board::WHITE);

  b.Move(Board::Pos(0,2,2), Board::WHITE);
  b.Move(Board::Pos(0,3,3), Board::WHITE);

  EXPECT_FALSE(b.FullUnder(Board::Pos(1,0,0)));
  EXPECT_TRUE(b.FullUnder(Board::Pos(1,0,1)));
  EXPECT_TRUE(b.FullUnder(Board::Pos(1,0,2)));
  EXPECT_FALSE(b.FullUnder(Board::Pos(1,1,0)));
  EXPECT_FALSE(b.FullUnder(Board::Pos(1,1,1)));
  EXPECT_FALSE(b.FullUnder(Board::Pos(1,1,2)));

  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      EXPECT_FALSE(b.FullUnder(Board::Pos(0, y, x)));
    }
  }
}

TEST(Board, FullUnder_Systematic) {
  Board b;
  for (int l = 1; l < 4; ++l) {
    for (int y = 0; y < 4-l; y++) {
      for (int x = 0; x < 4-l; x++) {
        b.Move(Board::Pos(l-1, y, x), Board::BLACK);
        b.Move(Board::Pos(l-1, y+1, x), Board::BLACK);
        b.Move(Board::Pos(l-1, y, x+1), Board::BLACK);
        b.Move(Board::Pos(l-1, y+1, x+1), Board::BLACK);
        EXPECT_TRUE(b.FullUnder(Board::Pos(l, y, x)))
          << l << ", " << y << ", " << x;
      }
    }
  }
}
