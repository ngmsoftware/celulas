#include<math.h>

class matriz
  {
    private:
        int l,c;

        float data[10][10];

    public:

    int GetL();
    int GetC();

    float GetData(int l, int c);

    matriz(int nl, int nc);
    matriz();

    void set_data(float *D);

    friend matriz operator+(matriz& v1, matriz& v2);
    friend matriz operator-(matriz& v1, matriz& v2);
    friend matriz operator*(matriz& v1, matriz& v2);
    friend matriz operator*(float esc, matriz& v1);
    friend matriz operator*(matriz& v1, float esc);

  };

matriz::matriz(int nl, int nc)
{
	int t1,t2;

	l=nl;
	c=nc;

	for (t1=0;t1<=l-1;t1++)
	  for (t2=0;t2<=c-1;t2++)
	    data[t1][t2]=0;
}

matriz::matriz()
{
	int t1,t2;

	l=3;
	c=3;

	for (t1=0;t1<=l-1;t1++)
	  for (t2=0;t2<=c-1;t2++)
	    data[t1][t2]=0;
}

 /*------------------------------------------------*/
void matriz::set_data(float *D)
{
    for (int t1=0;t1<=l-1;t1++)
        for (int t2=0;t2<=c-1;t2++)
            data[t1][t2]=D[t1*c+t2];

}

int matriz::GetL()
{
    return(l);
}

int matriz::GetC()
{
    return(c);
}

float matriz::GetData(int l, int c)
{
    return(data[l][c]);
}

/*---------------------Adicao-------------------------------------------*/
matriz operator+(matriz& v1, matriz& v2)
{
	int t1,t2;
	matriz m;

	if ((v1.GetL()==v2.GetL())&&(v1.GetC()==v2.GetC()))
	  {

	    for (t1=0;t1<=v1.l-1;t1++)
	      for (t2=0;t2<=v1.c-1;t2++)
            m.data[t1][t2]=v1.data[t1][t2]+v2.data[t1][t2];
	  }

	return(m);
}
 /*-------------------------------------------------------------------*/


 /*-----------------------------------subtracao------------------------*/

matriz operator-(matriz& v1, matriz& v2)
{
	int t1,t2;
	matriz m;

	if ((v1.l==v2.l)&&(v1.c==v2.c))
	  {

	    for (t1=0;t1<=v1.l-1;t1++)
	      for (t2=0;t2<=v1.c-1;t2++)
		m.data[t1][t2]=v1.data[t1][t2]-v2.data[t1][t2];
	  }

	return(m);
}

/*------------------------------------------------------------------------*/

/*-------------------------------multiplicacao por escalar----------------*/

matriz operator*(float esc, matriz& v1)
{
	int t1,t2;
	matriz m;

	for (t1=0;t1<=v1.l-1;t1++)
	  for (t2=0;t2<=v1.c-1;t2++)
	    m.data[t1][t2]=esc*v1.data[t1][t2];


	return(m);
}
/*------------------------------------------------------------------*/

/*-------------------------------multiplicacao por escalar----------------*/

matriz operator*(matriz& v1, float esc)
{
	int t1,t2;
	matriz m;

	for (t1=0;t1<=v1.l-1;t1++)
	  for (t2=0;t2<=v1.c-1;t2++)
	    m.data[t1][t2]=esc*v1.data[t1][t2];


	return(m);
}
/*------------------------------------------------------------------*/


/*---------------------produto matriz por matriz------------------------*/

matriz operator*(matriz& v1, matriz& v2)
{
	int t1,t2,t;
	matriz m;

	if (v1.c==v2.l)
	{
        m.l=v1.l;
    	m.c=v2.c;

	    for (t1=0;t1<=m.c-1;t1++)
            for (t2=0;t2<=m.l-1;t2++)
            {
                m.data[t2][t1]=0;
                for (t=0;t<=v1.c-1;t++)
                    m.data[t2][t1]+=v1.data[t2][t]*v2.data[t][t1];
            }
    }

	return(m);
}
/*----------------------------------------------------------------*/
