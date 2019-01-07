#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
 *  Classes, data types, variables, etc...
 *
 */


/*
 *  Linked list begin
 */

struct data
{
    int x,y, area;
    data *next;
};

class LISTA
{
public:
    data *D;

    LISTA();
    ~LISTA();

    int getx() { return D->x; };
    int gety() { return D->y; };
    void add(int xi, int yi, int areai);
    void del();
};

LISTA::LISTA()
{
    D = NULL;
}

void LISTA::add(int xi, int yi, int areai)
{
    data *temp;
    temp = new data;

    temp->x = xi;
    temp->y = yi;
    temp->area = areai;
    temp->next = D;
    D = temp;
}

void LISTA::del()
{
    data *temp;

    temp = D;
    D = D->next;
    delete temp;
}

LISTA::~LISTA()
{
    while(D)
        del();
}

/*
 *  Linked list end
 */

void sort(int *V, int size)
{
    int temp;

    for(int a=0; a<size; a++)
        for(int b=a+1; b<size; b++)
        {
            if (V[a]<V[b])
            {
                temp = V[a];
                V[a] = V[b];
                V[b] = temp;
            }
        }
}

float min(float *H, int sz, int *index)
{
    float esse;

    if (H)
    {
        *index = 0;
        esse = H[0];

        for(int a=0;a<sz;a++)
        {
            if (H[a]<esse)
            {
                esse = H[a];
                *index = a;
            }
        }

        return(esse);
    }
    return(0);
}



int min(int *H,int sz)
{
    int esse;

    esse = H[0];

    for(int a=0;a<sz;a++)
    {
        if (H[a]<esse)
            esse = H[a];
    }

    return(esse);
}


int max(int *H,int sz)
{
    int esse;

    esse = H[0];

    for(int a=0;a<sz;a++)
    {
        if (H[a]>esse)
            esse = H[a];
    }

    return(esse);
}

void _fill(int *IMG, int W, int H, int px, int py, int cor, int cori, int *centro_x, int *centro_y, int *area)
{
    if ((px>0) && (py>0) && (px<W) && (py<H))
    {
        if (IMG[px+W*py]==cori)
        {
            IMG[px+W*py] = cor;
            *area = *area+1;
            *centro_x = (*centro_x + px);
            *centro_y = (*centro_y + py);
            _fill(IMG,W,H,px+1,py,cor,cori,centro_x,centro_y,area);
            _fill(IMG,W,H,px-1,py,cor,cori,centro_x,centro_y,area);
            _fill(IMG,W,H,px,py+1,cor,cori,centro_x,centro_y,area);
            _fill(IMG,W,H,px,py-1,cor,cori,centro_x,centro_y,area);
            _fill(IMG,W,H,px+1,py+1,cor,cori,centro_x,centro_y,area);
            _fill(IMG,W,H,px-1,py-1,cor,cori,centro_x,centro_y,area);
            _fill(IMG,W,H,px-1,py+1,cor,cori,centro_x,centro_y,area);
            _fill(IMG,W,H,px+1,py-1,cor,cori,centro_x,centro_y,area);
        }
        else
            return;
    }
    else
        return;
}

void conta_ai(int *IMG, int W, int H, LISTA *centros, int *quantos, int cor_que_esta)
{
    int x,y, cen_x, cen_y, area;

    *quantos = 0;
    for (x=0; x<W; x++)
        for (y=0; y<H; y++)
        {
            if (IMG[x+W*y]==cor_que_esta)
            {
                area = 1;
                cen_x = x;
                cen_y = y;
                _fill(IMG,W,H,x,y,cor_que_esta-1,cor_que_esta,&cen_x,&cen_y,&area);
                if (centros) centros->add(cen_x / area, cen_y / area, area);
                (*quantos)++;
            }
        }
}

void histogram(int *IMGr, int *IMGg, int *IMGb, int W, int H, int *Hist_r,  int *Hist_g,  int *Hist_b)
{
    if (Hist_r) memset(Hist_r,0,256*sizeof(int));
    if (Hist_g) memset(Hist_g,0,256*sizeof(int));
    if (Hist_b) memset(Hist_b,0,256*sizeof(int));
/*        for (int a=0; a<256; a++)
        {
            Hist_r[a] = 0;
            Hist_g[a] = 0;
            Hist_b[a] = 0;
        }*/


    for (int x=0;x<W; x++)
        for (int y=0;y<H; y++)
        {
            if (Hist_r) Hist_r[IMGr[W*y + x]]++;
            if (Hist_g) Hist_g[IMGg[W*y + x]]++;
            if (Hist_b) Hist_b[IMGb[W*y + x]]++;
        }
}

void histogram_equalization(int *IMGr, int *IMGg, int *IMGb, int W, int H)
{
    int Hr[256], Hg[256], Hb[256], somar=0, somag=0, somab=0;

    if (IMGr && IMGg && IMGb)
    {

        histogram(IMGr, IMGg, IMGb, W, H, Hr, Hg, Hb);

        for (int a=0; a<256; a++)
        {
            somar += Hr[a];
            Hr[a] = somar;

            somag += Hg[a];
            Hg[a] = somag;

            somab += Hb[a];
            Hb[a] = somab;
        }    

        for (int a=0; a<256; a++)
        {
            Hr[a] = (int)( ((float)(Hr[a])/(float)(somar))*255.0 );
            Hg[a] = (int)( ((float)(Hg[a])/(float)(somag))*255.0 );
            Hb[a] = (int)( ((float)(Hb[a])/(float)(somab))*255.0 );
        }

        for (int x=0; x<W; x++)
            for (int y=0; y<H; y++)
            {
                IMGr[W*y+x] = Hr[IMGr[W*y+x]];
                IMGg[W*y+x] = Hg[IMGg[W*y+x]];
                IMGb[W*y+x] = Hb[IMGb[W*y+x]];
            }
    }
}

void median(int *IMG, int W, int H, int radius, int *IMGr)
{
    int ximg, yimg;
    int V[529], conta;

    for(int a=0;a<529; a++) V[a] = 0;

    if (radius<20)
    {
        for(int x=0; x<W; x++)
            for(int y=0; y<H; y++)
            {
                conta = 0;
                for(int a=0; a<(2*radius+1); a++)
                    for(int b=0; b<(2*radius+1); b++)
                    {
                        ximg = (x+a-radius+W)%W;
                        yimg = (y+b-radius+H)%H;
                        V[conta] = IMG[yimg*W+ximg];
                        conta++;
                    }
                sort(V,(2*radius+1)*(2*radius+1));

                IMGr[y*W+x] = V[((2*radius+1)*(2*radius+1))>>1];
            }
    }
}


void normalize_histogram(int *H, float *Hf)
{
    int soma=0;
    
    if (H && Hf)
    {
        for (int a=0; a<256; a++)
            soma += H[a];

        for (int a=0; a<256; a++)
            Hf[a] = (float)(H[a]) / (float)(soma);        
    }
}


int u(int x)
{
    return(x>0?(x<255?x:255):0);
}

void auto_contrast(int *IMGr, int *IMGg, int *IMGb, int W, int H, float per)
{
    int *Hr, *Hg, *Hb;
    float *Hrf, *Hgf, *Hbf;
    float somar=0.0, somag=0.0, somab=0.0;
    int lim_R_i=0, lim_G_i=0, lim_B_i=0, lim_R_s=255, lim_G_s=255, lim_B_s=255;
    
    Hr = new int[256]; Hg = new int[256]; Hb = new int[256];
    Hrf = new float[256]; Hgf = new float[256]; Hbf = new float[256];

    if (Hr && Hg && Hb && IMGr && IMGg && IMGb && Hrf && Hgf && Hbf)
    {
        histogram(IMGr,IMGg,IMGb,W,H,Hr,Hg,Hb);


        normalize_histogram(Hr,Hrf);
        normalize_histogram(Hg,Hgf);
        normalize_histogram(Hb,Hbf);


        for (int a=0; a<256; a++)
        {
            somar += Hrf[a];
            somag += Hgf[a];
            somab += Hbf[a];


            if (somab < per) lim_B_i = a;
            if (somar < per) lim_R_i = a;
            if (somag < per) lim_G_i = a;

            if (somar < (1-per)) lim_R_s = a;
            if (somag < (1-per)) lim_G_s = a;
            if (somab < (1-per)) lim_B_s = a;
        }

  //      printf("i - %d %d %d\n s - %d %d %d\n",lim_R_i, lim_G_i, lim_B_i, lim_R_s, lim_G_s, lim_B_s);


        for (int x=0; x<W; x++)
            for (int y=0; y<H; y++)
            {
                IMGr[y*W+x] = u( (int)(  ((float)(IMGr[y*W+x]-lim_R_i)/(float)(lim_R_s-lim_R_i))*255.0  )  );
                IMGg[y*W+x] = u( (int)(  ((float)(IMGg[y*W+x]-lim_G_i)/(float)(lim_G_s-lim_G_i))*255.0  )  );
                IMGb[y*W+x] = u( (int)(  ((float)(IMGb[y*W+x]-lim_B_i)/(float)(lim_B_s-lim_B_i))*255.0  )  );
            }
    }

    if (Hr) delete Hr;
    if (Hg) delete Hg;
    if (Hb) delete Hb;
    if (Hrf) delete Hrf;
    if (Hgf) delete Hgf;
    if (Hbf) delete Hbf;
}


void extract_type(int *IMG, int W, int H, int which)
{
    if (IMG)
    {
        for (int x=0; x<W; x++)
            for (int y=0; y<H; y++)
            {
                IMG[y*W+x] = IMG[y*W+x] == which;
            }
    }
}


int fill_nr(int *IMG, int W, int H, int xi, int yi, int newcolor)
{
    int atcolor, x, y, x1, y1, area=0;
    bool go_on=true;
    LISTA *pixels;

    pixels = new LISTA();

    if (IMG)
    {
        atcolor = IMG[W*yi+xi];
        pixels->add(xi,yi,0);
        while (go_on)
        {
            if (pixels->D)
            {
                go_on = true;

                x = pixels->D->x;
                y = pixels->D->y;
                pixels->del();

                x1 = (x+1+W)%W;
                y1 = (y+H)%H;
                if (IMG[W*y1+x1] == atcolor)
                {
                    pixels->add(x1,y1,0);
                    IMG[y1*W+x1] = newcolor;
                    area++;
                }

                x1 = (x-1+W)%W;
                y1 = (y+H)%H;
                if (IMG[W*y1+x1] == atcolor)
                {
                    pixels->add(x1,y1,0);
                    IMG[y1*W+x1] = newcolor;
                    area++;
                }

                x1 = (x+W)%W;
                y1 = (y+1+H)%H;
                if (IMG[W*y1+x1] == atcolor)
                {
                    pixels->add(x1,y1,0);
                    IMG[y1*W+x1] = newcolor;
                    area++;
                }

                x1 = (x+W)%W;
                y1 = (y-1+H)%H;
                if (IMG[W*y1+x1] == atcolor)
                {
                    pixels->add(x1,y1,0);
                    IMG[y1*W+x1] = newcolor;
                    area++;
                }


            }
            else
                go_on = false;

        }
    }

    if (pixels) delete pixels;
    return area;
                 
}


int extract_border2(int *IMG, int W, int H)
{
    int x, y, area=0;


    if (IMG)
    {
        for (x=0; x<W; x++)
        {
            y = 0;
            if (IMG[W*y+x])
                area += fill_nr(IMG, W, H, x, y, 0);

            y = H-1;
            if (IMG[W*y+x])
                area += fill_nr(IMG, W, H, x, y, 0);
        }


    
        for (y=0; y<H; y++)
        {
            x = 0;
            if (IMG[W*y+x])
                area += fill_nr(IMG, W, H, x, y, 0);

            x = W-1;
            if (IMG[W*y+x])
                area += fill_nr(IMG, W, H, x, y, 0);
        }
    }
    return area;

}


int number_of_colors(int *IMG, int W, int H, int *maxc, int *minc)
{
    int cmax=0, cmin=256;

    if (IMG)
    {
        for (int x=0; x<W; x++)
            for (int y=0; y<H; y++)
            {
                if (IMG[W*y+x]>cmax) cmax=IMG[W*y+x];
                if (IMG[W*y+x]<cmin) cmin=IMG[W*y+x];
            }
    }
    if (maxc) *maxc = cmax;
    if (minc) *minc = cmin;

    return (cmax-cmin);
}


void sort_segmented_background(int *IMG, int W, int H)
{
    int maxc, minc, *IMGaux, maxarea=0, area, cor_back;

    IMGaux = new int[W*H];

    if (IMG && IMGaux)
    {
        number_of_colors(IMG,W,H,&maxc, &minc);
        for (int a=minc; a<=maxc; a++)
        {
            memcpy(IMGaux,IMG,W*H*sizeof(int));
            extract_type(IMGaux,W,H,a);
            area = extract_border2(IMGaux,W,H);
            if (area>maxarea)
            {
                maxarea = area;
                cor_back = a;
            }
        }

     
        for (int x=0; x<W; x++)
            for (int y=0; y<H; y++)
            {
                if (IMG[W*y+x]==cor_back) IMG[W*y+x] = -1; 
                if (IMG[W*y+x]>cor_back) IMG[W*y+x] = IMG[W*y+x]-1; 
            }
    }
    if (IMGaux) delete IMGaux;
}

/*
 *   Fnction to find the 3x3 correlation matrix from a RGB image.
 */

void correlation_matrix(int *IMGr, int *IMGg, int *IMGb, int W, int H, float C[3][3], float median[3])
{
    float R, G, B;
   

    if (IMGr && IMGg && IMGb)
    {

      // calculate the RGB median.
       median[0] = 0;
       median[1] = 0;
       median[2] = 0;
        for (int x=0; x<W; x++)
            for (int y=0; y<H; y++)
            {
                median[0] += (float)IMGr[W*y+x];
                median[1] += (float)IMGg[W*y+x];
                median[2] += (float)IMGb[W*y+x];
            }
            median[0] /= (float)(W*H);
            median[1] /= (float)(W*H);
            median[2] /= (float)(W*H);


        for (int cx=0; cx<3; cx++)
            for (int cy=0; cy<3; cy++)
                C[cx][cy] = 0.0;

      // Calculate the correlation matrix
        for (int x=0; x<W; x++)
            for (int y=0; y<H; y++)
            {
                R = (float)IMGr[W*y+x];
                G = (float)IMGg[W*y+x];
                B = (float)IMGb[W*y+x];
                C[0][0] += R*R;  C[0][1] += R*G;  C[0][2] += R*B;
                C[1][0] += G*R;  C[1][1] += G*G;  C[1][2] += G*B;
                C[2][0] += B*R;  C[2][1] += B*G;  C[2][2] += B*B;
            }

        for (int cx=0; cx<3; cx++)
            for (int cy=0; cy<3; cy++)
                C[cx][cy] /= (float)(W*H);


        for (int cx=0; cx<3; cx++)
            for (int cy=0; cy<3; cy++)
                C[cx][cy] -= median[cx]*median[cy];
    }
  
}


/*
 *   Function to proced the rotation in the RGB space to new base
 *   given by the eigenvectors of correlation matrix.
 */

void change_base(int *IMGr, int *IMGg, int *IMGb, int W, int H, float C[3][3], float median[3])
{
    int R, G, B;
    float X_r, X_g, X_b;

    if (IMGr && IMGg && IMGb)
    {
        for (int x=0; x<W; x++)
            for (int y=0; y<H; y++)
            {
                X_r = (float)IMGr[W*y+x] - median[0];
                X_g = (float)IMGg[W*y+x] - median[1];
                X_b = (float)IMGb[W*y+x] - median[2];
                R = (int)(X_r*C[0][0] + X_g*C[1][0] + X_b*C[2][0]);
                G = (int)(X_r*C[0][1] + X_g*C[1][1] + X_b*C[2][1]);
                B = (int)(X_r*C[0][2] + X_g*C[1][2] + X_b*C[2][2]);

                IMGr[W*y+x] = R;
                IMGg[W*y+x] = G;
                IMGb[W*y+x] = B;
            }
    }
}


/*
 *   Jacobi method do find eigenvalues and eigenvectors for a 
 *   real simetrical matrix. 
 *
 *    Extracted from Numerical Recipes in C chapter 11. Adapted to
 *    work only with 3x3 matrix. Also used fixed auxiliary vectors 
 *    1x3 b and z (no memory alocations)
 *
 */

#define ROTATE(a,i,j,k,l) g=a[i][j]; h=a[k][l]; a[i][j]=g-s*(h+g*tau); a[k][l]=h+s*(g-h*tau);

void jacobi(float a[3][3], int n, float d[3], float v[3][3], int *nrot)
{
    int j,iq,ip,i;
    float tresh, theta, tau, t, sm, s, h, g, c, b[3], z[3];


    for (ip=0; ip<n; ip++)
    {
        for (iq=0; iq<n; iq++)
            v[ip][iq] = 0.0;
        v[ip][ip]=1.0;
    }
    for (ip=0; ip<n; ip++)
    {
        b[ip] = d[ip] = a[ip][ip];
        z[ip] = 0.0;
    }
        
    *nrot = 0;

     for (i=1; i<=50; i++)
     {
         sm = 0.0;
         for (ip=0; ip<n-1; ip++)
         {
             for (iq=ip+1; iq<n; iq++)
                 sm += fabs(a[ip][iq]);
         }
         if (sm == 0.0) 
         {
             return;
         }

         if (i<4)
             tresh = 0.2*sm/(n*n);
         else
             tresh = 0.0;
        
         for (ip=0; ip<n-1; ip++)
         {
             for (iq=ip+1; iq<n; iq++)
             {
                 g=100.0*fabs(a[ip][iq]);
                 if (i>4 && (float)(fabs(d[ip])+g) == (float)fabs(d[ip]) && (float)(fabs(d[iq]+g)) == (float)(d[iq]))
                     a[ip][iq] = 0.0;
                 else if (fabs(a[ip][iq])>tresh)
                 {
                     h = d[iq]-d[ip];
                     if ((float)(fabs(h)+g) == (float)fabs(h))
                         t = (a[ip][iq])/h;
                     else
                     {
                         theta=0.5*h/(a[ip][iq]);
                         t = 1.0/(fabs(theta)+sqrt(1.0+theta*theta));
                         if (theta<0.0) t = -t;
                     }
                     c = 1.0/sqrt(1+t*t);
                     s = t*c;
                     tau = s/(1.0+c);
                     h = t*a[ip][iq] ;
                     z[ip] -= h;
                     z[iq] += h;
                     d[ip] -= h;
                     d[iq] += h;
                     a[ip][iq] = 0.0;
                     for (j=0; j<=ip-1; j++)
                     {
                         ROTATE(a,j,ip,j,iq);
                     }
                     for (j=ip+1; j<=iq-1; j++)
                     {
                         ROTATE(a,ip,j,j,iq);
                     }
                     for (j=iq+1; j<n; j++)
                     {
                         ROTATE(a,ip,j,iq,j);
                     }
                     for (j=0; j<n; j++)
                     {
                         ROTATE(v,j,ip,j,iq);
                     }
                     ++(*nrot);
                 }
             }
         }
         for (ip=0; ip<n; ip++)
         {
             b[ip] += z[ip];
             d[ip] = b[ip];
             z[ip] = 0.0;
         }
     }
/*     erro*/
}


/*
 *    Adjust function... (to make range 0..255 for global min and max R, G, B)
 */
void adjust_RGB(int *IMGr, int *IMGg, int *IMGb, int W, int H)
{
    int imax_r, imin_r;
    int imax_g, imin_g;
    int imax_b, imin_b;
    int imax, imin;

    imax_r = max(IMGr,W*H);
    imax_g = max(IMGg,W*H);
    imax_b = max(IMGb,W*H);

    imax = ((imax_r + imax_g) + abs(imax_r - imax_g))/2;
    imax = ((imax + imax_b) + abs(imax - imax_b))/2;

    imin_r = min(IMGr,W*H);
    imin_g = min(IMGg,W*H);
    imin_b = min(IMGb,W*H);

    imin = ((imin_r + imin_g) - abs(imin_r - imin_g))/2;
    imin = ((imin + imin_b) - abs(imin - imin_b))/2;

    for (int x=0; x<W; x++)
        for (int y=0; y<H; y++)
        {
            IMGr[W*y+x] = (int)( ( ((float)IMGr[W*y+x]-(float)imin)/((float)imax-(float)imin) )*255.0 );
            IMGg[W*y+x] = (int)( ( ((float)IMGg[W*y+x]-(float)imin)/((float)imax-(float)imin) )*255.0 );
            IMGb[W*y+x] = (int)( ( ((float)IMGb[W*y+x]-(float)imin)/((float)imax-(float)imin) )*255.0 );
        }

}


// Put x´s in the image at location (x,y)

void put_x(int *IMG, int W, int H, int x, int y)
{
   int tam=4;
   IMG[W*y+x] = 255;
   if ((x>tam) && (x<(W-tam)) && (y>tam) && (y<(H-tam)))
   {
        for(int a=0; a<tam; a++)
        {
              IMG[W*y+(x+a)] = 255;
              IMG[W*(y+a)+x] = 255;
              IMG[W*y+(x-a)] = 255;
              IMG[W*(y-a)+x] = 255;
              IMG[W*(y+a)+(x+a)] = 255;
              IMG[W*(y-a)+(x+a)] = 255;
              IMG[W*(y+a)+(x-a)] = 255;
              IMG[W*(y-a)+(x-a)] = 255;
         }
   }
}
