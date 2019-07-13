
/*
The Below MATLAB implementation solves the objective function
of Multi-Level facility location problem and returns global minima
*/

fileIDD = fopen('./config.txt', 'r');
AA = textscan(fileIDD, '%f%f%f');
AA = cell2mat(AA);
final_result = [];
final_times = [];

amm = {'0', '1', '2'};

     for ii = 1:6
         for jj = 1:3
            for kk = 1:3
                for ll = 1:3

                    name = ['./input/', int2str(AA(ii,1)), '_', int2str(AA(ii,2)), '_', int2str(AA(ii,3)), '_'];
                    name = strcat(name, amm(jj));
                    name = strcat(name, amm(kk));
                    name = strcat(name, amm(ll));
                    name = strcat(name, '.txt') 

                    t = cputime;
                    fileID = fopen(char(name), 'r');
                    A = textscan(fileID, '%f');
                    A = cell2mat(A);

                    N = A(1, 1);
                    M1 = A(2, 1);
                    M2 = A(3, 1);

                    costList = [];
                    facCost1 =  A([4 : 3 + M1]) ;
                    facCost2 = A([4 + M1 : 3 + M1 + M2]) ;
                    connectionCost1 = A([4+ M1 + M2 : 3 + M1 + M2 + N*M1]);
                    connectionCost2 = A([4 + M1 + M2 + N*M1 : end]);

                    for l = 1 : N
                        costList = [costList, connectionCost2];
                    end

                    intcon = 1 : M1 + M2 + N*M1 + N*M1*M2;
                    objFunc = [facCost1, facCost2, connectionCost1, costList];

                    aNotEqual = zeros(M1*N + M2*N, M1 + M2 + N*M1 + N*M1*M2);
                    bNotEqual = zeros(M1*N + M2*N, 1);
                    aEqual = zeros(N + M1*N, M1 + M2 + N*M1 + N*M1*M2);
                    bEqual = zeros(N + M1*N, 1);
                    uBound = ones(1, M1 + M2 + N*M1 + N*M1*M2);
                    lBound = zeros(1, M1 + M2 + N*M1 + N*M1*M2);

                    for n = 1 : N
                        bEqual(n, 1) = 1;
                        for m = 1 : M1
                            aEqual(n, M1 + M2 + (n-1)*M1 + m) = 1;
                        end
                    end
                    i = N + 1;

                    for n = 1 : N
                        for m1 = 1 : M1

                            aEqual(i, M1 + M2 + (n-1)*M1 + m1) = -1;

                            for m2 = 1 : M2
                               aEqual(i, M1 + M2 + N*M1 + (n-1)*M1*M2 + (m1-1)*M2 + m2) = 1;
                            end
                            i = i + 1;

                        end
                    end

                    i = 1;
                    for n = 1 : N
                        for m1 = 1 : M1
                            aNotEqual(i, m1) = -1;
                            aNotEqual(i, M1 + M2 + (n-1)*M1 + m1) = 1;
                            i = i + 1;
                        end
                    end

                    for n = 1 : N
                        for m2 = 1 : M2
                            aNotEqual(i, M1 + m2) = -1;

                            for m1 = 1 : M1
                                aNotEqual(i, M1 + M2 + N*M1 + (n-1)*M1*M2 + (m1-1)*M2 + m2) = 1;
                            end
                            i = i + 1;
                        end
                    end

                    X = intlinprog(objFunc, intcon, aNotEqual, bNotEqual, aEqual, bEqual, lBound, uBound);
                    
                    openL1fac = X([1 : M1]);
                    openL2fac = X([M1+1 : M1 + M2]);
                    
                    facilityAssignL1 = reshape(X([(M1 + M2 +1) : (M1 + M2 + N*M1)]), [M1, N]);
                    facilityAssignL2 = reshape(X([M1 + M2 + N*M1 + 1: end]), [N,M1,M2]);
                    
                    cost = objFunc.*X;
                    p = sum(cost)
                    e = cputime-t
                    
                    final_result = [final_result; p];
                    final_times = [final_times; e];
                    
                    name=['./result/'];
                    name = strcat(name, amm(jj) );
                    name = strcat(name, amm(kk) );
                    name = strcat(name, amm(ll) );
                    name = strcat(name,'.txt')
                    fid = fopen(char(name), 'a+');

                    fprintf(fid, '%d %d\n', [p, e]);
                    fclose(fid);
                    
                end
            end
         end
     end