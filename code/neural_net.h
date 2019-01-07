/*
 *  Neural net header file
 */

class competitiva
{
private:
    int dimension;
    int num_centers;

    float **centers;
    float **centers_old;

public:
    competitiva(int Di, int Cen, float *data, int dsize);
    ~competitiva();

    void train(float *data, int dsize, int Max_i, float alpha, float erro);
    void comvert_imageRGB_to_flat_data(int *IMGr, int *IMGg, int *IMGb, int W, int H, float *data);
    void convert_imageRGB_to_moment_data(int *IMGr, int *IMGg, int *IMGb, int W, int H, float *data, int max_order, int Npoints, int B);

    float *get_center(int which);
    float **get_all_centers();
    void set_centers(float *data, int dsize);
};

/* auxiliary functions */

void classify(int *IMGr, int *IMGg, int *IMGb, int W, int H, float **centers, int ncenters, int *result, int max_order, int B);


/*
 *  Neural net (end)
 */



