#include "mpi.h"
#include <bits/stdc++.h>

using namespace std;

bool mod(float x,float y){

    return (x*x+y*y < 4);
}

void check(complex<float> z[],int* res, int l2,int D,int K){
    
    for(int i = 0; i < l2; i++){
        complex<float> comp = z[i];
        bool flag = false;
        for(int j = 0; j <= K; j++){
            if(!mod(real(comp),imag(comp))){
                res[i] = 0;
                flag = true;
                break;
            }
            comp = pow(comp,D)+z[i];
        }
        if(!flag) res[i] = 1;
    }

    return;

}
int main(int argc, char* argv[]){

    MPI_Init(&argc,&argv);

    int root_node = 0;
    MPI_Comm comm = MPI_COMM_WORLD;

    int size,rank;
    MPI_Comm_size(comm,&size);
    MPI_Comm_rank(comm,&rank);

    int K,D;

    if(rank == root_node){
        
        int N,M;
        cin >> N >> M >> D >> K;
        MPI_Bcast(&K,1,MPI_INT,root_node,comm);
        MPI_Bcast(&D,1,MPI_INT,root_node,comm);

        complex<float>* z;
        int* res;
        z = new complex<float>[M*N];
        res = new int[M*N];
        
        float l = 2.0/(M-1);
        float w = 2.5/(N-1);
        for(int i = 0; i < M*N; ++i){

            int row = i/N;
            int col = i%N;

            float x = -1.5+w*col;
            float y = 1-l*row;

            z[i].real(x);
            z[i].imag(y); 
        }

        int l2 = (M*N)/size; 
        int l1 = (M*N-(size-1)*l2); 
        
        check(z,res,l1,D,K);

        if((rank != size-1)){
            for(int i = 0; i < size-1; i++) {
            // MPI_Send(&l1,1,MPI_INT,i+1,0,comm);
               MPI_Send(&l2,1,MPI_INT,i+1,0,comm);
               MPI_Send(z+l1+i*l2,l2,MPI_C_FLOAT_COMPLEX,i+1,0,comm);
            }

            for(int i = 0; i < size-1; i++) {
                MPI_Recv(res+l1+i*l2,l2,MPI_INT,i+1,0,comm,MPI_STATUS_IGNORE);
            }
        }

        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                cout << ((res[i * N + j] == 1)?1:0) << " ";  // Access array elements accordingly
            }
            cout << endl;
        }

        
        delete[] z;
        delete[] res;
    }
    else{

        MPI_Bcast(&K,1,MPI_INT,root_node,comm);
        MPI_Bcast(&D,1,MPI_INT,root_node,comm);
        
        // int l1;
        int l2;
        // MPI_Recv(&l1,1,MPI_INT,root_node,0,comm,MPI_STATUS_IGNORE);
        MPI_Recv(&l2,1,MPI_INT,root_node,0,comm,MPI_STATUS_IGNORE);

        complex<float>* temp_z;
        int* temp_res;
        temp_z = new complex<float>[l2];
        temp_res = new int[l2];
        MPI_Recv(temp_z,l2,MPI_C_FLOAT_COMPLEX,root_node,0,comm,MPI_STATUS_IGNORE);

        check(temp_z,temp_res,l2,D,K);

        MPI_Send(temp_res,l2,MPI_INT,root_node,0,comm);

        
        delete[] temp_z;
        delete[] temp_res;

    }
    MPI_Finalize();
    
    return EXIT_SUCCESS;
}