//copyright 2007 Daniel Arzabe
//This is a fresnel shader that is independent of the active camera's angle of view.
//The angle must be connected explicitly using inMatrix and matrixEyeToWorld

#include <math.h>

#include <maya/MPxNode.h>
#include <maya/MIOStream.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFloatVector.h>
#include <maya/MFloatMatrix.h>
#include <maya/MMatrix.h>
#include <maya/MVector.h>
#include <maya/MFnPlugin.h>

#include <maya/MFnMatrixAttribute.h>

#include <iostream>

#define DISPLAY_INFO(print, object,message)		\
	if(print == 1){\
	std::cerr << message << "\n" << object << "\n";}	

#define McheckErr(status,message)		\
	if( MStatus::kSuccess != status ) {	\
		std::cerr << message << "\n";		\
	}




class freeFresnel : public MPxNode
{
public:
	freeFresnel();
	virtual       ~freeFresnel() override;

	virtual MStatus compute(const MPlug&, MDataBlock&) override;
	virtual void    postConstructor() override;
	
	SchedulingType schedulingType() const override { return SchedulingType::kGloballySerial; }

	static  void* creator();
	static  MStatus initialize();

	static  MTypeId id;

private:

	static MObject aColor1;
	static MObject aColor2;
	static MObject aNormalCamera;
	static MObject aPointCamera;

	static MObject aInPoint;
	static MObject aInMatrix;
	static MObject aMatrixEyeToWorld;

	static MObject aFront;
	static MObject aSide;
	static MObject aNoise;

	static MObject aOutColor;
	//static MObject aOutAlpha;
};

MTypeId freeFresnel::id(0x8200e);

MObject freeFresnel::aColor1;
MObject freeFresnel::aColor2;

MObject freeFresnel::aNormalCamera;
MObject freeFresnel::aPointCamera;

MObject freeFresnel::aInMatrix;
MObject freeFresnel::aInPoint;
MObject freeFresnel::aMatrixEyeToWorld;

MObject freeFresnel::aFront;
MObject freeFresnel::aSide;
MObject freeFresnel::aNoise;


MObject freeFresnel::aOutColor;

void freeFresnel::postConstructor()
{
	//setMPSafe(false);
	
}


freeFresnel::freeFresnel()
{
}


freeFresnel::~freeFresnel()
{
}


void* freeFresnel::creator()
{
	return new freeFresnel();
}























MStatus freeFresnel::initialize()
{
	MFnNumericAttribute nAttr;

	MStatus stat;

	aFront = nAttr.create("front", "fro", MFnNumericData::kFloat);
	nAttr.setDefault(0.8f);
	nAttr.setMin(0.0f);
	nAttr.setMax(1.0f);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setWritable(true);

	aSide = nAttr.create("side", "sid", MFnNumericData::kFloat);
	nAttr.setDefault(0.7f);
	nAttr.setMin(0.0f);
	nAttr.setMax(1.0f);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setWritable(true);

	// User defined color attribute
	aColor1 = nAttr.createColor("sideColor", "sc");
	nAttr.setDefault(0.0f, 0.0f, 0.0f);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setWritable(true);


	aColor2 = nAttr.createColor("frontColor", "fc");
	nAttr.setDefault(0.0f, 1.0f, 0.0f);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setUsedAsColor(true);
	nAttr.setReadable(true);
	nAttr.setWritable(true);

	aNoise = nAttr.create("noise", "no", MFnNumericData::kFloat);
	nAttr.setDefault(0.0f);
	nAttr.setMin(0.0f);
	nAttr.setMax(1.0f);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);

	nAttr.setWritable(true);


	// Surface Normal supplied by the render sampler

	aNormalCamera = nAttr.createPoint("normalCamera", "n");
	nAttr.setStorable(false);
	nAttr.setHidden(true);
	nAttr.setReadable(true);
	nAttr.setWritable(true);

	// Point on surface in camera space, will be used to compute view vector
	aPointCamera = nAttr.createPoint("pointCamera", "p");
	nAttr.setStorable(false);
	nAttr.setHidden(true);
	nAttr.setReadable(true);
	nAttr.setWritable(true);



	aInPoint = nAttr.createPoint("inPoint", "ip");
	nAttr.setStorable(false);
	nAttr.setHidden(true);
	nAttr.setReadable(true);
	nAttr.setWritable(true);
	nAttr.setDefault(0.0f, 0.0f, 1.0f);

	MFnMatrixAttribute nMAttr;

	aInMatrix = nMAttr.create("inMatrix", "im", MFnMatrixAttribute::kFloat, &stat);
	nMAttr.setKeyable(false);
	nMAttr.setStorable(false);
	nMAttr.setReadable(false);
	nMAttr.setWritable(true);
	nMAttr.setConnectable(true);
	nMAttr.setHidden(true);



	aMatrixEyeToWorld = nMAttr.create("matrixEyeToWorld", "wte", MFnMatrixAttribute::kFloat, &stat);
	nMAttr.setKeyable(false);
	nMAttr.setStorable(false);
	nMAttr.setReadable(false);
	nMAttr.setWritable(true);
	nMAttr.setConnectable(true);
	nMAttr.setHidden(true);


	// Outputs

	aOutColor = nAttr.createColor("outColor", "oc");
	nAttr.setStorable(false);
	nAttr.setHidden(false);
	nAttr.setReadable(true);
	nAttr.setWritable(false);


	








	addAttribute(aFront);
	addAttribute(aSide);

	addAttribute(aColor2);
	addAttribute(aColor1);

	addAttribute(aNoise);

	addAttribute(aNormalCamera);
	addAttribute(aPointCamera);

	addAttribute(aInPoint);
	addAttribute(aInMatrix);
	addAttribute(aMatrixEyeToWorld);

	addAttribute(aOutColor);



	//attributeAffects (aInputValue, aOutColor) ;
	attributeAffects(aColor1, aOutColor);
	attributeAffects(aColor2, aOutColor);
	attributeAffects(aNormalCamera, aOutColor);
	attributeAffects(aPointCamera, aOutColor);

	attributeAffects(aFront, aOutColor);
	attributeAffects(aSide, aOutColor);
	attributeAffects(aNoise, aOutColor);

	attributeAffects(aInPoint, aOutColor);
	attributeAffects(aInMatrix, aOutColor);
	attributeAffects(aMatrixEyeToWorld, aOutColor);


	return MS::kSuccess;
}




MStatus freeFresnel::compute(
	const MPlug& plug,
	MDataBlock& block)
{
	if ((plug != aOutColor) && (plug.parent() != aOutColor))
		return MS::kUnknownParameter;



	MFloatVector& SideColor = block.inputValue(aColor1).asFloatVector();
	//DISPLAY_INFO(print, SideColor, "sideColor: ");

	MFloatVector& FrontColor = block.inputValue(aColor2).asFloatVector();
	//DISPLAY_INFO(print, FrontColor, "frontColor: ");

	MFloatVector& surfNorm = block.inputValue(aNormalCamera).asFloatVector();
	//DISPLAY_INFO(print, surfNorm, "surfNorm: ");

	MFloatVector& viewVector = block.inputValue(aPointCamera).asFloatVector();
	//DISPLAY_INFO(print, viewVector, "viewVector: ");



	MFloatMatrix& inMatrix = block.inputValue(aInMatrix).asFloatMatrix();
	//DISPLAY_INFO(print,inMatrix,"inMatrix =")
	MFloatMatrix& matrixEyeToWorld = block.inputValue(aMatrixEyeToWorld).asFloatMatrix();

	MFloatVector& inPoint = block.inputValue(aInPoint).asFloatVector();

	float frontValue = block.inputValue(aFront).asFloat();

	float sideValue = block.inputValue(aSide).asFloat();

	float noise = block.inputValue(aNoise).asFloat();



	MFloatVector resultColor;

	MFloatVector pointingVect = inPoint;
	pointingVect *= inMatrix;

	// Normalize the view vector
	viewVector.normalize();

	MFloatVector etwVector = surfNorm;

	etwVector *= matrixEyeToWorld;


	// find dot product
	float dotProduct = (pointingVect.x * etwVector.x) +
		(pointingVect.y * etwVector.y) +
		(pointingVect.z * etwVector.z);

	dotProduct = (dotProduct + 1) / 2;

	float scalar = 1;
	float scalarNormal;

	if (sideValue > frontValue)
	{
		sideValue = frontValue - 0.001f;
	}

	if (dotProduct > frontValue)
	{
		scalar = 0;
	}
	else if (dotProduct < sideValue)
	{
		scalar = 1;
	}
	else
	{
		scalarNormal = frontValue - dotProduct;
		scalar = ((scalarNormal * noise) + scalarNormal) / (frontValue - sideValue);

		if (scalar < 0)
		{
			scalar = 0;
		}
		else if (scalar > 1)
		{
			scalar = 1;
		}

	}

	resultColor = FrontColor + ((SideColor - FrontColor) * scalar);



	// set ouput color attribute
	MDataHandle outColorHandle = block.outputValue(aOutColor);
	MFloatVector& outColor = outColorHandle.asFloatVector();
	outColor = resultColor;
	outColorHandle.setClean();

	return MS::kSuccess;
}


MStatus initializePlugin(MObject obj)
{
	const MString UserClassify("utility/general");

	MFnPlugin plugin(obj, "Daniel Arzabe", "2022", "Any");
	plugin.registerNode("freeFresnel", freeFresnel::id,
		freeFresnel::creator, freeFresnel::initialize,
		MPxNode::kDependNode, &UserClassify);

	return MS::kSuccess;
}


MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);
	plugin.deregisterNode(freeFresnel::id);

	return MS::kSuccess;
}
