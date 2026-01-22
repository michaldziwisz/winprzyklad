#include <windows.h>
#include <commctrl.h>
#include <uxtheme.h>

#include <array>
#include <string>

#include "../resources/resource.h"

namespace {

void SetStatusText(HWND dialog, const std::wstring& text) {
  SetDlgItemTextW(dialog, IDC_STATUS, text.c_str());
}

int GetSliderPos(HWND dialog) {
  HWND slider = GetDlgItem(dialog, IDC_SLIDER);
  return static_cast<int>(SendMessageW(slider, TBM_GETPOS, 0, 0));
}

void UpdateSliderValueLabel(HWND dialog) {
  wchar_t buf[32] = {};
  wsprintfW(buf, L"%d", GetSliderPos(dialog));
  SetDlgItemTextW(dialog, IDC_SLIDER_VALUE, buf);
}

void InitCombo(HWND dialog) {
  HWND combo = GetDlgItem(dialog, IDC_COMBO);
  SendMessageW(combo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"Opcja 1"));
  SendMessageW(combo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"Opcja 2"));
  SendMessageW(combo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"Opcja 3"));
  SendMessageW(combo, CB_SETCURSEL, 0, 0);
}

void InitListBox(HWND dialog) {
  HWND list = GetDlgItem(dialog, IDC_LISTBOX);
  SendMessageW(list, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"Element A"));
  SendMessageW(list, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"Element B"));
  SendMessageW(list, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"Element C"));
  SendMessageW(list, LB_SETCURSEL, 0, 0);
}

void InitListView(HWND dialog) {
  HWND listView = GetDlgItem(dialog, IDC_LISTVIEW);

  ListView_SetExtendedListViewStyleEx(
      listView, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

  LVCOLUMNW col = {};
  col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

  col.pszText = const_cast<LPWSTR>(L"Nazwa");
  col.cx = 100;
  col.iSubItem = 0;
  ListView_InsertColumn(listView, 0, &col);

  col.pszText = const_cast<LPWSTR>(L"Opis");
  col.cx = 120;
  col.iSubItem = 1;
  ListView_InsertColumn(listView, 1, &col);

  LVITEMW item = {};
  item.mask = LVIF_TEXT;

  item.iItem = 0;
  item.iSubItem = 0;
  item.pszText = const_cast<LPWSTR>(L"Pozycja 1");
  ListView_InsertItem(listView, &item);
  ListView_SetItemText(listView, 0, 1, const_cast<LPWSTR>(L"Pierwszy wiersz"));

  item.iItem = 1;
  item.iSubItem = 0;
  item.pszText = const_cast<LPWSTR>(L"Pozycja 2");
  ListView_InsertItem(listView, &item);
  ListView_SetItemText(listView, 1, 1, const_cast<LPWSTR>(L"Drugi wiersz"));
}

HTREEITEM InsertTreeItem(HWND tree, const wchar_t* text, HTREEITEM parent) {
  TVINSERTSTRUCTW ins = {};
  ins.hParent = parent;
  ins.hInsertAfter = TVI_LAST;
  ins.item.mask = TVIF_TEXT;
  ins.item.pszText = const_cast<LPWSTR>(text);
  return TreeView_InsertItem(tree, &ins);
}

void InitTreeView(HWND dialog) {
  HWND tree = GetDlgItem(dialog, IDC_TREEVIEW);
  HTREEITEM root = InsertTreeItem(tree, L"Kategoria", TVI_ROOT);
  InsertTreeItem(tree, L"Element 1", root);
  InsertTreeItem(tree, L"Element 2", root);
  TreeView_Expand(tree, root, TVE_EXPAND);
}

void InitSlider(HWND dialog) {
  HWND slider = GetDlgItem(dialog, IDC_SLIDER);
  SendMessageW(slider, TBM_SETRANGE, TRUE, MAKELONG(0, 100));
  SendMessageW(slider, TBM_SETTICFREQ, 10, 0);
  SendMessageW(slider, TBM_SETPAGESIZE, 0, 10);
  SendMessageW(slider, TBM_SETPOS, TRUE, 50);
  UpdateSliderValueLabel(dialog);
}

void InitProgress(HWND dialog) {
  HWND progress = GetDlgItem(dialog, IDC_PROGRESS);
  SendMessageW(progress, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
  SendMessageW(progress, PBM_SETPOS, 30, 0);
}

void IncrementProgress(HWND dialog) {
  HWND progress = GetDlgItem(dialog, IDC_PROGRESS);
  int pos = static_cast<int>(SendMessageW(progress, PBM_GETPOS, 0, 0));
  pos += 10;
  if (pos > 100) {
    pos = 0;
  }
  SendMessageW(progress, PBM_SETPOS, pos, 0);
  SetStatusText(dialog, L"Status: ustawiono postep na " + std::to_wstring(pos) + L"%.");
}

void UpdateStatusFromCombo(HWND dialog) {
  HWND combo = GetDlgItem(dialog, IDC_COMBO);
  int index = static_cast<int>(SendMessageW(combo, CB_GETCURSEL, 0, 0));
  wchar_t buf[128] = {};
  SendMessageW(combo, CB_GETLBTEXT, index, reinterpret_cast<LPARAM>(buf));
  SetStatusText(dialog, std::wstring(L"Status: wybrano w Combo: ") + buf);
}

void UpdateStatusFromListBox(HWND dialog) {
  HWND list = GetDlgItem(dialog, IDC_LISTBOX);
  int index = static_cast<int>(SendMessageW(list, LB_GETCURSEL, 0, 0));
  wchar_t buf[128] = {};
  SendMessageW(list, LB_GETTEXT, index, reinterpret_cast<LPARAM>(buf));
  SetStatusText(dialog, std::wstring(L"Status: wybrano w ListBox: ") + buf);
}

void UpdateStatusFromListView(HWND dialog) {
  HWND listView = GetDlgItem(dialog, IDC_LISTVIEW);
  int sel = ListView_GetNextItem(listView, -1, LVNI_SELECTED);
  if (sel < 0) {
    return;
  }
  wchar_t buf[128] = {};
  ListView_GetItemText(listView, sel, 0, buf, static_cast<int>(std::size(buf)));
  SetStatusText(dialog, std::wstring(L"Status: zaznaczono w ListView: ") + buf);
}

void UpdateStatusFromTreeView(HWND dialog) {
  HWND tree = GetDlgItem(dialog, IDC_TREEVIEW);
  HTREEITEM sel = TreeView_GetSelection(tree);
  if (!sel) {
    return;
  }
  TVITEMW item = {};
  wchar_t buf[128] = {};
  item.mask = TVIF_TEXT;
  item.hItem = sel;
  item.pszText = buf;
  item.cchTextMax = static_cast<int>(std::size(buf));
  if (TreeView_GetItem(tree, &item) == FALSE) {
    return;
  }
  SetStatusText(dialog, std::wstring(L"Status: zaznaczono w TreeView: ") + buf);
}

INT_PTR CALLBACK MainDialogProc(HWND dialog, UINT msg, WPARAM wParam, LPARAM lParam) {
  switch (msg) {
    case WM_INITDIALOG: {
      CheckDlgButton(dialog, IDC_CHECK1, BST_CHECKED);
      CheckDlgButton(dialog, IDC_CHECK3, BST_INDETERMINATE);
      CheckRadioButton(dialog, IDC_RADIO1, IDC_RADIO3, IDC_RADIO2);

      InitCombo(dialog);
      InitListBox(dialog);
      InitListView(dialog);
      InitTreeView(dialog);
      InitSlider(dialog);
      InitProgress(dialog);

      SetStatusText(dialog, L"Status: gotowe.");
      return TRUE;
    }
    case WM_COMMAND: {
      const int controlId = LOWORD(wParam);
      const int notification = HIWORD(wParam);

      if (controlId == IDOK || controlId == IDCANCEL) {
        EndDialog(dialog, 0);
        return TRUE;
      }

      if (controlId == IDC_PROGRESS_BTN && notification == BN_CLICKED) {
        IncrementProgress(dialog);
        return TRUE;
      }

      if (controlId == IDC_COMBO && notification == CBN_SELCHANGE) {
        UpdateStatusFromCombo(dialog);
        return TRUE;
      }

      if (controlId == IDC_LISTBOX && notification == LBN_SELCHANGE) {
        UpdateStatusFromListBox(dialog);
        return TRUE;
      }

      return FALSE;
    }
    case WM_HSCROLL: {
      if (reinterpret_cast<HWND>(lParam) == GetDlgItem(dialog, IDC_SLIDER)) {
        UpdateSliderValueLabel(dialog);
        SetStatusText(dialog, L"Status: zmieniono suwak na " + std::to_wstring(GetSliderPos(dialog)) +
                                 L".");
        return TRUE;
      }
      return FALSE;
    }
    case WM_NOTIFY: {
      const auto* hdr = reinterpret_cast<const NMHDR*>(lParam);
      if (hdr->idFrom == IDC_LISTVIEW && hdr->code == LVN_ITEMCHANGED) {
        UpdateStatusFromListView(dialog);
        return TRUE;
      }
      if (hdr->idFrom == IDC_TREEVIEW && hdr->code == TVN_SELCHANGEDW) {
        UpdateStatusFromTreeView(dialog);
        return TRUE;
      }
      return FALSE;
    }
    default:
      return FALSE;
  }
}

void EnableModernCommonControls(HINSTANCE instance) {
  INITCOMMONCONTROLSEX icc = {};
  icc.dwSize = sizeof(icc);
  icc.dwICC = ICC_WIN95_CLASSES | ICC_DATE_CLASSES | ICC_BAR_CLASSES | ICC_LINK_CLASS;
  InitCommonControlsEx(&icc);

  SetThemeAppProperties(STAP_ALLOW_NONCLIENT | STAP_ALLOW_WEBCONTENT);
  (void)instance;
}

}  // namespace

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE /*prev*/, PWSTR /*cmdLine*/, int /*show*/) {
  EnableModernCommonControls(instance);
  DialogBoxParamW(instance, MAKEINTRESOURCEW(IDD_MAIN), nullptr, MainDialogProc, 0);
  return 0;
}
