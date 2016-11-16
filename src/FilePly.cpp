/*************************************************************************
	> File Name: FilePly.cpp
	> Author: wangl
	> Mail: feixiaku@gmail.com
	> Created Time: Tue Nov 15 10:32:39 2016
 ************************************************************************/

#include "FilePly.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

//Read PLY File
bool ReadPly( Mesh& mesh, const std::string& file_name)
{
    std::ifstream file;
    std::string line, new_line;
    std::string word;
    int pos;
    int format;
    int vertexNum;
    int faceNum = 0;
    Vector3d vec3d;
    Vector3i vec3i;

    file.open(file_name.c_str(),  ios::in);
    if(file.is_open() == false)
    {
        std::cerr << "cann't open the ply file" << std::endl;
        return false;
    }

    //read ply header
    std::getline(file, line);
    
    // Get First Line
    pos = line.find("ply");
    if(pos > line.length())
    {
        std::cerr << std::endl << "File Header do not have 'ply'" << std::endl;
        return false;
    }
    
    // format
    line.erase(line.begin(), line.end());
    std::getline(file, line);

    if(line.find("ascii") != line.npos)
    {
        format = 0;   
    }
    
    if(line.find("binary_little_endian") != line.npos)
    {
        format = 1;     
    }

    if(line.find("binary_big_endian") != line.npos)
    {
        format = 2;     
    }

    int times = 0;
    //read ply header
    while(std::getline(file, line))
    {
        //cout << "ply line: " << line << endl;
        istringstream iss(line);

        // end_header
        if(line.find("end_header") != line.npos)
        {
            break;
        }

        // vertex number
        if(line.find("vertex") != line.npos && line.find("vertex_indices") == line.npos)
        {
            while(iss >> word)
            {
                if(times == 2) 
                {
                    vertexNum = atoi(word.c_str());
                }
                times++;
            }
            times = 0;
        }

        //face Number 
        if(line.find("face") != line.npos)
        {
            while(iss >> word)
            {
                if(times == 2) 
                {
                    faceNum = atoi(word.c_str());
                }
                times++;
            }
            times = 0;
        }

    }//end of while

    cout << endl;
    cout << "ply format: " << format << endl;
    cout << "vertex numver:" << vertexNum << endl;
    cout << "face numver:" << faceNum << endl;

    //Initialize Mesh
    mesh.ClearAll();

    if(format == 0)
    {
        //vertex point
        for(int i = 0; i < vertexNum; i++)
        {
            int j = 0;
            getline(file, line);
            istringstream iss(line);
            while(iss >> word)
            {
                vec3d[j] = atof(word.c_str());
                if(j == 2)
                {
                    //cout << "vec3d[" << i <<"] = "<< vec3d << endl;
                    mesh.AddVertex( vec3d );
                    j = 0;
                }
                j++;
            }
        }//end of vertex index

        //face index
        for(int i = 0; i < faceNum; i++)
        {
            int j = 0;
            getline(file, line);
            istringstream iss(line);
            while(iss >> word)
            {
                if(j != 0)
                {
                    vec3i[j-1] = atoi(word.c_str());
                    if(j == 3)
                    {
                        //cout << "vec3i[" << i <<"] = "<< vec3i << endl;
                        mesh.AddFace(vec3i);
                        j = 0;
                    }
                }
                j++;

            }
        }//end of face index

        file.close();
        return true;
    }//end of if

    // binary little-endian
    if(format == 1)
    {
        file.close();
        file.open(file_name, ios::in | ios::binary);
        if(!file.is_open()) 
        {
            std::cerr << "cann't open the ply file" << std::endl;
            return false;
        }

        while(getline(file, line))
        {
            pos = line.find("end_header");
            if(pos != line.npos) 
                break;
        }

        //read vertex
        float num[3];
        for(int i = 0; i < vertexNum; i++)
        {
            file.read((char*)&num, sizeof(num));
            for(int j = 0; j < 3; j++)
            {
                vec3d[j] = num[j];
                if(j == 2)
                {
                    mesh.AddVertex(vec3d);
                }
            }
        }

        unsigned char sides = 0;
        int face_index[3];
        for(int i = 0; i < faceNum; i++)
        {
            file.read((char*)&sides, sizeof(unsigned char));
            file.read((char*)&face_index, sizeof(face_index));
            for(int j = 0; j < 3; j++)
            {
                vec3i[j] = face_index[j];
                if(j == 2)
                {
                    mesh.AddFace(vec3i);
                }
            }
        }

        file.close();
        return true;
    }//end of if

    // binary big-endian
    if(format == 2)
    {
        
        return true;
    }
}
