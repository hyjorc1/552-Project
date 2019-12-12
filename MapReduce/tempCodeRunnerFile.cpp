 // MapReduceConfig config;
    // vector<int> data;
    // for( int i = 1; i <= dataLength; i++ )
    //     data.push_back(i);
    // vector<vector<int>> chunkRanges = config.getChuckRanges(data, chunksLength);
    // int (*mapper)(int) = MyMapper;
    // int (*reducer)(int, int) = MyReducer;
    // vector<int> results(chunkRanges.size(), -1);
    // vector<int> *resPtr = &results;
    // taskqueue *tasks = setTaskQueue(&data, &chunkRanges, resPtr, mapper, reducer);

    // if (debug > 0)
    //     write_taskqueue(tasks);

    // printf("How many threads ? ");
    // int nthreads;
    // scanf("%d", &nthreads);
    // vector<int> done = process_taskqueue(tasks, nthreads);
    // printf("done %d tasks with result %d\n", done[0], done[1]);