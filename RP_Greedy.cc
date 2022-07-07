// RP_Greedy.cc 

#include "RP_Greedy.hh"
#include "Random.hh"
#include <algorithm>
#include <numeric>

using namespace std;


bool IsFeasibleRouter (const RP_Input& in, RP_Output& out, point r)
{
    return (in.IsDot(r.row, r.col) || (in.RouterPrice() + (ManhattanDistance(r, out.ClosestBackbonePoint(r)) * in.BackbonePrice()) >= out.RemainingBudget()));
}


point SelectRandomRouter(const RP_Input& in, RP_Output& out)
{
    point r; 

    do
    {
        r.row = Random(0, in.Rows()-1);
        r.col = Random(0, in.Columns()-1);
    } while (!IsFeasibleRouter(in, out, r));

    return r;
}


// unsigned PlaceRouter(const RP_Input& in, RP_Output& out)
// {
//     point current, selected;
//     vector<point> router_degree;

//     for (unsigned r = 0; r < in.Rows(); r++)
//     {
//         for (unsigned c = 0; c < in.Columns(); c++)
//         {
//             current.row = r;
//             current.col = c;
//             if (!in.IsWall(r,c) && !out.IsCovered(current))
//             {
//                 router_degree.push_back(current);
//             }
//         }
//     }

//     // for (unsigned r = 0; r < in.Rows(); r++)
//     // {
//     //     for (unsigned c = 0; c < in.Columns(); c++)
//     //     {
//     //         sort(router_degree.begin(), router_degree.end());
//     //     }
//     // }

//     if (router_degree.size() > 0)
//     {
//         // selected = router_degree[router_degree.size()-1];
//         selected.row = 3;
//         selected.col = 7;
//         out.InsertRouter(selected.row, selected.col);
//         cout << "Covered: " << out.TotalCoveredPoints() << endl;
//         ConnectRouterWithBackbone(out, in, selected);
//         return 1;
//     }

//     cout << "No router placed." << endl;
//     return 0;
// }


void ConnectRouterWithBackbone(RP_Output& out, const RP_Input& in, point router)
{
    point temp, selected_bb_cell;
    unsigned new_distance;

    // select the backbone point
    selected_bb_cell = out.ClosestBackbonePoint(router);
    // cout << "selectedbbcell: " << selected_bb_cell.row << " " << selected_bb_cell.col << endl;

    unsigned current_distance = ManhattanDistance(selected_bb_cell, router);

    while (current_distance > 0)
    {
        temp = MakeRandomMove(selected_bb_cell);
        new_distance = ManhattanDistance(temp, router);
        if (new_distance < current_distance)
        {
            current_distance = new_distance;
            out.InsertBackboneCell(temp.row, temp.col);
            selected_bb_cell = temp;
        }
    }
}


// move from p to one point of his neighborhood
point MakeRandomMove(point p) 
{  
    point p2;

    unsigned r_diff = Random(-1, 1);
    unsigned c_diff = Random (-1, 1);
    p2.row = p.row + r_diff;
    p2.col = p.col + c_diff;

    return p2;
}


// void GreedyRP(const RP_Input& in, RP_Output& out)
// { 
//     unsigned new_coverage;
//     point router;

 
//     new_coverage = PlaceRouter(in, out);

//     // cout << "Covered: " << endl;
//     // PrintVec(out.Covered());
//     cout << out;
//     cout << "Final Score: " << out.ComputeTotalScore() << endl;
//     cout << "Remaining budget: " << out.RemainingBudget() << endl;
// }


bool PointIsInVec (point pt, vector<point> vec) 
{
    for (unsigned i = 0; i < vec.size(); i++)
        if (pt.row == vec[i].row && pt.col == vec[i].col)
            return 1;

    return 0;
}


void PrintVec (vector<point> vec) 
{
    for (unsigned i = 0; i < vec.size(); i++)
    {
        cout << vec[i].row << " " << vec[i].col;
        if (i != vec.size()-1)
            cout << " -> ";
    }
        
    cout << endl;
}


unsigned SelectRouter(const RP_Input& in, RP_Output& out)
{
    point selected;

    selected = MaxRouterDegree (in, out);

    unsigned size = out.FindCoverageArea(selected).size();
    cout << "size: " << size << endl;

    out.InsertRouter(selected.row, selected.col);
    cout << "Covered: " << out.TotalCoveredPoints() << endl;

    out.UpdateUncovered();

    ConnectRouterWithBackbone(out, in, selected);

    return size;
}


void GreedyRP(const RP_Input& in, RP_Output& out)
{ 
    unsigned new_coverage;

    do
    {    
        if (out.RemainingBudget() >= int(in.RouterPrice()))
        {
            new_coverage = SelectRouter(in, out);
            // cout << "Remaining Budget: "  << out.RemainingBudget() << endl;

            // cout << "New Backbone:" << endl;
            // PrintVec(out.Backbone());
            // // cout << "Remaining Budget: "  << out.RemainingBudget() << endl;
        }
        else
        {
            cout << "No router placed." << endl;
            new_coverage = 0;
        }
    } while(new_coverage > 0);

    // cout << "Covered: " << endl;
    // PrintVec(out.Covered());
    cout << out;
    cout << "Final Score: " << out.ComputeTotalScore() << endl;
    cout << "Remaining budget: " << out.RemainingBudget() << endl;
}

point MaxRouterDegree (const RP_Input& in, RP_Output& out) 
{
    int max_degree = 0;
    int router_degree;
    point temp, max;

    for (int i = 0; i < out.UncoveredSize(); i++)
    {
        temp.row = out.Uncovered(i).row;
        temp.col = out.Uncovered(i).col;
        
        if (IsFeasibleRouter(in, out, temp))
        {
            router_degree = out.RouterScore(temp);
        }

        if (router_degree > max_degree)
        {
            max_degree = router_degree;
            max.row = temp.row;
            max.col = temp.col;
        }
    }
    return max;
}

