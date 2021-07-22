# SPM-Graph Search

In order to compile and run the solution follow the next steps:

1. Compile the graph generator using the makefile in the graph directory:

    ```bash
    make graph_generator_er
    ```

2. Generate the graphs

    ```bash
    ./graph_generator_er <no_nodes> <density>
    ```

    For instance:

    ```bash
    ./graph_generator_er 10000 0.5
    ```

    The generation can imply more or less time based on the number of nodes and density.

3. Generated the graph let's compile and run the solutions code. 

    **For graph_filename means just the filename, like graph_file1000.txt;**

    For the sequential solution, go in the BFS_seq directory and run:

    ```bash
    make BFS_seq
    ./BFS_seq <starter_node_id> <value> <graph_filename>
    ```

    For the parallel static solution, go in the BFS_par directory and run:

    ```bash
    make BFS_par_static
    ./BFS_par_static <starter_node_id> <value> <nw> <graph_filename>
    ```

    For the parallel dynamic solution, go in the BFS_par directory and run:

    **The chunk size must be an integer**.

    ```bash
    make BFS_par_dy
    ./BFS_par_dy <starter_node_id> <value> <nw> <chunk_size> <graph_filename>
    ```

    For the Fast Flow solution, go in the BFS_ff directory and run:

    ```bash
    make BFS_ff
    ./BFS_ff <starter_node_id> <value> <nw> <graph_filename>
    ```

    ---

    For all the three C++ pthread solution there is the possibility to run with the code multiple times with a max number of worker or with timer for each step of the execution. 

    Its enough to execute the make command with the "_mul" or "_timer":

    ```bash
    make BFS_par_static_mul
    ./BFS_par_static <starter_node_id> <value> <max_workers> <graph_filename>
    ```

    Output:

    ```bash
    1 <time_1_worker>
    2 <time_2_worker>
    .
    .
    .
    max_workers <time_nw th_worker >
    ```

    or also:

    ```bash
    make BFS_par_static_timer
    ./BFS_par_static <starter_node_id> <value> <nw> <graph_filename>
    ```