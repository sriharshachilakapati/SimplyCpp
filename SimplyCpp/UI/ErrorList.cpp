#include "../stdafx.h"
#include "ErrorList.h"

using namespace SimplyCpp::UI;

ErrorList::ErrorList(wxWindow* parent) : wxListCtrl(parent, wxID_ANY, wxDefaultPosition,
    wxSize(640, 400), wxLC_REPORT)
{
    Clear();
}

void ErrorList::Clear()
{
    ClearAll();

    wxListItem col0;
    col0.SetId(ListID::FILE);
    col0.SetText("File");
    col0.SetWidth(125);

    wxListItem col1;
    col1.SetId(ListID::LINE);
    col1.SetText("Line");
    col1.SetWidth(50);

    wxListItem col2;
    col2.SetId(ListID::TYPE);
    col2.SetText("Type");
    col2.SetWidth(100);

    wxListItem col3;
    col3.SetId(ListID::DESCRIPTION);
    col3.SetText("Description");
    col3.SetWidth(500);

    InsertColumn(0, col0);
    InsertColumn(1, col1);
    InsertColumn(2, col2);
    InsertColumn(3, col3);
}

void ErrorList::Insert(const wxString& file, int line, ErrorType type, const wxString& message)
{
    long index = InsertItem(GetItemCount(), file);
    SetItem(index, 1, wxString::Format("%d", line));
    SetItem(index, 2, type == ErrorType::TYPE_ERROR ? "Error" : "Warning");
    SetItem(index, 3, message);

    type == ErrorType::TYPE_ERROR ? SetItemTextColour(index, wxColor(150, 0, 0))
                                  : SetItemTextColour(index, wxColor(100, 80, 0));
}