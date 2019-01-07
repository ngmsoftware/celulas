/*
 *  Mathematical morphology header file
 */


int cruz_1[9] = { 0, 1, 0,
                  1, 1, 1,
                  0, 1, 0 };

int full_3[9] = { 1, 1, 1, 
                  1, 1, 1,
                  1, 1, 1 };

int circulo_R_2[25] = { 0, 0, 1, 0, 0,
                        0, 1, 1, 1, 0,
                        1, 1, 1, 1, 1,
                        0, 1, 1, 1, 0,
                        0, 0, 1, 0, 0 };

int circulo_R_3[49] = { 0, 0, 1, 1, 1, 0, 0,
                        0, 1, 1, 1, 1, 1, 0,
                        1, 1, 1, 1, 1, 1, 1,
                        1, 1, 1, 1, 1, 1, 1,
                        1, 1, 1, 1, 1, 1, 1,
                        0, 1, 1, 1, 1, 1, 0,
                        0, 0, 1, 1, 1, 0, 0};

void uniao(int *IMG1, int *IMG2, int W, int H, int *result);

void intersecao(int *IMG1, int *IMG2, int W, int H, int *result);

void complemento(int *IMG, int W, int H, int *result);

void diferenca(int *A1, int *A2, int W, int H, int *result);

void erosao(int *IMG, int W, int H, int *elemento, int Te, int *IMGresult);

void dilatacao(int *IMG, int W, int H, int *elemento, int Te, int *IMGresult);

void abertura(int *X, int W, int H, int *B, int Te, int *result);

void fechamento(int *X, int W, int H, int *B, int Te, int *result);

void afinamento(int *X, int W, int H, int *B, int Te, int *result);

void esqueletizacao(int *X, int W, int H, int *B, int Te, int *result);

void erosao_geodesica(int *IMG, int *S, int W, int H, int *elemento, int Te, int *result);

void dilatacao_geodesica(int *IMG, int *S, int W, int H, int *elemento, int Te, int *result);

void reconstrucao(int *Z, int *X, int W, int H, int *elemento, int Te, int *result);
void reconstrucao2(int *Z, int *X, int W, int H, int *elemento, int Te, int *result);

void ultima_erosao(int *IMG, int W, int H, int *elemento, int Te, int *result);

void extract_border(int *IMG, int W, int H, int *result);

/*
 *  Mathematical morphology  (end)
 */

