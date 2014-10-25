#include "board.h"

#include "gtest/gtest.h"

TEST(BoardStatic, Pmap) {
  for (int l = 0; l < 4; ++l) {
    for (int y = 0; y < 4-l; y++) {
      for (int x = 0; x < 4-l; x++) {
        EXPECT_LE(0, Board::Pos(l, y, x));
        EXPECT_GE(31, Board::Pos(l, y, x));
      }
    }
  }
}

TEST(BoardStatic, BitWidth) {
  EXPECT_GT(4, Board::WHITE);
  EXPECT_GT(4, Board::BLACK);
}

TEST(BoardStatic, Binary) {
  EXPECT_EQ(3, 0b11);
}

TEST(Board, MoveClearGet) {
  Board b;
  EXPECT_EQ(Board::EMPTY, b.Get(Board::Pos(0,0,0)));
  EXPECT_EQ(Board::EMPTY, b.Get(Board::Pos(1,0,0)));
  EXPECT_EQ(Board::EMPTY, b.Get(Board::Pos(2,0,0)));
  EXPECT_EQ(0L, b.Raw());
  EXPECT_TRUE(b.ValidMovePos(Board::Pos(0,2,1)));
  b.Move(Board::Pos(2,1,0), Board::BLACK);
  b.Move(Board::Pos(1,1,0), Board::BLACK);
  b.Move(Board::Pos(0,2,1), Board::WHITE);
  EXPECT_EQ(Board::BLACK, b.Get(Board::Pos(2,1,0))) << b.Raw();
  EXPECT_EQ(Board::BLACK, b.Get(Board::Pos(1,1,0))) << b.Raw();
  EXPECT_EQ(Board::WHITE, b.Get(Board::Pos(0,2,1))) << b.Raw();
  EXPECT_EQ(Board::EMPTY, b.Get(Board::Pos(0,0,0)));
  EXPECT_EQ(Board::EMPTY, b.Get(Board::Pos(1,0,0)));
  EXPECT_EQ(Board::EMPTY, b.Get(Board::Pos(2,0,0)));
  EXPECT_FALSE(b.ValidMovePos(Board::Pos(0,2,1)));

  b.Clear(Board::Pos(2,1,0));
  EXPECT_EQ(Board::EMPTY, b.Get(Board::Pos(2,1,0))) << b.Raw();
  EXPECT_EQ(Board::BLACK, b.Get(Board::Pos(1,1,0))) << b.Raw();
  EXPECT_EQ(Board::WHITE, b.Get(Board::Pos(0,2,1))) << b.Raw();
  EXPECT_EQ(Board::EMPTY, b.Get(Board::Pos(0,0,0)));
  EXPECT_EQ(Board::EMPTY, b.Get(Board::Pos(1,0,0)));
  EXPECT_EQ(Board::EMPTY, b.Get(Board::Pos(2,0,0)));

  b.Clear(Board::Pos(1,1,0));
  b.Clear(Board::Pos(0,2,1));
  EXPECT_EQ(Board().Raw(), b.Raw());
}

TEST(Board, FullUnder_ValidMovePos) {
  Board b;
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      EXPECT_TRUE(b.FullUnder(Board::Pos(0, y, x)));
      EXPECT_TRUE(b.ValidMovePos(Board::Pos(0, y, x)));
    }
  }
  b.Move(Board::Pos(0,0,0), Board::WHITE);
  b.Move(Board::Pos(0,0,1), Board::WHITE);
  b.Move(Board::Pos(0,0,2), Board::BLACK);
  b.Move(Board::Pos(0,0,3), Board::WHITE);
  // (0,1,0) missing
  b.Move(Board::Pos(0,1,1), Board::BLACK);
  b.Move(Board::Pos(0,1,2), Board::BLACK);
  b.Move(Board::Pos(0,1,3), Board::BLACK);

  b.Move(Board::Pos(0,2,2), Board::WHITE);
  b.Move(Board::Pos(0,3,3), Board::WHITE);

  EXPECT_FALSE(b.FullUnder(Board::Pos(1,0,0)));
  EXPECT_TRUE(b.FullUnder(Board::Pos(1,0,1)));
  EXPECT_TRUE(b.FullUnder(Board::Pos(1,0,2)));
  EXPECT_FALSE(b.FullUnder(Board::Pos(1,1,0)));
  EXPECT_FALSE(b.FullUnder(Board::Pos(1,1,1)));
  EXPECT_FALSE(b.FullUnder(Board::Pos(1,1,2)));

  EXPECT_FALSE(b.ValidMovePos(Board::Pos(1,0,0)));
  EXPECT_TRUE(b.ValidMovePos(Board::Pos(1,0,1)));
  EXPECT_TRUE(b.ValidMovePos(Board::Pos(1,0,2)));
  EXPECT_FALSE(b.ValidMovePos(Board::Pos(1,1,0)));
  EXPECT_FALSE(b.ValidMovePos(Board::Pos(1,1,1)));
  EXPECT_FALSE(b.ValidMovePos(Board::Pos(1,1,2)));

  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      EXPECT_TRUE(b.FullUnder(Board::Pos(0, y, x)));
    }
  }
  EXPECT_FALSE(b.ValidMovePos(Board::Pos(0,0,0)));
  EXPECT_TRUE(b.ValidMovePos(Board::Pos(0,1,0)));
}

TEST(Board, Systematic) {
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

        EXPECT_TRUE(b.ValidMovePos(Board::Pos(l, y, x)));
        EXPECT_FALSE(b.ValidMovePos(Board::Pos(l-1, y, x)));
        EXPECT_FALSE(b.ValidMovePos(Board::Pos(l-1, y+1, x)));
        EXPECT_FALSE(b.ValidMovePos(Board::Pos(l-1, y, x+1)));
        EXPECT_FALSE(b.ValidMovePos(Board::Pos(l-1, y+1, x+1)));

        EXPECT_FALSE(b.ValidTakePos(Board::Pos(l, y, x)));
        EXPECT_TRUE(b.ValidTakePos(Board::Pos(l-1, y, x)));
        EXPECT_TRUE(b.ValidTakePos(Board::Pos(l-1, y+1, x)));
        EXPECT_TRUE(b.ValidTakePos(Board::Pos(l-1, y, x+1)));
        EXPECT_TRUE(b.ValidTakePos(Board::Pos(l-1, y+1, x+1)));
      }
    }
  }
}

TEST(Board, NotBlocked) {
  Board b;
  b.Move(Board::Pos(0,0,0), Board::BLACK);
  b.Move(Board::Pos(0,0,1), Board::BLACK);
  b.Move(Board::Pos(0,1,0), Board::BLACK);
  b.Move(Board::Pos(0,1,1), Board::BLACK);
  b.Move(Board::Pos(0,2,0), Board::BLACK);
  b.Move(Board::Pos(0,2,1), Board::BLACK);

  EXPECT_TRUE(b.NotBlocked(Board::Pos(0,0,0)));
  EXPECT_TRUE(b.NotBlocked(Board::Pos(0,0,1)));
  EXPECT_TRUE(b.NotBlocked(Board::Pos(0,1,0)));
  EXPECT_TRUE(b.NotBlocked(Board::Pos(0,1,1)));
  EXPECT_TRUE(b.NotBlocked(Board::Pos(0,2,0)));
  EXPECT_TRUE(b.NotBlocked(Board::Pos(0,2,1)));

  EXPECT_TRUE(b.ValidTakePos(Board::Pos(0,0,0)));
  EXPECT_TRUE(b.ValidTakePos(Board::Pos(0,0,1)));
  EXPECT_TRUE(b.ValidTakePos(Board::Pos(0,1,0)));
  EXPECT_TRUE(b.ValidTakePos(Board::Pos(0,1,1)));
  EXPECT_TRUE(b.ValidTakePos(Board::Pos(0,2,0)));
  EXPECT_TRUE(b.ValidTakePos(Board::Pos(0,2,1)));

  EXPECT_FALSE(b.ValidTakePos(Board::Pos(1,0,0)));
  EXPECT_FALSE(b.ValidTakePos(Board::Pos(1,0,1)));

  b.Move(Board::Pos(1,2,0), Board::BLACK);

  EXPECT_TRUE(b.NotBlocked(Board::Pos(0,0,0)));
  EXPECT_TRUE(b.NotBlocked(Board::Pos(0,0,1)));
  EXPECT_TRUE(b.NotBlocked(Board::Pos(0,1,0)));
  EXPECT_TRUE(b.NotBlocked(Board::Pos(0,1,1)));
  EXPECT_FALSE(b.NotBlocked(Board::Pos(0,2,0)));
  EXPECT_FALSE(b.NotBlocked(Board::Pos(0,2,1)));

  EXPECT_TRUE(b.ValidTakePos(Board::Pos(0,0,0)));
  EXPECT_TRUE(b.ValidTakePos(Board::Pos(0,0,1)));
  EXPECT_TRUE(b.ValidTakePos(Board::Pos(0,1,0)));
  EXPECT_TRUE(b.ValidTakePos(Board::Pos(0,1,1)));
  EXPECT_FALSE(b.ValidTakePos(Board::Pos(0,2,0)));
  EXPECT_FALSE(b.ValidTakePos(Board::Pos(0,2,1)));

  EXPECT_FALSE(b.ValidTakePos(Board::Pos(1,0,0)));
  EXPECT_FALSE(b.ValidTakePos(Board::Pos(1,0,1)));

  b.Move(Board::Pos(1,0,1), Board::BLACK);

  EXPECT_TRUE(b.NotBlocked(Board::Pos(0,0,0)));
  EXPECT_FALSE(b.NotBlocked(Board::Pos(0,0,1)));
  EXPECT_TRUE(b.NotBlocked(Board::Pos(0,1,0)));
  EXPECT_FALSE(b.NotBlocked(Board::Pos(0,1,1)));
  EXPECT_FALSE(b.NotBlocked(Board::Pos(0,2,0)));
  EXPECT_FALSE(b.NotBlocked(Board::Pos(0,2,1)));

  b.Move(Board::Pos(1,1,0), Board::BLACK);

  EXPECT_TRUE(b.NotBlocked(Board::Pos(0,0,0)));
  EXPECT_FALSE(b.NotBlocked(Board::Pos(0,0,1)));
  EXPECT_FALSE(b.NotBlocked(Board::Pos(0,1,0)));
  EXPECT_FALSE(b.NotBlocked(Board::Pos(0,1,1)));
  EXPECT_FALSE(b.NotBlocked(Board::Pos(0,2,0)));
  EXPECT_FALSE(b.NotBlocked(Board::Pos(0,2,1)));

  b.Move(Board::Pos(1,0,0), Board::BLACK);

  EXPECT_FALSE(b.NotBlocked(Board::Pos(0,0,0)));
  EXPECT_FALSE(b.NotBlocked(Board::Pos(0,0,1)));
  EXPECT_FALSE(b.NotBlocked(Board::Pos(0,1,0)));
  EXPECT_FALSE(b.NotBlocked(Board::Pos(0,1,1)));
  EXPECT_FALSE(b.NotBlocked(Board::Pos(0,2,0)));
  EXPECT_FALSE(b.NotBlocked(Board::Pos(0,2,1)));
}

TEST(Board, Remaining) {
  Board b;
  EXPECT_EQ(15, b.Remaining(Board::WHITE));
  EXPECT_EQ(15, b.Remaining(Board::BLACK));

  b.Move(Board::Pos(0,0,0), Board::BLACK);
  EXPECT_EQ(15, b.Remaining(Board::WHITE));
  EXPECT_EQ(14, b.Remaining(Board::BLACK));

  b.Move(Board::Pos(0,0,1), Board::BLACK);
  EXPECT_EQ(15, b.Remaining(Board::WHITE));
  EXPECT_EQ(13, b.Remaining(Board::BLACK));

  b.Move(Board::Pos(0,0,2), Board::WHITE);
  EXPECT_EQ(14, b.Remaining(Board::WHITE));
  EXPECT_EQ(13, b.Remaining(Board::BLACK));

  b.Clear(Board::Pos(0,0,1));
  EXPECT_EQ(14, b.Remaining(Board::WHITE));
  EXPECT_EQ(14, b.Remaining(Board::BLACK));
}

TEST(Board, Score) {
  Board b;
  EXPECT_EQ(0, b.Score());
  b.Move(Board::Pos(0,0,0), Board::WHITE);
  EXPECT_GT(0, b.Score());

  b.Move(Board::Pos(0,1,0), Board::BLACK);
}
