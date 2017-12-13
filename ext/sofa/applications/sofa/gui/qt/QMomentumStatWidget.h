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
#ifndef SOFA_GUI_QT_QMOMENTUMSTATWIDGET_H
#define SOFA_GUI_QT_QMOMENTUMSTATWIDGET_H

#include "QGraphStatWidget.h"

#include <sofa/simulation/common/MechanicalGetMomentumVisitor.h>

namespace sofa
{
namespace gui
{
namespace qt
{

class QMomentumStatWidget : public QGraphStatWidget
{

    Q_OBJECT


    simulation::MechanicalGetMomentumVisitor *m_momentumVisitor;

public:

    QMomentumStatWidget( QWidget* parent, simulation::Node* node );

    virtual ~QMomentumStatWidget();

    virtual void step();
};


} // qt
} // gui
} //sofa

#endif // SOFA_GUI_QT_QMOMENTUMSTATWIDGET_H

