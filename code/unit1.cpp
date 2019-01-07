//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------

//#include "jacobi.h"
//#include "my_math.h"
#include "neural_net.h"
//#include "specific.h"
#include "morphology.h"
#include "stufs.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;


//------------------------ - - - - - - - - - - - - - - - - - --------------
//------------------------ - - - - - - - - - - - - - - - - - --------------
//------------------------ [[[[[[[[[[[[[ INICIO ]]]]]]]]]]] ---------------
//------------------------ - - - - - - - - - - - - - - - - - --------------
//------------------------ - - - - - - - - - - - - - - - - - --------------


int order_=0, B_=2, num_samples_=5000, num_text_=3, W, H;
int *IMGr, *IMGg, *IMGb, *IMGr_o, *IMGg_o, *IMGb_o;

void init()
{
    W = 0; H = 0;

    IMGr = NULL; IMGg = NULL; IMGb = NULL;
    IMGr_o = NULL; IMGg_o = NULL; IMGb_o = NULL;

}

void finish()
{
    if (IMGr) delete IMGr;
    if (IMGg) delete IMGg;
    if (IMGb) delete IMGb;
    if (IMGr_o) delete IMGr_o;
    if (IMGg_o) delete IMGg_o;
    if (IMGb_o) delete IMGb_o;
}



void Update_Image_to_new_RGB_array(int val)
{
    int x,y;
    RGBTRIPLE *ptr;

    for (int y = 0; y < Form1->image->Height; y++)
    {
      ptr = (RGBTRIPLE *)Form1->image->Picture->Bitmap->ScanLine[y];
      for (int x = 0; x < Form1->image->Width; x++)
      {
            ptr[x].rgbtRed = IMGr[W*y + x]*val;
            ptr[x].rgbtGreen = IMGg[W*y + x]*val;
            ptr[x].rgbtBlue = IMGb[W*y + x]*val;
      }
    }
    Form1->image->Refresh();

}

void OpenImage()
{
    AnsiString s;
    RGBTRIPLE *ptr; 

  Form1->OpenDialog1->DefaultExt = "BMP";
  Form1->OpenDialog1->FileName = "*.bmp";
  if (Form1->OpenDialog1->Execute())
  {
    s = Form1->OpenDialog1->FileName;
  }
  else
    return;

  Form1->image->Picture->LoadFromFile(s);
//  Form1->image->Width = Form1->image->Width+10;
//  Form1->image->Height = Form1->image->Height+40;

  W = Form1->image->Width; H = Form1->image->Height;

    IMGr = new int[W*H];
    IMGg = new int[W*H];
    IMGb = new int[W*H];

    IMGr_o = new int[W*H];
    IMGg_o = new int[W*H];
    IMGb_o = new int[W*H];

    for (int y = 0; y < Form1->image->Height; y++)
    {
      ptr = (RGBTRIPLE *)Form1->image->Picture->Bitmap->ScanLine[y];
      for (int x = 0; x < Form1->image->Width; x++)
      {
            IMGr[W*y + x] = ptr[x].rgbtRed;
            IMGg[W*y + x] = ptr[x].rgbtGreen;
            IMGb[W*y + x] = ptr[x].rgbtBlue;

            IMGr_o[W*y + x] = IMGr[W*y + x];
            IMGg_o[W*y + x] = IMGg[W*y + x];
            IMGb_o[W*y + x] = IMGb[W*y + x];
      }
    }

    Form1->Label2->Caption = "Células verdes";
    Form1->Label1->Caption = "Células vermelhas";

}


void KLT()
{
    float M[3];
    float C[3][3];
    float V[3][3];
    float D[3];
    int nrot;
    char s[200];


    correlation_matrix(IMGr,IMGg,IMGb,W,H,C,M);

    jacobi(C, 3, D, V, &nrot);

    change_base(IMGr, IMGg, IMGb, W, H, V, M);

    adjust_RGB(IMGr, IMGg, IMGb, W, H);

    Update_Image_to_new_RGB_array(1);
}

void auto_contrast()
{
    char s[200];

    auto_contrast(IMGr, IMGg, IMGb, W, H, 0.01);

    Update_Image_to_new_RGB_array(1);
}

void morphology()
{
    int *ero1, *ero2;
    int v1, v2, xx, yy;
    int quantos1, quantos2;
    char s[100];
    LISTA CENTERS1, CENTERS2;

    ero1 = new int[W*H];
    ero2 = new int[W*H];

    if (IMGr && ero1 && ero2)
    {

        for (int x=0; x<W; x++)
            for (int y=0; y<H; y++)
            {
                IMGr[W*y+x] = IMGr[W*y+x]>0;
                IMGg[W*y+x] = IMGg[W*y+x]>0;
            }

        v1 = GetTickCount();
        erosao(IMGr, W, H, circulo_R_3, 7, ero1);
        erosao(IMGg, W, H, circulo_R_3, 7, ero2);
        v2 = GetTickCount();
        sprintf(s,"Ok!  Erosão: %d ms",v2-v1);
        Form1->StatusBar1->Panels->Items[0]->Text = AnsiString(s);
        Update_Image_to_new_RGB_array(100);
        Form1->Refresh();

        v1 = GetTickCount();
        fechamento(ero1, W, H, circulo_R_3, 7, IMGr);
        fechamento(ero2, W, H, circulo_R_3, 7, IMGg);
        v2 = GetTickCount();
        sprintf(s,"Ok! Fechamento: %d ms",v2-v1);
        Form1->StatusBar1->Panels->Items[0]->Text = AnsiString(s);
        Update_Image_to_new_RGB_array(100);
        Form1->Refresh();

        v1 = GetTickCount();
        ultima_erosao(IMGr, W, H, full_3, 3, ero1);
        ultima_erosao(IMGg, W, H, full_3, 3, ero2);
        v2 = GetTickCount();
        sprintf(s,"Ok!  Ultima erosão: %d ms",v2-v1);
        Form1->StatusBar1->Panels->Items[0]->Text = AnsiString(s);
        Update_Image_to_new_RGB_array(100);
        Form1->Refresh();

        v1 = GetTickCount();
        dilatacao(ero1, W, H, circulo_R_2, 5, IMGr);
        dilatacao(ero2, W, H, circulo_R_2, 5, IMGg);
        v2 = GetTickCount();
        sprintf(s,"Ok!  Dilatação: %d ms",v2-v1);
        Form1->StatusBar1->Panels->Items[0]->Text = AnsiString(s);
        Update_Image_to_new_RGB_array(100);
        Form1->Refresh();

       memcpy(IMGb,IMGg,W*H*sizeof(int));
       conta_ai(IMGb,W,H,&CENTERS1,&quantos1,1);
       sprintf(s,"Células verdes: %d",quantos1);
       Form1->Label1->Caption = AnsiString(s);
        Form1->Refresh();

        for(int a=0; a<quantos1; a++)
        {
                put_x(IMGr_o,W,H,CENTERS1.getx(),CENTERS1.gety());
                CENTERS1.del();
        }

       memcpy(IMGb,IMGr,W*H*sizeof(int));
       conta_ai(IMGb,W,H,&CENTERS2,&quantos2,1);
       sprintf(s,"Células vermelhas: %d",quantos2);
       Form1->Label2->Caption = AnsiString(s);
       Form1->Refresh();

        for(int a=0; a<quantos2; a++)
        {
                put_x(IMGg_o,W,H,CENTERS2.getx(),CENTERS2.gety());
                CENTERS2.del();
        }

       memcpy(IMGr,IMGr_o,W*H*sizeof(int));
       memcpy(IMGg,IMGg_o,W*H*sizeof(int));
       memcpy(IMGb,IMGb_o,W*H*sizeof(int));


      Update_Image_to_new_RGB_array(1);


    }
    if (ero1) delete ero1;
    if (ero2) delete ero2;

}


void neural()
{
    int v1,v2, area;
    float *data;
    competitiva *rn;
    char s[100];
    int *result, *result2;

//---
    int _ORDER = 0;
    int _B = 3;
    int _DIM;
    int _NUM_EXE = 50000;
    int _NUM_TEXT;

   _DIM = (order_+1)*3;
   _ORDER = order_;
   _NUM_EXE = num_samples_;
   _B = B_;
   _NUM_TEXT = num_text_;

    data = new float[W*H*_DIM];

    result = new int[W*H];
    result2 = new int[W*H];

    rn = new competitiva(_DIM,_NUM_TEXT,data,W*H); //3 = num_centers

    if (result && data && rn && result2)
    {

        rn->convert_imageRGB_to_moment_data(IMGr,IMGg,IMGb,W,H,data,_ORDER,_NUM_EXE,_B);


        rn->set_centers(data,_NUM_EXE);


        v1 = GetTickCount();
        rn->train(data,_NUM_EXE,50,0.5,0.01);
        v2 = GetTickCount();
        sprintf(s,"Treinamento da rede Neural: %d ms",v2-v1);
        Form1->StatusBar1->Panels->Items[0]->Text = AnsiString(s);
        Form1->Refresh();

        classify(IMGr, IMGg, IMGb, W, H, rn->get_all_centers(), _NUM_TEXT, result, _ORDER, _B);
        memcpy(IMGr, result, W*H*sizeof(int));

        v1 = GetTickCount();
        sort_segmented_background(IMGr,W,H);
        v2 = GetTickCount();
        sprintf(s,"Organização do fundo: %d ms",v2-v1);
        Form1->StatusBar1->Panels->Items[0]->Text = AnsiString(s);
        Form1->Refresh();


        memcpy(IMGg,IMGr,W*H*sizeof(int));


        v1 = GetTickCount();
        extract_type(IMGr,W,H,0);
        v2 = GetTickCount();
        sprintf(s,"Extração de tipo 1: %d ms",v2-v1);
        Form1->StatusBar1->Panels->Items[0]->Text = AnsiString(s);
        Form1->Refresh();


        v1 = GetTickCount();
        extract_type(IMGg,W,H,1);
        v2 = GetTickCount();
        sprintf(s,"Extração de tipo 2: %d ms",v2-v1);
        Form1->StatusBar1->Panels->Items[0]->Text = AnsiString(s);
        Form1->Refresh();


        v1 = GetTickCount();
        area = extract_border2(IMGr,W,H);
        area = extract_border2(IMGg,W,H);
        v2 = GetTickCount();
        sprintf(s,"Extração de bordas: %d ms (area = %d)",v2-v1,area);
        Form1->StatusBar1->Panels->Items[0]->Text = AnsiString(s);
        Form1->Refresh();

        for (int x=0; x<W; x++)
            for (int y=0; y<H; y++)
                IMGb[W*y+x] = 0;

        Update_Image_to_new_RGB_array(100);
    }

    if (data)  delete data;
    if (rn)  delete rn;
    if (result) delete result;
    if (result2) delete result2;
}


/*
 *  Another functuons from "base"
 */



//------------------------ - - - - - - - - - - - - - - - - - --------------
//------------------------ - - - - - - - - - - - - - - - - - --------------
//------------------------ [[[[[[[[[[[[[[[ FIM ]]]]]]]]]]]] ---------------
//------------------------ - - - - - - - - - - - - - - - - - --------------
//------------------------ - - - - - - - - - - - - - - - - - --------------



void contar()
{
int v1, v2;
char s[100];

v1 = GetTickCount();
KLT();

auto_contrast();

neural();

morphology();

v2 = GetTickCount();
sprintf(s,"Tempo para contagem: %d s",(v2-v1)/1000);
Form1->StatusBar1->Panels->Items[0]->Text = AnsiString(s);

}






//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TForm1::FormCreate(TObject *Sender)
{
init();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
finish();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
contar();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AbrirImagem1Click(TObject *Sender)
{
OpenImage();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Contar1Click(TObject *Sender)
{
contar();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SalvarImagem1Click(TObject *Sender)
{
        AnsiString s;

  Form1->SaveDialog1->DefaultExt = "BMP";
  Form1->SaveDialog1->FileName = "*.bmp";
  if (Form1->SaveDialog1->Execute())
  {
    s = Form1->SaveDialog1->FileName;
  }
  else
    return;

  Form1->image->Picture->SaveToFile(s);
}
//---------------------------------------------------------------------------

