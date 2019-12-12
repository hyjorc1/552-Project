debug > 0)
        write_taskqueue(tasks);

    printf("How many threads ? ");
    int nthreads;
    scanf("%d", &nthreads);
    vector<int> done = process_taskqueue(tasks, nthreads);
    printf("done %d tasks with result %d\n", done[0], done[1]);