#include <armadillo>

class CrankNicolsen
{

private:

int get_flat_index(int i, int j, unsigned int M);

public:

arma::mat A;
arma::mat B;


void create_A_matrix(float a0, float r);
void create_B_matrix(float b0, float r);



};