#include "Router_placement_data.hh"
#include "RP_Greedy.hh"

int main (int argc, char* argv[])
{
    string instance;
    if (argc == 1)
    {
        cout << "Name of the file containing the instance: ";
        cin >> instance;
    }
    else if (argc == 2)
        instance = argv[1];
    else
    {
        cerr << "Usage: " << argv[0] << " [input_file]" << endl;
        exit(1);
    }

    RP_Input in(instance);
    // cout << in;

    RP_Output out(in);
    GreedyRP(in, out);

    return 0;
}