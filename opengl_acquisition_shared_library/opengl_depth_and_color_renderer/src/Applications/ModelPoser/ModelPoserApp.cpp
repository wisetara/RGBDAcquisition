/***************************************************************
 * Name:      ModelPoserApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Ammar Qammaz ()
 * Created:   2018-10-20
 * Copyright: Ammar Qammaz (http://ammar.gr)
 * License:
 **************************************************************/

#include "ModelPoserApp.h"

//(*AppHeaders
#include "ModelPoserMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(ModelPoserApp);

bool ModelPoserApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	ModelPoserFrame* Frame = new ModelPoserFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
        