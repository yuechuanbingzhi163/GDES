#ifndef C_MY_WORD_H
#define C_MY_WORD_H

#include "msword.h"  
  
#define wdCharacter 1  
#define wdLine 5  
#define wdCell 12  
#define wdExtend 1  
#define wdMove 0  
//页眉视图
#define wdSeekCurrentPageHeader 9
//页脚视图
#define wdSeekCurrentPageFooter 10
//当前视图
#define wdSeekMainDocument 0

//左对齐
#define wdAlignParagraphJustify 3
//居中
#define wdAlignParagraphCenter 1
//右对齐
#define wdAlignParagraphRight 2

#include "atlbase.h"  
  
  
class CmyWord    
{  
public:  
    CmyWord();  
    virtual ~CmyWord();  
private:  
    _Application m_wdApp;  
    Documents m_wdDocs;  
    _Document m_wdDoc;  
    Selection m_wdSel;  
    Range     m_wdRange; 
	Table m_table;
	Window m_activeWind;
	View m_view;

public:  
	BOOL isUsing(CString fileName);
    //操作  
    //**********************创建新文档*******************************************  
    BOOL CreateApp();                    //创建一个新的WORD应用程序  
    BOOL CreateDocuments();                //创建一个新的Word文档集合  
    BOOL CreateDocument();                //创建一个新的Word文档  
    BOOL Create();                        //创建新的WORD应用程序并创建一个新的文档  
    void ShowApp();                        //显示WORD文档  
    void HideApp();                        //隐藏word文档  
  
    //**********************打开文档*********************************************  
    BOOL OpenDocument(CString fileName);//打开已经存在的文档。  
    BOOL Open(CString fileName);        //创建新的WORD应用程序并打开一个已经存在的文档。  
    BOOL SetActiveDocument(short i);    //设置当前激活的文档。  
  
    //**********************保存文档*********************************************  
    BOOL SaveDocument();                //文档是以打开形式，保存。  
    BOOL SaveDocumentAs(CString fileName);//文档以创建形式，保存。  
    BOOL CloseDocument();  
    void CloseApp();   
  
    //**********************文本书写操作*****************************************  
    void WriteText(CString szText);        //当前光标处写文本 
	void SetFont(CString fonName,int fontSize);
    void WriteNewLineText(CString szText, int nLineCount = 1); //换N行写字  
    void WriteEndLine(CString szText);    //文档结尾处写文本  
    void WholeStory();                    //全选文档内容  
    void Copy();                        //复制文本内容到剪贴板  
    void InsertFile(CString fileName);    //将本地的文件全部内容写入到当前文档的光标处。  
    //----------------------add by zxx--------------------------------------  
    //***********************光标操作********************************************  
    //上下按行选择  
    void SelectMoveDown(short lineCount, short unit);//有选择操作的移动  
    void NoneSelectMoveDown(short lineCount, short unit);//仅仅移动光标，不选中  
    void SelectMoveUp(short lineCount, short unit);//有选择操作的移动  
    void NoneSelectMoveUp(short lineCount, short unit);//仅仅移动光标，不选中  
    //左右按列选择  
    void SelectMoveLeft(short charCount, short unit);//有选择操作的移动  
    void NoneSelectMoveLeft(short charCount, short unit);//  
    void SelectMoveRight(short charCount, short unit);//有选择操作的移动  
    void NoneSelectMoveRight(short charCount, short unit);//  
  
	//查找字符串 然后全部替换
	void FindWord(CString FindW,CString RelWord);
	void CopyLine();
	//定位至书签
	BOOL Goto(const CString& strBMark);

    void MoveToFirst();  
    void MoveToNextPage();  
    void TypeParagraph();  
    void PasteAndFormat();  
    void Paste();  
    void TypeBackspace(int count);
	//在书签位置更新域
	void UpdateField(CString strBMark);
	//插入书签
	void InsetBookMark(CString strBMark);
	//插入符号
	void InsertSymbol(const CString& strFont,long charNum);
	//nRow行数，nCell列数
	void CreateTable( int nRow, int nCell );
	//合并表格中的单元格
	void MergeTable(int mergeFromRow, int mergeFromCol, int mergeToRow, int mergeToCol);

	//nRow行，nCell列
	void SetTableText( int nRow, int nCell, const CString& text, int fontSize = 10,long align = 1 );

	//删除N行
	void Delete(int nCount);

	//是否显示书签
	void ShowBookmarks(BOOL show = TRUE);
	void SetSeekView(const CString& text,long align, long currentView,const CString& findW = NULL);
}; 
extern CmyWord* MyWord;
#endif //C_MY_WORD_H