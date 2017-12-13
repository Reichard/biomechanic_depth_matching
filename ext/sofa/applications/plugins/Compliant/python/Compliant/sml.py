import os.path
import math
import xml.etree.ElementTree as etree
import sys

from Compliant import StructuralAPI

import SofaPython.Tools
import SofaPython.units
from SofaPython import Quaternion
from SofaPython.Tools import listToStr as concat
import SofaPython.mass
import SofaPython.sml
import Flexible.API
import Flexible.sml

printLog = True

def insertRigid(parentNode, rigidModel, density, param=None):
    """ create a StructuralAPI.RigidBody from the rigidModel, compute rigidMass from
        1) mass, com and inertia if present
        2) mesh if possible
        3) default to a unit sphere TODO: is it relevant to do so ?
    """

    if printLog:
        print "[Compliant.sml.insertRigid] ", rigidModel.name
        for mesh in rigidModel.mesh :
            if rigidModel.meshAttributes[mesh.id].collision is True:
                print "     collision mesh: ", mesh.name
        sys.stdout.flush()

    rigid = StructuralAPI.RigidBody(parentNode, rigidModel.name)

    # check mesh formats are supported by generateRigid
    meshFormatSupported = True
    for mesh in rigidModel.mesh :
        meshFormatSupported &= mesh.format=="obj" or mesh.format=="vtk"

    if not rigidModel.mass is None and not rigidModel.com is None and not rigidModel.inertia is None:
        # all inertial data is present, let's use it
        massinfo = SofaPython.mass.RigidMassInfo()
        massinfo.mass = rigidModel.mass # TODO: convert units ?
        massinfo.com = rigidModel.com # TODO: convert units ?

        if len(rigidModel.inertia)==3 and not rigidModel.inertia_rotation is None:
            massinfo.diagonal_inertia = rigidModel.inertia
            massinfo.inertia_rotation = rigidModel.inertia_rotation
        else:
            massinfo.setFromInertia(rigidModel.inertia[0], rigidModel.inertia[1], rigidModel.inertia[2], # Ixx, Ixy, Ixz
                                    rigidModel.inertia[3], rigidModel.inertia[4], # Iyy, Iyz
                                    rigidModel.inertia[5] ) # Izz
        rigid.setFromRigidInfo(massinfo, offset=rigidModel.position , inertia_forces = False )    # TODO: handle inertia_forces ?
    elif len(rigidModel.mesh)!=0 and meshFormatSupported:
        # get inertia from meshes and density
        massinfo = SofaPython.sml.getSolidRigidMassInfo(rigidModel, density)
        rigid.setFromRigidInfo(massinfo, offset=rigidModel.position , inertia_forces = False )    # TODO: handle inertia_forces ?

        #if not rigidModel.mass is None :
            ## no density but a mesh let's normalise computed mass with specified mass
            #mass= SofaPython.units.mass_from_SI(rigidModel.mass)
            #inertia = []
            #for inert,m in zip(rigid.mass.inertia, rigid.mass.mass):
                #for i in inert:
                    #inertia.append( i/m[0]*mass)
            #rigid.mass.inertia = concat(inertia)
            #rigid.mass.mass = mass
    else:
        # no mesh, get mass/inertia if present, default to a unit sphere
        print "WARNING: Compliant.sml.insertRigid using default rigidMass"
        mass = rigidModel.mass  if not rigidModel.mass is None else SofaPython.units.mass_from_SI(1.)
        inertia = [1,1,1]
        t = rigidModel.position
        if not rigidModel.com is None:
            t[0] += rigidModel.com[0]
            t[1] += rigidModel.com[1]
            t[2] += rigidModel.com[2]
        rigid.setManually(t, mass, inertia)

    if not param is None:
        rigid.dofs.showObject = param.showRigid
        rigid.dofs.showObjectScale = SofaPython.units.length_from_SI(param.showRigidScale)

    # walk around to handle multiple meshes
    # @todo: handle them in StructuralAPI ?
    rigid.collisions=dict()
    rigid.visuals=dict()
    for mesh in rigidModel.mesh :
        if rigidModel.meshAttributes[mesh.id].collision is True:
            rigid.collisions[mesh.id] = rigid.addCollisionMesh(mesh.source,name_suffix='_'+mesh.name)
            if rigidModel.meshAttributes[mesh.id].visual is True:
                rigid.visuals[mesh.id] = rigid.collisions[mesh.id].addVisualModel()
        elif rigidModel.meshAttributes[mesh.id].visual is True:
            rigid.visuals[mesh.id] = rigid.addVisualModel(mesh.source,name_suffix='_'+mesh.name)

    return rigid

def insertJoint(jointModel, rigids, param=None):
    """ create a StructuralAPI.GenericRigidJoint from the jointModel """

    if printLog:
        print "[Compliant.sml.insertJoint] joint: ", jointModel.name
        sys.stdout.flush()

    frames=list()
    for i,offset in enumerate(jointModel.offsets):
        rigid = rigids[jointModel.solids[i].id] # shortcut
        if not offset is None:
            if offset.isAbsolute():
                frames.append( rigid.addAbsoluteOffset(offset.name, offset.value))
            else:
                frames.append( rigid.addOffset(offset.name, offset.value) )
            if not param is None:
                frames[-1].dofs.showObject = param.showOffset
                frames[-1].dofs.showObjectScale = SofaPython.units.length_from_SI(param.showOffsetScale)
        else:
            frames.append(rigid)
    mask = [1]*6
    limits=[] # mask for limited dofs
    isLimited = True # does the joint have valid limits?
    for d in jointModel.dofs:
        if isLimited:
            if d.min==None or d.max==None:
                isLimited = False # as soon as a limit is not defined, the limits cannot work
            else:
                limits.append(d.min)
                limits.append(d.max)
        mask[d.index] = 0
    if not param is None:
        joint = StructuralAPI.GenericRigidJoint(jointModel.name, frames[0].node, frames[1].node, mask, compliance=param.jointCompliance)
    if isLimited:
        joint.addLimits(limits)
    return joint

class SceneArticulatedRigid(SofaPython.sml.BaseScene):
    """ Builds a (sub)scene from a model using compliant formulation
    [tag] rigid are simulated as RigidBody
    Compliant joints are setup between the rigids """
    
    def __init__(self, parentNode, model):
        SofaPython.sml.BaseScene.__init__(self, parentNode, model)
        
        self.rigids = dict()
        self.joints = dict()

        self.param.jointCompliance=0

        self.param.showRigid=False
        self.param.showRigidScale=0.5 # SI unit (m)
        self.param.showOffset=False
        self.param.showOffsetScale=0.1 # SI unit (m)    

    def insertMergeRigid(self, mergeNodeName="dofRigid", tag="rigid", rigidIndexById=None ):
        """ Merge all the rigids in a single MechanicalObject using a SubsetMultiMapping
        optionnaly give a tag to select the rigids which are merged
        return the created node"""
        mergeNode = None
        currentRigidIndex=0
        input=""
        indexPairs=""
        if tag in self.model.solidsByTag:
            for solid in self.model.solidsByTag[tag]:
                if not solid.id in self.rigids:
                    print "[Compliant.sml.SceneArticulatedRigid.insertMergeRigid] WARNING: "+solid.name+" is not a rigid"
                    continue
                rigid = self.rigids[solid.id]
                if mergeNode is None:
                    mergeNode = rigid.node.createChild(mergeNodeName)
                else:
                    rigid.node.addChild(mergeNode)
                input += '@'+rigid.node.getPathName()+" "
                indexPairs += str(currentRigidIndex) + " 0 "
                if not rigidIndexById is None:
                    rigidIndexById[solid.id]=currentRigidIndex
                currentRigidIndex+=1
        if input:
            mergeNode.createObject("MechanicalObject", template = "Rigid3", name="dofs")
            mergeNode.createObject('SubsetMultiMapping', template = "Rigid3,Rigid3", name="mapping", input = input , output = '@./', indexPairs=indexPairs, applyRestPosition=True )
        return mergeNode

    def createScene(self):
        self.node.createObject('RequiredPlugin', name = 'Flexible' )
        self.node.createObject('RequiredPlugin', name = 'Compliant' )
        
        SofaPython.sml.setupUnits(self.model.units)

        # rigids
        if "rigid" in self.model.solidsByTag:
            for rigidModel in self.model.solidsByTag["rigid"]:
                self.rigids[rigidModel.id] = insertRigid(self.node, rigidModel, self.material.density(self.getMaterial(rigidModel.id)) , self.param)
        
        # joints
        for jointModel in self.model.genericJoints.values():
            self.joints[jointModel.id] = insertJoint(jointModel, self.rigids, self.param)


class SceneSkinning(SceneArticulatedRigid) :
    """ Build a (sub-)scene based on SceneArticulatedRigid, add solids with skinning using the defined armature
    [tag] armature are simulated as RigidBody and used as bones for skinning
    """
    
    def __init__(self, parentNode, model):
        SceneArticulatedRigid.__init__(self, parentNode, model)
        self.deformables = dict()
        self.skinningArmatureBoneIndexById = dict() # keep track of bone armature index in the armature merge node

    def createScene(self):

        self.model.setTagFromTag("armature", "rigid")

        SceneArticulatedRigid.createScene(self)
        
        if "armature" in self.model.solidsByTag:
            # insert node containing all bones of the armature
            self.nodes["armature"] = self.insertMergeRigid(mergeNodeName="armature", tag="armature", rigidIndexById=self.skinningArmatureBoneIndexById)
            for solidModel in self.model.solids.values():
                print solidModel.name, len(solidModel.skinnings)
                if len(solidModel.skinnings)>0: # ignore solid if it has no skinning
                    # for each mesh create a Flexible.API.Deformable
                    for mesh in solidModel.mesh:
                        # take care only of visual meshes with skinning
                        if solidModel.meshAttributes[mesh.id].visual:
                            deformable = Flexible.API.Deformable(self.nodes["armature"], solidModel.name+"_"+mesh.name)
                            deformable.loadMesh(mesh.source)
                            deformable.addMechanicalObject()
                            (indices, weights) = Flexible.sml.getSolidSkinningIndicesAndWeights(solidModel, self.skinningArmatureBoneIndexById)
                            deformable.addSkinning(self.nodes["armature"], indices.values(), weights.values())
                            deformable.addVisual()
                            self.deformables[mesh.id] = deformable

#                        self.deformables[solidModel.id]=Flexible.API.Deformable(self.nodes["armature"], solidModel.name)
#                    self.deformables[solidModel.id]=Flexible.sml.insertDeformableWithSkinning(self.node, solidModel, self.nodes["armature"].getPathName(), bonesId)
        
        
