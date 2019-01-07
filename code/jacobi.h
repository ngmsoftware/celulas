//#include <sys/io.h>
//#include <string.h>
#include <stdio.h>
#include <math.h>


float *vector(int nh)
{
	float *v;

	v = new float[nh+1];

        return v;
}


void free_vector(float *v)
{
	delete v;
}


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


main()
{
    int a=0;
    int *hehe;
    float M[3][3];
    float V[3][3];
    float lambda[3];
    int N;

    M[0][0] = 0;  M[0][1] = 0.5;  M[0][2] = 2;
    M[1][0] = 0.5;  M[1][1] = 3;  M[1][2] = 5;
    M[2][0] = 2;  M[2][1] = 5;  M[2][2] = -0.19;

    V[0][0] = 15;  V[0][1] = 0;  V[0][2] = 0;
    V[1][0] = 0;  V[1][1] = 15;  V[1][2] = 0;
    V[2][0] = 0;  V[2][1] = 0;  V[2][2] = 15;


    printf("hehe\n");

    jacobi(M,3,lambda,V,&N);

   
    printf(" Eigenvalures: \n%.3f %.3f %.3f \n",lambda[0], lambda[1], lambda[2]);
    printf(" Eigenvectors: \n %.3f %.3f %.3f \n %.3f %.3f %.3f \n %.3f %.3f %.3f\n",V[0][0], V[0][1], V[0][2],
V[1][0], V[1][1], V[1][2], V[2][0], V[2][1], V[2][2]);
    

//    ioperm(0x378,2,0x379);

//    outb(1,0x379);





}
