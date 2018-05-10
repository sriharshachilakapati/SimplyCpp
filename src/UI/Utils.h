#pragma once
#include "../stdafx.h"

namespace SimplyCpp
{
    namespace UI
    {
        template <typename Functor>
        inline void ShowWindowModalThenDo(wxDialog *dlg, const Functor& onEndModal)
        {
#ifdef __WXMAC__
            dlg->ShowWindowModalThenDo(onEndModal);
#else
            onEndModal(dlg->ShowModal());
#endif
        }
    }
}
