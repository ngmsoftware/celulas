#include "morphology.h"
#include "stufs.h"

/*
 *  Header file to do something little more specific about 2 types cell counting.
 *
 */

void extract_type(int *IMG, int W, int H, int which, int *result)
{

    if (IMG && result)
    {
        for (int x=0; x<W; x++)
            for (int y=0; y<H; y++)
            {
                result[y*W+x] = IMG[y*W+x] == which;
            }
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


void fill_nr(int *IMG, int W, int H, int xi, int yi, int newcolor)
{
    int atcolor, x, y, x1, y1;
    bool go_on=true;
    LISTA *pixels;

    pixels = new LISTA();

    if (IMG)
    {
        atcolor = IMG[W*yi+xi];
        while (go_on)
        {
            if (pixels->data)
            {
                go_on = true;

                x = pixels->data->x;
                y = pixels->data->y;
                pixels->del();

                x1 = (xi-1+W)%W;
                y1 = (yi-1+H)%H;
                if (IMG[W*y1+x1] == atcolor)
                {
                    pixels->add(x1,y1,0);
                    IMG[y1*W+x1] = newcolor;
                }

                x1 = (xi+1+W)%W;
                y1 = (yi+1+H)%H;
                if (IMG[W*y1+x1] == atcolor)
                {
                    pixels->add(x1,y1,0);
                    IMG[y1*W+x1] = newcolor;
                }

                x1 = (xi+1+W)%W;
                y1 = (yi-1+H)%H;
                if (IMG[W*y1+x1] == atcolor)
                {
                    pixels->add(x1,y1,0);
                    IMG[y1*W+x1] = newcolor;
                }

                x1 = (xi-1+W)%W;
                y1 = (yi+1+H)%H;
                if (IMG[W*y1+x1] == atcolor)
                {
                    pixels->add(x1,y1,0);
                    IMG[y1*W+x1] = newcolor;
                }


            }
            else
                go_on = false;

        }
    }

    if (pixels) delete pixels;

}
