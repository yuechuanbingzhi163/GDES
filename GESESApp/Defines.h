#define Edit_LimitTextEx(hwndCtl, cchMax)((void)::SendMessage((hwndCtl), EM_LIMITTEXT, (WPARAM)(cchMax), 0L))
