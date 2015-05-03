#ifndef UIMICROBLOG_HPP
#define UIMICROBLOG_HPP

class CMicroBlogUI : public CListUI
{
public:
	CMicroBlogUI(CPaintManagerUI& paint_manager);
	~CMicroBlogUI();

private:
	CPaintManagerUI& paint_manager_;
};

#endif // UIMICROBLOG_HPP