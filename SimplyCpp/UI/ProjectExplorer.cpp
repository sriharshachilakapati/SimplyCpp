#include "../stdafx.h"
#include "ProjectExplorer.h"

using namespace SimplyCpp::UI;

ProjectExplorer::ProjectExplorer(wxWindow* parent) : wxTreeCtrl(parent)
{
    auto id = AppendItem(GetRootItem(), _("Untitled"), 1);
    AppendItem(id, _("Untitled"));
}
