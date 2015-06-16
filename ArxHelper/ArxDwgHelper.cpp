#include "StdAfx.h"
#include "ArxDwgHelper.h"

// insert函数的bug???
// http://forums.autodesk.com/t5/Autodesk-ObjectARX/Problem-with-inserting-blocks-from-other-DWG/m-p/2846814#M27764
// http://bbs.mjtd.com/thread-27378-1-1.html
// http://www.mouldsky.com/archiver/tid-66211.html
// 1) 它把原来的块复制了一份，尽管指定了新的块名，但新的块定义中根本就没有数据!!!
// 2) 每执行一次，它都会将所有的块复制1份???

// http://hi.baidu.com/slyrabbit/blog/item/447b2aff0b69623b5c600883.html (解决!!!)

// 从dwg文件中读取所有的块
static void ReadAllBlocks( AcDbDatabase* pDB, AcStringArray& allBlocks )
{
	AcDbBlockTable* pBlkTbl;
	if( Acad::eOk != pDB->getBlockTable( pBlkTbl, AcDb::kForRead ) ) return;

	AcDbBlockTableIterator* pBlockTableIterator;
	pBlkTbl->newIterator( pBlockTableIterator );
	for ( ; !pBlockTableIterator->done(); pBlockTableIterator->step() )
	{
		AcDbBlockTableRecord* pRecord;
		if( Acad::eOk == pBlockTableIterator->getRecord( pRecord, AcDb::kForRead ) )
		{
			AcString str;
			if( Acad::eOk == pRecord->getName( str ) )
			{
				// 排除所有*开头的块定义
				//    1) 系统自带的3个默认块(模型空间、图纸空间)
				//    2) 使用标注的时候，也会生成带有*号的块
				if( str.mid( 0, 1 ) != _T( "*" ) )
				{
					allBlocks.append( str );
				}
			}
			pRecord->close();
		}
	}
	delete pBlockTableIterator;
	pBlkTbl->close();
}

static void GetBlcokIds( AcDbDatabase* pDB, const AcStringArray& allBlocks, AcDbObjectIdArray& blockIds )
{
	int n = allBlocks.length();
	for( int i = 0; i < n; i++ )
	{
		blockIds.append( AcDbObjectId::kNull );
	}

	AcDbBlockTable* pBlkTbl;
	if( Acad::eOk != pDB->getBlockTable( pBlkTbl, AcDb::kForRead ) ) return;

	for( int i = 0; i < n; i++ )
	{
		AcDbBlockTableRecord* pRecord;
		if( Acad::eOk == pBlkTbl->getAt( allBlocks[i], pRecord, AcDb::kForRead ) )
		{
			AcString blkName;
			pRecord->getName( blkName );

			int pos = allBlocks.find( blkName );
			if( pos != -1 )
			{
				blockIds[i] = pRecord->objectId();
			}

			pRecord->close();
		}
	}

	pBlkTbl->close();
}

bool ArxDwgHelper::InsertDwg(const CString& dwgFilePath)
{
	// 读取模板dwg文件到一个空数据库
	AcDbDatabase db( false );
	if( Acad::eOk != db.readDwgFile( dwgFilePath ) ) return false;

	// 读取模板dwg文件中的所有不是*开头的块定义
	AcStringArray allBlocks;
	ReadAllBlocks( &db, allBlocks );

	// 读取模板dwg文件中的块定义id
	AcDbObjectIdArray oldBlockIds;
	GetBlcokIds( &db, allBlocks, oldBlockIds );

	// 当前打开的数据库
	AcDbDatabase* pCurDB = acdbHostApplicationServices()->workingDatabase();

	AcDbIdMapping idMap;
	idMap.setDestDb( pCurDB );
	return ( Acad::eOk ==
		pCurDB->wblockCloneObjects( oldBlockIds, pCurDB->blockTableId(), idMap, AcDb::kDrcReplace ) );
}

void ArxDwgHelper::UpdateDwg()
{
	acedCommand( RTSTR, _T( "REGEN" ), 0 ); // 执行regen命令
}
