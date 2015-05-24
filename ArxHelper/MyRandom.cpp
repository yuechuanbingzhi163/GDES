#include "MyRandom.h"

#include "randomc.h"
#include <ctime>

MyRandom::MyRandom()
{
    pTwister = new TRandomMersenne( ( unsigned long )time( NULL ) );
}

MyRandom::MyRandom( unsigned long seed )
{
    pTwister = new TRandomMersenne( seed );
}

MyRandom::~MyRandom()
{
    delete pTwister;
}

double MyRandom::randomDouble()
{
    return pTwister->Random();
}

int MyRandom::randomInt( int min, int max )
{
    return pTwister->IRandom( min, max );
}

void MyRandom::init( unsigned int seed )
{
    pTwister->RandomInit( seed );
}

bool MyRandom::flip()
{
    return randomDouble() > 0.5;
}

//全局变量，作为ga用的random generator
//在外部使用的时候，使用extern MyRandom rng; 即可
// 为了便于测试，使用MyRandom rng(100)形式
// 在正式的程序中，需要修改成MyRandom rng()，使用当前时间作为seed
//MyRandom rng(19847); // 19847这个数是随便指定的，没有任何特殊意义
MyRandom rng;
