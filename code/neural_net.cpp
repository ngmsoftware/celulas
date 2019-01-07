#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "neural_net.h"

extern float min(float *P, int ncenters, int *v);
extern void histogram(int *IMGr, int *IMGg, int *IMGb, int W, int H, int *Hist_r,  int *Hist_g,  int *Hist_b);

/*
 *  Neural net (begin)
 */

float moment(int *IMG, int W, int H, int order)
{
    int Hi[256];
    float Hif[256];
    float mo=0.0, med=0.0;

    if (IMG)
    {
       if (order==0) return(IMG[W*H/2]);
       
       for (int x=0; x<W; x++)
            for (int y=0; y<H; y++)
                med += (float)IMG[W*y+x]; 
        med /= (float)(W*H);
          
        if (order==1) return(med);

        histogram(IMG, NULL, NULL, W, H, Hi, NULL, NULL);
       
        float st = 0.0;
        for (int a=0; a<256; a++)
	{
	   st += Hi[a];
	}
        for (int a=0; a<256; a++)
	{
	   Hif[a]=(float)(Hi[a])/st;
	}
        
	   
        for (int a=0; a<256; a++)
        {
            mo += Hif[a]* pow(((float)(a)-med),(float)order);
        }

        return(mo);
    }

}

void competitiva::convert_imageRGB_to_moment_data(int *IMGr, int *IMGg, int *IMGb, int W, int H, float *data, int max_order, int Npoints, int B)
// B = Half block size
// max_order = Order of moments to be computed
// NPoints = Number os points to be sampled from image.
{
    int cont=0, dim, x, y;
    int *IMGbr;
    int *IMGbg;
    int *IMGbb, xx, yy;

    IMGbr = new int[(2*B+1)*(2*B+1)];
    IMGbg = new int[(2*B+1)*(2*B+1)];
    IMGbb = new int[(2*B+1)*(2*B+1)];

    if (IMGr && IMGg && IMGb && data)
    {
        dim = 3+max_order*3;

/*        for (int x=0; x<W; x++)
            for (int y=0; y<H; y++)  */
        for(int ppts=0; ppts<Npoints; ppts++)

            {
                x = random(10000) % W;
                y = random(10000) % H;

//                data[cont*dim+0] = (float)(IMGr[y*W+x]);
//                data[cont*dim+1] = (float)(IMGg[y*W+x]);
//                data[cont*dim+2] = (float)(IMGb[y*W+x]);
                for (int order=1; order<=(max_order+1); order++)
                {
                    for (int xb=-B; xb<=B; xb++)
                        for (int yb=-B; yb<=B; yb++)
                        {
                            xx = (x+xb+W)%W;
                            yy = (y+yb+H)%H;
                            IMGbr[(2*B+1)*(yb+B)+(xb+B)] = IMGr[W*yy+xx];
                            IMGbg[(2*B+1)*(yb+B)+(xb+B)] = IMGg[W*yy+xx];
                            IMGbb[(2*B+1)*(yb+B)+(xb+B)] = IMGb[W*yy+xx];
                        }
                    data[cont*dim+3*(order-1)+0] = moment(IMGbr,2*B+1,2*B+1,order);
                    data[cont*dim+3*(order-1)+1] = moment(IMGbg,2*B+1,2*B+1,order);
                    data[cont*dim+3*(order-1)+2] = moment(IMGbb,2*B+1,2*B+1,order);
                }

                cont++;
            }
    }

    delete  IMGbr;
    delete  IMGbg;
    delete  IMGbb;

}


void competitiva::comvert_imageRGB_to_flat_data(int *IMGr, int *IMGg, int *IMGb, int W, int H, float *data)
{
    int cont=0;

    if (IMGr && IMGg && IMGb && data)
    {
        for (int x=0; x<W; x++)
            for (int y=0; y<H; y++)
            {
                data[cont*3+0] = (float)(IMGr[y*W+x]);
                data[cont*3+1] = (float)(IMGg[y*W+x]);
                data[cont*3+2] = (float)(IMGb[y*W+x]);
                cont++;
            }
    }
}


competitiva::competitiva(int Di, int Cen, float *data, int dsize)
{
    long int a;

    centers = new float*[Cen];
    centers_old = new float*[Cen];

    for (int c=0; c<Cen; c++)
    {
        centers[c] = new float[Di];
        centers_old[c] = new float[Di];
    }

    dimension = Di;
    num_centers = Cen;

    if (data)
        for (int c=0; c<num_centers; c++)
        {
            a = random(1000000) % dsize;
            for (int d=0; d<dimension; d++)
            {
                centers[c][d] = data[dimension*a + d];
                centers_old[c][d] = data[dimension*a + d];
            }
        }
}

void competitiva::set_centers(float *data, int dsize)
{
    long int a;

    if (data)
    {



        for (int c=0; c<num_centers; c++)
        {
            a = random(1000000) % dsize;
            for (int d=0; d<dimension; d++)
            {
                centers[c][d] = data[dimension*a + d];
                centers_old[c][d] = data[dimension*a + d];
            }
        }
    }
}


float *competitiva::get_center(int which)
{
    return centers[which];
}

float **competitiva::get_all_centers()
{
    return(centers);
}


void competitiva::train(float *data, int dsize, int Max_i, float alpha, float erro)
{
    float tau;
    int cont = 0;
    float err = erro+1, soma;
    float *P, *x, *tra;
    int k;


    P = new float[num_centers];
    x = new float[dimension];
    tra = new float[dimension];

    if (P && x && tra && data)
    {

        tau = pow(0.001,1.0/(float)(Max_i));

        while (cont<Max_i) 
        {
            for (int nn=0; nn<dsize; nn++)
            {
                for (int dd=0; dd<dimension; dd++)
                    x[dd] = data[nn*dimension + dd];
    
                for (int a=0; a<num_centers; a++)
                    P[a] = 0.0;    

                for (int a=0; a<num_centers; a++)
                {
                    for (int dd=0; dd<dimension; dd++)
                        tra[dd] = centers[a][dd]-x[dd];

                    for (int dd=0; dd<dimension; dd++)
                        P[a] += tra[dd]*tra[dd];
                }
 
                min(P,num_centers,&k); // Just the index ... not shortest value...


                for (int dd=0; dd<dimension; dd++)
                    centers[k][dd] = centers[k][dd]+alpha*(x[dd]-centers[k][dd]);
    
                 
       
            }
            cont++;
            alpha = alpha*tau;
	   
              // Calculate the error and updade centers_old

	       
	       //-----------------
                err=0.0;
                for (int he=0; he<num_centers; he++)  
                {
                    soma = 0.0;
                    for (int dd=0; dd<dimension; dd++)
                    {
                        soma += (centers[he][dd]-centers_old[he][dd])*(centers[he][dd]-centers_old[he][dd]);
                    }
                    err += sqrt(soma);
                }
                err /= (float)(num_centers);
	       
	       printf("%.8f\n",err);
 
                for (int he=0; he<num_centers; he++)
                    for (int dd=0; dd<dimension; dd++)
                        centers_old[he][dd] = centers[he][dd];

                //---------------    
	   
        }


    }

    if (P) delete P; if (x) delete x; if (tra) delete tra;
  
}

competitiva::~competitiva()
{
    for (int c=0; c<num_centers; c++)
    {
        delete centers[c];
        delete centers_old[c];
    }
    delete centers;
    delete centers_old;
}



/* auxiliary functions */

void classify(int *IMGr, int *IMGg, int *IMGb, int W, int H, float **centers, int ncenters, int *result, int max_order, int B)
{
    float *P, *tra, *X;
    int k, dim, order, xx, yy;

    int *IMGbr;
    int *IMGbb;
    int *IMGbg;

    IMGbr = new int[(2*B+1)*(2*B+1)];
    IMGbg = new int[(2*B+1)*(2*B+1)];
    IMGbb = new int[(2*B+1)*(2*B+1)];


    dim = 3+3*max_order;


    P = new float[ncenters];
    X = new float[dim];
    tra = new float[dim];


    if (P && X && tra && result && IMGr && IMGg && IMGb)
    {

        for (int x=0; x<W; x++)
            for (int y=0; y<H; y++)
            {

//---------------------


//                X[0] = (float)(IMGr[y*W+x]);
//                X[1] = (float)(IMGg[y*W+x]);
//                X[2] = (float)(IMGb[y*W+x]);
                for (int order=1; order<=(max_order+1); order++)
                {
                    for (int xb=-B; xb<=B; xb++)
                        for (int yb=-B; yb<=B; yb++)
                        {
                            xx = (x+xb+W)%W;
                            yy = (y+yb+H)%H;
                            IMGbr[(2*B+1)*(yb+B)+(xb+B)] = IMGr[W*yy+xx];
                            IMGbg[(2*B+1)*(yb+B)+(xb+B)] = IMGg[W*yy+xx];
                            IMGbb[(2*B+1)*(yb+B)+(xb+B)] = IMGb[W*yy+xx];
                        }
                    X[3*(order-1)+0] = moment(IMGbr,2*B+1,2*B+1,order);
                    X[3*(order-1)+1] = moment(IMGbg,2*B+1,2*B+1,order);
                    X[3*(order-1)+2] = moment(IMGbb,2*B+1,2*B+1,order);
                }


//--------------------

                for (int a=0; a<ncenters; a++)
                {
                    P[a] = 0.0;
                    for (int pi=0; pi<dim; pi++)
                    {
                        tra[pi] = X[pi]-centers[a][pi];
                        P[a] += tra[pi]*tra[pi];
                    }
                }

                min(P,ncenters,&k);

/*                IMGr[W*y+x] = (int)(centers[k*3+0]);
                IMGg[W*y+x] = (int)(centers[k*3+1]);
                IMGb[W*y+x] = (int)(centers[k*3+2]);*/

                result[W*y+x] = k;


            }
    }

    if (X) delete X; if (P) delete P; if (tra) delete tra;

    delete IMGbr;
    delete IMGbg;
    delete IMGbb;

}


/*
 *  Neural net (end)
 */



