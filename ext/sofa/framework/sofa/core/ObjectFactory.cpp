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
#include "ObjectFactory.h"

#include <sofa/helper/Logger.h>
#include <sofa/defaulttype/TemplatesAliases.h>

using sofa::helper::Logger;

namespace sofa
{
namespace core
{

ObjectFactory::~ObjectFactory()
{
}

ObjectFactory::ClassEntry& ObjectFactory::getEntry(std::string classname)
{
    if (registry.find(classname) == registry.end()) {
        registry[classname] = ClassEntry::SPtr(new ClassEntry);
        registry[classname]->className = classname;
    }

    return *registry[classname];
}

/// Test if a creator exists for a given classname
bool ObjectFactory::hasCreator(std::string classname)
{
    ClassEntryMap::iterator it = registry.find(classname);
    if (it == registry.end())
        return false;
    ClassEntry::SPtr entry = it->second;
    return (!entry->creatorMap.empty());
}

std::string ObjectFactory::shortName(std::string classname)
{
    std::string shortname;

    ClassEntryMap::iterator it = registry.find(classname);
    if (it != registry.end())
    {
        ClassEntry::SPtr entry = it->second;
        if(!entry->creatorMap.empty())
        {
            CreatorMap::iterator it = entry->creatorMap.begin();
	    Creator::SPtr c = it->second;
            shortname = c->getClass()->shortName;
        }
    }
    return shortname;
}

bool ObjectFactory::addAlias(std::string name, std::string result, bool force,
                             ClassEntry::SPtr* previous)
{
    // Check that the pointed class does exist
    ClassEntryMap::iterator it = registry.find(result);
    if (it == registry.end())
    {
        Logger::getMainLogger().log(Logger::Error, "Target class for alias '" + result + "'not found: " + name, "ObjectFactory::addAlias()");
        return false;
    }

    ClassEntry::SPtr& pointedEntry = it->second;
    ClassEntry::SPtr& aliasEntry = registry[name];

    // Check that the alias does not already exist, unless 'force' is true
    if (aliasEntry.get()!=NULL && !force)
    {
        Logger::getMainLogger().log(Logger::Error, "Name already exists: " + name, "ObjectFactory::addAlias()");
        return false;
    }

    if (previous) {
        ClassEntry::SPtr& entry = aliasEntry;
        *previous = entry;
    }

    registry[name] = pointedEntry;
    pointedEntry->aliases.insert(name);
    return true;
}

void ObjectFactory::resetAlias(std::string name, ClassEntry::SPtr previous)
{
    registry[name] = previous;
}

objectmodel::BaseObject::SPtr ObjectFactory::createObject(objectmodel::BaseContext* context, objectmodel::BaseObjectDescription* arg)
{
    objectmodel::BaseObject::SPtr object = NULL;
    std::vector< std::pair<std::string, Creator::SPtr> > creators;
    std::string classname = arg->getAttribute( "type", "");
	std::string usertemplatename = arg->getAttribute( "template", "");
	std::string templatename = sofa::defaulttype::TemplateAliases::resolveAlias(usertemplatename); // Resolve template aliases
	std::string userresolved = templatename; // Copy in case we change for the default one

    ClassEntryMap::iterator it = registry.find(classname);
	if (it != registry.end()) // Found the classname
    {
        ClassEntry::SPtr entry = it->second;
		// If no template has been given or if the template does not exist, first try with the default one
        if(templatename.empty() || entry->creatorMap.find(templatename) == entry->creatorMap.end())
			templatename = entry->defaultTemplate;

        CreatorMap::iterator it2 = entry->creatorMap.find(templatename);
        if (it2 != entry->creatorMap.end())
        {
            Creator::SPtr c = it2->second;
            if (c->canCreate(context, arg))
                creators.push_back(*it2);
        }

		// If object cannot be created with the given template (or the default one), try all possible ones
        if (creators.empty())
        {
            CreatorMap::iterator it3;
            for (it3 = entry->creatorMap.begin(); it3 != entry->creatorMap.end(); ++it3)
            {
				Creator::SPtr c = it3->second;
                if (c->canCreate(context, arg))
                    creators.push_back(*it3);
            }
        }
    }

    if (creators.empty())
    {	// The object cannot be created
        arg->logError("Object type " + classname + std::string("<") + templatename + std::string("> creation failed"));
    }
    else
    {
        object = creators[0].second->createInstance(context, arg);

		// The object has been created, but not with the template given by the user
		if (!usertemplatename.empty() && object->getTemplateName() != userresolved)
		{
            std::string w = "Template <" + usertemplatename + std::string("> incorrect, used <") + object->getTemplateName() + std::string(">");
            object->serr << w << object->sendl;
        }
        else if (creators.size() > 1)
        {	// There was multiple possibilities, we used the first one (not necessarily the default, as it can be incompatible)
            std::string w = "Template <" + templatename + std::string("> incorrect, used <") + object->getTemplateName() + std::string("> in the list:");
            for(unsigned int i = 0; i < creators.size(); ++i)
                w += std::string("\n\t* ") + creators[i].first;
            object->serr << w << object->sendl;
        }
    }

    return object;
}

ObjectFactory* ObjectFactory::getInstance()
{
    static ObjectFactory instance;
    return &instance;
}

void ObjectFactory::getAllEntries(std::vector<ClassEntry::SPtr>& result)
{
    result.clear();
    for(ClassEntryMap::iterator it = registry.begin(), itEnd = registry.end();
        it != itEnd; ++it)
    {
        ClassEntry::SPtr entry = it->second;
        // Push the entry only if it is not an alias
        if (entry->className == it->first)
            result.push_back(entry);
    }
}

void ObjectFactory::getEntriesFromTarget(std::vector<ClassEntry::SPtr>& result, std::string target)
{
    result.clear();
    for(ClassEntryMap::iterator it = registry.begin(), itEnd = registry.end();
        it != itEnd; ++it)
    {
        ClassEntry::SPtr entry = it->second;
        bool inTarget = false;
        for (CreatorMap::iterator itc = entry->creatorMap.begin(), itcend = entry->creatorMap.end(); itc != itcend; ++itc)
        {
	    Creator::SPtr c = itc->second;
            if (target == c->getTarget())
                inTarget = true;
        }
        if (inTarget)
            result.push_back(entry);
    }
}

std::string ObjectFactory::listClassesFromTarget(std::string target, std::string separator)
{
    std::vector<ClassEntry::SPtr> entries;
    getEntriesFromTarget(entries, target);
    std::ostringstream oss;
    for (unsigned int i=0; i<entries.size(); ++i)
    {
        if (i) oss << separator;
        oss << entries[i]->className;
    }
    std::string result = oss.str();
    return result;
}

void ObjectFactory::dump(std::ostream& out)
{
    for (ClassEntryMap::iterator it = registry.begin(), itend = registry.end(); it != itend; ++it)
    {
        ClassEntry::SPtr entry = it->second;
        if (entry->className != it->first) continue;
        out << "class " << entry->className <<" :\n";
        if (!entry->aliases.empty())
        {
            out << "  aliases :";
            for (std::set<std::string>::iterator it = entry->aliases.begin(), itend = entry->aliases.end(); it != itend; ++it)
                out << " " << *it;
            out << "\n";
        }
        if (!entry->description.empty())
            out << entry->description;
        if (!entry->authors.empty())
            out << "  authors : " << entry->authors << "\n";
        if (!entry->license.empty())
            out << "  license : " << entry->license << "\n";
        for (CreatorMap::iterator itc = entry->creatorMap.begin(), itcend = entry->creatorMap.end(); itc != itcend; ++itc)
        {
            out << "  template instance : " << itc->first << "\n";
        }
    }
}

static std::string xmlencode(const std::string& str)
{
    std::string res;
    for (unsigned int i=0; i<str.length(); ++i)
    {
        switch(str[i])
        {
        case '<': res += "&lt;"; break;
        case '>': res += "&gt;"; break;
        case '&': res += "&amp;"; break;
        case '"': res += "&quot;"; break;
        case '\'': res += "&apos;"; break;
        default:  res += str[i];
        }
    }
    return res;
}

void ObjectFactory::dumpXML(std::ostream& out)
{
    for (ClassEntryMap::iterator it = registry.begin(), itend = registry.end(); it != itend; ++it)
    {
        ClassEntry::SPtr entry = it->second;
        if (entry->className != it->first) continue;
        out << "<class name=\"" << xmlencode(entry->className) <<"\">\n";
        for (std::set<std::string>::iterator it = entry->aliases.begin(), itend = entry->aliases.end(); it != itend; ++it)
            out << "<alias>" << xmlencode(*it) << "</alias>\n";
        if (!entry->description.empty())
            out << "<description>"<<entry->description<<"</description>\n";
        if (!entry->authors.empty())
            out << "<authors>"<<entry->authors<<"</authors>\n";
        if (!entry->license.empty())
            out << "<license>"<<entry->license<<"</license>\n";
        for (CreatorMap::iterator itc = entry->creatorMap.begin(), itcend = entry->creatorMap.end(); itc != itcend; ++itc)
        {
            out << "<creator";
            if (!itc->first.empty()) out << " template=\"" << xmlencode(itc->first) << "\"";
            out << "/>\n";
        }
        out << "</class>\n";
    }
}

void ObjectFactory::dumpHTML(std::ostream& out)
{
    out << "<ul>\n";
    for (ClassEntryMap::iterator it = registry.begin(), itend = registry.end(); it != itend; ++it)
    {
        ClassEntry::SPtr entry = it->second;
        if (entry->className != it->first) continue;
        out << "<li><b>" << xmlencode(entry->className) <<"</b>\n";
        if (!entry->description.empty())
            out << "<br/>"<<entry->description<<"\n";
        out << "<ul>\n";
        if (!entry->aliases.empty())
        {
            out << "<li>Aliases:<i>";
            for (std::set<std::string>::iterator it = entry->aliases.begin(), itend = entry->aliases.end(); it != itend; ++it)
                out << " " << xmlencode(*it);
            out << "</i></li>\n";
        }
        if (!entry->authors.empty())
            out << "<li>Authors: <i>"<<entry->authors<<"</i></li>\n";
        if (!entry->license.empty())
            out << "<li>License: <i>"<<entry->license<<"</i></li>\n";
        if (entry->creatorMap.size()>2 || (entry->creatorMap.size()==1 && !entry->creatorMap.begin()->first.empty()))
        {
            out << "<li>Template instances:<i>";
            for (CreatorMap::iterator itc = entry->creatorMap.begin(), itcend = entry->creatorMap.end(); itc != itcend; ++itc)
            {
                if (itc->first == entry->defaultTemplate)
                    out << " <b>" << xmlencode(itc->first) << "</b>";
                else
                    out << " " << xmlencode(itc->first);
            }
            out << "</i></li>\n";
        }
        out << "</ul>\n";
        out << "</li>\n";
    }
    out << "</ul>\n";
}

RegisterObject::RegisterObject(const std::string& description)
{
    if (!description.empty())
    {
        addDescription(description);
    }
}

RegisterObject& RegisterObject::addAlias(std::string val)
{
    entry.aliases.insert(val);
    return *this;
}

RegisterObject& RegisterObject::addDescription(std::string val)
{
    val += '\n';
    entry.description += val;
    return *this;
}

RegisterObject& RegisterObject::addAuthor(std::string val)
{
    val += ' ';
    entry.authors += val;
    return *this;
}

RegisterObject& RegisterObject::addLicense(std::string val)
{
    entry.license += val;
    return *this;
}

RegisterObject& RegisterObject::addCreator(std::string classname,
                                           std::string templatename,
                                           ObjectFactory::Creator::SPtr creator)
{

    if (!entry.className.empty() && entry.className != classname)
    {
        Logger::getMainLogger().log(Logger::Error, "Template already instanciated with a different classname: " + entry.className + " != " + classname, "ObjectFactory");
    }
    else if (entry.creatorMap.find(templatename) != entry.creatorMap.end())
    {
        Logger::getMainLogger().log(Logger::Error, "Component already registered: " + classname + "<" + templatename + ">", "ObjectFactory");
    }
    else
    {
        entry.className = classname;
        entry.creatorMap[templatename] =  creator;
    }
    return *this;
}

RegisterObject::operator int()
{
    if (entry.className.empty())
    {
        return 0;
    }
    else
    {
        ObjectFactory::ClassEntry& reg = ObjectFactory::getInstance()->getEntry(entry.className);
        reg.description += entry.description;
        reg.authors += entry.authors;
        reg.license += entry.license;
        if (!entry.defaultTemplate.empty())
        {
            if (!reg.defaultTemplate.empty())
            {
                Logger::getMainLogger().log(Logger::Warning, "Default template for class " + entry.className + " already registered (" + reg.defaultTemplate + "), do not register " + entry.defaultTemplate + " as the default", "ObjectFactory");
            }
            else
            {
                reg.defaultTemplate = entry.defaultTemplate;
            }
        }
        for (ObjectFactory::CreatorMap::iterator itc = entry.creatorMap.begin(), itcend = entry.creatorMap.end(); itc != itcend; ++itc)
        {
            if (reg.creatorMap.find(itc->first) != reg.creatorMap.end())
            {
                Logger::getMainLogger().log(Logger::Warning, "Class already registered: entry.className " + itc->first, "ObjectFactory");
            }
            else
            {
                reg.creatorMap.insert(*itc);
            }
        }
        for (std::set<std::string>::iterator it = entry.aliases.begin(), itend = entry.aliases.end(); it != itend; ++it)
        {
            if (reg.aliases.find(*it) == reg.aliases.end())
            {
                ObjectFactory::getInstance()->addAlias(*it,entry.className);
            }
        }
        return 1;
    }
}

} // namespace core

} // namespace sofa
