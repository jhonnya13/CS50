{
    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            int current_index = preferences[i][j];
            if (candidates[current_index].eliminated == false)
            {
                candidates[current_index].votes++;
                break;
            }
        }
    }
}