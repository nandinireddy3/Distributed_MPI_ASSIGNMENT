// #include <mpich/mpi.h>
#include "mpi.h"
#include <bits/stdc++.h>

using namespace std;

int MACRO_MAX  = (int)INT_MAX;

int main(int argc,char* argv[]){

    MPI_Init(&argc,&argv);

    int ROOT = 0;
    MPI_Comm comm = MPI_COMM_WORLD;
    int size,rank;
    srand(time(NULL));
    MPI_Comm_size(comm,&size);
    MPI_Comm_rank(comm,&rank);

    int N;
    if(rank == ROOT){
        cin >> N;
        
        int nrows1,nrows2;
        nrows1 = N/size;
        nrows2 = N-(size-1)*nrows1;

    }
    MPI_Bcast(&N,1,MPI_INT,ROOT,comm);

    int blocksize;
    blocksize = N/size;

    if(blocksize != 0){
        int startrow = rank*blocksize;
        int endrow = (rank != size-1)?(startrow+blocksize-1):(N-1);

        int* chunkdist = new int[(endrow-startrow+1)*N];

        int* Krow = new int[N];

        if(rank == ROOT){
            int* dist;
            dist = new int[N*N];
            for(int i = 0; i < N*N; i++) {
                cin >> dist[i];
                if(dist[i] == -1) dist[i] = (int)MACRO_MAX;
            }

            for(int i = 0; i < (endrow-startrow+1)*N; i++){
                    chunkdist[i] = dist[i];
            }

            if(size != 1){
                int cur_index = blocksize*N;
                for(int i = 1; i < size; i++){
                    if(i != size-1){
                        MPI_Send(dist+cur_index,blocksize*N,MPI_INT,i,0,comm);
                        cur_index += blocksize*N;
                    }
                    else{
                        MPI_Send(dist+cur_index,(N-(size-1)*blocksize)*N,MPI_INT,i,0,comm);
                    }
                }
            }

            delete[] dist;
        }

        if(rank != ROOT)
        MPI_Recv(chunkdist,(endrow-startrow+1)*N,MPI_INT,ROOT,0,comm,MPI_STATUS_IGNORE);


        for(int k = 0; k < N; k++){

            if((startrow <= k) && (k <= endrow)){
                for(int i = 0; i < N; i++){
                    Krow[i] = chunkdist[(k-startrow)*N+i];
                }
            }
            int processWithRowK = ((k/blocksize) >= size)? size-1:(k/blocksize);
            MPI_Bcast(Krow,N,MPI_INT,processWithRowK,comm);

            for(int i = 0; i < (endrow-startrow+1); i++){
                for(int j = 0; j < N; j++){
                    
                    int add = ((chunkdist[i*N+k] == (int)MACRO_MAX) || (Krow[j] == (int)MACRO_MAX))?(int)MACRO_MAX:chunkdist[i*N+k]+Krow[j];
                    chunkdist[i*N+j] = min(add,chunkdist[i*N+j]);
                    
                }
            }
        }

        if(rank !=ROOT)
        MPI_Send(chunkdist,(endrow-startrow+1)*N,MPI_INT,ROOT,0,comm);

        if(rank == ROOT){
            int* ans = new int[N*N];

            for(int i = 0; i < blocksize*N; i++){
                ans[i] = chunkdist[i];
            }

            if(size != 1){
                int cur_index = blocksize*N;
                for(int i = 1; i < size; i++){
                    if(i != size-1){
                        MPI_Recv(ans+cur_index,blocksize*N,MPI_INT,i,0,comm,MPI_STATUS_IGNORE);
                        cur_index += blocksize*N;
                    }
                    else{
                        MPI_Recv(ans+cur_index,(N-(size-1)*blocksize)*N,MPI_INT,i,0,comm,MPI_STATUS_IGNORE);
                    }
                }
            }

            for(int i = 0; i < N; i++){
                for(int j = 0; j < N; j++){
                    
                    cout << ((ans[i*N+j] == (int)MACRO_MAX)?-1:ans[i*N+j]) << " ";
                }
                cout << endl;
            }

            delete[] ans;
        }

        delete[] chunkdist;
        delete[] Krow;
    }
    else{
        
        if(rank == ROOT){
            int* dist;
            dist = new int[N*N];

            for(int i = 0; i < N*N; i++) {
                cin >> dist[i];
                if(dist[i] == -1) dist[i] = (int)MACRO_MAX;
            }

            for(int k = 0; k < N; k++){
                for(int i = 0; i < N; i++){
                    for(int j = 0; j < N; j++){
                        if((dist[i*N+k] != INT_MAX) && (dist[k*N+j] != INT_MAX)){
                            dist[i*N+j] = min(dist[i*N+j],dist[i*N+k]+dist[k*N+j]);
                        }
                    }
                }
            }

            for(int i = 0; i < N; i++){
                for(int j = 0; j < N; j++){
                    
                    cout << ((dist[i*N+j] == (int)MACRO_MAX)?-1:dist[i*N+j]) << " ";
                }
                cout << endl;
            }

            delete[] dist;

        }
    }
    

    MPI_Finalize();
}
