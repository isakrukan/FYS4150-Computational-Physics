#include <armadillo> 



int get_flat_index(int i, int j, unsigned int M){
    return i*(M-2) + j;
}

int main(){

    unsigned  int N = 6; 
    unsigned  int M = 6;
    unsigned int M_flat = (M-2) * (M-2);

    std::vector<arma::cx_mat> u_matrix(N, arma::cx_mat(M-1, M-1));
    std::vector<arma::cx_vec> u(N, arma::cx_vec(M_flat));

    for (int n=0; n<N; n++){
        u_matrix[n](0,0) = 1;           u[n](0) = 1;
        u_matrix[n](0,1) = 5;           u[n](1) = 5;
        u_matrix[n](0,2) = 9;           u[n](2) = 9;
        u_matrix[n](0,3) = 13;          u[n](3) = 13;
        u_matrix[n](1,0) = 2;           u[n](4) = 2;
        u_matrix[n](1,1) = 6;           u[n](5) = 6;
        u_matrix[n](1,2) = 10;          u[n](6) = 10;
        u_matrix[n](1,3) = 14;          u[n](7) = 14;
        u_matrix[n](2,0) = 3;           u[n](8) = 3;
        u_matrix[n](2,1) = 7;           u[n](9) = 7;
        u_matrix[n](2,2) = 11;          u[n](10) = 11;
        u_matrix[n](2,3) = 15;          u[n](11) = 15;
        u_matrix[n](3,0) = 4;           u[n](12) = 4;
        u_matrix[n](3,1) = 8;           u[n](13) = 8;
        u_matrix[n](3,2) = 12;          u[n](14) = 12;
        u_matrix[n](3,3) = 16;          u[n](15) = 16;
    }

    for (int i=0; i<M-2; i++){
        for (int j=0; j<M-2; j++){
            int k = get_flat_index(i,j,M);
            std::cout << u_matrix[0](i,j) << " == " << u[0](k) << std::endl;
        }
    }

}