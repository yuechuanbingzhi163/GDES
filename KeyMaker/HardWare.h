// HardWare.h: interface for the CHardWare class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HARDWARE_H__02E765E8_29A2_4DE0_99BE_350996E7C215__INCLUDED_)
#define AFX_HARDWARE_H__02E765E8_29A2_4DE0_99BE_350996E7C215__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHardWare  
{
public:
	CHardWare();
	virtual ~CHardWare();

// Var
public:
	void GetCpuNum( PDISKINFO pDiskInfo );
	void GetBiosNum(  PDISKINFO pDiskInfo  );
	int GetHDNum( PDISKINFO pDiskInfo );

protected:
	BOOL WinNTHDSerialNumAsScsiRead( BYTE* dwSerial, UINT* puSerialLen, UINT uMaxSerialLen );
	BOOL WinNTHDSerialNumAsPhysicalRead( BYTE* dwSerial, UINT* puSerialLen, UINT uMaxSerialLen );
	BOOL DoIdentify( HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP, 
				PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum, 
				PDWORD lpcbBytesReturned );
	UINT FindAwardBios( BYTE** ppBiosAddr );
	UINT FindAmiBios( BYTE** ppBiosAddr );
	UINT FindPhoenixBios( BYTE** ppBiosAddr );
};

#endif // !defined(AFX_HARDWARE_H__02E765E8_29A2_4DE0_99BE_350996E7C215__INCLUDED_)
