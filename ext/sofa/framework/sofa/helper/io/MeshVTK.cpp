/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2015 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to the Free Software Foundation,     *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
*******************************************************************************
*                              SOFA :: Framework                              *
*                                                                             *
* Authors: The SOFA Team (see Authors.txt)                                    *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#include <sofa/helper/io/MeshVTK.h>

#include <sstream>

#include <tinyxml.h>

#include <sofa/helper/system/FileRepository.h>
#include <sofa/helper/system/SetDirectory.h>

namespace sofa
{

namespace helper
{

namespace io
{

using namespace sofa::defaulttype;
using namespace sofa::core::loader;

SOFA_DECL_CLASS(MeshVTK)

Creator<Mesh::FactoryMesh,MeshVTK> MeshVTKClass("vtu");

void MeshVTK::init(std::string filename)
{
    if (!sofa::helper::system::DataRepository.findFile(filename))
    {
        std::cerr << "File " << filename << " not found " << std::endl;
        return;
    }
    loaderType = "vtu";
    readVTU(filename);
}

void MeshVTK::readVTU(const std::string &filename)
{

    TiXmlDocument vtu;
    vtu.LoadFile(filename);
    if (vtu.Error())
    {
        std::cerr << "Error while loading file " << filename << std::endl;
        std::cerr << vtu.ErrorDesc() << std::endl;
        return;
    }
    TiXmlElement* piece = vtu.FirstChildElement("VTKFile")->FirstChildElement("UnstructuredGrid")->FirstChildElement("Piece");

    unsigned int nbPoints, nbCells;
    piece->QueryUnsignedAttribute("NumberOfPoints", &nbPoints);
    piece->QueryUnsignedAttribute("NumberOfCells", &nbCells);

    // read vertices
    Vec3d vertex;
    std::stringstream pointsDataArray(piece->FirstChildElement("Points")->FirstChildElement("DataArray")->GetText());
    for (std::size_t i = 0 ; i < nbPoints ; ++i)
    {
        pointsDataArray >> vertex[0] >> vertex[1] >> vertex[2];
        vertices.push_back(vertex);
    }


    // read triangles
    std::stringstream connectivityDataArray;
    std::stringstream typesDataArray;
    TiXmlElement* cellDataArray = piece->FirstChildElement("Cells")->FirstChildElement("DataArray");
    std::string name;
    while(cellDataArray)
    {
        cellDataArray->QueryStringAttribute("Name", &name);
        if (name.compare("connectivity")==0)
            connectivityDataArray << cellDataArray->GetText();
        if (name.compare("types")==0)
            typesDataArray << cellDataArray->GetText();
        cellDataArray = cellDataArray->NextSiblingElement();
    }

    int cellType;
    vector< vector<int> > vertNormTexIndices;
    vector<int> vIndices(3,0), nIndices(3,0), tIndices(3,0), quad(4,0);
    for (std::size_t i = 0 ; i < nbCells ; ++i)
    {
        typesDataArray >> cellType;
        switch(cellType) {
        case 5: // triangle
            connectivityDataArray >> vIndices[0]  >> vIndices[1] >> vIndices[2];
            vertNormTexIndices.clear();
            vertNormTexIndices.push_back (vIndices);
            vertNormTexIndices.push_back (nIndices);
            vertNormTexIndices.push_back (tIndices);
            facets.push_back(vertNormTexIndices);
            break;
        case 9: // quad
            // split quad into two triangles
            connectivityDataArray >> quad[0] >> quad[1] >> quad[2] >> quad[3];
            // triangle #1
            vIndices[0]=quad[0];vIndices[1]=quad[1];vIndices[2]=quad[2];
            vertNormTexIndices.clear();
            vertNormTexIndices.push_back (vIndices);
            vertNormTexIndices.push_back (nIndices);
            vertNormTexIndices.push_back (tIndices);
            facets.push_back(vertNormTexIndices);
            // triangle #2
            vIndices[0]=quad[0];vIndices[1]=quad[2];vIndices[2]=quad[3];
            vertNormTexIndices.clear();
            vertNormTexIndices.push_back (vIndices);
            vertNormTexIndices.push_back (nIndices);
            vertNormTexIndices.push_back (tIndices);
            facets.push_back(vertNormTexIndices);
            break;
        default:
            std::cerr << "ERROR: " << filename << " - Unsupported cell type: " << cellType << std::endl;
            return;
        }
    }
}

} // namespace io

} // namespace helper

} // namespace sofa

