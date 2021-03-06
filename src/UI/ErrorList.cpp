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
    wxString errorType;
    wxColor itemColor;

    switch (type)
    {
    case ErrorType::TYPE_ERROR:
        itemColor = wxColor(150, 0, 0);
        errorType = "Error";
        break;

    case ErrorType::TYPE_FATAL:
        itemColor = wxColor(200, 0, 0);
        errorType = "Fatal";
        break;

    case ErrorType::TYPE_WARNING:
        itemColor = wxColor(100, 80, 0);
        errorType = "Warning";
        break;
    }

    long index = InsertItem(GetItemCount(), file);
    SetItem(index, 1, wxString::Format("%d", line));
    SetItem(index, 2, errorType);
    SetItem(index, 3, message);

    SetItemTextColour(index, itemColor);
}