//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("celulas.res");
USEFORM("Unit1.cpp", Form1);
USEUNIT("morphology.cpp");
USEUNIT("neural_net.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "Sistema de contagem de celulas";
                 Application->CreateForm(__classid(TForm1), &Form1);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
