// RP_Greedy.hh
#ifndef _RP_GREEDY_HH_
#define _RP_GREEDY_HH_
#include "Router_placement_data.hh"

point MaxRouterDegree (const RP_Input& in, RP_Output& out);
bool IsFeasibleRouter (const RP_Input& in, RP_Output& out, point r);
bool PointIsInVec (point pt, vector<point> vec);
void PrintVec (vector<point> vec);
point MakeRandomMove(point p);
point SelectRandomRouter(const RP_Input& in, RP_Output& out);
unsigned SelectRouter(const RP_Input& in, RP_Output& out);
void ConnectRouterWithBackbone(RP_Output& out, const RP_Input& in, point router);
// void UpdateCoverage(RP_Output& out, const RP_Input& in, point router);
void GreedyRP(const RP_Input& in, RP_Output& out);


// class RP_GreedySolver
// {
// public:
//     ConnectRouterWithBackbone()
//     PlaceRandomizedRouter()
// private:
//     const RP_Input& in;
//     unsigned max_routers = in.Budget() / in.RouterPrice();
// }

#endif

