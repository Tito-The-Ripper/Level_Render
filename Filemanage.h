#pragma once

#include <fstream>
#include<ios>
#include <string>
#include <sstream>
#include <algorithm>
#include<vector>
#include "Model.h"




std::vector<Model> ModelContainer;
Level_Data Level;

void FileParse()
{
		
	std::ifstream BlenderFile;
	std::stringstream Temp;
	std::string PlaceHolder;
	std::string  Deli = "(<>) ";
	std::string  num = "_0123456789";
	Model objects;
	H2B::Parser ParseModel;
	GW::MATH::GMATRIXF MatrixTemp;
	int counter = 0; int Incream = 0;
	float TotalMatrix [16];
	BlenderFile.open("../GameLevel.txt", std::ios::app);

	if (!BlenderFile.is_open())
		return;

		
	if (BlenderFile.is_open())
	{
		while (std::getline(BlenderFile, PlaceHolder).good())
		{
			if (!std::strcmp(PlaceHolder.c_str(), "MESH"))
			{
				std::getline(BlenderFile, PlaceHolder);

				if (PlaceHolder.find(num) != std::string::npos)
				{
					for (char R : num)
					{
						PlaceHolder.erase(std::remove(PlaceHolder.begin(), PlaceHolder.end(), R), PlaceHolder.end());
					}
				}

				objects.ObjectName = "../Meshs/" + PlaceHolder + ".h2b"; // Go get Name of object stuff

				counter = 0; int counter0 = 0; int counter1 = 1; int counter2 = 2; int counter3 = 3;

				while (std::getline(BlenderFile, PlaceHolder).good()) // Go get matrix stuff
				{
				
					if (PlaceHolder.find("Matrix 4x4") != std::string::npos)
					{
						std::string removeWord = "<Matrix 4x4";

						PlaceHolder.erase(0, removeWord.length());
					}
					
					for (char D : Deli)
					{
						PlaceHolder.erase(std::remove(PlaceHolder.begin(), PlaceHolder.end(), D), PlaceHolder.end());
					}

					std::sscanf(PlaceHolder.c_str(), "%F,%F,%F,%F", 
						&TotalMatrix[counter0],&TotalMatrix[counter1],&TotalMatrix[counter2],&TotalMatrix[counter3]);  
					
					
					counter++;  counter0 += 4;  counter1 += 4;  counter2 += 4; counter3 += 4;

					if (counter == 4)
					{
						
						for (int i = 0; i <= 15; i++)
						{
							MatrixTemp.data[i] = TotalMatrix[i];
						}

						objects.World.push_back(MatrixTemp);
						break;
					}
				
				}

				ParseModel.Parse(objects.ObjectName.c_str());

				for (int i = 0; i < ParseModel.materialCount; i++)
				{
					objects.Mats.push_back(ParseModel.materials[i]);
				}

				for (int i = 0; i < ParseModel.meshCount; i++)
				{
					objects.Meshes.push_back(ParseModel.meshes[i]);
				}

				for (int i = 0; i < ParseModel.vertexCount; i++)
				{
					Level.Vertices.push_back(ParseModel.vertices[i]);
				}

				for (int i = 0; i < ParseModel.indexCount; i++)
				{
					Level.Indices.push_back(ParseModel.indices[i]);
				}

				int Dup = 0;
				

				for (int  i = 0; i < ModelContainer.size(); i++)
				{
					if (objects.ObjectName == ModelContainer[i].ObjectName)
					{
						

						ModelContainer[i].World.push_back(objects.World[Incream]);
						ModelContainer.data()->NumInstaces++;
						Incream++;
						Dup++;
					}
				}
				
				if (Dup == 0)
				{
					ModelContainer.push_back(objects);
					ModelContainer.data()->NumInstaces++;
					
				}
				

				objects.World.erase(objects.World.begin());
				objects.World.shrink_to_fit();

				objects.Mats.erase(objects.Mats.begin());
				objects.Mats.shrink_to_fit();

				objects.Meshes.erase(objects.Meshes.begin());
				objects.Meshes.shrink_to_fit();

		
	
			}
		}

		BlenderFile.close();
	}
	
}

std::string ShaderAsString(const char* shaderFilePath) {
	std::string output;
	unsigned int stringLength = 0;
	GW::SYSTEM::GFile file; file.Create();
	file.GetFileSize(shaderFilePath, stringLength);
	if (stringLength && +file.OpenBinaryRead(shaderFilePath)) {
		output.resize(stringLength);
		file.Read(&output[0], stringLength);
	}
	else
		std::cout << "ERROR: Shader Source File \"" << shaderFilePath << "\" Not Found!" << std::endl;
	return output;
}


