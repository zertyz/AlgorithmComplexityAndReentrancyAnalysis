# AlgorithmComplexityAndReentrancyAnalysis
Measures the complexity of (database) algorithms AND Tests the reentrancy of such algorithms.

Complexity Analysis:                                                                                                                   
===================                                                                                                                    
                                                                                                                                       
Methodology: We will insert, update, select and delete in two passes, using the same amount of items each time.                        
             The algorithm complexity is measured on the time difference it takes to insert, update, select                            
             or delete each element on each pass. A little on Algorithm Complexity Analysis theory: Being n the                        
             number of elements on which the algorithm under analysis will work on, the algorithm is said to be:                       
             - O(1) complexity -- the operation times are independent on the number of elements already present;                       
             - O(n) complexity -- the operation times are linearly dependent on the number of elements;                                
             - O(log(n)) complexity -- operation duration times with log2(n) (base 2) proportionality.                                 
             - O(n*log(n)) complexity -- operation duration times with n * log2(n) (base 2) proportionality;                           
             - O(n^2) complexity -- the operation times are quadratically dependent on the number of elements;                         
Notes:                                                                                                                                 
 - We seek to build algorithms (and database queries) that are have O(1) complexity -- meaning the performance                         
   will not deteriorate over time, when the number of elements gets bigger and bigger;                                                 
 - This class will not test specifically for O(n*log(n)) and O(n^2), for they are so undesirable that we assume they                   
   will could only occur during the development phase. If those cases happen, we will only say "greater than O(log(n))".               
                                                                                                                                       
That being said, we're lead to the following usable formulas:                                                                          
                                                                                                                                       
  Variables:                                                                                                                           
    t(1)    -- time to insert / update / select / delete one element                                                                   
    t<z>(1) -- time measured on pass z (remember we only do two passes)                                                                
                                                                                                                                       
Inserts and Deletes of n elements (p threads):                                                                                         
O(1)       when  t2(1)/t1(1) / 1 ~= 1                                                                                                  
O(n)       when  t2(1)/t1(1) / 3 ~= 1                                                                                                  
O(log(n))  when  t2(1)/t1(1) / log(n*3)/log(n) ~= 1 +/- 10%                                                                            
                                                                                                                                       
Updates and Selects:                                                                                                                   
O(1)      when  t2 / t1                       ~= 1 for any p                                                                           
O(n)      when  t2 / t1  /  (n2 / n1)         ~= 1 for any p                                                                           
O(log n)  when  t2 / t1  /  log(n2) / log(n1) ~= 1 for any p                                                                           
                                                                                                                                       
                                                                                                                                       
Reentrancy Analysis:                                                                                                                   
===================                                                                                                                    
                                                                                                                                       
Methodology: We will insert, update, select and delete (all at the same time) the same number of elements and                          
             by the same routines as in the algorithm complexity analysis phase, with the following constraints:                       
             - All operations are performed by their own threads -- therefore, we'll have multiples of 4 threads;                      
             - Insertions occur freely, at the maximum speed;                                                                          
             - Selections occur only for already inserted elements;                                                                    
             - Updates occur only for already selected elements;                                                                       
             - Deletions occur only for already updated elements.                                                                      
Notes:                                                                                                                                 
 - The operation functions must test the data -- selects must check inserts, deletes must check updates. You may                       
   either print or throw an exception in case of errors;                                                                               
