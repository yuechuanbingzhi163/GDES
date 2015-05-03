#pragma once

#include "aced.h"

/*
 * 用于处理LinkedGE图元的闭合问题
 * 例如，对巷道执行移动操作，移动后，
 * 处理巷道原始位置的连接点闭合
 * 以及处理巷道在新位置的连接点闭合
 */
class LinkedGE_EditorReactor : public AcEditorReactor {

protected:
	bool mbAutoInitAndRelease ;

public:
	LinkedGE_EditorReactor (const bool autoInitAndRelease =true) ;
	virtual ~LinkedGE_EditorReactor () ;

	virtual void Attach () ;
	virtual void Detach () ;
	virtual AcEditor *Subject () const ;
	virtual bool IsAttached () const ;

	// 在命令执行结束后处理连接点闭合
	virtual void commandEnded(const ACHAR * cmdStr);
} ;
