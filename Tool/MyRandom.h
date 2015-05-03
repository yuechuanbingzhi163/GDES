#pragma once

class TRandomMersenne;

/*
* 后期考虑使用boost库的Random来代替，据说该库的效率还不错，功能比较全面（提供了多种random generator）
* 而在我的代码使用过的mersenne也是其中的一种random generator
*/
class MyRandom 
{
public:
	MyRandom();

	MyRandom(unsigned long seed);

	~MyRandom();

	double randomDouble();                /**< Get a double in the interval 0.0 <= x <= 1.0   */

	int randomInt(int min, int max);   /**< Get an integer in the interval min <= x <= max */

	void init(unsigned int seed);    /**< Seed the random number generator               */
	bool flip();

private:
	TRandomMersenne* pTwister;
};

extern MyRandom rng;