#include "mpi.h"
// #include <mpich/mpi.h>
#include <bits/stdc++.h>

using namespace std;

void check(int grid[],int ans[],int nrows,int M,int* up,int* down,int rank){

    int dx[8] = {-1,-1,-1,0,1,1,1,0};
    int dy[8] = {-1,0,1,1,1,0,-1,-1};

    int up_size = (up != NULL)?1:0;
    int down_size = (down != NULL)?1:0;
    int len = nrows+(up_size)+(down_size);

    //cout << "rank: " << rank << " ups: " << up_size << " dns: " << down_size << endl;
    int** arr = new int*[len];
    for(int i = 0; i < len; i++)
        arr[i] = new int[M];

    //vector<vector<int>> arr(len,vector<int>(M));

    
    if(up_size != 0){
        for(int i = 0; i < M; i++) arr[0][i] = up[i];
    }    

    int k = 0;
    for(int i = up_size; i < len-down_size; i++){
        for (int j = 0; j < M; ++j) {
            arr[i][j] = grid[k++];
        }
    }

    if(down_size != 0){
        for(int i = 0; i < M; i++) arr[len-down_size][i] = down[i];
    }

    // cout << "rank: " << rank << " ups: " << up_size << " dns: " << down_size << endl;
    // for(int i = 0; i < len; i++){
    //     for(int j = 0; j < M; j++){
    //         cout << arr[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    k = 0;
    for(int i = up_size; i < len-down_size; i++){
        for(int j = 0; j < M; j++){
            int lives = 0;
            for(int t = 0; t < 8; t++){
                if((0 <= i+dx[t]) &&(i+dx[t] < len) && (0 <= j+dy[t]) && (j+dy[t] < M)){
                    if(arr[i+dx[t]][j+dy[t]] == 1) lives++;
                }
            }

            if(arr[i][j] == 1){
                if((lives < 2) || (lives > 3)) ans[k] = 0;
                else ans[k] = 1;
            }
            else{
                if(lives == 3) ans[k] = 1;
                else ans[k] = 0; 
            }
            k++;
        }
    }
    free(arr);

    // cout << "rank: " << rank << endl;
    // for(int i = 0; i < nrows*M; i++){
    //     cout << ans[i] << " ";
    // }
    // cout << endl;
}

void copyArray(int grid[],int ans[],int nrows,int M){

    for(int x = 0; x < nrows*M; x++){
        grid[x] = ans[x];
    }

    return;
}

int main(int argc,char* argv[]){

    MPI_Init(&argc,&argv);
    
    MPI_Comm comm = MPI_COMM_WORLD;
    int size,rank;
    MPI_Comm_size(comm,&size);
    MPI_Comm_rank(comm,&rank);

    int N,M,T;
    if(rank == 0){
        int* grid;
        int* ans;
        cin >> N >> M >> T;
        grid = new int[N*M];
        ans = new int[N*M];

        for(int i = 0; i < N*M; i++){
            cin >> grid[i];
        }

        MPI_Bcast(&M,1,MPI_INT,0,comm);
        MPI_Bcast(&T,1,MPI_INT,0,comm);

        int nrows1 = N/size;
        int nrows2 = N-(size-1)*nrows1;
        MPI_Bcast(&nrows1,1,MPI_INT,0,comm);

        if((size != 1) && (nrows1 != 0)){
            int cur_index = 0;
            for(int i = 1; i < size; i++){
                // MPI_Send(&nrows1,1,MPI_INT,i,0,comm);
                MPI_Send(grid+nrows2*M+cur_index,(nrows1)*M,MPI_INT,i,0,comm);
                cur_index += nrows1*M;  
            }
        }
        // write some code
        // for(int i = 0; i < nrows2*M; i++) ans[i] = 2;
        
        int* down = new int[M];
        while(T){
            if((size != 1) && (nrows1 != 0)){
                // sends, receives
                // vector<int> down(M);
                MPI_Send(grid+nrows2*M-M,M,MPI_INT,rank+1,0,comm);
                MPI_Recv(down,M,MPI_INT,rank+1,0,comm,MPI_STATUS_IGNORE);

                check(grid,ans,nrows2,M,NULL,down,rank);
            }
            else{
                check(grid,ans,nrows2,M,NULL,NULL,rank);
            }
            copyArray(grid,ans,nrows2,M);
            // for(int i = 0; i < nrows2*M; i++){
            //     grid[i] = ans[i];
            // }
            T--;
        }
        free(down);

        if((size != 1) && (nrows1 != 0)){
            int cur_index = 0;
            for(int i = 1; i < size; i++){
                MPI_Recv(ans+nrows2*M+cur_index,nrows1*M,MPI_INT,i,0,comm,MPI_STATUS_IGNORE);
                cur_index += nrows1*M;    
            }
        }

        for(int i = 0; i < N*M; i++){
            grid[i] = ans[i];
        }

        // cout << "** Grid **" << endl;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                cout << grid[i * M + j] << " ";  // Access array elements accordingly
            }
            cout << endl;
        }

        delete[] ans;
        delete[] grid;
    }
    else{


        MPI_Bcast(&M,1,MPI_INT,0,comm);
        MPI_Bcast(&T,1,MPI_INT,0,comm);

        int nrows1;
        // MPI_Recv(&nrows1,1,MPI_INT,0,0,comm,MPI_STATUS_IGNORE);
        MPI_Bcast(&nrows1,1,MPI_INT,0,comm);

        if(nrows1 != 0){
            int* temp_grid;
            int* temp_ans;

            temp_grid = new int[(nrows1)*M];
            MPI_Recv(temp_grid,(nrows1)*M,MPI_INT,0,0,comm,MPI_STATUS_IGNORE);
            temp_ans = new int[nrows1*M];

            int* up;
            int* down;
            if(rank != size-1){
                up = new int[M];
                down = new int[M];
            }
            else{
                up = new int[M];
            }
            // write some code
            while(T){

                if(rank != size-1){
                    // 
                    MPI_Send(temp_grid,M,MPI_INT,rank-1,0,comm);
                    MPI_Send(temp_grid+nrows1*M-M,M,MPI_INT,rank+1,0,comm);

                    MPI_Recv(up,M,MPI_INT,rank-1,0,comm,MPI_STATUS_IGNORE);
                    MPI_Recv(down,M,MPI_INT,rank+1,0,comm,MPI_STATUS_IGNORE);

                    // cout << rank << " :up" << endl;
                    // for(int i = 0; i < M; i++){
                    //     cout << up[i] << " ";
                    // }
                    // cout << endl;

                    // cout << rank << " :down" << endl;
                    // for(int i = 0; i < M; i++){
                    //     cout << down[i] << " ";
                    // }
                    // cout << endl;

                    check(temp_grid,temp_ans,nrows1,M,up,down,rank);
                }
                else{
                    // vector<int> up(M);
                    MPI_Send(temp_grid,M,MPI_INT,rank-1,0,comm);
                    MPI_Recv(up,M,MPI_INT,rank-1,0,comm,MPI_STATUS_IGNORE);

                    check(temp_grid,temp_ans,nrows1,M,up,NULL,rank);
                }
                copyArray(temp_grid,temp_ans,nrows1,M);
                // for(int i = 0; i < nrows1*M; i++){
                //     temp_grid[i] = temp_ans[i];
                // }
                T--;
            }

            if(rank != size-1){
                free(up);
                free(down);
            }
            else{
                free(up);
            }

            // for(int i = 0; i < nrows1*M; i++) temp_ans[i] = 2;

            MPI_Send(temp_ans,nrows1*M,MPI_INT,0,0,comm);

            delete[] temp_ans;
            delete[] temp_grid;
        }
        


    }

    MPI_Finalize();
}