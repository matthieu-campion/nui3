/*
  NUI3 - C++ cross-platform GUI framework for OpenGL based applications
  Copyright (C) 2002-2003 Sebastien Metrot

  licence: see nui3/LICENCE.TXT
*/

#pragma once

class nuiHTML;
class nuiHTMLNode;
class nuiHTMLAttrib;

class nuiHTMLAttrib
{
public:
  /// Known HTML attributes
  typedef enum
  {
    eAttrib_UNKNOWN,           /**< UNKNOWN= */
    eAttrib_ABBR,              /**< ABBR= */
    eAttrib_ACCEPT,            /**< ACCEPT= */
    eAttrib_ACCEPT_CHARSET,    /**< ACCEPT_CHARSET= */
    eAttrib_ACCESSKEY,         /**< ACCESSKEY= */
    eAttrib_ACTION,            /**< ACTION= */
    eAttrib_ADD_DATE,          /**< ADD_DATE= */
    eAttrib_ALIGN,             /**< ALIGN= */
    eAttrib_ALINK,             /**< ALINK= */
    eAttrib_ALT,               /**< ALT= */
    eAttrib_ARCHIVE,           /**< ARCHIVE= */
    eAttrib_AXIS,              /**< AXIS= */
    eAttrib_BACKGROUND,        /**< BACKGROUND= */
    eAttrib_BGCOLOR,           /**< BGCOLOR= */
    eAttrib_BGPROPERTIES,      /**< BGPROPERTIES= */
    eAttrib_BORDER,            /**< BORDER= */
    eAttrib_BORDERCOLOR,       /**< BORDERCOLOR= */
    eAttrib_BOTTOMMARGIN,      /**< BOTTOMMARGIN= */
    eAttrib_CELLPADDING,       /**< CELLPADDING= */
    eAttrib_CELLSPACING,       /**< CELLSPACING= */
    eAttrib_CHAR,              /**< CHAR= */
    eAttrib_CHAROFF,           /**< CHAROFF= */
    eAttrib_CHARSET,           /**< CHARSET= */
    eAttrib_CHECKED,           /**< CHECKED= */
    eAttrib_CITE,              /**< CITE= */
    eAttrib_CLASS,             /**< CLASS= */
    eAttrib_CLASSID,           /**< CLASSID= */
    eAttrib_CLEAR,             /**< CLEAR= */
    eAttrib_CODE,              /**< CODE= */
    eAttrib_CODEBASE,          /**< CODEBASE= */
    eAttrib_CODETYPE,          /**< CODETYPE= */
    eAttrib_COLOR,             /**< COLOR= */
    eAttrib_COLS,              /**< COLS= */
    eAttrib_COLSPAN,           /**< COLSPAN= */
    eAttrib_COMPACT,           /**< COMPACT= */
    eAttrib_CONTENT,           /**< CONTENT= */
    eAttrib_COORDS,            /**< COORDS= */
    eAttrib_DATA,              /**< DATA= */
    eAttrib_DATAFLD,           /**< DATAFLD= */
    eAttrib_DATAFORMATAS,      /**< DATAFORMATAS= */
    eAttrib_DATAPAGESIZE,      /**< DATAPAGESIZE= */
    eAttrib_DATASRC,           /**< DATASRC= */
    eAttrib_DATETIME,          /**< DATETIME= */
    eAttrib_DECLARE,           /**< DECLARE= */
    eAttrib_DEFER,             /**< DEFER= */
    eAttrib_DIR,               /**< DIR= */
    eAttrib_DISABLED,          /**< DISABLED= */
    eAttrib_ENCODING,          /**< ENCODING= */
    eAttrib_ENCTYPE,           /**< ENCTYPE= */
    eAttrib_FACE,              /**< FACE= */
    eAttrib_FOR,               /**< FOR= */
    eAttrib_FRAME,             /**< FRAME= */
    eAttrib_FRAMEBORDER,       /**< FRAMEBORDER= */
    eAttrib_FRAMESPACING,      /**< FRAMESPACING= */
    eAttrib_GRIDX,             /**< GRIDX= */
    eAttrib_GRIDY,             /**< GRIDY= */
    eAttrib_HEADERS,           /**< HEADERS= */
    eAttrib_HEIGHT,            /**< HEIGHT= */
    eAttrib_HREF,              /**< HREF= */
    eAttrib_HREFLANG,          /**< HREFLANG= */
    eAttrib_HSPACE,            /**< HSPACE= */
    eAttrib_HTTP_EQUIV,        /**< HTTP_EQUIV= */
    eAttrib_ID,                /**< ID= */
    eAttrib_ISMAP,             /**< ISMAP= */
    eAttrib_LABEL,             /**< LABEL= */
    eAttrib_LANG,              /**< LANG= */
    eAttrib_LANGUAGE,          /**< LANGUAGE= */
    eAttrib_LAST_MODIFIED,     /**< LAST_MODIFIED= */
    eAttrib_LAST_VISIT,        /**< LAST_VISIT= */
    eAttrib_LEFTMARGIN,        /**< LEFTMARGIN= */
    eAttrib_LINK,              /**< LINK= */
    eAttrib_LONGDESC,          /**< LONGDESC= */
    eAttrib_LOWSRC,            /**< LOWSRC= */
    eAttrib_MARGINHEIGHT,      /**< MARGINHEIGHT= */
    eAttrib_MARGINWIDTH,       /**< MARGINWIDTH= */
    eAttrib_MAXLENGTH,         /**< MAXLENGTH= */
    eAttrib_MEDIA,             /**< MEDIA= */
    eAttrib_METHOD,            /**< METHOD= */
    eAttrib_MULTIPLE,          /**< MULTIPLE= */
    eAttrib_NAME,              /**< NAME= */
    eAttrib_NOHREF,            /**< NOHREF= */
    eAttrib_NORESIZE,          /**< NORESIZE= */
    eAttrib_NOSHADE,           /**< NOSHADE= */
    eAttrib_NOWRAP,            /**< NOWRAP= */
    eAttrib_OBJECT,            /**< OBJECT= */
    eAttrib_OnAFTERUPDATE,     /**< OnAFTERUPDATE= */
    eAttrib_OnBEFOREUNLOAD,    /**< OnBEFOREUNLOAD= */
    eAttrib_OnBEFOREUPDATE,    /**< OnBEFOREUPDATE= */
    eAttrib_OnBLUR,            /**< OnBLUR= */
    eAttrib_OnCHANGE,          /**< OnCHANGE= */
    eAttrib_OnCLICK,           /**< OnCLICK= */
    eAttrib_OnDATAAVAILABLE,   /**< OnDATAAVAILABLE= */
    eAttrib_OnDATASETCHANGED,  /**< OnDATASETCHANGED= */
    eAttrib_OnDATASETCOMPLETE, /**< OnDATASETCOMPLETE= */
    eAttrib_OnDBLCLICK,        /**< OnDBLCLICK= */
    eAttrib_OnERRORUPDATE,     /**< OnERRORUPDATE= */
    eAttrib_OnFOCUS,           /**< OnFOCUS= */
    eAttrib_OnKEYDOWN,         /**< OnKEYDOWN= */
    eAttrib_OnKEYPRESS,        /**< OnKEYPRESS= */
    eAttrib_OnKEYUP,           /**< OnKEYUP= */
    eAttrib_OnLOAD,            /**< OnLOAD= */
    eAttrib_OnMOUSEDOWN,       /**< OnMOUSEDOWN= */
    eAttrib_OnMOUSEMOVE,       /**< OnMOUSEMOVE= */
    eAttrib_OnMOUSEOUT,        /**< OnMOUSEOUT= */
    eAttrib_OnMOUSEOVER,       /**< OnMOUSEOVER= */
    eAttrib_OnMOUSEUP,         /**< OnMOUSEUP= */
    eAttrib_OnRESET,           /**< OnRESET= */
    eAttrib_OnROWENTER,        /**< OnROWENTER= */
    eAttrib_OnROWEXIT,         /**< OnROWEXIT= */
    eAttrib_OnSELECT,          /**< OnSELECT= */
    eAttrib_OnSUBMIT,          /**< OnSUBMIT= */
    eAttrib_OnUNLOAD,          /**< OnUNLOAD= */
    eAttrib_PROFILE,           /**< PROFILE= */
    eAttrib_PROMPT,            /**< PROMPT= */
    eAttrib_RBSPAN,            /**< RBSPAN= */
    eAttrib_READONLY,          /**< READONLY= */
    eAttrib_REL,               /**< REL= */
    eAttrib_REV,               /**< REV= */
    eAttrib_RIGHTMARGIN,       /**< RIGHTMARGIN= */
    eAttrib_ROWS,              /**< ROWS= */
    eAttrib_ROWSPAN,           /**< ROWSPAN= */
    eAttrib_RULES,             /**< RULES= */
    eAttrib_SCHEME,            /**< SCHEME= */
    eAttrib_SCOPE,             /**< SCOPE= */
    eAttrib_SCROLLING,         /**< SCROLLING= */
    eAttrib_SELECTED,          /**< SELECTED= */
    eAttrib_SHAPE,             /**< SHAPE= */
    eAttrib_SHOWGRID,          /**< SHOWGRID= */
    eAttrib_SHOWGRIDX,         /**< SHOWGRIDX= */
    eAttrib_SHOWGRIDY,         /**< SHOWGRIDY= */
    eAttrib_SIZE,              /**< SIZE= */
    eAttrib_SPAN,              /**< SPAN= */
    eAttrib_SRC,               /**< SRC= */
    eAttrib_STANDBY,           /**< STANDBY= */
    eAttrib_START,             /**< START= */
    eAttrib_STYLE,             /**< STYLE= */
    eAttrib_SUMMARY,           /**< SUMMARY= */
    eAttrib_TABINDEX,          /**< TABINDEX= */
    eAttrib_TARGET,            /**< TARGET= */
    eAttrib_TEXT,              /**< TEXT= */
    eAttrib_TITLE,             /**< TITLE= */
    eAttrib_TOPMARGIN,         /**< TOPMARGIN= */
    eAttrib_TYPE,              /**< TYPE= */
    eAttrib_USEMAP,            /**< USEMAP= */
    eAttrib_VALIGN,            /**< VALIGN= */
    eAttrib_VALUE,             /**< VALUE= */
    eAttrib_VALUETYPE,         /**< VALUETYPE= */
    eAttrib_VERSION,           /**< VERSION= */
    eAttrib_VLINK,             /**< VLINK= */
    eAttrib_VSPACE,            /**< VSPACE= */
    eAttrib_WIDTH,             /**< WIDTH= */
    eAttrib_WRAP,              /**< WRAP= */
    eAttrib_XML_LANG,          /**< XML_LANG= */
    eAttrib_XML_SPACE,         /**< XML_SPACE= */
    eAttrib_XMLNS,             /**< XMLNS= */
    
    eAttrib_EVENT,             /**< EVENT= */
    eAttrib_METHODS,           /**< METHODS= */
    eAttrib_N,                 /**< N= */
    eAttrib_SDAFORM,           /**< SDAFORM= */
    eAttrib_SDAPREF,           /**< SDAPREF= */
    eAttrib_SDASUFF,           /**< SDASUFF= */
    eAttrib_URN,               /**< URN= */
  } AttributeType;

  AttributeType GetType() const;
  const nglString& GetName() const;
  const nglString& GetValue() const;
  
private:
  friend class nuiHTMLNode;
  nuiHTMLAttrib(AttributeType type, const nglString& rName, const nglString& rValue);
  nuiHTMLAttrib(const void* tattrib, nglTextEncoding encoding);
  ~nuiHTMLAttrib();

  AttributeType mType;
  nglString mName;
  nglString mValue;
};

class nuiHTMLNode : public nuiTokenHolder // We make nuiHTMLNode an nuiTokenHolder in order to store private date on the object (might be needed by a HTML renderer)
{
public:
  /// Node types
  typedef enum 
  {
    eNode_Root,        /**< Root */
    eNode_DocType,     /**< DOCTYPE */
    eNode_Comment,     /**< Comment */
    eNode_ProcIns,     /**< Processing Instruction */
    eNode_Text,        /**< Text */
    eNode_Start,       /**< Start Tag */
    eNode_End,         /**< End Tag */
    eNode_StartEnd,    /**< Start/End (empty) Tag */
    eNode_CDATA,       /**< Unparsed Text */
    eNode_Section,     /**< XML Section */
    eNode_Asp,         /**< ASP Sou§rce */
    eNode_Jste,        /**< JSTE Source */
    eNode_Php,         /**< PHP Source */
    eNode_XmlDecl      /**< XML Declaration */
  } NodeType;
  
  
  /// Known HTML element types
  typedef enum
  {
    eTag_UNKNOWN,  /**< Unknown tag! */
    eTag_A,        /**< A */
    eTag_ABBR,     /**< ABBR */
    eTag_ACRONYM,  /**< ACRONYM */
    eTag_ADDRESS,  /**< ADDRESS */
    eTag_ALIGN,    /**< ALIGN */
    eTag_APPLET,   /**< APPLET */
    eTag_AREA,     /**< AREA */
    eTag_B,        /**< B */
    eTag_BASE,     /**< BASE */
    eTag_BASEFONT, /**< BASEFONT */
    eTag_BDO,      /**< BDO */
    eTag_BGSOUND,  /**< BGSOUND */
    eTag_BIG,      /**< BIG */
    eTag_BLINK,    /**< BLINK */
    eTag_BLOCKQUOTE,   /**< BLOCKQUOTE */
    eTag_BODY,     /**< BODY */
    eTag_BR,       /**< BR */
    eTag_BUTTON,   /**< BUTTON */
    eTag_CAPTION,  /**< CAPTION */
    eTag_CENTER,   /**< CENTER */
    eTag_CITE,     /**< CITE */
    eTag_CODE,     /**< CODE */
    eTag_COL,      /**< COL */
    eTag_COLGROUP, /**< COLGROUP */
    eTag_COMMENT,  /**< COMMENT */
    eTag_DD,       /**< DD */
    eTag_DEL,      /**< DEL */
    eTag_DFN,      /**< DFN */
    eTag_DIR,      /**< DIR */
    eTag_DIV,      /**< DIF */
    eTag_DL,       /**< DL */
    eTag_DT,       /**< DT */
    eTag_EM,       /**< EM */
    eTag_EMBED,    /**< EMBED */
    eTag_FIELDSET, /**< FIELDSET */
    eTag_FONT,     /**< FONT */
    eTag_FORM,     /**< FORM */
    eTag_FRAME,    /**< FRAME */
    eTag_FRAMESET, /**< FRAMESET */
    eTag_H1,       /**< H1 */
    eTag_H2,       /**< H2 */
    eTag_H3,       /**< H3 */
    eTag_H4,       /**< H4 */
    eTag_H5,       /**< H5 */
    eTag_H6,       /**< H6 */
    eTag_HEAD,     /**< HEAD */
    eTag_HR,       /**< HR */
    eTag_HTML,     /**< HTML */
    eTag_I,        /**< I */
    eTag_IFRAME,   /**< IFRAME */
    eTag_ILAYER,   /**< ILAYER */
    eTag_IMG,      /**< IMG */
    eTag_INPUT,    /**< INPUT */
    eTag_INS,      /**< INS */
    eTag_ISINDEX,  /**< ISINDEX */
    eTag_KBD,      /**< KBD */
    eTag_KEYGEN,   /**< KEYGEN */
    eTag_LABEL,    /**< LABEL */
    eTag_LAYER,    /**< LAYER */
    eTag_LEGEND,   /**< LEGEND */
    eTag_LI,       /**< LI */
    eTag_LINK,     /**< LINK */
    eTag_LISTING,  /**< LISTING */
    eTag_MAP,      /**< MAP */
    eTag_MARQUEE,  /**< MARQUEE */
    eTag_MENU,     /**< MENU */
    eTag_META,     /**< META */
    eTag_MULTICOL, /**< MULTICOL */
    eTag_NOBR,     /**< NOBR */
    eTag_NOEMBED,  /**< NOEMBED */
    eTag_NOFRAMES, /**< NOFRAMES */
    eTag_NOLAYER,  /**< NOLAYER */
    eTag_NOSAVE,   /**< NOSAVE */
    eTag_NOSCRIPT, /**< NOSCRIPT */
    eTag_OBJECT,   /**< OBJECT */
    eTag_OL,       /**< OL */
    eTag_OPTGROUP, /**< OPTGROUP */
    eTag_OPTION,   /**< OPTION */
    eTag_P,        /**< P */
    eTag_PARAM,    /**< PARAM */
    eTag_PLAINTEXT,/**< PLAINTEXT */
    eTag_PRE,      /**< PRE */
    eTag_Q,        /**< Q */
    eTag_RB,       /**< RB */
    eTag_RBC,      /**< RBC */
    eTag_RP,       /**< RP */
    eTag_RT,       /**< RT */
    eTag_RTC,      /**< RTC */
    eTag_RUBY,     /**< RUBY */
    eTag_S,        /**< S */
    eTag_SAMP,     /**< SAMP */
    eTag_SCRIPT,   /**< SCRIPT */
    eTag_SELECT,   /**< SELECT */
    eTag_SERVER,   /**< SERVER */
    eTag_SERVLET,  /**< SERVLET */
    eTag_SMALL,    /**< SMALL */
    eTag_SPACER,   /**< SPACER */
    eTag_SPAN,     /**< SPAN */
    eTag_STRIKE,   /**< STRIKE */
    eTag_STRONG,   /**< STRONG */
    eTag_STYLE,    /**< STYLE */
    eTag_SUB,      /**< SUB */
    eTag_SUP,      /**< SUP */
    eTag_TABLE,    /**< TABLE */
    eTag_TBODY,    /**< TBODY */
    eTag_TD,       /**< TD */
    eTag_TEXTAREA, /**< TEXTAREA */
    eTag_TFOOT,    /**< TFOOT */
    eTag_TH,       /**< TH */
    eTag_THEAD,    /**< THEAD */
    eTag_TITLE,    /**< TITLE */
    eTag_TR,       /**< TR */
    eTag_TT,       /**< TT */
    eTag_U,        /**< U */
    eTag_UL,       /**< UL */
    eTag_VAR,      /**< VAR */
    eTag_WBR,      /**< WBR */
    eTag_XMP,      /**< XMP */
    eTag_NEXTID,   /**< NEXTID */
  } TagType;
    
  NodeType GetType() const;
  TagType GetTagType() const;
  const nglString& GetName() const;
  const nglString& GetText() const;

  uint32 GetNbChildren() const;
  const nuiHTMLNode* GetChild(uint32 index) const;
  nuiHTMLNode* GetChild(uint32 index);

  uint32 GetNbAttributes() const;
  const nuiHTMLAttrib* GetAttribute(uint32 index) const;
  nuiHTMLAttrib* GetAttribute(uint32 index);
  const nuiHTMLAttrib* GetAttribute(const nglString& rAttribName) const;
  nuiHTMLAttrib* GetAttribute(const nglString& rAttribName);
  const nuiHTMLAttrib* GetAttribute(nuiHTMLAttrib::AttributeType attr_type) const;
  nuiHTMLAttrib* GetAttribute(nuiHTMLAttrib::AttributeType attr_type);

  void GetSimpleText(nglString& rString) const; ///< Add the text contents of this node to rString and then do the same for this its children. This is only needed to create a simple text only preview ignoring all the actual markup.
  
  virtual const nglString& GetSourceURL() const;
  nuiHTMLNode* GetParent() const;
protected:
  nuiHTMLNode(const nglString& rName, NodeType Type, TagType TagType, const nglString& rText);
  nuiHTMLNode(const void* _tdoc, const void* _tnod, nglTextEncoding encoding);
  ~nuiHTMLNode();
  void BuildTree(const void* _tdoc, const void* _tnod, nglTextEncoding encoding);
  void SetFromNode(const void* _tdoc, const void* _tnod, nglTextEncoding encoding);

  void Clear();

  NodeType mType;
  TagType mTagType;
  nglString mName;
  nglString mText;
  
  void SetParent(nuiHTMLNode* pParent);
  nuiHTMLNode* mpParent;
  std::vector<nuiHTMLNode*> mChildren;
  std::vector<nuiHTMLAttrib*> mAttributes;
};

class nuiHTML : public nuiHTMLNode
{
public:
  nuiHTML();
  ~nuiHTML();
  
  bool Load(nglIStream& rStream, nglTextEncoding OverrideContentsEncoding = eEncodingUnknown, const nglString& rSourceURL = nglString::Null); ///< By default we don't override the encoding indicated by the contents

  const nglString& GetSourceURL() const;
  void SetSourceURL(const nglString& rURL);

private:
  nglString mSourceURL;
};
