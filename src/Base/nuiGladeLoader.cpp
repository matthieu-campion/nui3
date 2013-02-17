/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#include "nui.h"

bool NodeToAttribute(nuiXMLNode* pFrom, nuiXMLNode* pTo, nglChar* from, nglChar* to = NULL);

bool NodeToAttribute(nuiXMLNode* pFrom, nuiXMLNode* pTo, nglChar* from, nglChar* to)
{
  nuiXMLNode* pNode = NULL;
  if (!from) 
    return false;
  if (!to) 
    to = from;

  pNode = pFrom->GetChild(nglString(from));
  if (pNode)
  {
    nuiXMLNode* pTextNode = pNode->GetChild(nglString(_T("##text")));
    if (pTextNode)
    {
      pTo->SetAttribute(nglString(to), pTextNode->GetValue());
      return true;
    }
    else 
      return false;
  }
  else 
    return false;
}

nglString GetNodeText(nuiXMLNode* pFrom, nglChar* from)
{
  if (!from) 
    return _T("");

  nuiXMLNode* pNode = pFrom->GetChild(nglString(from));
  if (pNode)
  {
    nuiXMLNode* pTextNode = pNode->GetChild(nglString(_T("##text")));
    if (pTextNode)
    {
      return pTextNode->GetValue();
    }
  }
  return nglString();
}

bool RecursiveGladeImport(nuiXMLNode* pFrom, nuiXMLNode* pTo)
{
  bool havechildren = true;
  nglString Name;
  if (! pFrom || !pTo)
    return false;

  Name = pFrom->GetName();
  if (Name == _T("widget"))
  {
    nuiXMLNode* pNode = pFrom->GetChild(nglString(_T("class")));

    if (pNode)
    {
      nuiXMLNode* pTextNode = pNode->GetChild(nglString(_T("##text")));
      if (pTextNode)
      {
        if (pTextNode->GetValue() == nglString(_T("GtkWindow")) )
        {
          pTo->SetName( nglString(_T("nuiWindow")) );
          pTo->SetAttribute( _T("Visible"), _T("true"));
          NodeToAttribute(pFrom, pTo, (nglChar*)_T("Title"));
          havechildren = true;
        }
        else if (pTextNode->GetValue() == nglString(_T("GtkFixed")) )
        {
          pTo->SetName( nglString(_T("nuiFixed")) );
          havechildren = true;
        }
        else if (pTextNode->GetValue() == nglString(_T("GtkList")) )
        {
          pTo->SetName( nglString(_T("nuiList")) );
          havechildren = true;
        }
        else if (pTextNode->GetValue() == nglString(_T("GtkVBox")) )
        {
          pTo->SetName( nglString(_T("nuiVBox")) );
          havechildren = true;
        }
        else if (pTextNode->GetValue() == nglString(_T("GtkHBox")) )
        {
          pTo->SetName( nglString(_T("nuiHBox")) );
          havechildren = true;
        }
        else if (pTextNode->GetValue() == nglString(_T("GtkButton")) )
        {
          pTo->SetName( nglString(_T("nuiButton")) );
          nglString label = GetNodeText(pFrom, (nglChar*)_T("label"));
          if (label!= _T(""))
          {
            nuiXMLNode* pLabel = new nuiXMLNode(nglString(_T("nuiLabel")),pTo);
            nuiXMLNode* pText = new nuiXMLNode(nglString(_T("##text")),pLabel);
            pText->SetValue(label);
          }
          havechildren = false;
        }
        else if (pTextNode->GetValue() == nglString(_T("GtkLabel")) )
        {
          pTo->SetName( nglString(_T("nuiLabel")) );
          nuiXMLNode* pText = new nuiXMLNode(nglString(_T("##text")),pTo);
          pText->SetValue(GetNodeText(pFrom, (nglChar*)_T("label")));

          havechildren = false;
        }
        else if (pTextNode->GetValue() == nglString(_T("GtkText")) )
        {
          if (GetNodeText(pFrom, (nglChar*)_T("editable")) == nglString(_T("True")))
            pTo->SetName( nglString(_T("nuiEditText")) );
          else
            pTo->SetName( nglString(_T("nuiText")) );
          nglString text = GetNodeText(pFrom, (nglChar*)_T("text"));
          if (text!= _T(""))
          {
            nuiXMLNode* pText = new nuiXMLNode(nglString(_T("##text")),pTo);
            pText->SetValue(text);
          }
          havechildren = false;
        }
        else 
          return false;
      }
      else 
        return false;
    }
    else 
      return false;

    if (!NodeToAttribute(pFrom, pTo, (nglChar*)_T("name")))
      return false;

    NodeToAttribute(pFrom, pTo, (nglChar*)_T("x"),(nglChar*) _T("X"));
    NodeToAttribute(pFrom, pTo, (nglChar*)_T("y"), (nglChar*)_T("Y"));
    NodeToAttribute(pFrom, pTo, (nglChar*)_T("width"), (nglChar*)_T("Width"));
    NodeToAttribute(pFrom, pTo, (nglChar*)_T("height"), (nglChar*)_T("Height"));


    if (havechildren)
    {
      uint i, count = pFrom->GetChildrenCount();
      for (i=0; i<count; i++)
      {
        nuiXMLNode* pFromNode = pFrom->GetChild(i);
        if (pFromNode->GetName() == nglString(_T("widget")))
        {
          nuiXMLNode* pToNode = new nuiXMLNode(nglString(_T("importing")), pTo);
          if (!RecursiveGladeImport(pFromNode,pToNode))
          {
            // If there was an error just kill the new node...
            pTo->DelChild(pToNode);
            delete pToNode;
          }
        }
      }
    }

  }

  return true;
}

nuiXML* ImportGladeXML(nglChar* xmlfile)
{
  nuiXML* pFrom = new nuiXML(nglString(_T("gladefile")));
  nuiXML* pTo   = new nuiXML(nglString(_T("NUI-Interface")));
  nglIFile fromfile(nglPath((char*)xmlfile));
  if (pFrom->Load(fromfile))
  {
    if (pFrom->GetName()== _T("GTK-Interface"))
    {
      uint i, count = pFrom->GetChildrenCount();

      for (i=0; i<count; i++)
      {
        nuiXMLNode* pNode = pFrom->GetChild(i);
        if (!RecursiveGladeImport(pNode,pTo))
        {
          delete pFrom;
          delete pTo;
          return NULL;
        }
      }

      delete pFrom;
      return pTo;
    }
    
  }

  delete pFrom;
  delete pTo;
  return NULL;
}

