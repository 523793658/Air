/*
 *  Progressive Mesh type Polygon Reduction Algorithm
 *  by Stan Melax (c) 1998
 *  Permission to use any of this code wherever you want is granted..
 *  Although, please do acknowledge authorship if appropriate.
 *
 *  See the header file progmesh.h for a description of this module
 */

#include <stdio.h>  
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <windows.h>
#include <list>

#include "vector.h"
#include "progmesh.h"

/*
 *  For the polygon reduction algorithm we use data structures
 *  that contain a little bit more information than the usual
 *  indexed face set type of data structure.
 *  From a vertex we wish to be able to quickly get the
 *  neighboring faces and vertices.
 */
class Triangle;
class Vertex;
using namespace std;
class Triangle {
  public:
	Vertex *         vertex[3]; // the 3 points that make this tri
	Vector           normal;    // unit vector othogonal to this face
	                 Triangle(Vertex *v0,Vertex *v1,Vertex *v2);
	                 ~Triangle();
	void             ComputeNormal();
	void             ReplaceVertex(Vertex *vold,Vertex *vnew);
	int              HasVertex(Vertex *v);
};
class Vertex {
  public:
	Vector           position; // location of point in euclidean space
	int              id;       // place of vertex in original list
	list<Vertex *>   neighbor; // adjacent vertices
	list<Triangle *> face;     // adjacent triangles
	float            objdist;  // cached cost of collapsing edge
	Vertex *         collapse; // candidate vertex for collapse
	                 Vertex(Vector v,int _id);
	                 ~Vertex();
	void             RemoveIfNonNeighbor(Vertex *n);
};
list<Vertex *>   vertices;
list<Triangle *> triangles;


Triangle::Triangle(Vertex *v0,Vertex *v1,Vertex *v2){
	assert(v0!=v1 && v1!=v2 && v2!=v0);
	vertex[0]=v0;
	vertex[1]=v1;
	vertex[2]=v2;
	ComputeNormal();
	triangles.push_back(this);
	for(int i=0;i<3;i++) {
		vertex[i]->face.push_back(this);
		for(int j=0;j<3;j++) if(i!=j) {
			if (std::find(vertex[i]->neighbor.begin(), vertex[i]->neighbor.end(), vertex[j]) == vertex[i]->neighbor.end())
			{
				vertex[i]->neighbor.push_back(vertex[j]);
			}
		}
	}
}
Triangle::~Triangle(){
	int i;
	triangles.remove(this);
	for(i=0;i<3;i++) {
		if (vertex[i]) vertex[i]->face.remove(this);
	}
	for(i=0;i<3;i++) {
		int i2 = (i+1)%3;
		if(!vertex[i] || !vertex[i2]) continue;
		vertex[i]->RemoveIfNonNeighbor(vertex[i2]);
		vertex[i2]->RemoveIfNonNeighbor(vertex[i ]);
	}
}
int Triangle::HasVertex(Vertex *v) {
	return (v==vertex[0] ||v==vertex[1] || v==vertex[2]);
}
void Triangle::ComputeNormal(){
	Vector v0=vertex[0]->position;
	Vector v1=vertex[1]->position;
	Vector v2=vertex[2]->position;
	normal = (v1-v0)*(v2-v1);
	if(magnitude(normal)==0)return;
	normal = normalize(normal);
}
void Triangle::ReplaceVertex(Vertex *vold,Vertex *vnew) {
	assert(vold && vnew);
	assert(vold==vertex[0] || vold==vertex[1] || vold==vertex[2]);
	assert(vnew!=vertex[0] && vnew!=vertex[1] && vnew!=vertex[2]);
	if(vold==vertex[0]){
		vertex[0]=vnew;
	}
	else if(vold==vertex[1]){
		vertex[1]=vnew;
	}
	else {
		assert(vold==vertex[2]);
		vertex[2]=vnew;
	}
	int i;
	vold->face.remove(this);
	vnew->face.push_back(this);
	for(i=0;i<3;i++) {
		vold->RemoveIfNonNeighbor(vertex[i]);
		vertex[i]->RemoveIfNonNeighbor(vold);
	}
	for(i=0;i<3;i++) {
		for(int j=0;j<3;j++) if(i!=j) {
			if (std::find(vertex[i]->neighbor.begin(), vertex[i]->neighbor.end(), vertex[j]) == vertex[i]->neighbor.end())
			{
				vertex[i]->neighbor.push_back(vertex[j]);
			}
		}
	}
	ComputeNormal();
}

Vertex::Vertex(Vector v,int _id) {
	position =v;
	id=_id;
	vertices.push_back(this);
}

Vertex::~Vertex(){
	while(neighbor.size()) {
		list<Vertex*>::iterator it;
		it = neighbor.begin();
		(*it)->neighbor.remove(this);
		neighbor.remove(*it);
	}
	vertices.remove(this);
}
void Vertex::RemoveIfNonNeighbor(Vertex *n) {
	// removes n from neighbor list if n isn't a neighbor.
	if (std::find(neighbor.begin(), neighbor.end(), n) == neighbor.end()) return;
	for (auto it = face.begin(); it != face.end(); it++)
	{
		if ((*it)->HasVertex(n)){
			return;
		}
	}
	neighbor.remove(n);
}


float ComputeEdgeCollapseCost(Vertex *u,Vertex *v) {
	// if we collapse edge uv by moving u to v then how 
	// much different will the model change, i.e. how much "error".
	// Texture, vertex normal, and border vertex code was removed
	// to keep this demo as simple as possible.
	// The method of determining cost was designed in order 
	// to exploit small and coplanar regions for
	// effective polygon reduction.
	// Is is possible to add some checks here to see if "folds"
	// would be generated.  i.e. normal of a remaining face gets
	// flipped.  I never seemed to run into this problem and
	// therefore never added code to detect this case.
	int i;
	Vector dir = v->position - u->position;
	float edgelength = magnitude(dir);
	float curvature=0;
	dir = normalize(dir);
	float dirValue = 10000000;

	for (auto n : u->neighbor)
	{
		Vector dir1 = normalize(n->position - u->position);
		float d = (1 + (dir1 ^ dir)) / 2;
		dirValue = min(d, dirValue);
	}
	dirValue += 0.001;


	// find the "sides" triangles that are on the edge uv
	list<Triangle *> sides;
	for (auto it : u->face)
	{
		if (it->HasVertex(v))
		{
			sides.push_back(it);
		}
	}


	// use the triangle facing most away from the sides 
	// to determine our curvature term
	for (auto i : u->face)
	{
		float mincurv = 1;
		for (auto j : sides)
		{
			float dotprod = i->normal^ j->normal;
			mincurv = min(mincurv, (1 - dotprod) / 2.0f);
		}
		curvature = max(curvature, mincurv);
	}
	curvature += 0.001;
	// the more coplanar the lower the curvature term   
	return curvature * dirValue * edgelength;
}

void ComputeEdgeCostAtVertex(Vertex *v) {
	// compute the edge collapse cost for all edges that start
	// from vertex v.  Since we are only interested in reducing
	// the object by selecting the min cost edge at each step, we
	// only cache the cost of the least cost edge at this vertex
	// (in member variable collapse) as well as the value of the 
	// cost (in member variable objdist).
	if(v->neighbor.size()==0) {
		// v doesn't have neighbors so it costs nothing to collapse
		v->collapse=NULL;
		v->objdist=-0.01f;
		return;
	}
	v->objdist = 1000000;
	v->collapse=NULL;
	// search all neighboring edges for "least cost" edge
	for (auto i : v->neighbor)
	{
		float dist;
		dist = ComputeEdgeCollapseCost(v, i);
		if (dist < v->objdist)
		{
			v->collapse = i;
			v->objdist = dist;
		}
	}
}
void ComputeAllEdgeCollapseCosts() {
	// For all the edges, compute the difference it would make
	// to the model if it was collapsed.  The least of these
	// per vertex is cached in each vertex object.
	for (auto vertex : vertices)
	{
		ComputeEdgeCostAtVertex(vertex);
	}
}

void Collapse(Vertex *u,Vertex *v){
	// Collapse the edge uv by moving vertex u onto v
	// Actually remove tris on uv, then update tris that
	// have u to have v, and then remove u.
	if(!v) {
		// u is a vertex all by itself so just delete it
		delete u;
		return;
	}
	int i;
	list<Vertex *>tmp;
	// make tmp a list of all the neighbors of u
	for (auto vertex : u->neighbor)
	{
		tmp.push_back(vertex);
	}
	// delete triangles on edge uv:
	
	for (i = u->face.size() - 1; i >= 0; i--)
	{
		auto it = u->face.begin();
		advance(it, i);
		if ((*it)->HasVertex(v))
		{
			delete (*it);
		}
	}
	// update remaining triangles to have v instead of u
	for (i = u->face.size() -1; i >= 0; i--)
	{
		auto it = u->face.begin();
		advance(it, i);
		(*it)->ReplaceVertex(u, v);
	}
	delete u; 
	// recompute the edge collapse costs for neighboring vertices
	for (auto it : tmp)
	{
		ComputeEdgeCostAtVertex(it);
	}
}

void AddVertex(list<Vector> &vert){
	int i = 0;
	for (auto it : vert)
	{
		Vertex *v = new Vertex(it, i);
		i++;
	}
}
void AddFaces(list<tridata> &tri){
	for (auto tridatadata : tri)
	{
		auto v0 = vertices.begin();
		advance(v0, tridatadata.v[0]);
		auto v1 = vertices.begin();
		advance(v1, tridatadata.v[1]);
		auto v2 = vertices.begin();
		advance(v2, tridatadata.v[2]);

		Triangle *t = new Triangle(*v0, *v1, *v2);
	}
}

Vertex *MinimumCostEdge(){
	// Find the edge that when collapsed will affect model the least.
	// This funtion actually returns a Vertex, the second vertex
	// of the edge (collapse candidate) is stored in the vertex data.
	// Serious optimization opportunity here: this function currently
	// does a sequential search through an unsorted list :-(
	// Our algorithm could be O(n*lg(n)) instead of O(n*n)
	Vertex *mn = *vertices.begin();
	for (auto vertex : vertices)
	{
		if (vertex->objdist < mn->objdist)
		{
			mn = vertex;
		}
	}
	return mn;
}

void ProgressiveMesh(List<Vector> &vert, List<tridata> &tri, 
                     List<int> &map, List<int> &permutation)
{
	list<Vector> vert1;
	list<tridata> tri1;
	for (int i = 0; i < vert.num; i++)
	{
		vert1.push_back(vert[i]);
	}
	for (int i = 0; i < tri.num; i++)
	{
		tri1.push_back(tri[i]);
	}

	AddVertex(vert1);  // put input data into our data structures
	AddFaces(tri1);
	ComputeAllEdgeCollapseCosts(); // cache all edge collapse costs
	permutation.SetSize(vertices.size());  // allocate space
	map.SetSize(vertices.size());          // allocate space
	// reduce the object down to nothing:
	while(vertices.size() > 0) {
		// get the next vertex to collapse
		Vertex *mn = MinimumCostEdge();
		// keep track of this vertex, i.e. the collapse ordering
		permutation[mn->id]=vertices.size()-1;
		// keep track of vertex to which we collapse to
		map[vertices.size()-1] = (mn->collapse)?mn->collapse->id:-1;
		// Collapse this edge
		Collapse(mn,mn->collapse);
	}
	// reorder the map list based on the collapse ordering
	for(int i=0;i<map.num;i++) {
		map[i] = (map[i]==-1)?0:permutation[map[i]];
	}
	// The caller of this function should reorder their vertices
	// according to the returned "permutation".
}

