```QUESTION 1A:[MULTIBROT]```

TIME COMPLEXITY: O(K*log(D)*(N*M)/size), 
K = no.of iterations, log(D) -> pow()

MESSAGE COMPLEXITY: 
2*(size-1)*(N*M)/size for sending
(size-1)*(N*M)/size for receiving
Total = 3*(size-1)*(N*M)/size

SPACE COMPLEXITY: 
2*N*M for root process, 2(N*M)/size for remaining processes
Total = 2*N*M+(2*N*M)*(size-1)/size

size = number of processess

PERFORMANCE SCALING:
Considered Input as 16 16 4 2000
- 1 process  Total time (s): 7.227
- 2 process  Total time (s): 3.379
- 3 process  Total time (s): 1.781
- 4 process  Total time (s): 4.082
- 5 process  Total time (s): 1.894
- 6 process  Total time (s): 1.836
- 7 process  Total time (s): 3.422
- 8 process  Total time (s): 1.576
- 9 process  Total time (s): 3.377
- 10 process Total time (s): 1.562
- 11 process Total time (s): 3.409
- 12 process Total time (s): 1.198

QUESTION 2:[FLOYD_WARSHALL]

TIME COMPLEXITY: O(N*(N/size)*N)

MESSAGE COMPLEXITY:
(size-1)*N*N/size for sending
(size-1)*N*N/size for receiving
N*N for Broadcasting

SPACE COMPLEXITY:
2*N*N for root process, (N*(N/size)+N) for all processes
Total = 2*N*N +size*(N*(N/size)+N)


PERFORMANCE SCALING:
Considered Input for n = 40
- 1 process  Total time (s): 6.785
- 2 process  Total time (s): 3.369
- 3 process  Total time (s): 2.160
- 4 process  Total time (s): 2.391
- 5 process  Total time (s): 3.843
- 6 process  Total time (s): 3.363
- 7 process  Total time (s): 2.570
- 8 process  Total time (s): 2.246
- 9 process  Total time (s): 1.967
- 10 process Total time (s): 2.423
- 11 process Total time (s): 2.107
- 12 process Total time (s): 1.759

QUESTION 3:[GAME_OF_LIFE]

TIME COMPLEXITY: O(T*(N*M)/size)

MESSAGE COMPLEXITY:

(size-1)*(N*M)/size for sending
(size-1)*(N*M)/size for receiving
2*(size-1)*M*T for communication over the blocks

SPACE COMPLEXITY:
2*N*M+M for root process, N*M/size+2*M for processes with rank != size-1,  N*M/size+M for process with rank = size-1
Total =  (2*N*M+M)+(size-2)*(N*M/size+2*M)+(N*M/size+M)

PERFORMANCE SCALING:
Considered Input for 10 10 5000 
- 1 process  Total time (s): 2.524
- 2 process  Total time (s): 3.387
- 3 process  Total time (s): 3.405
- 4 process  Total time (s): 2.398
- 5 process  Total time (s): 4.695
- 6 process  Total time (s): 2.781
- 7 process  Total time (s): 1.968
- 8 process  Total time (s): 3.479
- 9 process  Total time (s): 3.020
- 10 process Total time (s): 3.985
- 11 process Total time (s): 4.870
- 12 process Total time (s): 3.284
