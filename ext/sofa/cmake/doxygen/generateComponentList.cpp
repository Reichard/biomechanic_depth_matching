/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2015 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program; if not, write to the Free Software Foundation, Inc., 51  *
* Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.                   *
*******************************************************************************
*                            SOFA :: Applications                             *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#include <SofaComponentMain/init.h>
#include <sofa/core/ObjectFactory.h>

#include <iostream>
#include <fstream>
#include <string>

using std::string;
using namespace sofa::core;
using namespace sofa::core::objectmodel;

string make_link(ObjectFactory::Creator::SPtr creator) {
    const BaseClass* Class = creator->getClass();
    string str(Class->namespaceName + "::" + Class->className);
    size_t index;
    while ((index = str.find("::", 0)) != string::npos)
        str.replace(index, 2, "_1_1");
    for (char c = 'A'; c <= 'Z' ; c++)
        while ((index = str.find(c, 0)) != string::npos)
            str.replace(index, 1, "_"+string(1, c-'A'+'a'));
    string link(string(creator->getTarget()) + "/class" + str + ".html");
    return "<a href=\"../" + link + "\">" + Class->className +"</a>";
}


void print(const std::string& s) {
    std::cout << s << std::endl;
}

int main()
{
    sofa::component::init();
    std::vector<ObjectFactory::ClassEntry::SPtr> entries;
    ObjectFactory::getInstance()->getAllEntries(entries);
    print("/**");
    print("   \\page sofa_modules_component_list Component List");
    print("  <ul>");
    for (size_t i = 0 ; i != entries.size() ; i++)
        if (!entries[i]->creatorMap.empty()) {
            const ObjectFactory::Creator::SPtr creator = entries[i]->creatorMap.begin()->second;
            print("    <li>" + make_link(creator) + "</li>");
        }
    print("  </ul>");
    print("*/");
    return 0;
}
