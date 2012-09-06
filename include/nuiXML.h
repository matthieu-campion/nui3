/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#ifndef __nuiXML_h__
#define __nuiXML_h__

//#include "nui.h"
#include "nuiFlags.h"
#include "nglString.h"
#include "nglIStream.h"
#include "nglOStream.h"
//#include "nuiApplication.h"

class xmlLexer;

class NUI_API nuiStringLessFunctor : public std::binary_function <std::string, std::string, bool>
{
public:
  bool operator()(const std::string& rLeft, const std::string& rRight) const
  {
    return strcmp(rLeft.c_str(), rRight.c_str()) < 0;
  }
};

/*
typedef STDEXT::hash_compare<std::string, nuiStringLessFunctor> nuiStringHashCompare;

typedef STDEXT::hash_map<std::string,std::string, nuiStringHashCompare > nuiXMLAttributeList;
*/
//typedef std::map<std::string,std::string, nuiStringLessFunctor > nuiXMLAttributeList;
typedef std::map<nglString,nglString, nglString::NaturalLessFunctor> nuiXMLAttributeList;
class nuiXMLNode;
typedef std::list<nuiXMLNode*> nuiXMLNodeList;

/// This class implements a very basic (but useful) XML node based on nglString. You will probaly use it in conjonction with the nuiXML class.
class NUI_API nuiXMLNode
{
public:
  nuiXMLNode(const char* pName, nuiXMLNode* pParent);
  nuiXMLNode(const nglString& rName, nuiXMLNode* pParent=NULL); ///< Create a node with the given name and attach it to its parent if it is not NULL.
  virtual ~nuiXMLNode();

  nuiXMLNode* Clone(nuiXMLNode* pParent=NULL) const;

  // Node management:
  const nglString& GetName() const; ///< Return the name of the node.
  void SetName(const nglString& rName); ///< Set the name of the node.

  const nglString& GetValue() const; ///< Return the text value of the node or an empty string if the node is not a text node (its name != ##text).
  void SetValue(const nglString& rValue); ///< Transforms the node into a text node if it is not allready one and sets its text.

  // Node attributes management:

  void SetAttribute(const nglString& rName, const nglString& rValue); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const nglString& rName, const nglChar* pValue); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const nglString& rName, uint8 value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const nglString& rName, int8 value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const nglString& rName, uint16 value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const nglString& rName, int16 value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const nglString& rName, uint32 value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const nglString& rName, int32 value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const nglString& rName, uint64 value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const nglString& rName, int64 value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const nglString& rName, bool value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const nglString& rName, float value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const nglString& rName, double value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
#ifndef _MINUI3_
  void SetAttribute(const nglString& rName, nuiPosition value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const nglString& rName, nuiDirection value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const nglString& rName, nuiOrientation value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
#endif

  bool DelAttribute(const nglString& rName); ///< Remove an attribute from this node. If the attribute doesn't exists on the node nothing happens.

  bool HasAttribute(const nglString& rName) const; ///< Return true if the given attribute is present on the node.

  const nglString& GetAttribute(const nglString& rName) const; ///< Return the value of the given attribute. If the attribute doesn't exists on this object the returned string will be nglString::Null.

  // char* versions:
  void SetName(const char* pName); ///< Set the name of the node.

  void SetValue(const char* pValue); ///< Transforms the node into a text node if it is not allready one and sets its text.

  // Node attributes management:

  void SetAttribute(const char* pName, const nglString& rValue);
  void SetAttribute(const char* pName, const nglChar* pValue); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const char* pName, uint8 value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const char* pName, int8 value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const char* pName, uint16 value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const char* pName, int16 value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const char* pName, uint32 value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const char* pName, int32 value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const char* pName, uint64 value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const char* pName, int64 value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const char* pName, bool value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const char* pName, float value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const char* pName, double value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
#ifndef _MINUI3_
  void SetAttribute(const char* pName, nuiPosition value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const char* pName, nuiDirection value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
  void SetAttribute(const char* pName, nuiOrientation value); ///< Set the value of an attribute given its name. If the attribute doesn't exits on this object it will be added and its value updated.
#endif

  bool DelAttribute(const char* pName); ///< Remove an attribute from this node. If the attribute doesn't exists on the node nothing happens.

  bool HasAttribute(const char* pName) const; ///< Return true if the given attribute is present on the node.

  const nglString& GetAttribute(const char* pName) const; ///< Return the value of the given attribute. If the attribute doesn't exists on this object the returned string will be empty.




  //////
  uint      GetAttributeCount() const; ///< Return the number of attributes presents on this object.
  const nglString& GetAttributeName (uint index) const; ///< Return the name of the attribute with the given index.
  const nglString& GetAttributeValue(uint index) const; ///< Return the value of the attribute with the given index.

  // Node tree management:
  const nuiXMLNodeList& GetChildren() const; ///< Return a copy of the list of children of this node. You must not change any child directly from this list. Removing or adding a child from the returned list will not remove if from the node, you must use the other node methods to do that. You must not change a node directly either.
  bool AddChild(nuiXMLNode* pNode); ///< Add the given node to the list of children. If the child is allready there nothing happens. Return true on success.
  bool DelChild(nuiXMLNode* pNode); ///< Remove the given node from the list of children.
  uint GetChildrenCount() const; ///< Return the number of children of this node.
  nuiXMLNode* GetChild(uint index) const; ///< Return the child with the given index on this node's children list.
  nuiXMLNode* GetChild(const nglString& rName) const; ///< Return the child with the given name on this node's children list.

  nuiXMLNode* Find(const nglString& rName) const; ///< Finds a node given its full path relative to the current node. Eg. Find("background/color/red").

  // Debug:
  nglString Dump(uint level = 0) const; ///< Output the tree in the console (set the tab indent to \param level)..
  const nuiXMLNode* GetParent() const;
  nuiXMLNode* GetParent();
  const nuiXMLNode* GetRoot() const;
  nuiXMLNode* GetRoot();

  void SetTag(void* pTag);
  void* GetTag() const;

  virtual int64 Write(nglOStream& rStream, uint level = 0) const;
protected:
  nuiXMLNode(xmlLexer* pLexer, nuiXMLNode* pParent);
  virtual bool ParseXMLNode(xmlLexer* pLexer, nglString& str);
  virtual bool ParseXMLCommand(xmlLexer* pLexer, nglString& str);

  nglString mName;
  nglString mValue;
  nuiXMLAttributeList mAttributes;
  nuiXMLNodeList mpChildren;
  nuiXMLNode* mpParent;
  void SetParent(nuiXMLNode* pParent);
  void* mpTag;
private:
  nuiXMLNode(const nuiXMLNode& rOriginal);
};

/// This class implements a very basic (but useful) XML parser/saver based on nglString. Is the basis of the xml widget tree loading/saving scheme.
class NUI_API nuiXML : public nuiXMLNode
{
public:
  nuiXML(const nglString& rName = nglString::Null);
  nuiXML(const char* pName);
  virtual ~nuiXML();

  virtual bool Load(nglIStream& rStream); ///< Load an existing xml file from a stream.
  virtual bool Save(nglOStream& rStream) const; ///< Save the current tree to a stream.

  virtual nglString Dump(uint level = 0) const; ///< Return the contents of the object as a string.
  virtual void SetDTD(nglString name, nglString file); ///< Set the DTD of this XML doc.
  virtual void SetStyleSheet(nglString type, nglString file); ///< Set the Style sheet of this xml doc.
  virtual const nglString& GetDTDName(); ///< Get the DTD Name of this XML doc.
  virtual const nglString& GetDTDFile(); ///< Get the DTD File name of this XML doc.
  virtual const nglString& GetStyleSheetType(); ///< Get the Style sheet type of this xml doc.
  virtual const nglString& GetStyleSheetFile(); ///< Get the Style sheet file name of this xml doc.

  virtual int64 Write(nglOStream& rStream, uint level = 0) const;
protected:
  virtual bool ParseXMLHeader(xmlLexer* pLexer, nglString& str);
private:
  nuiXMLNode* mpChild;
  nglString mDTDName;
  nglString mDTDFile;
  nglString mStyleSheetType;
  nglString mStyleSheetFile;

};

typedef char nuiXML_Char;
struct XML_ParserStruct;
typedef struct XML_ParserStruct *XML_Parser;

class nuiXMLParser ///< nui's SAX Parser
{
public:
  nuiXMLParser();
  virtual ~nuiXMLParser();

  virtual bool Parse(nglIStream* pStream);
  void Stop();

  virtual void StartElement(const nuiXML_Char* name, const nuiXML_Char** atts);
  virtual void EndElement(const nuiXML_Char* name);
  virtual void Characters(const nuiXML_Char* s, int len);
  virtual void ProcessingInstruction(const nuiXML_Char* target, const nuiXML_Char* data);
  virtual void Comment(const nuiXML_Char* data);

protected:
  static void StartElement(void* pThis, const nuiXML_Char* name, const nuiXML_Char** atts);
  static void EndElement(void* pThis, const nuiXML_Char* name);
  static void Characters(void* pThis, const nuiXML_Char* s, int len);
  static void ProcessingInstruction(void* pThis, const nuiXML_Char* target, const nuiXML_Char* data);
  static void Comment(void* pThis, const nuiXML_Char* data);

  nglIStream* mpStream;

  //    static const uint32 BufferSize = 1024;
  static const uint32 BufferSize = 4096;

  XML_Parser mParser;
};


#endif // __nuiXML_h__
