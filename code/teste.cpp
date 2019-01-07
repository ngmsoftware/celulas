/*
 *  Test program to use stufs.h ... 
 *
 */

#include "teste.h"

#include "stufs.h"
#include "memory.h"
#include "morphology.h"
#include "neural_net.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <qpainter.h>
#include <qfiledialog.h>
#include <qframe.h>
#include <qimage.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qvariant.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

int order_=0, B_=2, num_samples_=50000, num_text_=5;

//---------------------------
// MyDialog methods...
//

MyDialog::MyDialog( QWidget *parent, const char *name)
  : QDialog( parent, name , TRUE)
{  
   
      bt_ok = new QPushButton("Ok",this,"bt_ok");
      bt_ok->setGeometry(10,10,60,30);   
   
      bt_cancel = new QPushButton("Cancel",this,"bt_cancel");
      bt_cancel->setGeometry(10,40,60,30);   
   
      lb_order = new QLabel("Ordem",this,"lb_order");
      lb_order->setGeometry(80,10,100,20);   
   
      lb_B = new QLabel("Num. de Blocos",this,"lb_B");
      lb_B->setGeometry(80,40,100,20);   

      lb_num_samples = new QLabel("Num. de amostras",this,"lb_num_samples");
      lb_num_samples->setGeometry(80,70,100,20);   

      ed_order = new QLineEdit(this,"ed_order");
      ed_order->setGeometry(200,10,40,20);   
   
      ed_B = new QLineEdit(this,"ed_B");
      ed_B->setGeometry(200,40,40,20);   

      ed_num_samples = new QLineEdit(this,"ed_num_samples");
      ed_num_samples->setGeometry(200,70,40,20);   

      ed_num_text = new QLineEdit(this,"ed_num_text");
      ed_num_text->setGeometry(200,100,40,20);   

      _B = NULL;
      _order = NULL;
      _num_samples = NULL;
      _num_text = NULL;
   
      connect(bt_ok, SIGNAL(clicked()), this, SLOT(myaccept()) );
      connect(bt_cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
   
      this->setGeometry(100,100,300,150);
}

void MyDialog::vars(int *order, int *B, int *num_samples, int *num_text)
{
   
     _B = B;
     _order = order;
     _num_samples = num_samples;
     _num_text = num_text;
}

void MyDialog::myaccept()
{
   
     QString s;
   
     if ( (_order!=NULL) && (_B!=NULL) && (_num_samples!=NULL) && (_num_text!=NULL))
     {
	       s = ed_order->text();
	       *_order = s.toInt();
	
	       s = ed_B->text();
	       *_B = s.toInt();
	
	       s = ed_num_samples->text();
	       *_num_samples = s.toInt();

	       s = ed_num_text->text();
	       *_num_text = s.toInt();

	
     }
     accept();
}


/* 
 *  Constructs a base which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
base::base( QWidget* parent,  const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "base" );
    resize( 320, 200 ); 
    setCaption( tr( "%-)"  ) );

    open_image = new QPushButton( this, "open_image" );
    open_image->setGeometry( QRect( 0, 0, 95, 32 ) ); 
    open_image->setText( tr( "Abrir imagem..."  ) );

    do_it21 = new QPushButton( this );
    do_it21->setGeometry( QRect( 100, 0, 95, 32 ) ); 
    do_it21->setText( tr( "KLT"  ) );

    do_it22 = new QPushButton( this );
    do_it22->setGeometry( QRect( 200, 0, 95, 32 ) ); 
    do_it22->setText( tr( "auto contraste"  ) );

    do_it = new QPushButton( this );
    do_it->setGeometry( QRect( 300, 0, 95, 32 ) ); 
    do_it->setText( tr( "Rede neural"  ) );

    do_it3 = new QPushButton( this );
    do_it3->setGeometry( QRect( 400, 0, 95, 32 ) ); 
    do_it3->setText( tr( "resultado!"  ) );


    do_it4 = new QPushButton( this );
    do_it4->setGeometry( QRect( 500, 0, 95, 32 ) ); 
    do_it4->setText( tr( "parametros"  ) );

    do_it5 = new QPushButton( this );
    do_it5->setGeometry( QRect( 600, 0, 95, 32 ) ); 
    do_it5->setText( tr( "Salvar image"  ) );

    image = new QImage(310,190,32);

    connect(open_image, SIGNAL(clicked()), this, SLOT(OpenImage()));
    connect(do_it, SIGNAL(clicked()), this, SLOT(do_it_slot()));
    connect(do_it21, SIGNAL(clicked()), this, SLOT(do_it21_slot()));
    connect(do_it22, SIGNAL(clicked()), this, SLOT(do_it22_slot()));
    connect(do_it3, SIGNAL(clicked()), this, SLOT(do_it3_slot()));
    connect(do_it4, SIGNAL(clicked()), this, SLOT(do_it4_slot()));
    connect(do_it5, SIGNAL(clicked()), this, SLOT(SaveImage()));

    W = 0; H = 0;

    IMGr = NULL; IMGg = NULL; IMGb = NULL;
    IMGr_o = NULL; IMGg_o = NULL; IMGb_o = NULL;

}

/*  
 *  Destroys the object and frees any allocated resources
 */
base::~base()
{
    // no need to delete child widgets, Qt does it all for us
    if (IMGr) delete IMGr;
    if (IMGg) delete IMGg;
    if (IMGb) delete IMGb;
    if (IMGr_o) delete IMGr_o;
    if (IMGg_o) delete IMGg_o;
    if (IMGb_o) delete IMGb_o;
}

/*
 *  paintEvent... Where the images are drawned %)... %-)
 */
void base::paintEvent(QPaintEvent *e)
{
    QPainter p;

    p.begin(this);
    p.drawImage(5,35,*image);
    p.end();
}


/*
 *  Slots "base" ...
 */
void base::OpenImage()
{    
    QString s;

    s = QFileDialog::getOpenFileName("./","*.*");

    if (!s.isNull())
        if (image->load(s))
            resize(image->width()+10,image->height()+40);

    W = image->width(); H = image->height();

    IMGr = new int[W*H];
    IMGg = new int[W*H];
    IMGb = new int[W*H];

    IMGr_o = new int[W*H];
    IMGg_o = new int[W*H];
    IMGb_o = new int[W*H];

    for (int x=0; x<W; x++)
        for (int y=0; y<H; y++)
        {
            IMGr[W*y + x] = qRed(image->pixel(x,y));
            IMGg[W*y + x] = qGreen(image->pixel(x,y));
            IMGb[W*y + x] = qBlue(image->pixel(x,y));

            IMGr_o[W*y + x] = qRed(image->pixel(x,y));
            IMGg_o[W*y + x] = qGreen(image->pixel(x,y));
            IMGb_o[W*y + x] = qBlue(image->pixel(x,y));
        }
   
    repaint(TRUE);

}


void base::SaveImage()
{    
    QString s;

    s = QFileDialog::getSaveFileName("./","*.*");

   if (!s.isNull())  image->save(s,"PNG");

  repaint(TRUE);

}


/*
void base::do_it2_slot_01()
{
    int result;
    int v1,v2;
    char s[100];
*/
/*
    qWarning("Starting 1000 copies in gcc mode...");
    v1 = time(NULL);    
    for (int a=0; a<1000; a++)
    {
        memcpy(IMGg,IMGr,W*H*sizeof(int));
        memcpy(IMGb,IMGr,W*H*sizeof(int));
    }
    v2 = time(NULL);

    sprintf(s,"Finished, gcc mode = %d",v2-v1);
    qWarning(s);


    qWarning("Starting 1000 copies in asm mode...");
    v1 = time(NULL);    
    for (int a=0; a<1000; a++)
    {
        memcpy__(IMGr,IMGg,W*H*2);
        memcpy__(IMGr,IMGb,W*H*2);
    }
    qWarning("Finished...");
    v2 = time(NULL);

    sprintf(s,"Finished, asm mode = %d",v2-v1);
    qWarning(s);
*/

/*
    memcpy__(IMGr,IMGg,W*H*2);

    qWarning("Starting 1000 comparations in gcc mode...");
    v1 = time(NULL);    
    for (int a=0; a<1000; a++)
    {
        result = memcmp(IMGg,IMGr,W*H*sizeof(int));
    }
    v2 = time(NULL);

    sprintf(s,"Finished, gcc mode = %d  (result = %d)",v2-v1,result);
    qWarning(s);


    qWarning("Starting 1000 copies in asm mode...");
    v1 = time(NULL);    
    for (int a=0; a<1000; a++)
    {
        memcmp__(IMGr,IMGg,W*H*2,&result);
    }
    qWarning("Finished...");
    v2 = time(NULL);

    sprintf(s,"Finished, asm mode = %d (result = %d)",v2-v1,result);
    qWarning(s);
*/
/*
    qWarning("Starting 1000 fills in gcc mode...");
    v1 = time(NULL);    
    for (int a=0; a<1000; a++)
    {
        memset(IMGr,128,W*H*sizeof(int));
        memset(IMGg,128,W*H*sizeof(int));
    }
    v2 = time(NULL);

    sprintf(s,"Finished, gcc mode = %d",v2-v1);
    qWarning(s);

    qWarning("Starting 1000 fills in asm mode...");
    v1 = time(NULL);    
    for (int a=0; a<1000; a++)
    {
        memset__(IMGr,W*H*2,128);
        memset__(IMGg,W*H*2,128);
    }
    qWarning("Finished...");
    v2 = time(NULL);

    sprintf(s,"Finished, asm mode = %d",v2-v1);
    qWarning(s);



    Update_image_to_new_RGB_array();

}
*/

void base::do_it4_slot()
{
   MyDialog m;
   int hehe1, hehe2, hehe3, hehe4;
   
   m.vars(&hehe1, &hehe2, &hehe3, &hehe4); 
   
   if (m.exec())
   {
      order_ = hehe1;
      B_ = hehe2; 
      num_samples_ = hehe3;
      num_text_ = hehe4;
   }
}

void base::do_it21_slot() // KLT
{
    float M[3];
    float C[3][3];
    float V[3][3];
    float D[3];
    int nrot;
    char s[200];


    correlation_matrix(IMGr,IMGg,IMGb,W,H,C,M);

    sprintf(s,"Correlation Matrix: \n %.4f %4f %4f \n %.4f %4f %4f \n %.4f %4f %4f", C[0][0], C[0][1], C[0][2], 
                                                             C[1][0], C[1][1], C[1][2],
                                                             C[2][0], C[2][1], C[2][2] );
    qWarning(s);
 
    sprintf(s,"median vector:\n %.4f %4f %4f", M[0], M[1], M[2]);
    qWarning(s);

    jacobi(C, 3, D, V, &nrot);

    sprintf(s," %.4f %4f %4f \n %.4f %4f %4f \n %.4f %4f %4f", V[0][0], V[0][1], V[0][2], 
                                                             V[1][0], V[1][1], V[1][2],
                                                             V[2][0], V[2][1], V[2][2] );
    qWarning(s);

    change_base(IMGr, IMGg, IMGb, W, H, V, M);

    adjust_RGB(IMGr, IMGg, IMGb, W, H);

    Update_image_to_new_RGB_array();
}

void base::do_it22_slot() // Auto-contraste
{
    char s[200];

    sprintf(s,"Applying auto-contrast");
   
    qWarning(s);

//    histogram_equalization(IMGr, IMGg, IMGb, W, H);
    auto_contrast(IMGr, IMGg, IMGb, W, H, 0.01);

    Update_image_to_new_RGB_array();
}

void base::do_it3_slot()
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
 
        qWarning("Eroding...");
        v1 = time(NULL);
        erosao(IMGr, W, H, circulo_R_3, 7, ero1);
        erosao(IMGg, W, H, circulo_R_3, 7, ero2);
        

       
       
       
        v2 = time(NULL);
        sprintf(s,"Ok!  Time to erode = %d",v2-v1);
        qWarning(s);

        qWarning("Closing...");
        v1 = time(NULL);
//        abertura(ero1, W, H, circulo_R_3, 7, IMGr);
//        abertura(ero2, W, H, circulo_R_3, 7, IMGg);
        fechamento(ero1, W, H, circulo_R_3, 7, IMGr);
        fechamento(ero2, W, H, circulo_R_3, 7, IMGg);
             
       
        v2 = time(NULL);
        sprintf(s,"Ok!  Time to close = %d",v2-v1);
        qWarning(s);

        qWarning("Last eroding...");
        v1 = time(NULL);
        ultima_erosao(IMGr, W, H, full_3, 3, ero1);
        ultima_erosao(IMGg, W, H, full_3, 3, ero2);
              
       
        v2 = time(NULL);
        sprintf(s,"Ok!  Time to last erode = %d",v2-v1);
        qWarning(s);

        qWarning("Dilating...");
        v1 = time(NULL);
        dilatacao(ero1, W, H, circulo_R_2, 5, IMGr);
        dilatacao(ero2, W, H, circulo_R_2, 5, IMGg);
        v2 = time(NULL);
        sprintf(s,"Ok!  Time to dilate = %d",v2-v1);
        qWarning(s);

        for (int a=0; a<H; a++)
	 for (int b=0; b<W; b++)
	   {
	      IMGr[a*W+b] = 100*IMGr[a*W+b];
	      IMGg[a*W+b] = 100*IMGg[a*W+b];
	      IMGb[a*W+b] = 0;
	   }


/*       memcpy(IMGb,IMGg,W*H*sizeof(int));*/
       conta_ai(IMGb,W,H,&CENTERS1,&quantos1,1);
       
       sprintf(s,"------KIND 1 CELL = %d",quantos1);
       qWarning(s);
       
/*       memcpy(IMGb,IMGr,W*H*sizeof(int));*/
       conta_ai(IMGb,W,H,&CENTERS2,&quantos2,1);
       
       sprintf(s,"------KIND 2 CELL = %d",quantos2);
       qWarning(s);

       
/*       for (int x=0; x<W; x++)
            for (int y=0; y<H; y++)
            {
	       IMGg[W*y+x] = IMGg_o[W*y+x];
	       IMGb[W*y+x] = IMGb_o[W*y+x];
	       IMGr[W*y+x] = IMGr_o[W*y+x];
            }

       
       for (int a=0; a<quantos1; a++)
       {
	  xx = CENTERS1.getx();
	  yy = CENTERS1.gety();
	  CENTERS1.del();
	  put_x(IMGg,W,H,xx,yy);
       }

       for (int a=0; a<quantos2; a++)
       {
	  xx = CENTERS2.getx();
	  yy = CENTERS2.gety();
	  CENTERS2.del();
	  put_x(IMGr,W,H,xx,yy);
       }*/



        Update_image_to_new_RGB_array();

    }
    if (ero1) delete ero1;
    if (ero2) delete ero2;
    
}


void base::do_it_slot()
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

    rn = new competitiva(_DIM,_NUM_TEXT,data,W*H); // 3 = num_centers

    if (result && data && rn && result2)
    {

        qWarning("converting RGB -> data");

        rn->convert_imageRGB_to_moment_data(IMGr,IMGg,IMGb,W,H,data,_ORDER,_NUM_EXE,_B);
        qWarning("Ok!");


        qWarning("Setting centers from converted data");
        rn->set_centers(data,_NUM_EXE);
        qWarning("Ok!");


        qWarning("Centers before training");
        for (int a=0; a<_NUM_TEXT; a++)
        {
//            sprintf(s,"%.3f %.3f %.3f %.3f %.3f %.3f",
//            (rn->get_center(a))[0],(rn->get_center(a))[1],(rn->get_center(a))[2],
//            (rn->get_center(a))[3],(rn->get_center(a))[4],(rn->get_center(a))[5]);
//            qWarning(s);
        }

        qWarning("Training...");
        v1 = time(NULL);
        rn->train(data,_NUM_EXE,1000,0.5,0.01);
        v2 = time(NULL);
        sprintf(s,"Time to train the Neural Net = %d",v2-v1);
        qWarning(s);
        qWarning("Ok...");


        qWarning("Centers after training");
        for (int a=0; a<3; a++)
        {
//            sprintf(s,"%.3f %.3f %.3f %.3f %.3f %.3f",
//            (rn->get_center(a))[0],(rn->get_center(a))[1],(rn->get_center(a))[2],
//            (rn->get_center(a))[3],(rn->get_center(a))[4],(rn->get_center(a))[5]);
//            qWarning(s);
        }

        qWarning("Classifying...");
        classify(IMGr, IMGg, IMGb, W, H, rn->get_all_centers(), _NUM_TEXT, result, _ORDER, _B);
        qWarning("Ok...");
        memcpy(IMGr, result, W*H*sizeof(int));

//        qWarning("Sorting backgroundr...");
//        v1 = time(NULL);
//        sort_segmented_background(IMGr,W,H);
//        v2 = time(NULL);
//        sprintf(s,"Time ellapsed = %d",v2-v1);
//        qWarning(s);
//        qWarning("Ok...");


//        memcpy(IMGg,IMGr,W*H*sizeof(int));


//        qWarning("Extracting type 1...");
//        v1 = time(NULL);
//        extract_type(IMGr,W,H,0);
//        v2 = time(NULL);
//        sprintf(s,"Time ellapsed = %d",v2-v1);
//        qWarning(s);
//        qWarning("Ok...");

//        qWarning("Extracting type 2...");
//        v1 = time(NULL);
//        extract_type(IMGg,W,H,1);
//        v2 = time(NULL);
//        sprintf(s,"Time ellapsed = %d",v2-v1);
//        qWarning(s);
//        qWarning("Ok...");


//        qWarning("Extracting border...");
//        v1 = time(NULL);
//        area = extract_border2(IMGr,W,H);
//        area = extract_border2(IMGg,W,H);
//        v2 = time(NULL);
//        sprintf(s,"Time extracting the border = %d (area = %d)",v2-v1,area);
//        qWarning(s);
//        qWarning("Ok...");

        for (int x=0; x<W; x++)
            for (int y=0; y<H; y++)
            {
	        IMGg[W*y+x] = result[W*y+x]*50;
                IMGb[W*y+x] = result[W*y+x]*50;
	        IMGr[W*y+x] = result[W*y+x]*50;
            }

        Update_image_to_new_RGB_array();
    }
 
    if (data)  delete data;
    if (rn)  delete rn;
    if (result) delete result;
    if (result2) delete result2;
}


/*
 *  Another functuons from "base"
 */

void base::Update_image_to_new_RGB_array()
{
    for (int x=0; x<W; x++)
        for (int y=0; y<H; y++)
        {
            image->setPixel(x,y,qRgb(IMGr[W*y+x],IMGg[W*y+x],IMGb[W*y+x]));
        }

    repaint(true);    
}


//---------------------------------------------
//---------------------------------------------
int main( int argc, char **argv )
{ 
    QApplication a( argc, argv );

    base w;
    a.setMainWidget( &w );
    w.show();
    return a.exec();
}   

