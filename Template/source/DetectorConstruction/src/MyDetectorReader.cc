//*********************************************
//  This is Geant4 Template
//                                  author:Qian
//

#include "MyDetectorReader.hh"

#include "G4GDMLReadStructure.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4AssemblyVolume.hh"
#include "G4ReflectionFactory.hh"
#include "G4PVDivisionFactory.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4VisAttributes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyDetectorReader::MyDetectorReader()
    : G4GDMLReadStructure()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MyDetectorReader::~MyDetectorReader()
{
    std::map<G4String, G4VisAttributes *>::iterator pos;
    for (pos = fAttribs.begin(); pos != fAttribs.end(); pos++)
    {
        delete pos->second;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyDetectorReader::ExtensionRead(const xercesc::DOMElement *const extElement)
{
    G4cout << "G4GDML: Reading GDML extension..." << G4endl;

    for (xercesc::DOMNode *iter = extElement->getFirstChild();
         iter != 0; iter = iter->getNextSibling())
    {
        if (iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE)
        {
            continue;
        }

        const xercesc::DOMElement *const child = dynamic_cast<xercesc::DOMElement *>(iter);
        const G4String tag = Transcode(child->getTagName());

        if (tag == "color")
        {
            ColorRead(child);
        }
        else
        {
            G4String error_msg = "Unknown tag in structure: " + tag;
            G4Exception("MyDetectorReader::ExtensionRead()",
                        "ReadError", FatalException, error_msg);
        }
    }
}

G4LogicalVolume *MyDetectorReader::MyFileRead(const xercesc::DOMElement *const fileElement)
{
    G4String name;
    G4String volname;

    const xercesc::DOMNamedNodeMap *const attributes = fileElement->getAttributes();
    XMLSize_t attributeCount = attributes->getLength();

    for (XMLSize_t attribute_index = 0;
         attribute_index < attributeCount; attribute_index++)
    {
        xercesc::DOMNode *attribute_node = attributes->item(attribute_index);

        if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
        {
            continue;
        }

        const xercesc::DOMAttr *const attribute = dynamic_cast<xercesc::DOMAttr *>(attribute_node);
        if (!attribute)
        {
            G4Exception("G4GDMLReadStructure::FileRead()",
                        "InvalidRead", FatalException, "No attribute found!");
            return 0;
        }
        const G4String attName = Transcode(attribute->getName());
        const G4String attValue = Transcode(attribute->getValue());

        if (attName == "name")
        {
            name = attValue;
        }
        else if (attName == "volname")
        {
            volname = attValue;
        }
    }

    const G4bool isModule = true;
    MyDetectorReader structure;
    structure.Read(name, validate, isModule);

    // Register existing auxiliar information defined in child module
    //
    const G4GDMLAuxMapType *aux = structure.GetAuxMap();
    if (!aux->empty())
    {
        G4GDMLAuxMapType::const_iterator pos;
        for (pos = aux->begin(); pos != aux->end(); ++pos)
        {
            auxMap.insert(std::make_pair(pos->first, pos->second));
        }
    }

    // Return volume structure from child module
    //

    if (volname.empty())
    {
        return structure.GetVolume(structure.GetSetup("Default"));
    }
    else
    {
        return structure.GetVolume(structure.GenerateName(volname));
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyDetectorReader::MyPhysvolRead(const xercesc::DOMElement *const physvolElement, G4AssemblyVolume *pAssembly)
{
    G4String name;
    G4LogicalVolume *logvol = 0;
    G4AssemblyVolume *assembly = 0;
    G4ThreeVector position(0.0, 0.0, 0.0);
    G4ThreeVector rotation(0.0, 0.0, 0.0);
    G4ThreeVector scale(1.0, 1.0, 1.0);
    G4int copynumber = 0;

    const xercesc::DOMNamedNodeMap *const attributes = physvolElement->getAttributes();
    XMLSize_t attributeCount = attributes->getLength();

    for (XMLSize_t attribute_index = 0;
         attribute_index < attributeCount; attribute_index++)
    {
        xercesc::DOMNode *attribute_node = attributes->item(attribute_index);

        if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
        {
            continue;
        }

        const xercesc::DOMAttr *const attribute = dynamic_cast<xercesc::DOMAttr *>(attribute_node);
        if (!attribute)
        {
            G4Exception("G4GDMLReadStructure::PhysvolRead()",
                        "InvalidRead", FatalException, "No attribute found!");
            return;
        }
        const G4String attName = Transcode(attribute->getName());
        const G4String attValue = Transcode(attribute->getValue());

        if (attName == "name")
        {
            name = attValue;
        }
        if (attName == "copynumber")
        {
            copynumber = eval.EvaluateInteger(attValue);
        }
    }

    for (xercesc::DOMNode *iter = physvolElement->getFirstChild();
         iter != 0; iter = iter->getNextSibling())
    {
        if (iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE)
        {
            continue;
        }

        const xercesc::DOMElement *const child = dynamic_cast<xercesc::DOMElement *>(iter);
        if (!child)
        {
            G4Exception("G4GDMLReadStructure::PhysvolRead()",
                        "InvalidRead", FatalException, "No child found!");
            return;
        }
        const G4String tag = Transcode(child->getTagName());

        if (tag == "volumeref")
        {
            const G4String &child_name = GenerateName(RefRead(child));
            assembly = GetAssembly(child_name);
            if (!assembly)
            {
                logvol = GetVolume(child_name);
            }
        }
        else if (tag == "file")
        {
            logvol = MyFileRead(child);
        }
        else if (tag == "position")
        {
            VectorRead(child, position);
        }
        else if (tag == "rotation")
        {
            VectorRead(child, rotation);
        }
        else if (tag == "scale")
        {
            VectorRead(child, scale);
        }
        else if (tag == "positionref")
        {
            position = GetPosition(GenerateName(RefRead(child)));
        }
        else if (tag == "rotationref")
        {
            rotation = GetRotation(GenerateName(RefRead(child)));
        }
        else if (tag == "scaleref")
        {
            scale = GetScale(GenerateName(RefRead(child)));
        }
        else
        {
            G4String error_msg = "Unknown tag in physvol: " + tag;
            G4Exception("G4GDMLReadStructure::PhysvolRead()", "ReadError",
                        FatalException, error_msg);
            return;
        }
    }

    G4Transform3D transform(GetRotationMatrix(rotation).inverse(), position);
    transform = transform * G4Scale3D(scale.x(), scale.y(), scale.z());

    if (pAssembly) // Fill assembly structure
    {
        if (!logvol)
        {
            return;
        }
        pAssembly->AddPlacedVolume(logvol, transform);
    }
    else // Generate physical volume tree or do assembly imprint
    {
        if (assembly)
        {
            assembly->MakeImprint(pMotherLogical, transform, 0, check);
        }
        else
        {
            if (!logvol)
            {
                return;
            }
            G4String pv_name = logvol->GetName() + "_PV";
            G4PhysicalVolumesPair pair = G4ReflectionFactory::Instance()
                                             ->Place(transform, pv_name, logvol, pMotherLogical, false, copynumber, check);

            if (pair.first != 0)
            {
                GeneratePhysvolName(name, pair.first);
            }
            if (pair.second != 0)
            {
                GeneratePhysvolName(name, pair.second);
            }
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyDetectorReader::Volume_contentRead(const xercesc::DOMElement *const volumeElement)
{
    for (xercesc::DOMNode *iter = volumeElement->getFirstChild();
         iter != 0; iter = iter->getNextSibling())
    {
        if (iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE)
        {
            continue;
        }

        const xercesc::DOMElement *const child = dynamic_cast<xercesc::DOMElement *>(iter);
        if (!child)
        {
            G4Exception("G4GDMLReadStructure::Volume_contentRead()",
                        "InvalidRead", FatalException, "No child found!");
            return;
        }
        const G4String tag = Transcode(child->getTagName());

        if ((tag == "auxiliary") || (tag == "materialref") || (tag == "solidref"))
        {
            // These are already processed in VolumeRead()
        }
        else if (tag == "paramvol")
        {
            ParamvolRead(child, pMotherLogical);
        }
        else if (tag == "physvol")
        {
            MyPhysvolRead(child);
        }
        else if (tag == "replicavol")
        {
            G4int number = 1;
            const xercesc::DOMNamedNodeMap *const attributes = child->getAttributes();
            XMLSize_t attributeCount = attributes->getLength();
            for (XMLSize_t attribute_index = 0;
                 attribute_index < attributeCount; attribute_index++)
            {
                xercesc::DOMNode *attribute_node = attributes->item(attribute_index);
                if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
                {
                    continue;
                }
                const xercesc::DOMAttr *const attribute = dynamic_cast<xercesc::DOMAttr *>(attribute_node);
                if (!attribute)
                {
                    G4Exception("G4GDMLReadStructure::Volume_contentRead()",
                                "InvalidRead", FatalException, "No attribute found!");
                    return;
                }
                const G4String attName = Transcode(attribute->getName());
                const G4String attValue = Transcode(attribute->getValue());
                if (attName == "number")
                {
                    number = eval.EvaluateInteger(attValue);
                }
            }
            ReplicavolRead(child, number);
        }
        else if (tag == "divisionvol")
        {
            DivisionvolRead(child);
        }
        else if (tag == "loop")
        {
            LoopRead(child, &G4GDMLRead::Volume_contentRead);
        }
        else if (tag == "colorref")
        {
        }
        else
        {
            G4cout << "Treating unknown GDML tag in volume '" << tag
                   << "' as GDML extension..." << G4endl;
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyDetectorReader::VolumeRead(const xercesc::DOMElement *const volumeElement)
{
    G4VSolid *solidPtr = 0;
    G4Material *materialPtr = 0;
    G4VisAttributes *attrPtr = 0;
    G4GDMLAuxListType auxList;

    XMLCh *name_attr = xercesc::XMLString::transcode("name");
    const G4String name = Transcode(volumeElement->getAttribute(name_attr));
    xercesc::XMLString::release(&name_attr);

    for (xercesc::DOMNode *iter = volumeElement->getFirstChild();
         iter != 0; iter = iter->getNextSibling())
    {
        if (iter->getNodeType() != xercesc::DOMNode::ELEMENT_NODE)
        {
            continue;
        }

        const xercesc::DOMElement *const child = dynamic_cast<xercesc::DOMElement *>(iter);
        if (!child)
        {
            G4Exception("G4GDMLReadStructure::VolumeRead()",
                        "InvalidRead", FatalException, "No child found!");
            return;
        }
        const G4String tag = Transcode(child->getTagName());

        if (tag == "auxiliary")
        {
            auxList.push_back(AuxiliaryRead(child));
        }
        else if (tag == "materialref")
        {
            materialPtr = GetMaterial(GenerateName(RefRead(child), true));
        }
        else if (tag == "solidref")
        {
            solidPtr = GetSolid(GenerateName(RefRead(child)));
        }
        else if (tag == "colorref")
        {
            attrPtr = GetVisAttribute(GenerateName(RefRead(child)));
        }
    }

    pMotherLogical = new G4LogicalVolume(solidPtr, materialPtr,
                                         GenerateName(name), 0, 0, 0);
    pMotherLogical->SetVisAttributes(attrPtr);

    if (!auxList.empty())
    {
        auxMap[pMotherLogical] = auxList;
    }

    Volume_contentRead(volumeElement);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyDetectorReader::ColorRead(const xercesc::DOMElement *const colorElement)
{
    G4String name;
    G4VisAttributes *color = 0;
    G4double r = 0., g = 0., b = 0., a = 0.;

    const xercesc::DOMNamedNodeMap *const attributes = colorElement->getAttributes();
    XMLSize_t attributeCount = attributes->getLength();

    for (XMLSize_t attribute_index = 0;
         attribute_index < attributeCount; attribute_index++)
    {
        xercesc::DOMNode *attribute_node = attributes->item(attribute_index);

        if (attribute_node->getNodeType() != xercesc::DOMNode::ATTRIBUTE_NODE)
        {
            continue;
        }

        const xercesc::DOMAttr *const attribute = dynamic_cast<xercesc::DOMAttr *>(attribute_node);
        const G4String attName = Transcode(attribute->getName());
        const G4String attValue = Transcode(attribute->getValue());

        if (attName == "name")
        {
            name = GenerateName(attValue);
        }
        else if (attName == "R")
        {
            r = eval.Evaluate(attValue);
        }
        else if (attName == "G")
        {
            g = eval.Evaluate(attValue);
        }
        else if (attName == "B")
        {
            b = eval.Evaluate(attValue);
        }
        else if (attName == "A")
        {
            a = eval.Evaluate(attValue);
        }
    }

    G4cout << "Color attribute (R,G,B,A) is: "
           << r << ", " << g << ", " << b << ", " << a << " !" << G4endl;
    color = new G4VisAttributes(G4Color(r, g, b, a));
    fAttribs.insert(std::make_pair(name, color));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VisAttributes *MyDetectorReader::GetVisAttribute(const G4String &ref)
{
    G4VisAttributes *col = 0;
    std::map<G4String, G4VisAttributes *>::iterator pos = fAttribs.find(ref);

    if (pos != fAttribs.end())
    {
        col = pos->second;
    }
    else
    {
        G4String err_mess = "Attribute: " + ref + " NOT found !";
        G4Exception("MyDetectorReader::GetVisAttribute()",
                    "ReadError", FatalException, err_mess);
    }
    return col;
}