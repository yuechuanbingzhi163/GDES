

// HardWare.cpp: implementation of the CHardWare class.
//
//////////////////////////////////////////////////////////////////////

#include <Windows.h>
#include "HardWare.h"


CHardWare::CHardWare()
{

}

CHardWare::~CHardWare()
{

}

int CHardWare::GetHDNum( PDISKINFO pDiskInfo )
{	
	CString strBuf;    // 临时缓冲区

	// 缓冲区初使化
	memset( pDiskInfo->szSystemInfo, '\0', 4096 );
	pDiskInfo->uSystemInfoLen = 0;

	// 获取系统版本号
	OSVERSIONINFO ovi = { 0 }; 
	ovi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO ); 
	GetVersionEx( &ovi ); 
	
	// 如果不是Windows 2000, WindowsXP, Windows server 2003...就返回
	if( ovi.dwPlatformId != VER_PLATFORM_WIN32_NT ) 
	{ 
		// Only Windows 2000, Windows XP, Windows Server 2003...
		return FALSE; 
	} 
	else 
	{ 
		if( !WinNTHDSerialNumAsPhysicalRead( pDiskInfo->szSystemInfo, 
			&(pDiskInfo->uSystemInfoLen), 1024 ) ) 
		{ 
			if ( !WinNTHDSerialNumAsScsiRead( pDiskInfo->szSystemInfo, 
				&(pDiskInfo->uSystemInfoLen), 1024 ) )
			{
				return FALSE;
			}
		} 
	}

	// 去掉字符串中所有空格
	for ( int i = 0; i < (int)(pDiskInfo->uSystemInfoLen); i++ )
	{
		if ( pDiskInfo->szSystemInfo[ i ] == 32 )
		{
			int j;
			for ( j = i; j < (int)(pDiskInfo->uSystemInfoLen) - 1; j++ )
			{
				pDiskInfo->szSystemInfo[ j ] = pDiskInfo->szSystemInfo[ j + 1 ];
			}
			pDiskInfo->szSystemInfo[ j ] = '\0';
		}
	}

	// 取字符串的前8位
	strBuf = pDiskInfo->szSystemInfo;
	CopyMemory( pDiskInfo->szSystemInfo, strBuf.Left(8), strBuf.GetLength() );

	return TRUE;
}

BOOL CHardWare::WinNTHDSerialNumAsScsiRead(BYTE *dwSerial, UINT *puSerialLen, UINT uMaxSerialLen)
{
	BOOL bInfoLoaded = FALSE; 
	
    for( int iController = 0; iController < 2; ++ iController ) 
    { 
        HANDLE hScsiDriveIOCTL = 0; 
        char   szDriveName[256]; 
		
        //  Try to get a handle to PhysicalDrive IOCTL, report failure 
        //  and exit if can't. 
        sprintf( szDriveName, "\\\\.\\Scsi%d:", iController ); 
		
        //  Windows NT, Windows 2000, any rights should do 
        hScsiDriveIOCTL = CreateFile( szDriveName, 
            GENERIC_READ | GENERIC_WRITE,  
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, 
            OPEN_EXISTING, 0, NULL); 
		
        if( hScsiDriveIOCTL != INVALID_HANDLE_VALUE ) 
        { 
            int iDrive = 0; 
            for( iDrive = 0; iDrive < 2; ++ iDrive ) 
            { 
                char szBuffer[sizeof( SRB_IO_CONTROL ) + SENDIDLENGTH] = { 0 }; 
				
                SRB_IO_CONTROL* p = ( SRB_IO_CONTROL* )szBuffer; 
                SENDCMDINPARAMS* pin = ( SENDCMDINPARAMS* )( szBuffer + sizeof( SRB_IO_CONTROL ) ); 
                DWORD dwResult; 
				
                p->HeaderLength = sizeof( SRB_IO_CONTROL ); 
                p->Timeout = 10000; 
                p->Length = SENDIDLENGTH; 
                p->ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY; 
                strncpy( ( char* )p->Signature, "SCSIDISK", 8 ); 
				
                pin->irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY; 
                pin->bDriveNumber = iDrive; 
				
                if( DeviceIoControl( hScsiDriveIOCTL, IOCTL_SCSI_MINIPORT,  
                    szBuffer, 
                    sizeof( SRB_IO_CONTROL ) + sizeof( SENDCMDINPARAMS ) - 1, 
                    szBuffer, 
                    sizeof( SRB_IO_CONTROL ) + SENDIDLENGTH, 
                    &dwResult, NULL ) ) 
                { 
                    SENDCMDOUTPARAMS* pOut = ( SENDCMDOUTPARAMS* )( szBuffer + sizeof( SRB_IO_CONTROL ) ); 
                    IDSECTOR* pId = ( IDSECTOR* )( pOut->bBuffer ); 
                    if( pId->sModelNumber[0] ) 
                    { 
                        if( * puSerialLen + 20U <= uMaxSerialLen ) 
                        { 
                            // 序列号 
                            CopyMemory( dwSerial + * puSerialLen, ( ( USHORT* )pId ) + 10, 20 ); 
							
                            // Cut off the trailing blanks 
							UINT i;
                            for( i = 20; i != 0U && ' ' == dwSerial[* puSerialLen + i - 1]; -- i ) 
                            {} 
                            * puSerialLen += i; 
							
                            // 型号 
                            CopyMemory( dwSerial + * puSerialLen, ( ( USHORT* )pId ) + 27, 40 ); 
                            // Cut off the trailing blanks 
                            for(  i = 40; i != 0U && ' ' == dwSerial[* puSerialLen + i - 1]; -- i ) 
                            {} 
                            * puSerialLen += i; 

                            bInfoLoaded = TRUE; 
                        } 
                        else 
                        { 
                            ::CloseHandle( hScsiDriveIOCTL ); 
                            return bInfoLoaded; 
                        } 
                    } 
                } 
            } 
            ::CloseHandle( hScsiDriveIOCTL ); 
        } 
    } 
    return bInfoLoaded; 
}

BOOL CHardWare::DoIdentify( HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP, 
				PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum, 
				PDWORD lpcbBytesReturned ) 
{ 
    // Set up data structures for IDENTIFY command. 
    pSCIP->cBufferSize                  = IDENTIFY_BUFFER_SIZE; 
    pSCIP->irDriveRegs.bFeaturesReg     = 0; 
    pSCIP->irDriveRegs.bSectorCountReg  = 1; 
    pSCIP->irDriveRegs.bSectorNumberReg = 1; 
    pSCIP->irDriveRegs.bCylLowReg       = 0; 
    pSCIP->irDriveRegs.bCylHighReg      = 0; 
	
    // calc the drive number. 
    pSCIP->irDriveRegs.bDriveHeadReg = 0xA0 | ( ( bDriveNum & 1 ) << 4 ); 
	
    // The command can either be IDE identify or ATAPI identify. 
    pSCIP->irDriveRegs.bCommandReg = bIDCmd; 
    pSCIP->bDriveNumber = bDriveNum; 
    pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE; 
	
    return DeviceIoControl( hPhysicalDriveIOCTL, DFP_RECEIVE_DRIVE_DATA, 
        ( LPVOID ) pSCIP, 
        sizeof( SENDCMDINPARAMS ) - 1, 
        ( LPVOID ) pSCOP, 
        sizeof( SENDCMDOUTPARAMS ) + IDENTIFY_BUFFER_SIZE - 1, 
        lpcbBytesReturned, NULL ); 
} 

BOOL CHardWare::WinNTHDSerialNumAsPhysicalRead( BYTE* dwSerial, UINT* puSerialLen, UINT uMaxSerialLen ) 
{ 
	#define  DFP_GET_VERSION          0x00074080 
    BOOL bInfoLoaded = FALSE;
	
    for( UINT uDrive = 0; uDrive < 4; ++ uDrive ) 
    { 
        HANDLE hPhysicalDriveIOCTL = 0; 
		
        //  Try to get a handle to PhysicalDrive IOCTL, report failure 
        //  and exit if can't. 
        char szDriveName [256]; 
        sprintf( szDriveName, "\\\\.\\PhysicalDrive%d", uDrive ); 
		
        //  Windows NT, Windows 2000, must have admin rights 
        hPhysicalDriveIOCTL = CreateFile( szDriveName, 
            GENERIC_READ | GENERIC_WRITE,  
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, 
            OPEN_EXISTING, 0, NULL); 
		
        if( hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE ) 
        { 
            GETVERSIONOUTPARAMS VersionParams = { 0 }; 
            DWORD               cbBytesReturned = 0; 
			
            // Get the version, etc of PhysicalDrive IOCTL 
            if( DeviceIoControl( hPhysicalDriveIOCTL, DFP_GET_VERSION, 
                NULL,  
                0, 
                &VersionParams, 
                sizeof( GETVERSIONOUTPARAMS ), 
                &cbBytesReturned, NULL ) ) 
            { 
                // If there is a IDE device at number "i" issue commands 
                // to the device 
                if( VersionParams.bIDEDeviceMap != 0 ) 
                { 
                    BYTE             bIDCmd = 0;   // IDE or ATAPI IDENTIFY cmd 
                    SENDCMDINPARAMS  scip = { 0 }; 
					
                    // Now, get the ID sector for all IDE devices in the system. 
                    // If the device is ATAPI use the IDE_ATAPI_IDENTIFY command, 
                    // otherwise use the IDE_ATA_IDENTIFY command 
                    bIDCmd = ( VersionParams.bIDEDeviceMap >> uDrive & 0x10 ) ? IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY; 
                    BYTE IdOutCmd[sizeof( SENDCMDOUTPARAMS ) + IDENTIFY_BUFFER_SIZE - 1] = { 0 }; 
					
                    if( DoIdentify( hPhysicalDriveIOCTL,  
                        &scip,  
                        ( PSENDCMDOUTPARAMS )&IdOutCmd,  
                        ( BYTE )bIDCmd, 
                        ( BYTE )uDrive, 
                        &cbBytesReturned ) ) 
                    { 
                        if( * puSerialLen + 20U <= uMaxSerialLen ) 
                        { 
                            CopyMemory( dwSerial + * puSerialLen, ( ( USHORT* )( ( ( PSENDCMDOUTPARAMS )IdOutCmd )->bBuffer ) ) + 10, 20 );  // 序列号 
							
                            // Cut off the trailing blanks 
							UINT i;
                            for( i = 20; i != 0U && ' ' == dwSerial[* puSerialLen + i - 1]; -- i )  {} 
                            * puSerialLen += i; 
							
                            CopyMemory( dwSerial + * puSerialLen, ( ( USHORT* )( ( ( PSENDCMDOUTPARAMS )IdOutCmd )->bBuffer ) ) + 27, 40 ); // 型号 
							
                            // Cut off the trailing blanks 
                            for( i = 40; i != 0U && ' ' == dwSerial[* puSerialLen + i - 1]; -- i )  {} 
                            * puSerialLen += i;
							
                            bInfoLoaded = TRUE; 
                        } 
                        else 
                        { 
                            ::CloseHandle( hPhysicalDriveIOCTL ); 
                            return bInfoLoaded; 
                        } 
                    } 
                } 
            } 
            CloseHandle( hPhysicalDriveIOCTL ); 
        } 
    } 
    return bInfoLoaded; 
}

void CHardWare::GetBiosNum( PDISKINFO pDiskInfo )
{
	CString strBuf;    // 临时缓冲区

	memset( pDiskInfo->szSystemInfo, '\0', 4096 );
	pDiskInfo->uSystemInfoLen = 0;
	SIZE_T ssize;  
	
	LARGE_INTEGER so;  
	so.LowPart=0x000f0000; 
	so.HighPart=0x00000000;  
	ssize=0xffff;  
	wchar_t strPH[30]=L"\\device\\physicalmemory";  
	
	DWORD ba=0; 
	
	UNICODE_STRING struniph;  
	struniph.Buffer=strPH;  
	struniph.Length=0x2c;  
	struniph.MaximumLength =0x2e;  
	
	OBJECT_ATTRIBUTES obj_ar;  
	obj_ar.Attributes =64; 
	obj_ar.Length =24; 
	obj_ar.ObjectName=&struniph; 
	obj_ar.RootDirectory=0;  
	obj_ar.SecurityDescriptor=0;  
	obj_ar.SecurityQualityOfService =0;  
	
	HMODULE hinstLib = LoadLibrary("ntdll.dll");  
	ZWOS ZWopenS=(ZWOS)GetProcAddress(hinstLib,"ZwOpenSection");  
	ZWMV ZWmapV=(ZWMV)GetProcAddress(hinstLib,"ZwMapViewOfSection");  
	ZWUMV ZWunmapV=(ZWUMV)GetProcAddress(hinstLib,"ZwUnmapViewOfSection");  
	
	//调用函数，对物理内存进行映射  
	HANDLE hSection;  
	if( 0 == ZWopenS(&hSection,4,&obj_ar) &&  
		0 == ZWmapV(  
		( HANDLE )hSection,   //打开Section时得到的句柄  
		( HANDLE )0xFFFFFFFF, //将要映射进程的句柄，  
		&ba,                  //映射的基址  
		0, 
		0xFFFF,               //分配的大小  
		&so,                  //物理内存的地址  
		&ssize,               //指向读取内存块大小的指针  
		1,                    //子进程的可继承性设定  
		0,                    //分配类型  
		2                     //保护类型  
		) ) 
        //执行后会在当前进程的空间开辟一段64k的空间，并把f000:0000到f000:ffff处的内容映射到这里  
        //映射的基址由ba返回,如果映射不再有用,应该用ZwUnmapViewOfSection断开映射  
	{ 
		BYTE* pBiosSerial = ( BYTE* )ba; 
		UINT uBiosSerialLen = FindAwardBios( &pBiosSerial ); 
		if( uBiosSerialLen == 0U ) 
		{ 
			uBiosSerialLen = FindAmiBios( &pBiosSerial ); 
			if( uBiosSerialLen == 0U ) 
			{ 
				uBiosSerialLen = FindPhoenixBios( &pBiosSerial ); 
			} 
		} 
		if( uBiosSerialLen != 0U ) 
		{ 
			CopyMemory( pDiskInfo->szSystemInfo + pDiskInfo->uSystemInfoLen, 
				pBiosSerial, uBiosSerialLen ); 
			pDiskInfo->uSystemInfoLen += uBiosSerialLen; 
		} 
		ZWunmapV( ( HANDLE )0xFFFFFFFF, ( void* )ba ); 
	}

	// 去掉字符串中所有空格
	for ( int i = 0; i < (int)(pDiskInfo->uSystemInfoLen); i++ )
	{
		if ( pDiskInfo->szSystemInfo[ i ] == 32 )
		{
			int j;
			for ( j = i; j < (int)(pDiskInfo->uSystemInfoLen) - 1; j++ )
			{
				pDiskInfo->szSystemInfo[ j ] = pDiskInfo->szSystemInfo[ j + 1 ];
			}
			pDiskInfo->szSystemInfo[ j ] = _T('\0');
		}
	}

	// 取字符串的前8位
	strBuf = pDiskInfo->szSystemInfo;
	CopyMemory( pDiskInfo->szSystemInfo, strBuf.Left(8), strBuf.GetLength() );
}

UINT CHardWare::FindAwardBios( BYTE** ppBiosAddr ) 
{ 
    BYTE* pBiosAddr = * ppBiosAddr + 0xEC71; 
	
    BYTE szBiosData[128]; 
    CopyMemory( szBiosData, pBiosAddr, 127 ); 
    szBiosData[127] = 0; 
	
    int iLen = lstrlen( ( char* )szBiosData ); 
    if( iLen > 0 && iLen < 128 ) 
    { 
        //AWard:         07/08/2002-i845G-ITE8712-JF69VD0CC-00  
        //Phoenix-Award: 03/12/2002-sis645-p4s333 
        if( szBiosData[2] == '/' && szBiosData[5] == '/' ) 
        { 
            BYTE* p = szBiosData; 
            while( * p ) 
            { 
                if( * p < ' ' || * p >= 127 ) 
                { 
                    break; 
                } 
                ++ p; 
            } 
            if( * p == 0 ) 
            { 
                * ppBiosAddr = pBiosAddr; 
                return ( UINT )iLen; 
            } 
        } 
    } 
    return 0; 
} 

UINT CHardWare::FindAmiBios( BYTE** ppBiosAddr ) 
{ 
    BYTE* pBiosAddr = * ppBiosAddr + 0xF478; 
	
    BYTE szBiosData[128]; 
    CopyMemory( szBiosData, pBiosAddr, 127 ); 
    szBiosData[127] = 0; 
	
    int iLen = lstrlen( ( char* )szBiosData ); 
    if( iLen > 0 && iLen < 128 ) 
    { 
        // Example: "AMI: 51-2300-000000-00101111-030199-" 
        if( szBiosData[2] == '-' && szBiosData[7] == '-' ) 
        { 
            BYTE* p = szBiosData; 
            while( * p ) 
            { 
                if( * p < ' ' || * p >= 127 ) 
                { 
                    break; 
                } 
                ++ p; 
            } 
            if( * p == 0 ) 
            { 
                * ppBiosAddr = pBiosAddr; 
                return ( UINT )iLen; 
            } 
        } 
    } 
    return 0; 
} 

UINT CHardWare::FindPhoenixBios( BYTE** ppBiosAddr ) 
{ 
    UINT uOffset[3] = { 0x6577, 0x7196, 0x7550 }; 
    for( UINT i = 0; i < 3; ++ i ) 
    { 
        BYTE* pBiosAddr = * ppBiosAddr + uOffset[i]; 
		
        BYTE szBiosData[128]; 
        CopyMemory( szBiosData, pBiosAddr, 127 ); 
        szBiosData[127] = 0; 
		
        int iLen = lstrlen( ( char* )szBiosData ); 
        if( iLen > 0 && iLen < 128 ) 
        { 
            // Example: Phoenix "NITELT0.86B.0044.P11.9910111055" 
            if( szBiosData[7] == '.' && szBiosData[11] == '.' ) 
            { 
                BYTE* p = szBiosData; 
                while( * p ) 
                { 
                    if( * p < ' ' || * p >= 127 ) 
                    { 
                        break; 
                    } 
                    ++ p; 
                } 
                if( * p == 0 ) 
                { 
                    * ppBiosAddr = pBiosAddr; 
                    return ( UINT )iLen; 
                } 
            } 
        } 
    } 
    return 0; 
}


void CHardWare::GetCpuNum(  PDISKINFO pDiskInfo  )
{
	memset( pDiskInfo->szSystemInfo, '\0', 4096 );
	pDiskInfo->uSystemInfoLen = 0;
	
	BOOL bException = FALSE; 
	BYTE szCpu[16]  = { 0 }; 
	UINT uCpuID     = 0U; 
	
	__try  
	{ 
		_asm  
		{ 
			mov eax, 0 
			cpuid 
			mov dword ptr szCpu[0], ebx 
			mov dword ptr szCpu[4], edx 
			mov dword ptr szCpu[8], ecx 
			mov eax, 1 
			cpuid 
			mov uCpuID, edx 
		} 
	} 
	__except( EXCEPTION_EXECUTE_HANDLER ) 
	{ 
		bException = TRUE; 
	} 
	
	if( !bException ) 
	{ 
		/*CopyMemory( m_diskInfo.szSystemInfo + m_diskInfo.uSystemInfoLen, 
			&uCpuID, sizeof( UINT ) ); 
		m_diskInfo.uSystemInfoLen += sizeof( UINT );*/ 
		
		uCpuID = strlen( ( char* )szCpu ); 
		CopyMemory( pDiskInfo->szSystemInfo + pDiskInfo->uSystemInfoLen, 
			szCpu, uCpuID ); 
		pDiskInfo->uSystemInfoLen += uCpuID; 
	} 
}

