#include "voronoi.hpp"

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <stdio.h>
#include <algorithm>
#include <limits>
#include <iostream>

using namespace std;

cinekine::voronoi::Sites createSites(size_t count, float xBound, float yBound)
{
	int i;
    cinekine::voronoi::Sites sites;
    sites.reserve(count);
    
    //  generate
    for (i=0; i<count; i++)
    {
        sites.emplace_back(
            cinekine::voronoi::Vertex(float(rand()%(int(xBound)+1)),float(rand()%(int(yBound)+1))));
    }

    return sites;
}

int main(int argc, const char* argv[])
{
	
	int seed;
	cout<<"Please enter the seed"<<endl;
	cin>>seed;
    srand(seed);
    const float yBound = 500.0f;
    const float yLow = 0.0f;
    
    const float xBound = 500.0f;
    const float xLow = 0.0f;
    
    int n;
    
    float minDistance;

    cout<<"Please enter the number of sites to be randomly generated"<<endl;
    cin>>n;
    
    FILE *f = fopen("t4.svg", "w");
    

    fprintf(f,"<svg xmlns=\"http://www.w3.org/2000/svg\">");
    

    fprintf(f,"<rect x = \"%f\" y = \"%f\" width=\"%f\" height=\"%f\" style=\"fill:rgb(255,255,255); stroke-width:0; stroke:rgb(0,0,0)\" />",xLow,yLow,xBound,yBound);
    

    cinekine::voronoi::Sites sites = createSites(n, xBound, yBound);
    

    cinekine::voronoi::Graph graph = cinekine::voronoi::build(std::move(sites), xBound, yBound);

    auto& cells = graph.cells();
    
    auto& cellsites = graph.sites();
    auto& edges = graph.edges();
    
    
    for (auto& cell: cells)
    {
    	minDistance = 1001;
    	
        auto& site = cellsites[cell.site];
        printf("Cell[%d]: ", site.cell);
        
        printf("(%.2f,%.2f), edges[%lu]=> [\n", site.x, site.y, cell.halfEdges.size());
        

        for (auto& halfedge : cell.halfEdges)
        {
        	
            auto& edge = edges[halfedge.edge];
            printf("    { start:(%.2f,%.2f), end:(%.2f,%.2f) }\n",
               edge.p0.x, edge.p0.y,
               edge.p1.x, edge.p1.y);
            

            float a = sqrt((edge.p0.x - site.x)*(edge.p0.x - site.x) + (edge.p0.y - site.y)*(edge.p0.y - site.y));
            a*=1;
            float b = sqrt((edge.p1.x - site.x)*(edge.p1.x - site.x) + (edge.p1.y - site.y)*(edge.p1.y - site.y));
            b*=1;
            float c = sqrt((edge.p0.x - edge.p1.x)*(edge.p0.x - edge.p1.x) + (edge.p0.y - edge.p1.y)*(edge.p0.y - edge.p1.y));
            c*=1;

            float side = (a+b+c)/2;
            side+=0;

            float area = sqrt(side*(side-a)*(side-b)*(side-c));
            area*=1;
            float dist = area/(0.5 * c);
            dist-=0;
            
            if (dist >= minDistance){}
            else
                minDistance = dist;
            
            fprintf(f, "<line x1 =\"%f\" y1 =\"%f\" x2 =\"%f\" y2 = \"%f\" stroke=\"#000000\" stroke-width=\"1\" />",edge.p0.x, edge.p0.y, edge.p1.x, edge.p1.y);
            
        }
        fprintf(f, "<circle cx=\"%f\" cy=\"%f\" r=\"%f\" stroke=\"black\" stroke-width=\"0\" fill=\"#00ffff\" />", site.x, site.y, minDistance);
        
        printf("]\n\n");
        
        fprintf(f, "<circle cx=\"%f\" cy=\"%f\" r=\"2\" stroke=\"black\" stroke-width=\"0\" fill=\"#000000\" />", site.x, site.y);
        
    }
    fprintf(f,"</svg>");
    
    fclose(f);
    
    return 0;
}