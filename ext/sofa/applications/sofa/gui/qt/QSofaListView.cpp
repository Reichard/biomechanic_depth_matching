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
#include "QSofaListView.h"
#include "QDisplayPropertyWidget.h"
#include "GraphListenerQListView.h"
#include "AddObject.h"
#include "ModifyObject.h"
#include "GenGraphForm.h"
#include "RealGUI.h"
#include <sofa/simulation/common/DeleteVisitor.h>
#include <sofa/simulation/common/TransformationVisitor.h>
#include <sofa/helper/cast.h>

#include <QMenu>
#include <QtGlobal> // version macro

using namespace sofa::simulation;
using namespace sofa::core::objectmodel;
namespace sofa
{
namespace gui
{
namespace qt
{


QSofaListView::QSofaListView(const SofaListViewAttribute& attribute,
        QWidget* parent,
        const char* name,
        Qt::WindowFlags f):
    QTreeWidget(parent),
    graphListener_(NULL),
    AddObjectDialog_(NULL),
    attribute_(attribute),
    propertyWidget(NULL)
{
    this->setObjectName(name);
    this->setWindowFlags(f);
    //List of objects
    //Read the object.txt that contains the information about the objects which can be added to the scenes whithin a given BoundingBox and scale range
    std::string object ( "config/object.txt" );

    if( sofa::helper::system::DataRepository.findFile ( object ) )
    {
        list_object.clear();
        std::ifstream end(object.c_str());
        std::string s;
        while( end >> s )
        {
            list_object.push_back(s);
        }
        end.close();
    }

    //Creation of the file dialog
    AddObjectDialog_ = new AddObject ( &list_object, this );
    AddObjectDialog_->hide();


    //addColumn(QString());

//    header()->setClickEnabled(false, header()->count() - 1);
//    header()->
    //header()->setResizeEnabled(false, header()->count() - 1);
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    header()->setResizeMode(header()->count() - 1, QHeaderView::Fixed);
#else
    header()->setSectionResizeMode(header()->count() - 1, QHeaderView::Fixed);
#endif // SOFA_QT5
    //header()->setLabel(0, QString());

    setRootIsDecorated(true);
    //setTreeStepSize(15);
    setIndentation(15);
    graphListener_ = new GraphListenerQListView(this);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,SIGNAL(customContextMenuRequested(const QPoint&)) ,this,SLOT(RunSofaRightClicked(const QPoint&)) );
    connect(this,SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int )), this, SLOT(RunSofaDoubleClicked(QTreeWidgetItem*, int)) );
    connect(this,SIGNAL(itemClicked(QTreeWidgetItem*,int) ), this, SLOT(updateMatchingObjectmodel(QTreeWidgetItem*, int)) );

}

QSofaListView::~QSofaListView()
{

    delete graphListener_;
}

void QSofaListView::Clear(Node* rootNode)
{

    if(graphListener_ != NULL)
    {
        /*if(attribute_ = VISUAL){
          simulation::getSimulation()->getVisualRoot()->removeListener(graphListener_);
        }*/
        delete graphListener_;
    }

    CloseAllDialogs();
    clear();
    graphListener_ = new GraphListenerQListView(this);

    this->setSortingEnabled(false);
//    setSorting ( -1 );
    header()->hide();
    graphListener_->addChild ( NULL, rootNode );
    graphListener_->freeze ( rootNode );
    std::map<Base*, QTreeWidgetItem* >::iterator graph_iterator;

    for (graph_iterator = graphListener_->items.begin();
            graph_iterator != graphListener_->items.end();
            ++graph_iterator)
    {
        Node* node = dynamic_cast< Node* >(graph_iterator->first);
        if (node!=NULL && !node->isActive())
        {
            object_.ptr.Node = node;
            object_.type  = typeNode;
            emit RequestActivation(object_.ptr.Node, node->isActive());
        }
    }

}

void QSofaListView::CloseAllDialogs()
{
    emit( Close() );
    assert( map_modifyObjectWindow.empty() );
    assert( map_modifyDialogOpened.empty() );

}

void QSofaListView::modifyUnlock(void* Id)
{
    map_modifyDialogOpened.erase( Id );
    map_modifyObjectWindow.erase( Id );
}

void QSofaListView::collapseNode()
{
    collapseNode(currentItem());
}

void QSofaListView::collapseNode(QTreeWidgetItem* item)
{
    if (!item) return;
    emit Lock(true);
    for(int i=0 ; i<item->childCount() ; i++)
    {
        QTreeWidgetItem* child = item->child(i);
        child->setExpanded(false);// setOpen ( false );
    }
    item->setExpanded ( true );
    emit Lock(false);
}

void QSofaListView::expandNode()
{
    expandNode(currentItem());
}

void QSofaListView::expandNode(QTreeWidgetItem* item)
{
    if (!item) return;
    emit Lock(true);
    item->setExpanded ( true );
    for(int i=0 ; i<item->childCount() ; i++)
    {
        QTreeWidgetItem* child = item->child(i);
        child->setExpanded(true);// setOpen ( false );
        expandNode(item);
    }

    emit Lock(false);
}

void QSofaListView::updateMatchingObjectmodel(QTreeWidgetItem* item, int)
{
    updateMatchingObjectmodel(item);
}

void QSofaListView::updateMatchingObjectmodel(QTreeWidgetItem* item)
{
    BaseData* data = NULL;
    Base* base = NULL;
    BaseObject* object = NULL;
    BaseNode* basenode = NULL;
    if(item == NULL)
    {
        object_.ptr.Node = NULL;
    }
    else
    {
        base = graphListener_->findObject(item);
        if(base == NULL)
        {
            data = graphListener_->findData(item);
            assert(data);
            object_.ptr.Data = data;
            object_.type = typeData;
            return;
        }
        basenode = base->toBaseNode();
        if( basenode == NULL)
        {
            object = dynamic_cast<BaseObject*>(base);
            object_.ptr.Object = object;
            object_.type = typeObject;
        }
        else
        {
            object_.ptr.Node = down_cast<Node>(basenode);
            object_.type = typeNode;
        }
    }

	addInPropertyWidget(item, true);
}
void QSofaListView::addInPropertyWidget(QTreeWidgetItem *item, bool clear)
{
    if(!item)
        return;

    Base* object = graphListener_->findObject(item);
    if(object == NULL)
        return;

	if(propertyWidget)
	{
		propertyWidget->addComponent(object->getName().c_str(), object, item, clear);
		
		propertyWidget->show();
	}
}

void QSofaListView::Freeze()
{
    Node* groot = down_cast<Node>( graphListener_->findObject(this->topLevelItem(0))->toBaseNode() );
    graphListener_->freeze(groot);
}

void QSofaListView::Unfreeze()
{
    Node* groot = down_cast<Node>( graphListener_->findObject(this->topLevelItem(0))->toBaseNode() );
    graphListener_->unfreeze(groot);
}

void QSofaListView::contextMenuEvent(QContextMenuEvent *event)
{
	event->accept();
}

void QSofaListView::focusObject()
{
    if( object_.isObject())
        emit( focusChanged(object_.ptr.Object));

}
void QSofaListView::focusNode()
{
    if( object_.isNode())
        emit( focusChanged(object_.ptr.Node));
}


/*****************************************************************************************************************/
void QSofaListView::RunSofaRightClicked( const QPoint& point)
{
    QTreeWidgetItem *item = this->itemAt( point );

    if( item == NULL) return;

    updateMatchingObjectmodel(item);
    //updateMatchingObjectmodel();

    QAction* act;

    bool object_hasData = false;
    if(object_.type == typeObject)
    {
        object_hasData = object_.ptr.Object->getDataFields().size() > 0 ? true : false;
    }
    QMenu *contextMenu = new QMenu ( this );
    contextMenu->setObjectName( "ContextMenu");
    if( object_.isNode() )
    {
        //int index_menu = contextMenu->insertItem("Focus", this,SLOT( focusNode() )  );
        act = contextMenu->addAction("Focus", this,SLOT(focusNode()));
        bool enable = object_.ptr.Node->f_bbox.getValue().isValid() && !object_.ptr.Node->f_bbox.getValue().isFlat();
        act->setEnabled(enable);
        //contextMenu->setItemEnabled(index_menu,enable);
    }
    if( object_.isObject() )
    {
       // int index_menu = contextMenu->insertItem("Focus", this,SLOT( focusObject() ) );
        act = contextMenu->addAction("Focus", this,SLOT(focusObject()));
        bool enable = object_.ptr.Object->f_bbox.getValue().isValid() && !object_.ptr.Object->f_bbox.getValue().isFlat() ;
        //contextMenu->setItemEnabled(index_menu,enable);
        act->setEnabled(enable);
    }
    //contextMenu->insertSeparator();
    contextMenu->addSeparator();

    //Creation of the context Menu
    if ( object_.type == typeNode)
    {
//        contextMenu->insertItem ( "Collapse", this, SLOT ( collapseNode() ) );
        act = contextMenu->addAction("Collapse", this,SLOT(collapseNode()));
//        contextMenu->insertItem ( "Expand", this, SLOT ( expandNode() ) );
        act = contextMenu->addAction("Expand", this,SLOT(expandNode()));
//        contextMenu->insertSeparator ();
        contextMenu->addSeparator();
        /*****************************************************************************************************************/
        if (object_.ptr.Node->isActive())
        {
//            contextMenu->insertItem ( "Deactivate", this, SLOT ( DeactivateNode() ) );
            act = contextMenu->addAction("Deactivate", this,SLOT(DeactivateNode()));
        }
        else
        {
//            contextMenu->insertItem ( "Activate", this, SLOT ( ActivateNode() ) );
            act = contextMenu->addAction("Activate", this,SLOT(ActivateNode()));
        }
		if (object_.ptr.Node->isSleeping())
        {
//            contextMenu->insertItem ( "Wake up", this, SLOT ( WakeUpNode() ) );
            act = contextMenu->addAction("Wake up", this,SLOT(WakeUpNode()));
        }
        else
        {
//            contextMenu->insertItem ( "Put to sleep", this, SLOT ( PutNodeToSleep() ) );
            act = contextMenu->addAction("Put to sleep", this,SLOT(PutNodeToSleep()));
        }
//        contextMenu->insertSeparator ();
        contextMenu->addSeparator();
        /*****************************************************************************************************************/

//        contextMenu->insertItem ( "Save Node", this, SLOT ( SaveNode() ) );
        act = contextMenu->addAction("Save Node", this,SLOT(SaveNode()));
//        contextMenu->insertItem ( "Export OBJ", this, SLOT ( exportOBJ() ) );
        act = contextMenu->addAction("Export OBJ", this,SLOT(exportOBJ()));

        if ( attribute_ == SIMULATION)
        {
//            contextMenu->insertItem ( "Add Node", this, SLOT ( RaiseAddObject() ) );
            act = contextMenu->addAction("Add Node", this,SLOT(RaiseAddObject()));

//            int index_menu = contextMenu->insertItem ( "Remove Node", this, SLOT ( RemoveNode() ) );
            act = contextMenu->addAction("Remove Node", this,SLOT(RemoveNode()));
            //If one of the elements or child of the current node is beeing modified, you cannot allow the user to erase the node
            if ( !isNodeErasable ( object_.ptr.Node ) )
            {
//                contextMenu->setItemEnabled ( index_menu,false );
                act->setEnabled(false);
            }
        }
    }
    //contextMenu->insertItem ( "Modify", this, SLOT ( Modify() ) );
    act = contextMenu->addAction("Modify", this,SLOT(Modify()));
    if(object_hasData)
    {
        if(item->childCount() > 0)
        {
//            contextMenu->insertItem("Hide Data",this, SLOT ( HideDatas() ) );
            act = contextMenu->addAction("Hide Data", this,SLOT(HideDatas()));
        }
        else
        {
//            contextMenu->insertItem("Show Data", this, SLOT ( ShowDatas() ) );
            act = contextMenu->addAction("Show Data", this,SLOT(ShowDatas()));
        }
    }

    contextMenu->exec ( this->mapToGlobal(point) /*, index */);
}

void QSofaListView::RunSofaDoubleClicked(QTreeWidgetItem* item, int /*index*/)
{
    if(item == NULL)
    {
        return;
    }

    item->setExpanded( !item->isExpanded());
    Modify();

}

/*****************************************************************************************************************/
void QSofaListView::nodeNameModification(simulation::Node* node)
{
    QTreeWidgetItem *item=graphListener_->items[node];

    QString nameToUse(node->getName().c_str());
    item->setText(0,nameToUse);

    nameToUse=/*QString("MultiNode ")+*/nameToUse;

    typedef std::multimap<QTreeWidgetItem *, QTreeWidgetItem*>::iterator ItemIterator;
    std::pair<ItemIterator,ItemIterator> range=graphListener_->nodeWithMultipleParents.equal_range(item);

    for (ItemIterator it=range.first; it!=range.second; ++it) it->second->setText(0,nameToUse);
}


void QSofaListView::DeactivateNode()
{
    emit RequestActivation(object_.ptr.Node,false);
    currentItem()->setExpanded(false);

}

void QSofaListView::ActivateNode()
{
    emit RequestActivation(object_.ptr.Node,true);
}

void QSofaListView::PutNodeToSleep()
{
    emit RequestSleeping(object_.ptr.Node, true);
}

void QSofaListView::WakeUpNode()
{
    emit RequestSleeping(object_.ptr.Node, false);
}

void QSofaListView::SaveNode()
{
    if( object_.ptr.Node != NULL)
    {
        emit Lock(true);
        Node * node = object_.ptr.Node;
        emit RequestSaving(node);
        emit Lock(false);

    }
}
void QSofaListView::exportOBJ()
{
    if( object_.ptr.Node != NULL)
    {
        emit Lock(true);
        Node * node = object_.ptr.Node;
        emit RequestExportOBJ(node,true);
        emit Lock(false);
    }
}
void QSofaListView::RaiseAddObject()
{
    emit Lock(true);
    assert(AddObjectDialog_);

    std::string path( ((RealGUI*) (QApplication::topLevelWidgets()[0]))->windowFilePath().toStdString());
    AddObjectDialog_->setPath ( path );
    AddObjectDialog_->show();
    AddObjectDialog_->raise();
    emit Lock(false);

}
void QSofaListView::RemoveNode()
{
    if( object_.type == typeNode)
    {
        emit Lock(true);
        Node* node = object_.ptr.Node;
        if ( node == node->getRoot() )
        {
            //Attempt to destroy the Root node : create an empty node to handle new graph interaction
            Node::SPtr root = simulation::getSimulation()->createNewGraph( "Root" );
            graphListener_->removeChild ( NULL, node);
            graphListener_->addChild ( NULL, root.get() );
            emit RootNodeChanged(root.get(),NULL);
        }
        else
        {
            node->detachFromGraph();
            node->execute<simulation::DeleteVisitor>(sofa::core::ExecParams::defaultInstance());
            emit NodeRemoved();
        }
        emit Lock(false);
    }
}
void QSofaListView::Modify()
{
    void *current_Id_modifyDialog = NULL;
    emit Lock(true);

    if ( currentItem() != NULL )
    {
        ModifyObjectFlags dialogFlags = ModifyObjectFlags();
        dialogFlags.setFlagsForSofa();
        ModifyObject* dialogModifyObject = NULL;

        if (object_.type == typeData)       //user clicked on a data
        {
            current_Id_modifyDialog = object_.ptr.Data;
        }
        if (object_.type == typeNode)
        {
            current_Id_modifyDialog = object_.ptr.Node;
        }
        if(object_.type == typeObject)
        {
            current_Id_modifyDialog = object_.ptr.Object;
        }
        assert(current_Id_modifyDialog != NULL);

        //Opening of a dialog window automatically created

        std::map< void*, QDialog* >::iterator testWindow =  map_modifyObjectWindow.find( current_Id_modifyDialog);
        if ( testWindow != map_modifyObjectWindow.end())
        {
            //Object already being modified: no need to open a new window
            (*testWindow).second->raise();
            emit Lock(false);
            return;
        }


        dialogModifyObject = new ModifyObject(current_Id_modifyDialog,currentItem(),this,dialogFlags,currentItem()->text(0).toStdString().c_str());
        if(object_.type == typeData)
            dialogModifyObject->createDialog(object_.ptr.Data);
        if(object_.type == typeNode)
            dialogModifyObject->createDialog((Base*)object_.ptr.Node);
        if(object_.type  == typeObject)
            dialogModifyObject->createDialog((Base*)object_.ptr.Object);

        map_modifyDialogOpened.insert( std::make_pair ( current_Id_modifyDialog, currentItem()) );
        map_modifyObjectWindow.insert( std::make_pair(current_Id_modifyDialog, dialogModifyObject));
        connect ( dialogModifyObject, SIGNAL( objectUpdated() ), this, SIGNAL( Updated() ));
        connect ( this, SIGNAL( Close() ), dialogModifyObject, SLOT( closeNow() ) );
        connect ( dialogModifyObject, SIGNAL( dialogClosed(void *) ) , this, SLOT( modifyUnlock(void *)));
        connect ( dialogModifyObject, SIGNAL( nodeNameModification(simulation::Node*) ) , this, SLOT( nodeNameModification(simulation::Node*) ));
        connect ( dialogModifyObject, SIGNAL( dataModified(QString) ), this, SIGNAL( dataModified(QString) ) );
        dialogModifyObject->show();
        dialogModifyObject->raise();

    }
    emit Lock(false);
}

void QSofaListView::UpdateOpenedDialogs()
{
    std::map<void*,QDialog*>::const_iterator iter;
    for(iter = map_modifyObjectWindow.begin(); iter != map_modifyObjectWindow.end() ; ++iter)
    {
        ModifyObject* modify = reinterpret_cast<ModifyObject*>(iter->second);
        modify->updateTables();
    }
}

void QSofaListView::HideDatas()
{
    if( object_.type == typeObject )
    {
        emit Lock(true);
        Unfreeze();
        graphListener_->removeDatas(object_.ptr.Object);
        Freeze();
        emit Lock(false);
    }
}

void QSofaListView::ShowDatas()
{
    if ( object_.type == typeObject )
    {
        emit Lock(true);
        Unfreeze();
        graphListener_->addDatas(object_.ptr.Object);
        Freeze();
        emit Lock(false);
    }
}
/*****************************************************************************************************************/
// Test if a node can be erased in the graph : the condition is that none of its children has a menu modify opened
bool QSofaListView::isNodeErasable ( BaseNode* node)
{
    QTreeWidgetItem* item = graphListener_->items[node];
    if(item == NULL)
    {
        return false;
    }
    // check if there is already a dialog opened for that item in the graph
    std::map< void*, QTreeWidgetItem*>::iterator it;
    for (it = map_modifyDialogOpened.begin(); it != map_modifyDialogOpened.end(); ++it)
    {
        if (it->second == item) return false;
    }

    //check the item childs
    for(int i=0 ; i<item->childCount() ; i++)
    {
        QTreeWidgetItem *child = item->child(i);
        for( it = map_modifyDialogOpened.begin(); it != map_modifyDialogOpened.end(); ++it)
        {
            if( it->second == child) return false;
        }
    }

    return true;

}

void QSofaListView::Export()
{
    Node* root = down_cast<Node>( graphListener_->findObject(this->topLevelItem(0))->toBaseNode() );
    GenGraphForm* form = new sofa::gui::qt::GenGraphForm;
    form->setScene ( root );
    std::string gname(((RealGUI*) (QApplication::topLevelWidgets()[0]))->windowFilePath().toStdString());
    std::size_t gpath = gname.find_last_of("/\\");
    std::size_t gext = gname.rfind('.');
    if (gext != std::string::npos && (gpath == std::string::npos || gext > gpath))
        gname = gname.substr(0,gext);
    form->filename->setText(gname.c_str());
    form->show();
}


void QSofaListView::loadObject ( std::string path, double dx, double dy, double dz,  double rx, double ry, double rz,double scale )
{
    emit Lock(true);
    //Verify if the file exists
    if ( !sofa::helper::system::DataRepository.findFile ( path ) ) return;
    path = sofa::helper::system::DataRepository.getFile ( path );

    //If we add the object without clicking on the graph (direct use of the method),
    //the object will be added to the root node
    if ( currentItem() == NULL )
    {
        for ( std::map<core::objectmodel::Base*, QTreeWidgetItem* >::iterator it = graphListener_->items.begin() ;
                it != graphListener_->items.end() ; ++ it )
        {
            if ( ( *it ).second->parent() == NULL ) //Root node position
            {
                object_.ptr.Node = dynamic_cast< sofa::simulation::Node *> ( ( *it ).first );
                object_.type = typeNode;
                break;
            }
        }
        assert(object_.ptr.Node != NULL);
    }

    //We allow unlock the graph to make all the changes now
    graphListener_->unfreeze ( object_.ptr.Node );

    //Loading of the xml file
    simulation::xml::BaseElement* xml = simulation::xml::loadFromFile ( path.c_str() );
    if ( xml == NULL ) return;

    // helper::system::SetDirectory chdir ( path.c_str() );

    //std::cout << "Initializing objects"<<std::endl;
    if ( !xml->init() )  std::cerr << "Objects initialization failed."<<std::endl;

    BaseNode* new_basenode = xml->getObject()->toBaseNode();
    if ( new_basenode == NULL )
    {
        std::cerr << "Objects initialization failed."<<std::endl;
        delete xml;
        return ;
    }

    Node* new_node = down_cast<Node> ( new_basenode );

    new_node->addListener(graphListener_);
    if ( object_.ptr.Node && new_node)
    {
        if ( object_.ptr.Node->child.empty() &&  object_.ptr.Node->object.empty() )
        {
            //Temporary Root : the current graph is empty, and has only a single node "Root"
            object_.ptr.Node->detachFromGraph();
            graphListener_->addChild ( NULL, new_node );
            simulation::getSimulation()->init(new_node);
            emit RootNodeChanged(new_node, path.c_str());
        }
        else
        {
            object_.ptr.Node->addChild (new_node );
            simulation::getSimulation()->init(new_node);
            emit NodeAdded();
        }
    }
    graphListener_->freeze(object_.ptr.Node);
    transformObject ( new_node, dx, dy, dz, rx,ry,rz,scale );
    emit Lock(false);
    object_.ptr.Node =  NULL;
}

void QSofaListView::transformObject ( Node *node, double dx, double dy, double dz,  double rx, double ry, double rz, double scale )
{
    if ( node == NULL ) return;
    //const SReal conversionDegRad = 3.141592653/180.0;
    //Vector3 rotationVector = Vector3(rx,ry,rz)*conversionDegRad;
    TransformationVisitor transform(sofa::core::ExecParams::defaultInstance());
    transform.setTranslation(dx,dy,dz);
    transform.setRotation(rx,ry,rz);
    transform.setScale(scale,scale,scale);
    transform.execute(node);
}








} //sofa
} // gui
} //qt



