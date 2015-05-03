

// Reg.cpp: implementation of the CReg class.
//
//////////////////////////////////////////////////////////////////////

#include <Windows.h>
#include "Reg.h"
#include <ctime>
#include <cstring>
#include <cstdlib>

CReg::CReg()
{
	m_pMachineNum = new char[17];
	m_pRegNum = new char[17];
}

CReg::~CReg()
{
	delete m_pMachineNum;
	delete m_pRegNum;
}

CString CReg::GetRegNum( CString userID )
{
	CString strTmp;

	srand( ( unsigned )time( NULL ) );
	memset( m_pRegNum, '\0', 17 );

	// 为什么是9位,注册ID为8位,选择次数为1位
	long tmpIntRegNum[ 9 ] = { '\0' };
	
	// 把每一位存放在数组中.
	for ( int i = 0; i < userID.GetLength(); i++ )
	{
		strTmp = userID.GetAt( i );
		tmpIntRegNum[ i ] = atoi(strTmp);
	}

	for ( int j = 0; j < userID.GetLength() - 1; j++ )
	{
		while ( true )
		{
			// 产生随机数0~90;
			char tmpChar = rand() % 91;

			// 产生的如果是数字.
			if ( tmpChar >= 48 && tmpChar <= 57 )
			{
				m_pRegNum[ userID.GetLength() + j - 1 ] = tmpChar;

				// 如果相加后的值超过了9.
				if ( ( ( tmpIntRegNum[ j ] + tmpChar ) - 57 ) > 0 )
				{
					m_pRegNum[ j ] = ( tmpIntRegNum[ j ] + tmpChar ) % 57 + 47;
				}
				else
				{
					m_pRegNum[ j ] = tmpIntRegNum[ j ] + tmpChar;
				}
				break;
			}
			else if ( tmpChar >= 65 && tmpChar <= 90 )   // 如果是大写字母
			{
				m_pRegNum[ userID.GetLength() + j - 1 ] = tmpChar;

				if ( ( ( tmpIntRegNum[ j ] + tmpChar ) - 90 ) > 0 )
				{
					m_pRegNum[ j ] = ( tmpIntRegNum[ j ] + tmpChar ) % 90 + 64;
				}
				else
				{
					m_pRegNum[ j ] = tmpIntRegNum[ j ] + tmpChar;
				}
				break;
			}
			else
			{
				continue;
			}
		}
	}

	// 以下为生成日期(第16位).
	// tmpIntRegNum[ 8 ]为你选择的使用次数的索引: 0、1、2、3
	m_pRegNum[ 15 ] = tmpIntRegNum[ 8 ] + ( ( m_pRegNum[ 13 ] + m_pRegNum[ 14 ] ) % 23 ) + 65;

	if ( ( ( tmpIntRegNum[ 7 ] + m_pRegNum[ 15 ] ) - 90 ) > 0 )
	{
		m_pRegNum[ 7 ] = ( tmpIntRegNum[ 7 ] + m_pRegNum[ 15 ] ) % 90 + 64;
	}
	else
	{
		m_pRegNum[ 7 ] = tmpIntRegNum[ 7 ] + m_pRegNum[ 15 ];
	}

	m_pRegNum[ 16 ] = '\0';
	
	strTmp = m_pRegNum;
	
	return strTmp;
}

CString CReg::DisPoseID( CString machineNum )
{
	CString strProcess;    // 存经过处理后的用户ID

	strcpy( m_pMachineNum, machineNum );
	int i;
	for ( i = 0; i < machineNum.GetLength(); i++ )
	{
		// 如果是大、小写字母就让它变为相应的数字.
		// 例: a - 0 、 A - 0.
		if ( m_pMachineNum[ i ] >= 65 && m_pMachineNum[ i ] <= 90 )
		{
			m_pMachineNum[ i ] = ( ( m_pMachineNum[ i ] - 65 ) % 10 ) + 48;
		}
		else if ( m_pMachineNum[ i ] >= 97 && m_pMachineNum[ i ] <= 122 )
		{
			m_pMachineNum[ i ] = ( ( m_pMachineNum[ i ] - 97 ) % 10 ) + 48;
		}
		else  if ( m_pMachineNum[ i ] >= 48 && m_pMachineNum[ i ] <= 57 )
		{
			;  // 如果是数字就不变
		}
		else   // 如果是大、小写字母以外的就让它变0
		{
			m_pMachineNum[ i ] = 48;
		}
	}
	
	m_pMachineNum[ i + 1 ] = '\0';
	
	strProcess = m_pMachineNum;
	return strProcess;
}


int CReg::CmpReg(CString userID, CString userReg)
{
	long    nTmpUserID[9] = { '\0' };    // 存放数字化后的用户ID.
	int     nDate;                       // 存放选择的次数索引.

	CString strTmp;                    // 临时变量
	char    cmpTmp;                    // 临时变量

	strcpy( m_pMachineNum, userReg );

	for ( int i = 0; i < userID.GetLength(); i++ )
	{
		strTmp = userID.GetAt( i );
		nTmpUserID[ i ] = atoi(strTmp); 

		int l = userID.GetLength();
		// 如果待比较的是数字.
		if ( m_pMachineNum[ userID.GetLength() + i ]  >= 48
			&&  m_pMachineNum[ userID.GetLength() + i ]  <= 57 )
		{
			if ( ( ( nTmpUserID[ i ] + m_pMachineNum[ userID.GetLength() + i ] ) 
				- 57 ) > 0 )
			{
				cmpTmp = ( nTmpUserID[ i ] + m_pMachineNum[ userID.GetLength() + i ] ) % 57 + 47;
			}
			else
			{
				cmpTmp = nTmpUserID[ i ] + m_pMachineNum[ userID.GetLength() + i ];
			}
		}

		// 如果待比较的是大写字母.
		else
		{
			if ( ( ( nTmpUserID[ i ] + m_pMachineNum[ userID.GetLength() + i ] ) 
				- 90 ) > 0 )
			{
				cmpTmp = ( nTmpUserID[ i ] + m_pMachineNum[ userID.GetLength() + i ] ) % 90 + 64;
			}
			else
			{
				cmpTmp = nTmpUserID[ i ] + m_pMachineNum[ userID.GetLength() + i ];
			}
			
		}

		if ( cmpTmp != m_pMachineNum[ i ] )
		{
			// 注册码错误.
			return 0;
		}
	}

	// 算出日期后加1是为了方便.
	//nDate = m_pMachineNum[ 15 ] - 65 - ( ( m_pMachineNum[ 13 ] + 
	//	m_pMachineNum[ 14 ] ) % 23 ) + 1;
	//
	//// 返回使用次数. 
	//switch ( nDate )
	//{
	//case 1:
	//	return 1;   // 30次
	//case 2:
	//	return 2;   // 60次
	//case 3:
	//	return 3;   // 90次
	//case 4:
	//	return 4;   // 永久
	//}

	// 注册码无误,但日期错误,可能是输入有误或是以发现规率.
	return 4;
}

int CReg::JmValue( int nValue ,int nSelect )
{
	nValue = nValue + nSelect;
	nValue = nValue ^ 111;

	return nValue;
}

int CReg::UnJmValue( int nJmValue, int nSelect ) 
{
	nJmValue = nJmValue ^ 111;
	nJmValue = nJmValue - nSelect;
	
	return nJmValue;    
}


