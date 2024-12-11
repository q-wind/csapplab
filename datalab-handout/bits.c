/* 
 * CS:APP Data Lab 
 * 
 * < q-wind >
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
    // xor(a, b) = or( and(not(a),b), and(a, not(b)) )
    // or(a, b)  = not( and(not(a), not(b)) )
    return ~( ~(~x&y) & ~(x&~y) );
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
    return 1 << 31;
}
//2
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
    // tmax ^ tmin == 0xffffffff
    int y = x + 1;      // tmin or other
    int z = ~(y ^ x);   // 0 or not zero
    // 区分特例 x=-1, y=0, z=0
    return !(z + !y);
}
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
    // 将所有奇数位&在一起，二叉树状两两合并
    x = x & (x >> 16);
    x = x & (x >> 8);
    x = x & (x >> 4);
    x = x & (x >> 2);
    // 最终结果在第2位
    return (x >> 1) & 0x1;
}
/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
    return ~x + 1;
}
//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
    // x - 0x30
    int a = x + (~0x30 + 1);
    // 0x39 - x
    int b = (~x + 1) + 0x39;
    // 以上结果均为>=0情况下才为true 
    return ((a >> 31) + 1) & ((b >> 31) + 1);   // n>>31+1 仅为0(n为负)或1(n非负)
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
    int select_z = (!x << 31) >> 31;
    int select_y = ~select_z;
    return (select_y & y) | (select_z & z);
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
    int sign_x = (x >> 31) & 1;
    int sign_y = (x >> 31) & 1;
    // 异号 x为-，y为+
    int a = (!sign_x) & sign_y;
    // 同号 y - x >= 0，即符号位为0
    int neg_x = ~x + 1;
    int b = !(sign_x ^ sign_y) & !(((y + neg_x) >> 31) & 1);
    return a | b;
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
    // 将所有位 or 到x[31]
    x = x | (x<<16);
    x = x | (x<<8);
    x = x | (x<<4);
    x = x | (x<<2);
    x = x | (x<<1);
    return (x>>31) + 1;    
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
    // 负数 符号1位 + 加1后求相反数最高位数
    // 正数 最高位数 + 1
    int sign_mask = x >> 31;
    int invert_mask = ~sign_mask;
    int h16, h8, h4, h2, h1, h0;
    // conditional 前面负数处理 后面正数处理
    x = (sign_mask & (~(x+1) + 1)) | (invert_mask & x);
    // 最高位1转为数值
    // 高16位是否有1
    h16 = !!(x >> 16) << 4;
    x = x >> h16;   // 若有右移16位
    // 当前16位的高8位
    h8 = !!(x >> 8) << 3;
    x = x >> h8;
    h4 = !!(x >> 4) << 2;
    x = x >> h4;
    h2 = !!(x >> 2) << 1;
    x = x >> h2;
    h1 = !!(x >> 1);    // 当前2位中的高1位
    x = x >> h1;
    h0 = x;     // 最终1位

    return h16+h8+h4+h2+h1+h0 + 1;
}
//float
/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
    // float: 1 sign bit, 8 exp bit, 23 mantissa bit
    unsigned s = uf >> 31 & 1;
    unsigned e = uf >> 23 & 0xff;
    unsigned m = uf & 0x7fffff;
    unsigned tmp;
    // printf("%d %d %d\t", s, e, m);
    if (!e) {  // 非规格值 denorm
        tmp = (m << 1) & 0x7fffff;
        if (tmp < m) {
            e = 1;
        }
        m = tmp;
    } else if (e != 0xff) {    // 规格值 norm
        e = (e + 1) & 0xff;
    } else {    // inf 或 NaN
    
    }    
    // printf("%d %d %d\n", s, e, m);
    return (s<<31) | (e<<23) | m;
}
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
    // int(uf)
    // float: 1 sign bit, 8 exp bit, 23 mantissa bit
    unsigned s = uf >> 31 & 1;
    unsigned e = uf >> 23 & 0xff;
    unsigned m = uf & 0x7fffff;
    int E = e - 127;
    int result;
    // printf("%d %d %d\t", s, e, m);
    if (e == 0xff) {
        return 0x80000000;
    }

    if (e < 127) {  // e 偏置为127
        return 0;
    }

    if (E > 31) {   // 超出范围
        return 0x80000000;
    }

    m = m | 0x800000;   // 隐藏1
    if (E > 23) {
        result = m << (E - 23);
    } else {
        result = m >> (23 - E);
    }
    
    if (s) {
        result = ~result + 1;
    }
    return result;
}
/* 
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 * 
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while 
 *   Max ops: 30 
 *   Rating: 4
 */
unsigned floatPower2(int x) {
    unsigned e = 0;

    if (x < -126) {
        return 0;
    }

    e = x + 127;
    if (e > 0xff) {
        e = 0xff;
    }

    return e << 23; // s==0, m==0
}
