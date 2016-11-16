/*************************************************************************
	> File Name: FilePly.h
	> Author: wangl
	> Mail: feixiaku@gmail.com
	> Created Time: Tue Nov 15 10:25:05 2016
 ************************************************************************/

#ifndef _FILEPLY_H
#define _FILEPLY_H

#include "Mesh.h"
#include <string>

// Read Ply File
extern bool ReadPly( Mesh &mesh, const std::string& file_name);

#endif
