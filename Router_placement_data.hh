//  Router_placement_data.hpp

#ifndef Router_placement_data_hh
#define Router_placement_data_hh

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

struct point
{
    int row, col; 
};

unsigned ManhattanDistance(point p1, point p2);

class RP_Input
{
    friend ostream& operator<<(ostream& os, const RP_Input& in);
public:
    RP_Input (string file_name);
    vector<vector<char> > Cells() const { return cells; }
    char Cell(unsigned r, unsigned c) const {return cells[r][c]; }
    unsigned Rows() const { return rows; }
    unsigned Columns() const { return columns; }
    unsigned Radius() const { return radius; }
    unsigned BackbonePrice() const { return b_price; }
    unsigned RouterPrice() const { return r_price; }
    unsigned Budget() const { return budget; }
    point StartBackbone() const { return start_backbone; }
    bool IsGridPoint(int r, int c) const { return r >= 0 && r < rows && c >= 0 && c < columns; }
    bool IsWall (int r, int c) const { return cells[r][c] == '#'; }
    bool IsDot (int r, int c) const { return cells[r][c] == '.'; }
    bool IsLine (int r, int c) const { return cells[r][c] == '-'; }
private:
    vector<vector<char> > cells;
    unsigned rows, columns; // dimensioni esterne
    unsigned radius; // of router range
    unsigned b_price; // of connecting one single cell to the backbone
    unsigned r_price; // of one wireless router
    unsigned budget; // maximum budget
    point start_backbone; // coordinates
};

class RP_Output
{
    friend ostream& operator<<(ostream& os, const RP_Output& out);
    // friend istream& operator>>(istream& is, BDS_Output& out);
public:
    RP_Output(const RP_Input& in);
    int RemainingBudget() const { return remaining_budget; }
    unsigned Rows() const { return in.Rows(); }
    unsigned Columns() const { return in.Columns(); }
    unsigned TotalCoveredPoints() const { return total_covered_points; }
    unsigned BackboneSize() const { return backbone.size(); }
    unsigned RoutersSize() const { return routers.size(); }
    int UncoveredSize() const { return uncovered.size(); }
    point Uncovered(unsigned index) const { return uncovered[index]; }
    vector<point> Backbone() const { return backbone; }
    vector<point> Routers() const { return routers; }
    point BackboneCell(unsigned i) const { return backbone[i]; }
    point Router(unsigned i) const { return routers[i]; }
    bool IsInBackbone (point pt) const;
    bool IsCovered(point pt) const;
    point ClosestBackbonePoint (point router) const;
    void UpdateUncovered();
    void InsertRouter(unsigned r, unsigned c);
    void InsertBackboneCell(unsigned r, unsigned c);
    vector<point> FindCoverageArea(point pt) const;
    int RouterScore (point router) const;
    int ComputeTotalScore() const { return 1000*TotalCoveredPoints()+(in.Budget()-(backbone.size()*in.BackbonePrice()+routers.size()*in.RouterPrice())); }
    // RemoveRouter
    // RemoveBackbone
private:
    const RP_Input& in;
    vector<vector<char> > out_cells;
    vector<point> routers, backbone;
    vector<point> uncovered;
    vector<vector<point> > covered;
    unsigned total_covered_points;
    int remaining_budget;
};


#endif /* Router_placement_data_hh */


