#pragma once
#include<ios>
#include <string>
#include<vector>
#include"h2bParser.h"
#include "Gateware.h"

#define GATEWARE_ENABLE_MATH 



class Model
{
private:

	

public:

	
	std::string ObjectName; // name of a mesh
	std::vector<GW::MATH::GMATRIXF> World; // world matrix of a mesh
	unsigned int NumInstaces = 1; // Number of a instaces of a model
	unsigned int VertexBase = 0;
	unsigned int IndicesBase = 0;
	std::vector<H2B::MATERIAL> Mats; // material of a object 
	std::vector<H2B::MESH> Meshes; // meshes of that object




};


class Level_Data
{
private:



public:


	std::vector<H2B::VERTEX> Vertices; // Vertex of a object 
	std::vector<unsigned> Indices; // Indices of that object
	std::vector<H2B::MATERIAL> Mats; // material of a object 
	std::vector<H2B::MESH> Meshes; // meshes of that object
	std::vector<GW::MATH::GMATRIXF> World; // world matrix of a mesh


};
