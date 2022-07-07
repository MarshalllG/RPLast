//  Router_placement_data.cpp

#include "Router_placement_data.hh"

unsigned ManhattanDistance(point p1, point p2)
{
    return (abs(int(p1.row - p2.row)) + abs(int(p1.col - p2.col)));
}

unsigned InfDistance (point p1, point p2)
{
    return max(abs(p1.row - p2.row), abs(p1.col - p2.col));
}

bool ThereAreWallsInside (const RP_Input& in, point p1, point p2)
{
    for (unsigned r=min(p1.row, p2.row); r <= max (p1.row, p2.row); r++)
    {
        for (unsigned c=min(p1.col, p2.col); c <= max (p1.col, p2.col); c++)
        {
            if (in.IsWall(r,c))
                return 1;
        }
    }
    return 0;
}


RP_Input::RP_Input(string file_name)
{  
    char ch;

    ifstream is(file_name);
    if (!is) 
    {
        cerr << "Cannot open file " <<  file_name << endl;
	    exit(1);
    }

    is >> rows >> columns >> radius
       >> b_price >> r_price >> budget
       >> start_backbone.row >> start_backbone.col;

    cells.resize(rows, vector<char>(columns));

    for (unsigned r = 0; r < rows; r++)
    {
        for (unsigned c = 0; c < columns; c++)
        {
            is >> ch;
            cells[r][c] = ch;
        }
    }
}


ostream& operator<<(ostream& os, const RP_Input& in)
{
    for (unsigned r = 0; r < in.rows; r++)
    {
        for (unsigned c = 0; c < in.columns; c++)
        {
            os << in.cells[r][c];
        }
        cout << endl;
    }
    return os;
}


RP_Output::RP_Output(const RP_Input& my_in) : in(my_in) 
{
    out_cells = in.Cells(); // initialize grid

    backbone.push_back(in.StartBackbone()); // initialize backbone
    out_cells[in.StartBackbone().row][in.StartBackbone().col] = 'b';
    remaining_budget = in.Budget()-in.BackbonePrice();

    total_covered_points = 0;

    point temp;
    for (int r=0; r < in.Rows(); r++)
    {
        for (int c=0; c < in.Columns(); c++)
        {
            if(in.IsDot(r,c))
            {
                temp.row = r;
                temp.col = c;
                uncovered.push_back(temp);
            }
        }
    }
}


void RP_Output::InsertRouter(unsigned r, unsigned c)
{
    point router;
    vector<point> router_coverage;

    if (!in.IsWall(r, c))
    {
        // update cell
        out_cells[r][c] = 'r';

        router.row = r;
        router.col = c;
    
        routers.push_back(router); // update routers vector
        cout << "Placed router in cell " << router.row << " " << router.col << endl;

        // update coverage
        router_coverage = FindCoverageArea(router); 
        cout << "Il router copre " << router_coverage.size() << " celle." << endl;
        total_covered_points += router_coverage.size();
        for (unsigned i = 0; i < router_coverage.size(); i++)
        {
            if (out_cells[router_coverage[i].row][router_coverage[i].col] != 'r')
                out_cells[router_coverage[i].row][router_coverage[i].col] = '1';
        }
            
            
        // update budget
        remaining_budget -= in.RouterPrice();
    }
}


void RP_Output::InsertBackboneCell(unsigned r, unsigned c)
{
    point bb_cell;
    bb_cell.row = r;
    bb_cell.col = c;

    if (!IsInBackbone(bb_cell))
        backbone.push_back(bb_cell);

    // update budget
    remaining_budget -= in.BackbonePrice();
}


bool RP_Output::IsCovered(point pt) const
{
    vector<point> router_coverage;

    if (RoutersSize() == 0)
        return 0;

    for (unsigned i = 0; i < RoutersSize(); i++)
    {
        router_coverage = FindCoverageArea(routers[i]);
        for (unsigned j = 0; j < router_coverage.size(); j++)
        {
            if (pt.row == router_coverage[j].row && pt.col == router_coverage[j].col)
            { 
                return 1;
            }
        }
    }
    return 0;
}


// dato un punto, restituisce che celle coprirebbe se fosse un router.
vector<point> RP_Output::FindCoverageArea(point router) const
{
    point temp;
    unsigned step = 1;
    vector<point> coverage;

    while (step <= in.Radius())
    {
        for (int r=router.row-step; r <= router.row+step; r++)
        {
            for (int c=router.col-step; c <= router.col+step; c++)
            {
                if (in.IsGridPoint(r,c))
                {
                    temp.row = r;
                    temp.col = c;
                    if (out_cells[r][c] == '.' && !ThereAreWallsInside(in, temp, router))
                    {
                        if (InfDistance(temp, router) == step)
                            coverage.push_back(temp);
                    }
                }
            }
        }
        step++;
    } 
    return coverage;
}


// void RP_Output::RemoveRouter(point router)
// {
//     out_cells[router.row][router.col] = in.cells[router.row][router.col];
//     routers.delete()
//     out.remaining_budget += in.RouterPrice();
// }

 
// void RP_Output::RemoveBackboneCell (point bb_cell)
// {
//     backbone.delete()
//     out.remaining_budget += in.BackbonePrice();
// }


ostream& operator<<(ostream& os, const RP_Output& out)
{
    for (unsigned r = 0; r < out.Rows(); r++)
    {
        for (unsigned c = 0; c < out.Columns(); c++)
        {
            os << out.out_cells[r][c];
        }
        cout << endl;
    }
    return os;
}


bool RP_Output::IsInBackbone (point pt) const
{
    for (unsigned i = 0; i < backbone.size(); i++)
        if (pt.row == backbone[i].row && pt.col == backbone[i].col)
            return 1;

    return 0;
}


point RP_Output::ClosestBackbonePoint (point router) const
{
    unsigned minimum_distance = ManhattanDistance(in.StartBackbone(), router);
    point closest = in.StartBackbone();
    unsigned current_distance;

    for (unsigned i = 0; i < BackboneSize(); i++)
    {
        current_distance = ManhattanDistance(BackboneCell(i), router);
        if (current_distance < minimum_distance)
        {
            closest = BackboneCell(i);
            minimum_distance = current_distance;
        }
    }
    
    return closest;
}

int RP_Output::RouterScore (point router) const
{
    return (1000*FindCoverageArea(router).size()-(ManhattanDistance(router, ClosestBackbonePoint(router)) * in.BackbonePrice()));
}

void RP_Output::UpdateUncovered()
{
    int i;
    while (i < uncovered.size())
    {
        if (out_cells[uncovered[i].row][uncovered[i].col] == '1' || out_cells[uncovered[i].row][uncovered[i].col] == 'r')
            uncovered.erase(uncovered.begin()+i);
        else
            i++;
    }
}