#pragma once

#include <fstream>
#include<ios>
#include <string>
#include <sstream>
#include <algorithm>
#include<vector>
#include "Model.h"


bool SwitchTxtFile = false;
std::string FileTxTSwitchHolder;
std::vector<Model> ModelContainer;
Level_Data Level;
Model camera;
int keystates = 1;


void FileParse()
{
	int InstaceCouter = 0;
	std::ifstream BlenderFile;
	std::stringstream Temp;
	std::string PlaceHolder;
	std::string  Deli = "(<>) ";
	std::string  num = ".0123456789";
	Model objects;
	H2B::Parser ParseModel;
	GW::MATH::GMATRIXF MatrixTemp;
	int counter = 0; int Incream = 0;
	float TotalMatrix [16];

	

	std::string SwitchTxt;

	if (SwitchTxtFile == false)
	{
		SwitchTxt = "../ObjectMatrix/GameLevel.txt";
	}
	else
	{
		SwitchTxt = FileTxTSwitchHolder;
		
		ModelContainer.clear();
		ModelContainer.shrink_to_fit();

		Level.Indices.clear();
		ModelContainer.shrink_to_fit();

		Level.Indices.clear(); Level.Indices.shrink_to_fit();

		Level.Mats.clear();	  Level.Mats.shrink_to_fit();

		Level.Meshes.clear(); Level.Meshes.shrink_to_fit();

		Level.Vertices.clear();	Level.Vertices.shrink_to_fit();

		Level.World.clear();  Level.World.shrink_to_fit();

		camera.World.clear(); camera.World.shrink_to_fit();
	}



	BlenderFile.open(SwitchTxt, std::ios::app);

	if (!BlenderFile.is_open())
		return;

		
	if (BlenderFile.is_open())
	{
		while (std::getline(BlenderFile, PlaceHolder).good())
		{
			if (!std::strcmp(PlaceHolder.c_str(), "MESH"))
			{
				std::getline(BlenderFile, PlaceHolder);

				if (PlaceHolder.find(".") != std::string::npos)
				{
					
					size_t IndexName = PlaceHolder.find_last_of(".");
					PlaceHolder = PlaceHolder.substr(0, IndexName);
					
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
						Level.World.push_back(MatrixTemp);
						break;
					}
				
				}

				ParseModel.Parse(objects.ObjectName.c_str());
				int Dup = 0;
			
				for (int i = 0; i < ModelContainer.size(); i++)
				{
					if (objects.ObjectName == ModelContainer[i].ObjectName)
					{

						Dup++;
					}
				}

				if (Dup == 0)
				{


					for (int i = 0; i < ParseModel.materialCount; i++)
					{
						objects.Mats.push_back(ParseModel.materials[i]);
						Level.Mats.push_back(ParseModel.materials[i]);

					}

					objects.VertexBase = Level.Vertices.size();
					for (int i = 0; i < ParseModel.vertexCount; i++)
					{
						Level.Vertices.push_back(ParseModel.vertices[i]);
					}

					objects.IndicesBase = Level.Indices.size();
					for (int i = 0; i < ParseModel.indexCount; i++)
					{
						Level.Indices.push_back(ParseModel.indices[i]);
					}

					objects.NumberofMats = Level.Meshes.size();
					for (int i = 0; i < ParseModel.meshCount; i++)
					{
						objects.Meshes.push_back(ParseModel.meshes[i]);
						objects.Meshes[i].drawInfo.indexOffset += objects.IndicesBase;
						objects.Meshes[i].materialIndex += objects.NumberofMats;
						Level.Meshes.push_back(ParseModel.meshes[i]);

					}
					
					
					ModelContainer.push_back(objects);
				}
				else
				{
					for (int  i = 0; i < ModelContainer.size(); i++)
					{
						if (objects.ObjectName == ModelContainer[i].ObjectName)
						{
						

							ModelContainer[i].World.push_back(objects.World[0]);
							ModelContainer[i].NumInstaces++;
							
						
						}
					}
				}
				

				objects.World.clear();
				objects.World.shrink_to_fit();

				objects.Mats.clear();
				objects.Mats.shrink_to_fit();

				objects.Meshes.clear();
				objects.Meshes.shrink_to_fit();

		
	
			}

			if (!std::strcmp(PlaceHolder.c_str(), "CAMERA"))
			{
				std::getline(BlenderFile, PlaceHolder);


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
						&TotalMatrix[counter0], &TotalMatrix[counter1], &TotalMatrix[counter2], &TotalMatrix[counter3]);


					counter++;  counter0 += 4;  counter1 += 4;  counter2 += 4; counter3 += 4;

					if (counter == 4)
					{

						for (int i = 0; i <= 15; i++)
						{
							MatrixTemp.data[i] = TotalMatrix[i];
						}

						camera.World.push_back(MatrixTemp);
						
						break;
					}
				}


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


