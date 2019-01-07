//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TOpenDialog *OpenDialog1;
        TImage *image;
        TButton *Button2;
        TStatusBar *StatusBar1;
        TLabel *Label1;
        TLabel *Label2;
        TMainMenu *MainMenu1;
        TMenuItem *Arquivo1;
        TMenuItem *Aes1;
        TMenuItem *Ajuda1;
        TMenuItem *AbrirImagem1;
        TMenuItem *SalvarImagem1;
        TMenuItem *N1;
        TMenuItem *Sair1;
        TMenuItem *Contar1;
        TMenuItem *Sobre1;
        TBevel *Bevel1;
        TImage *Image1;
        TSaveDialog *SaveDialog1;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall AbrirImagem1Click(TObject *Sender);
        void __fastcall Contar1Click(TObject *Sender);
        void __fastcall SalvarImagem1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
