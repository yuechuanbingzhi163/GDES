#include "stdafx.h"
#include "BlockDraw_ConfigDlg.h"

IMPLEMENT_DYNAMIC( BlockDraw_ConfigDlg, CDialog )

BlockDraw_ConfigDlg::BlockDraw_ConfigDlg( CWnd* pParent /*=NULL*/ )
    : CDialog( BlockDraw_ConfigDlg::IDD, pParent )
{

}

BlockDraw_ConfigDlg::~BlockDraw_ConfigDlg()
{
}

void BlockDraw_ConfigDlg::DoDataExchange( CDataExchange* pDX )
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_BLOCK_LIST, m_blockList );
}


BEGIN_MESSAGE_MAP( BlockDraw_ConfigDlg, CDialog )
    ON_BN_CLICKED( IDOK, &BlockDraw_ConfigDlg::OnBnClickedOk )
END_MESSAGE_MAP()

void BlockDraw_ConfigDlg::fillBlockList()
{
    // 清空
    m_blockList.ResetContent();

    // 填充块列表
    AcDbBlockTable* pBlockTable;
    acdbHostApplicationServices()->workingDatabase()
    ->getSymbolTable( pBlockTable, AcDb::kForRead );

    // Iterate through the block table and disaply the names in the list box.
    AcString name;
    AcDbBlockTableIterator* pBTItr;
    if ( pBlockTable->newIterator( pBTItr ) == Acad::eOk )
    {
        while ( !pBTItr->done() )
        {
            AcDbBlockTableRecord* pRecord;
            if ( pBTItr->getRecord( pRecord, AcDb::kForRead ) == Acad::eOk )
            {
                pRecord->getName( name );
                // 排除默认的2个块定义(模型空间和图纸空间)
                if( name.find( ACDB_MODEL_SPACE ) < 0 && name.find( ACDB_PAPER_SPACE ) < 0 )
                {
                    m_blockList.AddString( name.kACharPtr() );
                }
                pRecord->close();
            }
            pBTItr->step();
        }
    }
    pBlockTable->close();
}

BOOL BlockDraw_ConfigDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    fillBlockList();

    int sel = m_blockList.FindString( -1, m_blockName );
    if( sel == CB_ERR )
    {
        m_blockList.SetCurSel( sel );
    }
    else
    {
        m_blockList.SetCurSel( 0 );
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

CString BlockDraw_ConfigDlg::getSelBlock()
{
    int sel = m_blockList.GetCurSel();
    CString selType;
    m_blockList.GetLBText( sel, selType );
    return selType;
}

void BlockDraw_ConfigDlg::OnBnClickedOk()
{
    m_blockName = getSelBlock();

    OnOK();
}
