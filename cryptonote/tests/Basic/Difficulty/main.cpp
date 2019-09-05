// Copyright (c) 2018-2019 The Cash2 developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "gtest/gtest.h"
#include "CryptoNoteCore/Difficulty.h"
#include "CryptoNoteCore/Difficulty.cpp"
#include <random>

using namespace CryptoNote;

/*

My Notes

mul()
mul()
cadd()
cadc()()
check_hash1()

*/

uint32_t loopCount = 100;

// mul(uint64_t a, uint64_t b, uint64_t &low, uint64_t &high)
TEST(Difficulty, 1)
{
  uint64_t low;
  uint64_t high;

  // 1 x 1
  // store least significant 64 bits of result in low
  // store most significant 64 bits of result in high
  // result is 128 bits long
  mul(1, 1, low, high);
  ASSERT_EQ(low, 1);
  ASSERT_EQ(high, 0);

  mul(100, 100, low, high);
  ASSERT_EQ(low, 10000);
  ASSERT_EQ(high, 0);

  // 2^64 = 18446744073709551616
  // need suffix ULL because the number is too large for an int
  // a equals 2^64 - 1
  // a equals binary 1111111111111111111111111111111111111111111111111111111111111111
  uint64_t a = 18446744073709551615ULL;
  
  // result of a x 2 is 1|1111111111111111111111111111111111111111111111111111111111111110 where high is to the left of | and low is to the right of |
  mul(a, 2, low, high);
  // low equals binary 1111111111111111111111111111111111111111111111111111111111111110 which equals 18446744073709551614 in decimal
  ASSERT_EQ(low, 18446744073709551614ULL);
  ASSERT_EQ(high, 1);
}

// bool cadd(uint64_t a, uint64_t b)
// tells if a + b causes an overflow
TEST(Difficulty, 2)
{
  // 1 + 1 < 1
  ASSERT_FALSE(cadd(1, 1));
  // 100 + 5 < 100
  ASSERT_FALSE(cadd(100, 5));
  ASSERT_FALSE(cadd(18446744073709551614ULL, 1));

  // 18446744073709551615ULL equals 1111111111111111111111111111111111111111111111111111111111111111
  // adding 1 will cause the number to wrap around back to zero
  ASSERT_TRUE(cadd(18446744073709551615ULL, 1));
}

// bool cadc(uint64_t a, uint64_t b, bool c)
// tells if a + b causes an overflow
// if c is true, tells if a + b equals 2^64 - 1
TEST(Difficulty, 3)
{
  ASSERT_FALSE(cadc(1, 1, true));
  ASSERT_FALSE(cadc(100, 100, true));

  // sum equals 2^64 - 1
  ASSERT_FALSE(cadc(18446744073709551614ULL, 1, false));
  ASSERT_TRUE(cadc(18446744073709551614ULL, 1, true));

  // overflow has occurred
  ASSERT_TRUE(cadc(18446744073709551615ULL, 1, false));
  ASSERT_TRUE(cadc(18446744073709551615ULL, 1, true));
}

// check_hash2()
TEST(Difficulty, 4)
{
  // check that there are at least 10 leading zeros

  difficulty_type difficulty = 1;

  Crypto::Hash hash =
  {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
  };
  
  ASSERT_FALSE(check_hash2(hash, difficulty));

  hash =
  {
    0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
  };

  ASSERT_FALSE(check_hash2(hash, difficulty));
  
  hash =
  {
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
  };

  ASSERT_FALSE(check_hash2(hash, difficulty));

  hash =
  {
    0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
  };

  ASSERT_FALSE(check_hash2(hash, difficulty));

  hash =
  {
    0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
  };

  ASSERT_FALSE(check_hash2(hash, difficulty));

  hash =
  {
    0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
  };

  ASSERT_FALSE(check_hash2(hash, difficulty));

  hash =
  {
    0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
  };

  ASSERT_FALSE(check_hash2(hash, difficulty));

  hash =
  {
    0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
  };

  ASSERT_FALSE(check_hash2(hash, difficulty));

  hash =
  {
    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
  };

  ASSERT_FALSE(check_hash2(hash, difficulty));

  hash =
  {
    0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
  };

  ASSERT_FALSE(check_hash2(hash, difficulty));

  hash =
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
  };

  ASSERT_TRUE(check_hash2(hash, difficulty));

  hash =
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
  };

  ASSERT_TRUE(check_hash2(hash, difficulty));

  hash =
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
  };

  ASSERT_TRUE(check_hash2(hash, difficulty));
}

// check_hash2()
TEST(Difficulty, 5)
{
  // Trial and error shows that this hash is the largest value that will be accepted
  // for a difficulty of 100
  Crypto::Hash hash =
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
  };

  difficulty_type difficulty = 100;
  ASSERT_TRUE(check_hash2(hash, difficulty));
}

// check_hash2()
TEST(Difficulty, 6)
{
  // Trial and error shows that this hash is the largest value that will be accepted
  // for a difficulty of 500
  Crypto::Hash hash =
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
  };

  difficulty_type difficulty = 500;
  ASSERT_TRUE(check_hash2(hash, difficulty));
}

// check_hash1()
// 100 x 1,099,511,627,776 = 109,951,162,777,600
TEST(Difficulty, 7)
{
  Crypto::Hash hash =
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
  };
  difficulty_type difficulty = 109951162777600;
  ASSERT_TRUE(check_hash1(hash, difficulty));
}

// check_hash1()
// 500 x 1,099,511,627,776 = 549,755,813,888,000
TEST(Difficulty, 8)
{
  Crypto::Hash hash =
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
  };
  difficulty_type difficulty = 549755813888000;
  ASSERT_TRUE(check_hash1(hash, difficulty));
}

// check_hash2()
TEST(Difficulty, 9)
{
  // Trial and error shows that this hash is the largest value that will be accepted
  // for a difficulty of 1,000,000
  // Requires 15 zeros
  Crypto::Hash hash =
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
  };

  difficulty_type difficulty = 1000000;
  ASSERT_TRUE(check_hash2(hash, difficulty));
}

// check_hash2()
TEST(Difficulty, 10)
{
  // Trial and error shows that this hash is the largest value that will be accepted
  // for a difficulty of 1,000,000,000
  // Requires 17 zeros
  Crypto::Hash hash =
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
  };

  difficulty_type difficulty = 1000000000;
  ASSERT_TRUE(check_hash2(hash, difficulty));
}

// check_hash2()
TEST(Difficulty, 11)
{
  // Trial and error shows that this hash is the largest value that will be accepted
  // for a difficulty of 1,000,000,000,000
  // Requires 20 zeros
  Crypto::Hash hash =
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
  };

  difficulty_type difficulty = 1000000000000;
  ASSERT_TRUE(check_hash2(hash, difficulty));
}

// check_hash2()
TEST(Difficulty, 12)
{
  // Trial and error shows that this hash is the largest value that will be accepted
  // for a difficulty of 1,000,000,000,000,000
  // Requires 22 zeros
  Crypto::Hash hash =
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x47, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
  };

  difficulty_type difficulty = 1000000000000000;
  ASSERT_TRUE(check_hash2(hash, difficulty));
}

// check_hash2()
TEST(Difficulty, 13)
{
  // Trial and error shows that this hash is the largest value that will be accepted
  // for a difficulty of 1,000,000,000,000,000,000
  // Requires 24 zeros
  Crypto::Hash hash =
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x11, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
  };

  difficulty_type difficulty = 1000000000000000000;
  ASSERT_TRUE(check_hash2(hash, difficulty));
}

// check_hash2()
TEST(Difficulty, 14)
{
  // Trial and error shows that this hash is the largest value that will be accepted
  // for a difficulty of 18,446,744,073,709,551,615 
  // Requires 26 zeros
  Crypto::Hash hash =
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF
  };

  difficulty_type difficulty = 18446744073709551615ul;
  ASSERT_TRUE(check_hash2(hash, difficulty));
}

// check_hash2()
TEST(Difficulty, 15)
{
  Crypto::Hash hash =
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0xa2, 0xd5, 0x4c,
    0xa5, 0xc9, 0x01, 0xc7, 0xa1, 0x44, 0x47, 0x8b, 0xcc, 0xed,
    0x7a, 0x96, 0xa3, 0xe6, 0xe8, 0xe8, 0x63, 0x2c, 0xae, 0x9e,
    0xcc, 0x94
  };

  // Block 416,318
  // Original difficulty for this hash is 561,040,591,725,467
  // 561,040,591,725,467 / 1,099,511,627,776 = 510

  difficulty_type difficulty = 510;
  ASSERT_TRUE(check_hash2(hash, difficulty));
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}