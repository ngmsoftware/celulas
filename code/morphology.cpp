#include <string.h>
//#include "memory.h"

/*
 *  Mathematical morphology (begin)
 */

void uniao(int *IMG1, int *IMG2, int W, int H, int *result)
{
    if (result)
    {
        for (int x=0; x<W; x++)
            for (int y=0; y<H; y++)
            {
                result[x+W*y] = IMG1[x+W*y] || IMG2[x+W*y];
            }
    }
}

void intersecao(int *IMG1, int *IMG2, int W, int H, int *result)
{
    if (result)
    {
        for (int x=0; x<W; x++)
            for (int y=0; y<H; y++)
            {
                result[x+W*y] = IMG1[x+W*y] && IMG2[x+W*y];
            }
    }
}

void complemento(int *IMG, int W, int H, int *result)
{
    if (result)
    {
        for (int x=0; x<W; x++)
            for (int y=0; y<H; y++)
            {
                result[x+W*y] = !IMG[x+W*y];
            }
    }
}

void diferenca(int *A1, int *A2, int W, int H, int *result)
{
    if (result)
    {
        for (int x=0; x<W; x++)
            for (int y=0; y<H; y++)
                result[x+W*y] = (A1[x+W*y]>0) ^ (A2[x+W*y]>0);
    }
}

void erosao(int *IMG, int W, int H, int *elemento, int Te, int *IMGresult)
{
    int pertence, ximg, yimg;
    int img_blk, Te_2;

    if (IMGresult)
    {
        memcpy(IMGresult, IMG, W*H*sizeof(int));
        for(int x=0; x<W; x++)
            for(int y=0; y<H; y++)
            {
                pertence=1;

                if (IMG[W*y + x])
                {
                    for(int e_x=0; e_x<Te; e_x++)
                        for(int e_y=0; e_y<Te; e_y++)
                        {
                            Te_2 = (Te-1) >> 1;
                            ximg = (x+e_x-Te_2+W)%W;
                            yimg = (y+e_y-Te_2+H)%H;
                            img_blk = IMG[W*yimg + ximg];
                            pertence = pertence && ( (elemento[e_x + e_y*Te] && img_blk)  ||  !elemento[e_x + e_y*Te]);
                        }

                    IMGresult[W*y + x] = pertence;
                }
         }
    }
}

void dilatacao(int *IMG, int W, int H, int *elemento, int Te, int *IMGresult)
{
    int pertence, ximg, yimg;
    int img_blk, Te_2;

    if (IMGresult)
    {
        memcpy(IMGresult, IMG, W*H*sizeof(int));
        for(int x=0; x<W; x++)
            for(int y=0; y<H; y++)
            {
                pertence=0;

                for(int e_x=0; e_x<Te; e_x++)
                    for(int e_y=0; e_y<Te; e_y++)
                    {
                        Te_2 = (Te-1) >> 1;
                        ximg = (x+e_x-Te_2+W)%W;
                        yimg = (y+e_y-Te_2+H)%H;
                        img_blk = IMG[W*yimg + ximg];
                        pertence = pertence || (elemento[e_x + Te*e_y] && img_blk);
                    }
                IMGresult[W*y + x] = pertence;
            }
    }
}


void abertura(int *X, int W, int H, int *B, int Te, int *result)
{
    int *temp;

    temp = new int[W*H*sizeof(int)];
    if (result && temp)
    {
        erosao(X,W,H,B,Te,temp);
        dilatacao(temp,W,H,B,Te,result);

        delete temp;
    }
}

void fechamento(int *X, int W, int H, int *B, int Te, int *result)
{
    int *temp;

    temp = new int[W*H*sizeof(int)];
    if (result && temp)
    {
        dilatacao(X,W,H,B,Te,temp);
        erosao(temp,W,H,B,Te,result);

        delete temp;
    }
}

void afinamento(int *X, int W, int H, int *B, int Te, int *result)
{
    int *Bc, *ero1, *ero2, *temp, *Xc;

    Xc = new int[W*H*sizeof(int)];
    ero1 = new int[W*H*sizeof(int)];
    ero2 = new int[W*H*sizeof(int)];
    temp = new int[W*H*sizeof(int)];
    Bc = new int[(Te+1)*(Te+1)*sizeof(int)];

    Bc[0] = 0;     Bc[1] = 1;     Bc[2] = 1;
    Bc[3] = 0;     Bc[4] = 1;     Bc[5] = 1;
    Bc[6] = 0;     Bc[7] = 0;     Bc[8] = 0;

    B[0] = 0;     B[1] = 0;     B[2] = 0;
    B[3] = 1;     B[4] = 0;     B[5] = 0;
    B[6] = 1;     B[7] = 1;     B[8] = 0;

    if (result && ero1 && ero2 && temp && Xc)
    {
//        complemento(B,Te+1,Te+1,Bc);

        complemento(X,W,H,Xc);
        erosao(X,W,H,B,Te,ero1);
        erosao(Xc,W,H,Bc,Te,ero2);
        intersecao(ero1,ero2,W,H,temp);

        diferenca(X,temp,W,H,result);

        delete ero1;
        delete ero2;
        delete temp;
        delete Xc;
        delete Bc;
    }
}

void esqueletizacao(int *X, int W, int H, int *B, int Te, int *result)
{
    int *temp, *temp2, *temp3, *ero;
    bool continua=true;

    temp = new int[W*H];
    temp2 = new int[W*H];
    temp3 = new int[W*H];
    ero = new int[W*H];

    memset(result,0,W*H*sizeof(int));
    if (result && temp && ero && temp2 && temp3)
    {
        memset(result,0,sizeof(int)*W*H);

        memcpy(ero,X,W*H*sizeof(int));

        while(continua)
        {
            abertura(ero,W,H,B,Te,temp);

            diferenca(ero,temp,W,H,temp2);

            uniao(result,temp2,W,H,temp3);

            memcpy(result,temp3,W*H*sizeof(int));

            erosao(ero,W,H,B,Te,temp);
            memcpy(ero,temp,W*H*sizeof(int));
            continua = memchr(ero,1,sizeof(int)*W*H);
        }
        delete temp;
    }
}

void erosao_geodesica(int *IMG, int *S, int W, int H, int *elemento, int Te, int *result)
{
    int *temp, *temp2, *temp3;

    temp = new int[W*H];
    temp2 = new int[W*H];
    temp3 = new int[W*H];
    if (result && temp && temp2 && temp3)
    {
        complemento(S,W,H,temp);

        uniao(IMG,temp,W,H,temp2);

        erosao(temp2,W,H,elemento,Te,temp3);

        intersecao(temp3,S,W,H,result);

        delete temp;
        delete temp2;
        delete temp3;
    }
}

void dilatacao_geodesica(int *IMG, int *S, int W, int H, int *elemento, int Te, int *result)
{
    int *temp;

    temp = new int[W*H];
    if (result && temp)
    {
        dilatacao(IMG,W,H,elemento,Te,temp);

        intersecao(temp,S,W,H,result);

        delete temp;
    }
}



void reconstrucao(int *Z, int *X, int W, int H, int *elemento, int Te, int *result)
{
    int *dil;
    bool continua=true;

    dil = new int[H*W];
    if (result && dil)
    {
        memcpy(result, Z, W*H*sizeof(int));
        while (continua)
        {
            dilatacao_geodesica(result,X,W,H,elemento,Te,dil);

            continua = !(memcmp(result, dil, W*H*sizeof(int)) == 0);
            memcpy(result, dil, W*H*sizeof(int));
        }

        delete dil;
    }
}


void reconstrucao2(int *Z, int *X, int W, int H, int *elemento, int Te, int *result)
{
    int *dil;
    int continua=true;

    dil = new int[H*W];
    if (result && dil)
    {
        memcpy(result, Z, W*H*sizeof(int));
        while (continua)
        {
            dilatacao_geodesica(result,X,W,H,elemento,Te,dil);

            continua = memcmp(result, dil, W*H*2)!=0;
            memcpy(result, dil, W*H*sizeof(int));
        }

        delete dil;
    }
}

void ultima_erosao(int *IMG, int W, int H, int *elemento, int Te, int *result)
{
    int *Xero_n, *Xero_n_1, *temp1, *temp2;
    bool continua = true;

    Xero_n = new int[W*H];
    Xero_n_1 = new int[W*H];
    temp1 = new int[W*H];
    temp2 = new int[W*H];
    if (result, Xero_n && Xero_n_1 && temp1 && temp2)
    {
        memset(result,0,W*H*sizeof(int));
        memcpy(Xero_n_1,IMG,W*H*sizeof(int));

        while(continua)
        {
            erosao(Xero_n_1,W,H,elemento,Te,Xero_n);

            reconstrucao2(Xero_n,Xero_n_1,W,H,elemento,Te,temp1);

            diferenca(Xero_n_1,temp1,W,H,temp2);

            uniao(result,temp2,W,H,temp1);

            memcpy(result,temp1,W*H*sizeof(int));

            int bla = (int)(memchr(Xero_n,1,W*H*sizeof(int)));
            continua = (bool)(bla);
            memcpy(Xero_n_1,Xero_n,W*H*sizeof(int));

        }
        delete Xero_n;
        delete Xero_n_1;
    }
}

void extract_border(int *IMG, int W, int H, int *result)
{
    int *B;
    int elemento[9] = {0, 1, 0, 1, 1, 1, 0, 1, 0};
   
    B = new int[W*H*sizeof(int)];

    memset(B,0,W*H*sizeof(int));

    if (B && IMG && result)
    {
        for (int x=0; x<W; x++)
        {
            B[0*W+x] = 1;
            B[(H-1)*W+x] = 1;
        }
     
 
        for (int y=0; y<H; y++)
        {
            B[y*W+0] = 1;
            B[y*W+W-1] = 1;
        }  

        reconstrucao2(B,IMG,W,H,elemento,3,result);

    }

    if (B) delete B;

}


/*
 *  Mathematical morphology  (end)
 */

