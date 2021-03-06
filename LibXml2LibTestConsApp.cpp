// LibXml2LibTestConsApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>

#include <stdio.h>

#include <memory>
#include <string>
#include <climits>

//#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>

//#include <windows.h>

//#if defined(LIBXML_TREE_ENABLED) && defined(LIBXML_OUTPUT_ENABLED)


void createXmlExample()
{
	xmlDocPtr doc = NULL;       /* document pointer */
	xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;/* node pointers */
	char buff[256];
	int i, j;

	LIBXML_TEST_VERSION;

	//
	//Creates a new document, a node and set it as a root node
	//
	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST "root");
	xmlDocSetRootElement(doc, root_node);

	//
	//Creates a DTD declaration. Isn't mandatory.
	//
	xmlCreateIntSubset(doc, BAD_CAST "root", NULL, BAD_CAST "tree2.dtd");

	//
	//xmlNewChild() creates a new node, which is "attached" as child node
	//of root_node node.
	//
	xmlNewChild(root_node, NULL, BAD_CAST "node1",
		BAD_CAST "content of node 1");
	//
	//The same as above, but the new child node doesn't have a content
	//
	xmlNewChild(root_node, NULL, BAD_CAST "node2", NULL);

	//
	//xmlNewProp() creates attributes, which is "attached" to an node.
	//It returns xmlAttrPtr, which isn't used here.
	//
	node =
		xmlNewChild(root_node, NULL, BAD_CAST "node3",
			BAD_CAST "this node has attributes");
	xmlNewProp(node, BAD_CAST "attribute", BAD_CAST "yes");
	xmlNewProp(node, BAD_CAST "foo", BAD_CAST "bar");

	//
	//Here goes another way to create nodes. xmlNewNode() and xmlNewText
	//creates a node and a text node separately. They are "attached"
	//by xmlAddChild()
	//
	node = xmlNewNode(NULL, BAD_CAST "node4");
	node1 = xmlNewText(BAD_CAST
		"other way to create content (which is also a node)");
	xmlAddChild(node, node1);
	xmlAddChild(root_node, node);

	//
	//A simple loop that "automates" nodes creation
	//
	for (i = 5; i < 7; i++)
	{
		sprintf_s(buff, "node%d", i);
		node = xmlNewChild(root_node, NULL, BAD_CAST buff, NULL);
		for (j = 1; j < 4; j++)
		{
			sprintf_s(buff, "node%d%d", i, j);
			node1 = xmlNewChild(node, NULL, BAD_CAST buff, NULL);
			xmlNewProp(node1, BAD_CAST "odd", BAD_CAST((j % 2) ? "no" : "yes"));
		}
	}

	//
	//Dumping document to stdio or file
	//
	//xmlSaveFormatFileEnc(argc > 1 ? argv[1] : "-", doc, "UTF-8", 1);
	xmlSaveFormatFileEnc((1 == 1) ? "mySuperXml.xml" : "-", doc, "UTF-8", 1);

	//free the document //
	xmlFreeDoc(doc);

	//
	//Free the global variables that may
	//have been allocated by the parser.
	//
	xmlCleanupParser();

	//
	//this is to debug memory for regression tests
	//
	xmlMemoryDump();
}



void createBigXmlExample()
{
	xmlDocPtr doc = NULL;       /* document pointer */
	xmlNodePtr root_node = NULL, node = NULL, node1 = NULL;/* node pointers */
	char buff[256];
	int i, j;

	LIBXML_TEST_VERSION;

	//
	//Creates a new document, a node and set it as a root node
	//
	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST "root");
	xmlDocSetRootElement(doc, root_node);

	//
	//Creates a DTD declaration. Isn't mandatory.
	//
	xmlCreateIntSubset(doc, BAD_CAST "root", NULL, BAD_CAST "tree2.dtd");

	//
	//xmlNewChild() creates a new node, which is "attached" as child node
	//of root_node node.
	//
	xmlNewChild(root_node, NULL, BAD_CAST "node1",
		BAD_CAST "content of node 1");
	//
	//The same as above, but the new child node doesn't have a content
	//
	xmlNewChild(root_node, NULL, BAD_CAST "node2", NULL);

	//
	//xmlNewProp() creates attributes, which is "attached" to an node.
	//It returns xmlAttrPtr, which isn't used here.
	//
	node =
		xmlNewChild(root_node, NULL, BAD_CAST "node3",
			BAD_CAST "this node has attributes");
	xmlNewProp(node, BAD_CAST "attribute", BAD_CAST "yes");
	xmlNewProp(node, BAD_CAST "foo", BAD_CAST "bar");

	//
	//Here goes another way to create nodes. xmlNewNode() and xmlNewText
	//creates a node and a text node separately. They are "attached"
	//by xmlAddChild()
	//
	node = xmlNewNode(NULL, BAD_CAST "node4");
	node1 = xmlNewText(BAD_CAST
		"other way to create content (which is also a node)");
	xmlAddChild(node, node1);
	xmlAddChild(root_node, node);

	//
	//A simple loop that "automates" nodes creation
	//


	int nodesNum = 0, atributesNum = 0;
	nodesNum = 10, atributesNum = 100; // 16 Mb
									   //nodesNum = 1000, atributesNum = 100; // 16 Mb
									   //nodesNum = 5000, atributesNum = 100; // 85 Mb
									   //nodesNum = 7000, atributesNum = 100; // 119 Mb
									   //nodesNum = 8000, atributesNum = 100; // 136 Mb

									   //for (i = 5; i < INT_MAX; i++)
									   //for (i = 5; i < 7; i++)
	for (i = 5; i < nodesNum; i++)
	{
		sprintf_s(buff, "node%d", i);
		node = xmlNewChild(root_node, NULL, BAD_CAST buff, NULL);

		//for (i = 5; i < INT_MAX; i++)
		//for (j = 1; j < 4; j++) 
		for (j = 1; j < atributesNum; j++)
		{
			sprintf_s(buff, "node%d%d", i, j);
			node1 = xmlNewChild(node, NULL, BAD_CAST buff, NULL);
			xmlNewProp(node1, BAD_CAST "odd", BAD_CAST((j % 2) ? "no" : "yes"));

			xmlNewProp(node1, BAD_CAST "BandType", BAD_CAST((j % 2) ? "one" : "two"));

			xmlNewProp(node1, BAD_CAST "LowBandRange", BAD_CAST(!(j % 2) ? "treee" : "four"));

			xmlNewProp(node1, BAD_CAST "HighBandRange", BAD_CAST((j % 2) ? "five" : "six"));

			xmlNewProp(node1, BAD_CAST "HighFrequency", BAD_CAST(!(j % 2) ? "seven" : "eight"));

			xmlNewProp(node1, BAD_CAST "SpectrumType", BAD_CAST((j % 2) ? "twentyone" : "seventyhunderd"));

			xmlNewProp(node1, BAD_CAST "HighWarningLevel", BAD_CAST(!(j % 2) ? "fivethousandsixtyeight" : "whysofewwhenweneedmuchmore"));

		}
	}

	//
	//Dumping document to stdio or file
	//
	//xmlSaveFormatFileEnc(argc > 1 ? argv[1] : "-", doc, "UTF-8", 1);
	xmlSaveFormatFileEnc((1 == 1) ? "mySuperXml-big-0.xml" : "-", doc, "UTF-8", 1);

	//free the document //
	xmlFreeDoc(doc);

	//
	//Free the global variables that may
	//have been allocated by the parser.
	//
	xmlCleanupParser();

	//
	//this is to debug memory for regression tests
	//
	xmlMemoryDump();
}







class ParseFSM
{
public:
	/** SAX2 callback when an element start has been detected by the parser. It provides the namespace informations for the element, as well as the new namespace declarations on the element.
	ctx:  the user data (XML parser context)
	localname:  the local name of the element
	prefix:  the element namespace prefix if available
	URI:  the element namespace name if available
	nb_namespaces: number of namespace definitions on that node
	namespaces: pointer to the array of prefix/URI pairs namespace definitions
	nb_attributes: the number of attributes on that node
	nb_defaulted:  the number of defaulted attributes. The defaulted ones are at the end of the array
	attributes: pointer to the array of (localname/prefix/URI/value/end) attribute values.
	*/
	static void startElementNs(void * ctx,
		const xmlChar * localname,
		const xmlChar * prefix,
		const xmlChar * URI,
		int nb_namespaces,
		const xmlChar ** namespaces,
		int nb_attributes,
		int nb_defaulted,
		const xmlChar ** attributes)
	{
		ParseFSM &fsm = *(static_cast<ParseFSM *>(ctx));
		printf("startElementNs: name = '%s' prefix = '%s' uri = (%p)'%s'\n", localname, prefix, URI, URI);
		for (int indexNamespace = 0; indexNamespace < nb_namespaces; ++indexNamespace)
		{
			const xmlChar *prefix = namespaces[indexNamespace * 2];
			const xmlChar *nsURI = namespaces[indexNamespace * 2 + 1];
			printf("  namespace: name='%s' uri=(%p)'%s'\n", prefix, nsURI, nsURI);
		}

		unsigned int index = 0;
		for (int indexAttribute = 0;
			indexAttribute < nb_attributes;
			++indexAttribute, index += 5)
		{
			const xmlChar *localname = attributes[index];
			const xmlChar *prefix = attributes[index + 1];
			const xmlChar *nsURI = attributes[index + 2];
			const xmlChar *valueBegin = attributes[index + 3];
			const xmlChar *valueEnd = attributes[index + 4];
			std::string value((const char *)valueBegin, (const char *)valueEnd);
			printf("  %sattribute: localname='%s', prefix='%s', uri=(%p)'%s', value='%s'\n",
				indexAttribute >= (nb_attributes - nb_defaulted) ? "defaulted " : "",
				localname,
				prefix,
				nsURI,
				nsURI,
				value.c_str());
		}
	}

	/** SAX2 callback when an element end has been detected by the parser. It provides the namespace informations for the element.
	ctx:  the user data (XML parser context)
	localname:  the local name of the element
	prefix:  the element namespace prefix if available
	URI:  the element namespace name if available
	*/
	static void endElementNs(void * ctx,
		const xmlChar * localname,
		const xmlChar * prefix,
		const xmlChar * URI)
	{
		ParseFSM &fsm = *(static_cast<ParseFSM *>(ctx));
		printf("endElementNs: name = '%s' prefix = '%s' uri = '%s'\n", localname, prefix, URI);
	}

	/** Display and format an error messages, callback.
	ctx:  an XML parser context
	msg:  the message to display/transmit
	...:  extra parameters for the message display
	*/
	static void error(void * ctx,
		const char * msg,
		...)
	{
		ParseFSM &fsm = *(static_cast<ParseFSM *>(ctx));
		va_list args;
		va_start(args, msg);
		vprintf(msg, args);
		va_end(args);
	}

	/** Display and format a warning messages, callback.
	ctx:  an XML parser context
	msg:  the message to display/transmit
	...:  extra parameters for the message display
	*/
	static void warning(void * ctx,
		const char * msg,
		...)
	{
		ParseFSM &fsm = *(static_cast<ParseFSM *>(ctx));
		va_list args;
		va_start(args, msg);
		vprintf(msg, args);
		va_end(args);
	}
};


void readFromXmlToTreeAndPrintTreeElementsSAXClass()
{
	std::string xmlIn =
		"<test:Plan xmlns:test='http://test.org/schema'>"
		"<test:Case name='test1' emptyAttribute='' test:ns_id='auio'>"
		"</test:Case>"
		"</test:Plan>";

	std::string xmlFilePath = "D:/_test/test-big-3.xml";

	// this initialize the library and check potential ABI mismatches
	// between the version it was compiled for and the actual shared library used.
	LIBXML_TEST_VERSION

		xmlSAXHandler saxHandler; // See http://xmlsoft.org/html/libxml-tree.html#xmlSAXHandler
	memset(&saxHandler, 0, sizeof(saxHandler));
	// Using xmlSAXVersion( &saxHandler, 2 ) generate crash as it sets plenty of other pointers...
	saxHandler.initialized = XML_SAX2_MAGIC;  // so we do this to force parsing as SAX2.
	saxHandler.startElementNs = &ParseFSM::startElementNs;
	saxHandler.endElementNs = &ParseFSM::endElementNs;
	saxHandler.warning = &ParseFSM::warning;
	saxHandler.error = &ParseFSM::error;

	ParseFSM fsm;
	int result = xmlSAXUserParseMemory
	(
		&saxHandler
		, &fsm
		, xmlIn.c_str()
		, int(xmlIn.length())
	);

	if (result != 0)
	{
		std::cout << ("Failed to parse document.\n");
		return;
	}

	// Cleanup function for the XML library.
	xmlCleanupParser();
	// this is to debug memory for regression tests
	xmlMemoryDump();

	//return 0;
}



//class ParseSax
//{

//public:

static int debug = 0;
static int copy = 0;
static int recovery = 0;
static int push = 0;
static int speed = 0;
static int noent = 0;
static int quiet = 0;
static int nonull = 0;
static int sax2 = 0;
static int repeat = 0;
static int callbacks = 0;
static int timing = 0;

/*
* Timing routines.
*/
/*
* Internal timing routines to remove the necessity to have unix-specific
* function calls
*/

#ifndef HAVE_GETTIMEOFDAY
#ifdef HAVE_SYS_TIMEB_H
#ifdef HAVE_SYS_TIME_H
#ifdef HAVE_FTIME

static int
my_gettimeofday(struct timeval *tvp, void *tzp)
{
	struct timeb timebuffer;

	ftime(&timebuffer);
	if (tvp) {
		tvp->tv_sec = timebuffer.time;
		tvp->tv_usec = timebuffer.millitm * 1000L;
	}
	return (0);
}
#define HAVE_GETTIMEOFDAY 1
#define gettimeofday my_gettimeofday

#endif /* HAVE_FTIME */
#endif /* HAVE_SYS_TIME_H */
#endif /* HAVE_SYS_TIMEB_H */
#endif /* !HAVE_GETTIMEOFDAY */

#if defined(HAVE_GETTIMEOFDAY)
static struct timeval begin, end;

/*
* startTimer: call where you want to start timing
*/
static void
startTimer(void)
{
	gettimeofday(&begin, NULL);
}

/*
* endTimer: call where you want to stop timing and to print out a
*           message about the timing performed; format is a printf
*           type argument
*/
static void XMLCDECL
endTimer(const char *fmt, ...)
{
	long msec;
	va_list ap;

	gettimeofday(&end, NULL);
	msec = end.tv_sec - begin.tv_sec;
	msec *= 1000;
	msec += (end.tv_usec - begin.tv_usec) / 1000;

#ifndef HAVE_STDARG_H
#error "endTimer required stdarg functions"
#endif
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	fprintf(stderr, " took %ld ms\n", msec);
}
#elif defined(HAVE_TIME_H)
/*
* No gettimeofday function, so we have to make do with calling clock.
* This is obviously less accurate, but there's little we can do about
* that.
*/
#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 100
#endif

static clock_t begin, end;
static void
startTimer(void)
{
	begin = clock();
}
static void XMLCDECL
endTimer(const char *fmt, ...)
{
	long msec;
	va_list ap;

	end = clock();
	msec = ((end - begin) * 1000) / CLOCKS_PER_SEC;

#ifndef HAVE_STDARG_H
#error "endTimer required stdarg functions"
#endif
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, " took %ld ms\n", msec);
}
#else

/*
* We don't have a gettimeofday or time.h, so we just don't do timing
*/
static void
startTimer(void)
{
	/*
	* Do nothing
	*/
}
static void XMLCDECL
endTimer(char *format, ...)
{
	/*
	* We cannot do anything because we don't have a timing function
	*/
#ifdef HAVE_STDARG_H
	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);
	fprintf(stderr, " was not timed\n", msec);
#else
	/* We don't have gettimeofday, time or stdarg.h, what crazy world is
	* this ?!
	*/
#endif
}
#endif

/*
* empty SAX block
*/
static xmlSAXHandler emptySAXHandlerStruct = {
	NULL, /* internalSubset */
	NULL, /* isStandalone */
	NULL, /* hasInternalSubset */
	NULL, /* hasExternalSubset */
	NULL, /* resolveEntity */
	NULL, /* getEntity */
	NULL, /* entityDecl */
	NULL, /* notationDecl */
	NULL, /* attributeDecl */
	NULL, /* elementDecl */
	NULL, /* unparsedEntityDecl */
	NULL, /* setDocumentLocator */
	NULL, /* startDocument */
	NULL, /* endDocument */
	NULL, /* startElement */
	NULL, /* endElement */
	NULL, /* reference */
	NULL, /* characters */
	NULL, /* ignorableWhitespace */
	NULL, /* processingInstruction */
	NULL, /* comment */
	NULL, /* xmlParserWarning */
	NULL, /* xmlParserError */
	NULL, /* xmlParserError */
	NULL, /* getParameterEntity */
	NULL, /* cdataBlock; */
	NULL, /* externalSubset; */
	1,
	NULL,
	NULL, /* startElementNs */
	NULL, /* endElementNs */
	NULL  /* xmlStructuredErrorFunc */
};

static xmlSAXHandlerPtr emptySAXHandler = &emptySAXHandlerStruct;
extern xmlSAXHandlerPtr debugSAXHandler;

/************************************************************************
*									*
*				Debug Handlers				*
*									*
************************************************************************/

/**
* isStandaloneDebug:
* @ctxt:  An XML parser context
*
* Is this document tagged standalone ?
*
* Returns 1 if true
*/
static int
isStandaloneDebug(void *ctx ATTRIBUTE_UNUSED)
{
	callbacks++;
	if (quiet)
		return(0);
	fprintf(stdout, "SAX.isStandalone()\n");
	return(0);
}

/**
* hasInternalSubsetDebug:
* @ctxt:  An XML parser context
*
* Does this document has an internal subset
*
* Returns 1 if true
*/
static int
hasInternalSubsetDebug(void *ctx ATTRIBUTE_UNUSED)
{
	callbacks++;
	if (quiet)
		return(0);
	fprintf(stdout, "SAX.hasInternalSubset()\n");
	return(0);
}

/**
* hasExternalSubsetDebug:
* @ctxt:  An XML parser context
*
* Does this document has an external subset
*
* Returns 1 if true
*/
static int
hasExternalSubsetDebug(void *ctx ATTRIBUTE_UNUSED)
{
	callbacks++;
	if (quiet)
		return(0);
	fprintf(stdout, "SAX.hasExternalSubset()\n");
	return(0);
}

/**
* internalSubsetDebug:
* @ctxt:  An XML parser context
*
* Does this document has an internal subset
*/
static void
internalSubsetDebug(void *ctx ATTRIBUTE_UNUSED, const xmlChar *name,
	const xmlChar *ExternalID, const xmlChar *SystemID)
{
	callbacks++;
	if (quiet)
		return;
	fprintf(stdout, "SAX.internalSubset(%s,", name);
	if (ExternalID == NULL)
		fprintf(stdout, " ,");
	else
		fprintf(stdout, " %s,", ExternalID);
	if (SystemID == NULL)
		fprintf(stdout, " )\n");
	else
		fprintf(stdout, " %s)\n", SystemID);
}

/**
* externalSubsetDebug:
* @ctxt:  An XML parser context
*
* Does this document has an external subset
*/
static void
externalSubsetDebug(void *ctx ATTRIBUTE_UNUSED, const xmlChar *name,
	const xmlChar *ExternalID, const xmlChar *SystemID)
{
	callbacks++;
	if (quiet)
		return;
	fprintf(stdout, "SAX.externalSubset(%s,", name);
	if (ExternalID == NULL)
		fprintf(stdout, " ,");
	else
		fprintf(stdout, " %s,", ExternalID);
	if (SystemID == NULL)
		fprintf(stdout, " )\n");
	else
		fprintf(stdout, " %s)\n", SystemID);
}

/**
* resolveEntityDebug:
* @ctxt:  An XML parser context
* @publicId: The public ID of the entity
* @systemId: The system ID of the entity
*
* Special entity resolver, better left to the parser, it has
* more context than the application layer.
* The default behaviour is to NOT resolve the entities, in that case
* the ENTITY_REF nodes are built in the structure (and the parameter
* values).
*
* Returns the xmlParserInputPtr if inlined or NULL for DOM behaviour.
*/
static xmlParserInputPtr
resolveEntityDebug(void *ctx ATTRIBUTE_UNUSED, const xmlChar *publicId, const xmlChar *systemId)
{
	callbacks++;
	if (quiet)
		return(NULL);
	/* xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx; */


	fprintf(stdout, "SAX.resolveEntity(");
	if (publicId != NULL)
		fprintf(stdout, "%s", (char *)publicId);
	else
		fprintf(stdout, " ");
	if (systemId != NULL)
		fprintf(stdout, ", %s)\n", (char *)systemId);
	else
		fprintf(stdout, ", )\n");
	/*********
	if (systemId != NULL) {
	return(xmlNewInputFromFile(ctxt, (char *) systemId));
	}
	*********/
	return(NULL);
}

/**
* getEntityDebug:
* @ctxt:  An XML parser context
* @name: The entity name
*
* Get an entity by name
*
* Returns the xmlParserInputPtr if inlined or NULL for DOM behaviour.
*/
static xmlEntityPtr
getEntityDebug(void *ctx ATTRIBUTE_UNUSED, const xmlChar *name)
{
	callbacks++;
	if (quiet)
		return(NULL);
	fprintf(stdout, "SAX.getEntity(%s)\n", name);
	return(NULL);
}

/**
* getParameterEntityDebug:
* @ctxt:  An XML parser context
* @name: The entity name
*
* Get a parameter entity by name
*
* Returns the xmlParserInputPtr
*/
static xmlEntityPtr
getParameterEntityDebug(void *ctx ATTRIBUTE_UNUSED, const xmlChar *name)
{
	callbacks++;
	if (quiet)
		return(NULL);
	fprintf(stdout, "SAX.getParameterEntity(%s)\n", name);
	return(NULL);
}


/**
* entityDeclDebug:
* @ctxt:  An XML parser context
* @name:  the entity name
* @type:  the entity type
* @publicId: The public ID of the entity
* @systemId: The system ID of the entity
* @content: the entity value (without processing).
*
* An entity definition has been parsed
*/
static void
entityDeclDebug(void *ctx ATTRIBUTE_UNUSED, const xmlChar *name, int type,
	const xmlChar *publicId, const xmlChar *systemId, xmlChar *content)
{
	const xmlChar *nullstr = BAD_CAST "(null)";
	/* not all libraries handle printing null pointers nicely */
	if (publicId == NULL)
		publicId = nullstr;
	if (systemId == NULL)
		systemId = nullstr;
	if (content == NULL)
		content = (xmlChar *)nullstr;
	callbacks++;
	if (quiet)
		return;
	fprintf(stdout, "SAX.entityDecl(%s, %d, %s, %s, %s)\n",
		name, type, publicId, systemId, content);
}

/**
* attributeDeclDebug:
* @ctxt:  An XML parser context
* @name:  the attribute name
* @type:  the attribute type
*
* An attribute definition has been parsed
*/
static void
attributeDeclDebug(void *ctx ATTRIBUTE_UNUSED, const xmlChar * elem,
	const xmlChar * name, int type, int def,
	const xmlChar * defaultValue, xmlEnumerationPtr tree)
{
	callbacks++;
	if (quiet)
		return;
	if (defaultValue == NULL)
		fprintf(stdout, "SAX.attributeDecl(%s, %s, %d, %d, NULL, ...)\n",
			elem, name, type, def);
	else
		fprintf(stdout, "SAX.attributeDecl(%s, %s, %d, %d, %s, ...)\n",
			elem, name, type, def, defaultValue);
	xmlFreeEnumeration(tree);
}

/**
* elementDeclDebug:
* @ctxt:  An XML parser context
* @name:  the element name
* @type:  the element type
* @content: the element value (without processing).
*
* An element definition has been parsed
*/
static void
elementDeclDebug(void *ctx ATTRIBUTE_UNUSED, const xmlChar *name, int type,
	xmlElementContentPtr content ATTRIBUTE_UNUSED)
{
	callbacks++;
	if (quiet)
		return;
	fprintf(stdout, "SAX.elementDecl(%s, %d, ...)\n",
		name, type);
}

/**
* notationDeclDebug:
* @ctxt:  An XML parser context
* @name: The name of the notation
* @publicId: The public ID of the entity
* @systemId: The system ID of the entity
*
* What to do when a notation declaration has been parsed.
*/
static void
notationDeclDebug(void *ctx ATTRIBUTE_UNUSED, const xmlChar *name,
	const xmlChar *publicId, const xmlChar *systemId)
{
	callbacks++;
	if (quiet)
		return;
	fprintf(stdout, "SAX.notationDecl(%s, %s, %s)\n",
		(char *)name, (char *)publicId, (char *)systemId);
}

/**
* unparsedEntityDeclDebug:
* @ctxt:  An XML parser context
* @name: The name of the entity
* @publicId: The public ID of the entity
* @systemId: The system ID of the entity
* @notationName: the name of the notation
*
* What to do when an unparsed entity declaration is parsed
*/
static void
unparsedEntityDeclDebug(void *ctx ATTRIBUTE_UNUSED, const xmlChar *name,
	const xmlChar *publicId, const xmlChar *systemId,
	const xmlChar *notationName)
{
	const xmlChar *nullstr = BAD_CAST "(null)";

	if (publicId == NULL)
		publicId = nullstr;
	if (systemId == NULL)
		systemId = nullstr;
	if (notationName == NULL)
		notationName = nullstr;
	callbacks++;
	if (quiet)
		return;
	fprintf(stdout, "SAX.unparsedEntityDecl(%s, %s, %s, %s)\n",
		(char *)name, (char *)publicId, (char *)systemId,
		(char *)notationName);
}

/**
* setDocumentLocatorDebug:
* @ctxt:  An XML parser context
* @loc: A SAX Locator
*
* Receive the document locator at startup, actually xmlDefaultSAXLocator
* Everything is available on the context, so this is useless in our case.
*/
static void
setDocumentLocatorDebug(void *ctx ATTRIBUTE_UNUSED, xmlSAXLocatorPtr loc ATTRIBUTE_UNUSED)
{
	callbacks++;
	if (quiet)
		return;
	fprintf(stdout, "SAX.setDocumentLocator()\n");
}

/**
* startDocumentDebug:
* @ctxt:  An XML parser context
*
* called when the document start being processed.
*/
static void
startDocumentDebug(void *ctx ATTRIBUTE_UNUSED)
{
	callbacks++;
	if (quiet)
		return;
	fprintf(stdout, "SAX.startDocument()\n");
}

/**
* endDocumentDebug:
* @ctxt:  An XML parser context
*
* called when the document end has been detected.
*/
static void
endDocumentDebug(void *ctx ATTRIBUTE_UNUSED)
{
	callbacks++;
	if (quiet)
		return;
	fprintf(stdout, "SAX.endDocument()\n");
}

/**
* startElementDebug:
* @ctxt:  An XML parser context
* @name:  The element name
*
* called when an opening tag has been processed.
*/
static void
startElementDebug(void *ctx ATTRIBUTE_UNUSED, const xmlChar *name, const xmlChar **atts)
{
	int i;

	callbacks++;
	if (quiet)
		return;
	fprintf(stdout, "SAX.startElement(%s", (char *)name);
	if (atts != NULL) {
		for (i = 0; (atts[i] != NULL); i++) {
			fprintf(stdout, ", %s='", atts[i++]);
			if (atts[i] != NULL)
				fprintf(stdout, "%s'", atts[i]);
		}
	}
	fprintf(stdout, ")\n");
}

/**
* endElementDebug:
* @ctxt:  An XML parser context
* @name:  The element name
*
* called when the end of an element has been detected.
*/
static void
endElementDebug(void *ctx ATTRIBUTE_UNUSED, const xmlChar *name)
{
	callbacks++;
	if (quiet)
		return;
	fprintf(stdout, "SAX.endElement(%s)\n", (char *)name);
}

/**
* charactersDebug:
* @ctxt:  An XML parser context
* @ch:  a xmlChar string
* @len: the number of xmlChar
*
* receiving some chars from the parser.
* Question: how much at a time ???
*/
static void
charactersDebug(void *ctx ATTRIBUTE_UNUSED, const xmlChar *ch, int len)
{
	char output[40];
	int i;

	callbacks++;
	if (quiet)
		return;
	for (i = 0; (i<len) && (i < 30); i++)
		output[i] = ch[i];
	output[i] = 0;

	fprintf(stdout, "SAX.characters(%s, %d)\n", output, len);
}

/**
* referenceDebug:
* @ctxt:  An XML parser context
* @name:  The entity name
*
* called when an entity reference is detected.
*/
static void
referenceDebug(void *ctx ATTRIBUTE_UNUSED, const xmlChar *name)
{
	callbacks++;
	if (quiet)
		return;
	fprintf(stdout, "SAX.reference(%s)\n", name);
}

/**
* ignorableWhitespaceDebug:
* @ctxt:  An XML parser context
* @ch:  a xmlChar string
* @start: the first char in the string
* @len: the number of xmlChar
*
* receiving some ignorable whitespaces from the parser.
* Question: how much at a time ???
*/
static void
ignorableWhitespaceDebug(void *ctx ATTRIBUTE_UNUSED, const xmlChar *ch, int len)
{
	char output[40];
	int i;

	callbacks++;
	if (quiet)
		return;
	for (i = 0; (i<len) && (i < 30); i++)
		output[i] = ch[i];
	output[i] = 0;
	fprintf(stdout, "SAX.ignorableWhitespace(%s, %d)\n", output, len);
}

/**
* processingInstructionDebug:
* @ctxt:  An XML parser context
* @target:  the target name
* @data: the PI data's
* @len: the number of xmlChar
*
* A processing instruction has been parsed.
*/
static void
processingInstructionDebug(void *ctx ATTRIBUTE_UNUSED, const xmlChar *target,
	const xmlChar *data)
{
	callbacks++;
	if (quiet)
		return;
	if (data != NULL)
		fprintf(stdout, "SAX.processingInstruction(%s, %s)\n",
		(char *)target, (char *)data);
	else
		fprintf(stdout, "SAX.processingInstruction(%s, NULL)\n",
		(char *)target);
}

/**
* cdataBlockDebug:
* @ctx: the user data (XML parser context)
* @value:  The pcdata content
* @len:  the block length
*
* called when a pcdata block has been parsed
*/
static void
cdataBlockDebug(void *ctx ATTRIBUTE_UNUSED, const xmlChar *value, int len)
{
	callbacks++;
	if (quiet)
		return;
	fprintf(stdout, "SAX.pcdata(%.20s, %d)\n",
		(char *)value, len);
}

/**
* commentDebug:
* @ctxt:  An XML parser context
* @value:  the comment content
*
* A comment has been parsed.
*/
static void
commentDebug(void *ctx ATTRIBUTE_UNUSED, const xmlChar *value)
{
	callbacks++;
	if (quiet)
		return;
	fprintf(stdout, "SAX.comment(%s)\n", value);
}

/**
* warningDebug:
* @ctxt:  An XML parser context
* @msg:  the message to display/transmit
* @...:  extra parameters for the message display
*
* Display and format a warning messages, gives file, line, position and
* extra parameters.
*/
static void XMLCDECL
warningDebug(void *ctx ATTRIBUTE_UNUSED, const char *msg, ...)
{
	va_list args;

	callbacks++;
	if (quiet)
		return;
	va_start(args, msg);
	fprintf(stdout, "SAX.warning: ");
	vfprintf(stdout, msg, args);
	va_end(args);
}

/**
* errorDebug:
* @ctxt:  An XML parser context
* @msg:  the message to display/transmit
* @...:  extra parameters for the message display
*
* Display and format a error messages, gives file, line, position and
* extra parameters.
*/
static void XMLCDECL
errorDebug(void *ctx ATTRIBUTE_UNUSED, const char *msg, ...)
{
	va_list args;

	callbacks++;
	if (quiet)
		return;
	va_start(args, msg);
	fprintf(stdout, "SAX.error: ");
	vfprintf(stdout, msg, args);
	va_end(args);
}

/**
* fatalErrorDebug:
* @ctxt:  An XML parser context
* @msg:  the message to display/transmit
* @...:  extra parameters for the message display
*
* Display and format a fatalError messages, gives file, line, position and
* extra parameters.
*/
static void XMLCDECL
fatalErrorDebug(void *ctx ATTRIBUTE_UNUSED, const char *msg, ...)
{
	va_list args;

	callbacks++;
	if (quiet)
		return;
	va_start(args, msg);
	fprintf(stdout, "SAX.fatalError: ");
	vfprintf(stdout, msg, args);
	va_end(args);
}

static xmlSAXHandler debugSAXHandlerStruct = {
	internalSubsetDebug,
	isStandaloneDebug,
	hasInternalSubsetDebug,
	hasExternalSubsetDebug,
	resolveEntityDebug,
	getEntityDebug,
	entityDeclDebug,
	notationDeclDebug,
	attributeDeclDebug,
	elementDeclDebug,
	unparsedEntityDeclDebug,
	setDocumentLocatorDebug,
	startDocumentDebug,
	endDocumentDebug,
	startElementDebug,
	endElementDebug,
	referenceDebug,
	charactersDebug,
	ignorableWhitespaceDebug,
	processingInstructionDebug,
	commentDebug,
	warningDebug,
	errorDebug,
	fatalErrorDebug,
	getParameterEntityDebug,
	cdataBlockDebug,
	externalSubsetDebug,
	1,
	NULL,
	NULL,
	NULL,
	NULL
};

xmlSAXHandlerPtr debugSAXHandler = &debugSAXHandlerStruct;

/*
* SAX2 specific callbacks
*/
/**
* startElementNsDebug:
* @ctxt:  An XML parser context
* @name:  The element name
*
* called when an opening tag has been processed.
*/
static void
startElementNsDebug(void *ctx ATTRIBUTE_UNUSED,
	const xmlChar *localname,
	const xmlChar *prefix,
	const xmlChar *URI,
	int nb_namespaces,
	const xmlChar **namespaces,
	int nb_attributes,
	int nb_defaulted,
	const xmlChar **attributes)
{
	int i;

	callbacks++;
	if (quiet)
		return;
	fprintf(stdout, "SAX.startElementNs(%s", (char *)localname);
	if (prefix == NULL)
		fprintf(stdout, ", NULL");
	else
		fprintf(stdout, ", %s", (char *)prefix);
	if (URI == NULL)
		fprintf(stdout, ", NULL");
	else
		fprintf(stdout, ", '%s'", (char *)URI);
	fprintf(stdout, ", %d", nb_namespaces);

	if (namespaces != NULL) {
		for (i = 0; i < nb_namespaces * 2; i++) {
			fprintf(stdout, ", xmlns");
			if (namespaces[i] != NULL)
				fprintf(stdout, ":%s", namespaces[i]);
			i++;
			fprintf(stdout, "='%s'", namespaces[i]);
		}
	}
	fprintf(stdout, ", %d, %d", nb_attributes, nb_defaulted);
	if (attributes != NULL) {
		for (i = 0; i < nb_attributes * 5; i += 5) {
			if (attributes[i + 1] != NULL)
				fprintf(stdout, ", %s:%s='", attributes[i + 1], attributes[i]);
			else
				fprintf(stdout, ", %s='", attributes[i]);
			fprintf(stdout, "%.4s...', %d", attributes[i + 3],
				(int)(attributes[i + 4] - attributes[i + 3]));
		}
	}
	fprintf(stdout, ")\n");
}

/**
* endElementDebug:
* @ctxt:  An XML parser context
* @name:  The element name
*
* called when the end of an element has been detected.
*/
static void
endElementNsDebug(void *ctx ATTRIBUTE_UNUSED,
	const xmlChar *localname,
	const xmlChar *prefix,
	const xmlChar *URI)
{
	callbacks++;
	if (quiet)
		return;
	fprintf(stdout, "SAX.endElementNs(%s", (char *)localname);
	if (prefix == NULL)
		fprintf(stdout, ", NULL");
	else
		fprintf(stdout, ", %s", (char *)prefix);
	if (URI == NULL)
		fprintf(stdout, ", NULL)\n");
	else
		fprintf(stdout, ", '%s')\n", (char *)URI);
}

static xmlSAXHandler debugSAX2HandlerStruct = {
	internalSubsetDebug,
	isStandaloneDebug,
	hasInternalSubsetDebug,
	hasExternalSubsetDebug,
	resolveEntityDebug,
	getEntityDebug,
	entityDeclDebug,
	notationDeclDebug,
	attributeDeclDebug,
	elementDeclDebug,
	unparsedEntityDeclDebug,
	setDocumentLocatorDebug,
	startDocumentDebug,
	endDocumentDebug,
	NULL,
	NULL,
	referenceDebug,
	charactersDebug,
	ignorableWhitespaceDebug,
	processingInstructionDebug,
	commentDebug,
	warningDebug,
	errorDebug,
	fatalErrorDebug,
	getParameterEntityDebug,
	cdataBlockDebug,
	externalSubsetDebug,
	XML_SAX2_MAGIC,
	NULL,
	startElementNsDebug,
	endElementNsDebug,
	NULL
};

static xmlSAXHandlerPtr debugSAX2Handler = &debugSAX2HandlerStruct;



static void parseAndPrintFile(char *filename)
{
	int res;

#ifdef LIBXML_PUSH_ENABLED
	if (push) {
		FILE *f;

		if ((!quiet) && (!nonull)) {
			/*
			* Empty callbacks for checking
			*/

			errno_t err;

#if defined(_WIN32) || defined (__DJGPP__) && !defined (__CYGWIN__)
			// Open for read (will fail if file "filename.xml" does not exist)  
			err = fopen_s(&f, filename, "rb");
#else
			err = fopen_s(&f, filename, "r");
#endif

			if (err == 0)
				printf("The file 'filename.xml' was opened\n");
			else
			{
				printf("The file 'filename.xml' was not opened\n");
				return;
			}

			//#if defined(_WIN32) || defined (__DJGPP__) && !defined (__CYGWIN__)
			//	    f = fopen(filename, "rb");
			//#else
			//	    f = fopen(filename, "r");
			//#endif

			if (f != NULL) {
				int ret;
				char chars[10];
				xmlParserCtxtPtr ctxt;

				ret = fread(chars, 1, 4, f);
				if (ret > 0) {
					ctxt = xmlCreatePushParserCtxt(emptySAXHandler, NULL,
						chars, ret, filename);
					while ((ret = fread(chars, 1, 3, f)) > 0) {
						xmlParseChunk(ctxt, chars, ret, 0);
					}
					xmlParseChunk(ctxt, chars, 0, 1);
					xmlFreeParserCtxt(ctxt);
				}
				fclose(f);
			}
			else {
				xmlGenericError(xmlGenericErrorContext,
					"Cannot read file %s\n", filename);
			}
		}

		/*
		* Debug callback
		*/
		//#if defined(_WIN32) || defined (__DJGPP__) && !defined (__CYGWIN__)
		//	f = fopen(filename, "rb");
		//#else
		//	f = fopen(filename, "r");
		//#endif

		errno_t err_callback;

#if defined(_WIN32) || defined (__DJGPP__) && !defined (__CYGWIN__)
		// Open for read (will fail if file "filename.xml" does not exist)  
		err_callback = fopen_s(&f, filename, "rb");
#else
		err_callback = fopen_s(&f, filename, "r");
#endif

		if (err_callback == 0)
			printf("The file 'filename.xml' was opened\n");
		else
		{
			printf("The file 'filename.xml' was not opened\n");
			return;
		}


		if (f != NULL) {
			int ret;
			char chars[10];
			xmlParserCtxtPtr ctxt;

			ret = fread(chars, 1, 4, f);
			if (ret > 0) {
				if (sax2)
					ctxt = xmlCreatePushParserCtxt(debugSAX2Handler, NULL,
						chars, ret, filename);
				else
					ctxt = xmlCreatePushParserCtxt(debugSAXHandler, NULL,
						chars, ret, filename);
				while ((ret = fread(chars, 1, 3, f)) > 0) {
					xmlParseChunk(ctxt, chars, ret, 0);
				}
				ret = xmlParseChunk(ctxt, chars, 0, 1);
				xmlFreeParserCtxt(ctxt);
				if (ret != 0) {
					fprintf(stdout, "xmlSAXUserParseFile returned error %d\n", ret);
				}
			}
			fclose(f);
		}
	}
	else {
#endif /* LIBXML_PUSH_ENABLED */
		if (!speed) {
			/*
			* Empty callbacks for checking
			*/
			if ((!quiet) && (!nonull)) {
				res = xmlSAXUserParseFile(emptySAXHandler, NULL, filename);
				if (res != 0) {
					fprintf(stdout, "xmlSAXUserParseFile returned error %d\n", res);
				}
			}

			/*
			* Debug callback
			*/
			callbacks = 0;
			if (repeat) {
				int i;
				for (i = 0; i < 99; i++) {
					if (sax2)
						res = xmlSAXUserParseFile(debugSAX2Handler, NULL,
							filename);
					else
						res = xmlSAXUserParseFile(debugSAXHandler, NULL,
							filename);
				}
			}
			if (sax2)
				res = xmlSAXUserParseFile(debugSAX2Handler, NULL, filename);
			else
				res = xmlSAXUserParseFile(debugSAXHandler, NULL, filename);
			if (res != 0) {
				fprintf(stdout, "xmlSAXUserParseFile returned error %d\n", res);
			}
			if (quiet)
				fprintf(stdout, "%d callbacks generated\n", callbacks);
		}
		else {
			/*
			* test 100x the SAX parse
			*/
			int i;

			for (i = 0; i<100; i++)
				res = xmlSAXUserParseFile(emptySAXHandler, NULL, filename);
			if (res != 0) {
				fprintf(stdout, "xmlSAXUserParseFile returned error %d\n", res);
			}
		}
#ifdef LIBXML_PUSH_ENABLED
	}
#endif
}

//};


void readFromXmlToTreeAndPrintTreeElementsSAX()
{
	// 30% CPU
	// 25 -> 70 * 4 = 280 Mb RAM on one CPU 
	// 10 Min Cashing a 200.000  * 100 attributes
	// circa 30 * 4 = 2 hours time to read whole file

	std::string xmlFilePath = "D:/_test/test-big-3.xml";

	// less then a minute

	//std::string xmlFilePath = "D:/_test/3.xml";

	parseAndPrintFile(const_cast<char *>(xmlFilePath.c_str()));
}




std::wstring xmlCharToWideString(const xmlChar *xmlString)
{
	if (!xmlString) //provided string was null
	{
		return L"";
		//abort();
	}
	int charLength = xmlStrlen(xmlString); //excludes null terminator
	wchar_t *wideBuffer = new wchar_t[charLength];
	size_t wcharLength = mbstowcs(wideBuffer, (const char *)xmlString, charLength);
	if (wcharLength == (size_t)(-1)) { abort(); } //mbstowcs failed
	std::wstring wideString(wideBuffer, wcharLength);
	delete[] wideBuffer;
	return wideString;
}



std::string xmlCharToString(const xmlChar *xmlString)
{
	if (!xmlString) //provided string was null
	{
		return "";
		//abort();
	} 
	int charLength = xmlStrlen(xmlString); //excludes null terminator
	char *wideBuffer = new char[charLength];
	void* wcharLength = memcpy(wideBuffer, (const char *)xmlString, charLength);
	if ( (int)wcharLength == (size_t)(-1)) { abort(); } //mbstowcs failed
	//std::string wideString(wideBuffer, wcharLength);
	std::string wideString(wideBuffer);
	delete[] wideBuffer;
	return wideString;
}


std::wstring xmlCharToWString(const xmlChar *xmlString)
{
	if (!xmlString) { abort(); } //provided string was null

	std::wstring wideString;

	int charLength = xmlStrlen(xmlString);
	if (charLength > 0)
	{
		char *origLocale = setlocale(LC_CTYPE, NULL);
		setlocale(LC_CTYPE, "en_US.UTF-8");

		size_t wcharLength = mbtowc(NULL, (const char*)xmlString, charLength); //excludes null terminator
		if (wcharLength != (size_t)(-1))
		{
			wideString.resize(wcharLength);
			mbtowc(&wideString[0], (const char*)xmlString, charLength);
		}

		setlocale(LC_CTYPE, origLocale);
		if (wcharLength == (size_t)(-1)) { abort(); } //mbstowcs failed
	}

	return wideString;
}

/*
std::wstring utf8_to_wstring(const char *utf8)
{
	size_t utf8len = std::strlen(utf8);
	int wclen = MultiByteToWideChar(
		CP_UTF8, 0, utf8, utf8len, NULL, 0);
	wchar_t *wc = NULL;
	try {
		wc = new wchar_t[wclen];
		MultiByteToWideChar(
			CP_UTF8, 0, utf8, utf8len, wc, wclen);
		std::wstring wstr(wc, wclen);
		delete[] wc;
		wc = NULL;
		return wstr;
	}
	catch (std::exception &) {
		if (wc)
			delete[] wc;
	}
}

std::wstring xmlCharToWideString(const xmlChar *xmlString)
{
	if (!xmlString) { abort(); } //provided string was null
	try
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;
		return conv.from_bytes((const char*)xmlString);
	}
	catch (const std::range_error& e)
	{
		abort(); //wstring_convert failed
	}
}
*/

//std::string ws_s(const std::wstring& s, UINT cp = CP_ACP);
//std::wstring s_ws(const std::string& s, UINT cp = CP_ACP);

/*
std::string ws_s(const std::wstring& s, UINT cp = CP_ACP)
{
	int slength = (int)s.length(); // + 1;
	int len = WideCharToMultiByte(cp, 0, s.c_str(), slength, 0, 0, 0, 0);
	std::string r(len, '\0');
	WideCharToMultiByte(cp, 0, s.c_str(), slength, &r[0], len, 0, 0);
	return r;
}

std::wstring s_ws(const std::string& s, UINT cp = CP_ACP)
{
	int slength = (int)s.length(); // + 1;
	int len = MultiByteToWideChar(cp, 0, s.c_str(), slength, 0, 0);
	std::wstring r(len, L'\0');
	MultiByteToWideChar(cp, 0, s.c_str(), slength, &r[0], len);
	return r;
}
//*/

void print_element_names(xmlNode * a_node)
{
	setlocale(LC_ALL, "Russian");

	xmlNode *cur_node = NULL;
	xmlAttr *cur_attr = NULL;
	xmlNode *cur_attr_val = NULL;
	//xmlNode *children;	/* the value of the property */
	//struct _xmlNode *last;	/* NULL */
	//struct _xmlNode *parent;	/* child->parent link */

	for (cur_node = a_node; cur_node; cur_node = cur_node->next)
	{
		if (cur_node->type == XML_ELEMENT_NODE)
		{
			printf("node type: Element, name: %s\n", cur_node->name);
		}

		for (cur_attr = cur_node->properties; cur_attr; cur_attr = cur_attr->next)
		{
			if (cur_attr->type == XML_ATTRIBUTE_NODE)
			{
				printf("node type: Attr, name: %s\n", cur_attr->name);

				for (cur_attr_val = cur_attr->children; cur_attr_val; cur_attr_val = cur_attr_val->next)
				//cur_attr_val = cur_attr->children;
				{
				//if (cur_attr_val->type == XML_ATTRIBUTE_NODE)
				{
					printf("node type: Attr, Val: %s\n", cur_attr_val->content);

					//printf("node type: Attr, Val: %s\n", ws_s((const char *)cur_attr_val->content));
					//std::wstring ws = s_ws((const char *)cur_attr_val->content);
					//std::string  s  = ws_s(ws);

					//std::wstring ws = xmlCharToWString( cur_attr_val->content );
					std::wstring ws = xmlCharToWideString( cur_attr_val->content );
					std::wcout << L"node type: Attr, Val: " + ws + L"\n";


					//printf( "node type: Attr, Val: %s\n", s.c_str());

					//std::string valContent ( xmlCharToString( cur_attr_val->content ) );
					//printf("node type: Attr, Val: %s\n", valContent);

					//std::string valContent ( xmlCharToString( cur_attr_val->content ) );
					//printf("node type: Attr, Val: %s\n", valContent);
						
					//std::wstring valContent ( xmlCharToWideString( cur_attr_val->content ) );
					//printf("node type: Attr, Val: %s\n", valContent);
				}
				}

			}
		}

		//printf("node type: Element, name: %s\n", cur_node->properties->name);

		print_element_names(cur_node->children);
	}
}


void readFromXmlToTreeAndPrintTreeElements()
{
	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;

	//if (argc != 2) return(1);

	LIBXML_TEST_VERSION

	//std::string str = "D:/_test/test-big-3.xml";
	std::string str = "../k3.xml";

	doc = xmlReadFile(str.c_str(), NULL, 0); //parse the file and get the DOM

	if (doc == NULL)
	{
		printf("error: could not parse file %s\n", str.c_str());
	}

	root_element = xmlDocGetRootElement(doc); //Get the root element node

	print_element_names(root_element);

	xmlFreeDoc(doc); //free the document
	xmlCleanupParser(); //Free the global variables that may have been allocated by the parser.

	int z = 0;					//return 0;
}





//int main(int argc, char **argv)
int main()
{
	// WRITE //
	//createXmlExample();
	//createBigXmlExample();

	///////////////////
	// READ //
	///////////////////

	// DOM - TREE
	readFromXmlToTreeAndPrintTreeElements();
	//readFromXmlToTreeAndPrintTreeElementsSAXClass();
	
	// SAX
	//readFromXmlToTreeAndPrintTreeElementsSAX();

	return 0;
}





