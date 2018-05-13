#pragma once
#include "../stdafx.h"

namespace SimplyCpp
{
    namespace UI
    {
        typedef std::function<void()> Callback;
        typedef std::function<void(int)> ModalReturnCallback;
        
        const wxString FILE_SELECTOR_PROMPT_STRING =
#ifdef __WXMAC__
            wxEmptyString;
#else
            wxFileSelectorPromptStr;
#endif
        
        inline void ShowWindowModalThenDo(wxDialog *dlg, const ModalReturnCallback& onEndModal)
        {
#ifdef __WXMAC__
            dlg->ShowWindowModalThenDo(onEndModal);
#else
            onEndModal(dlg->ShowModal());
#endif
        }
    }
}
