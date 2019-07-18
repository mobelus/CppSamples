//const UA_ClientConfig UA_ClientConfig_default

//#include "opcua_global.h"
//using namespace opcua;

#include "OpcUaClient.h"

#include "OpcUaExtra.h"

//*
std::string uaToString(UA_String uaString)
{
	std::string result;
	char *buf = new char[uaString.length + 1];
	memcpy(buf, uaString.data, uaString.length);
	buf[uaString.length] = 0;
	result = buf;

	return result;
}


QString uaToQString(UA_String uaString)
{
	QString result;
	char *buf = new char[uaString.length + 1];
	memcpy(buf, uaString.data, uaString.length);
	buf[uaString.length] = 0;
	result = buf;

	return result;
}
//*/


#ifdef UA_ENABLE_SUBSCRIPTIONS
static void
handler_TheAnswerChanged(UA_Client *client, UA_UInt32 subId, void *subContext,
	UA_UInt32 monId, void *monContext, UA_DataValue *value)
{
	printf("The Answer has changed!\n");
}
#endif

static UA_StatusCode
nodeIter(UA_NodeId childId, UA_Boolean isInverse, UA_NodeId referenceTypeId, void *handle)
{
	if (isInverse)
		return UA_STATUSCODE_GOOD;
	UA_NodeId *parent = (UA_NodeId *)handle;
	printf("%d, %d --- %d ---> NodeId %d, %d\n",
		parent->namespaceIndex, parent->identifier.numeric,
		referenceTypeId.identifier.numeric, childId.namespaceIndex,
		childId.identifier.numeric);
	return UA_STATUSCODE_GOOD;
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

/*
HMIServer-main

ObjectName
PT1

Command
unblock
block

Result
Command 'unblock' for object 'PT1' has been execute successfully
*/

void sendCommand(UA_NodeId objectId, const std::string &command)
{
	//UA_NodeId var = resolveVariableId(objectId, command);
	//if (isNull(&var)) return;

	//UA_Variant v = readVariable(var);
	//uint32_t *value = (uint32_t *)(v.data);

	enum PointCommands
	{
		cmdNone = 0, cmdBlock = 1, cmdUnblock = 2, cmdToPlus = 3, cmdToMinus = 4
	};
	enum PointIndication
	{
		indUndefined, indIsBlocked = 1, indIsUnblocked = 2, indIsPlusPosition = 3, indIsMinusPosition = 4
	};


	QMap <std::string, PointCommands > algs;
	algs[""] = cmdNone;
	algs["block"] = cmdBlock;
	algs["unblock"] = cmdUnblock;
	algs["toPlus"] = cmdToPlus;
	algs["toMinus"] = cmdToMinus;

	PointCommands cmd = algs[command]; //dgi::EncryptionAlgType::EAlg_Unknown;


	/*

	UA_NodeId idBlock = resolveVariableId(objectId, "Block"),
		idPosition = resolveVariableId(objectId, "Position");

	//	PointCommands cmdValue = static_cast<PointCommands>(*value);
	/*
	switch (cmdValue)
	{
	case cmdNone: break;


	case cmdBlock:
	case cmdUnblock:
	{
		UA_Variant vBlock;
		uint32_t indBlock = static_cast<PointIndication>(cmdValue == cmdBlock ? indIsBlocked : indIsUnblocked);
		UA_Variant_setScalar(&vBlock, &indBlock, &UA_TYPES[UA_TYPES_UINT32]);
		writeVariable(idBlock, vBlock);
	}
	break;

	case cmdToPlus:
	case cmdToMinus:
	{
		PointIndication block = *(static_cast<PointIndication *>(readVariable(idBlock).data));
		if (block == indIsBlocked)
		{

		}
		else
		{
			UA_Variant vPosition;
			uint32_t position = static_cast<PointIndication>(cmdValue == cmdToPlus ? indIsPlusPosition : indIsMinusPosition);
			UA_Variant_setScalar(&vPosition, &position, &UA_TYPES[UA_TYPES_UINT32]);
			writeVariable(idPosition, vPosition);
		}
	}
	break;

	}
	*/



	//*(value) = *(value) + 1;
	//*value = 0; // release command

	//writeVariable(var, v);


	///* Find the NodeId of the status child variable */
	//UA_RelativePathElement rpe;
	//UA_RelativePathElement_init(&rpe);
	//rpe.referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT);
	//rpe.isInverse = false;
	//rpe.includeSubtypes = false;
	//rpe.targetName = UA_QUALIFIEDNAME(1, "Position");

	//UA_BrowsePath bp;
	//UA_BrowsePath_init(&bp);
	//bp.startingNode = objectId;
	//bp.relativePath.elementsSize = 1;
	//bp.relativePath.elements = &rpe;

	//UA_BrowsePathResult bpr = UA_Server_translateBrowsePathToNodeIds(_impl->_server, &bp);
	//if (bpr.statusCode != UA_STATUSCODE_GOOD || bpr.targetsSize < 1)
	//	return /*bpr.statusCode*/;


	///* Read current value */
	//UA_NodeId varId = bpr.targets[0].targetId.nodeId;
	//
	//UA_Variant varValue;
	//UA_Server_readValue(_impl->_server, varId, &varValue);
	//uint32_t *cnt = (uint32_t *)(varValue.data);
	//*(cnt) = *(cnt)+1;

	//UA_Server_writeValue(_impl->_server, varId, varValue);

	///* Set the status value */
	//UA_BrowsePathResult_deleteMembers(&bpr);
}


//////////////////////////////////////////////////////////////////////////
// ######################################################################
//////////////////////////////////////////////////////////////////////////

int testOpcUaClientCode()
{
	/*
		UA_Client *client = UA_Client_new(UA_ClientConfig_default);
		//UA_Client *client = UA_Client_new();
		//UA_ClientConfig_setDefault(UA_Client_getConfig(client));

		// Listing endpoints
		UA_EndpointDescription* endpointArray = NULL;
		size_t endpointArraySize = 0;
		UA_StatusCode retval = UA_Client_getEndpoints(client, "opc.tcp://localhost:4840",
			&endpointArraySize, &endpointArray);

		if (retval != UA_STATUSCODE_GOOD)
		{
			int z = 0;
	//		UA_Array_delete(endpointArray, endpointArraySize, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION]);
	//		UA_Client_delete(client);
	//		return EXIT_FAILURE;
		}

	*/

	UA_Client *client = UA_Client_new(UA_ClientConfig_default);
	//UA_ClientConfig_setDefault(UA_Client_getConfig(client));
	UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://192.168.1.119:4840");
	if (retval != UA_STATUSCODE_GOOD)
	{
		UA_Client_delete(client);
		return (int)retval;
	}

	/*	  // End POINTS
		size_t endpointArraySize = 0;
		UA_EndpointDescription* endpointArray = NULL;
		UA_StatusCode retval = UA_Client_getEndpoints(client, "opc.tcp://localhost:4840",
			&endpointArraySize, &endpointArray);

		printf("%i endpoints found\n", (int)endpointArraySize);
		for (size_t i = 0; i < endpointArraySize; i++)
		{
			printf("URL of endpoint %i is %.*s\n", (int)i,
				(int)endpointArray[i].endpointUrl.length,
				endpointArray[i].endpointUrl.data);
		}
		UA_Array_delete(endpointArray, endpointArraySize, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION]);
	*/


	// Connect to a server 
	// anonymous connect would be: retval = UA_Client_connect(client, "opc.tcp://localhost:4840"); 
	retval = UA_Client_connect_username(client, "opc.tcp://localhost:4840", "user1", "password");
	if (retval != UA_STATUSCODE_GOOD)
	{
		UA_Client_delete(client);
		return EXIT_FAILURE;
	}

	// Browse some objects 
	printf("Browsing nodes in objects folder:\n");
	UA_BrowseRequest bReq;
	UA_BrowseRequest_init(&bReq);
	bReq.requestedMaxReferencesPerNode = 0;
	bReq.nodesToBrowse = UA_BrowseDescription_new();
	bReq.nodesToBrowseSize = 1;
	bReq.nodesToBrowse[0].nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER); // browse objects folder 
	bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL; // return everything 
	UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);
	printf("%-9s %-16s %-16s %-16s\n", "NAMESPACE", "NODEID", "BROWSE NAME", "DISPLAY NAME");
	for (size_t i = 0; i < bResp.resultsSize; ++i)
	{
		for (size_t j = 0; j < bResp.results[i].referencesSize; ++j)
		{
			UA_ReferenceDescription *ref = &(bResp.results[i].references[j]);
			if (ref->nodeId.nodeId.identifierType == UA_NODEIDTYPE_NUMERIC)
			{
				printf("%-9d %-16d %-16.*s %-16.*s\n", ref->nodeId.nodeId.namespaceIndex,
					ref->nodeId.nodeId.identifier.numeric, (int)ref->browseName.name.length,
					ref->browseName.name.data, (int)ref->displayName.text.length,
					ref->displayName.text.data);
			}
			else if (ref->nodeId.nodeId.identifierType == UA_NODEIDTYPE_STRING)
			{
				printf("%-9d %-16.*s %-16.*s %-16.*s\n", ref->nodeId.nodeId.namespaceIndex,
					(int)ref->nodeId.nodeId.identifier.string.length,
					ref->nodeId.nodeId.identifier.string.data,
					(int)ref->browseName.name.length, ref->browseName.name.data,
					(int)ref->displayName.text.length, ref->displayName.text.data);
			}
			/* TODO: distinguish further types */
		}
	}

	////	UA_BrowseRequest_clear(&bReq);
	////	UA_BrowseResponse_clear(&bResp);

		/* Same thing, this time using the node iterator... */
	UA_NodeId *parent = UA_NodeId_new();
	*parent = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
	UA_Client_forEachChildNodeCall(client, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
		nodeIter, (void *)parent);
	UA_NodeId_delete(parent);

#ifdef UA_ENABLE_SUBSCRIPTIONS
	/* Create a subscription */
	UA_CreateSubscriptionRequest request = UA_CreateSubscriptionRequest_default();
	UA_CreateSubscriptionResponse response = UA_Client_Subscriptions_create(client, request,
		NULL, NULL, NULL);

	UA_UInt32 subId = response.subscriptionId;
	if (response.responseHeader.serviceResult == UA_STATUSCODE_GOOD)
		printf("Create subscription succeeded, id %u\n", subId);

	UA_MonitoredItemCreateRequest monRequest =
		UA_MonitoredItemCreateRequest_default(UA_NODEID_STRING(1, "the.answer"));

	UA_MonitoredItemCreateResult monResponse =
		UA_Client_MonitoredItems_createDataChange(client, response.subscriptionId,
			UA_TIMESTAMPSTORETURN_BOTH,
			monRequest, NULL, handler_TheAnswerChanged, NULL);
	if (monResponse.statusCode == UA_STATUSCODE_GOOD)
		printf("Monitoring 'the.answer', id %u\n", monResponse.monitoredItemId);


	/* The first publish request should return the initial value of the variable */
////	UA_Client_run_iterate(client, 1000);

#endif

	/* Read attribute */
	UA_Int32 value = 0;
	printf("\nReading the value of node (1, \"the.answer\"):\n");
	UA_Variant *val = UA_Variant_new();
	retval = UA_Client_readValueAttribute(client, UA_NODEID_STRING(1, "the.answer"), val);
	if (retval == UA_STATUSCODE_GOOD && UA_Variant_isScalar(val) &&
		val->type == &UA_TYPES[UA_TYPES_INT32])
	{
		value = *(UA_Int32*)val->data;
		printf("the value is: %i\n", value);
	}
	UA_Variant_delete(val);

	/* Write node attribute */
	value++;
	printf("\nWriting a value of node (1, \"the.answer\"):\n");
	UA_WriteRequest wReq;
	UA_WriteRequest_init(&wReq);
	wReq.nodesToWrite = UA_WriteValue_new();
	wReq.nodesToWriteSize = 1;
	wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(1, "the.answer");
	wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
	wReq.nodesToWrite[0].value.hasValue = true;
	wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_INT32];
	wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; /* do not free the integer on deletion */
	wReq.nodesToWrite[0].value.value.data = &value;
	UA_WriteResponse wResp = UA_Client_Service_write(client, wReq);
	if (wResp.responseHeader.serviceResult == UA_STATUSCODE_GOOD)
		printf("the new value is: %i\n", value);

	////	UA_WriteRequest_clear(&wReq);
	////	UA_WriteResponse_clear(&wResp);

		/* Write node attribute (using the highlevel API) */
	value++;
	UA_Variant *myVariant = UA_Variant_new();
	UA_Variant_setScalarCopy(myVariant, &value, &UA_TYPES[UA_TYPES_INT32]);
	UA_Client_writeValueAttribute(client, UA_NODEID_STRING(1, "the.answer"), myVariant);
	UA_Variant_delete(myVariant);

#ifdef UA_ENABLE_SUBSCRIPTIONS
	/* Take another look at the.answer */
////	UA_Client_run_iterate(client, 100);
	/* Delete the subscription */
	if (UA_Client_Subscriptions_deleteSingle(client, subId) == UA_STATUSCODE_GOOD)
		printf("Subscription removed\n");
#endif

#ifdef UA_ENABLE_METHODCALLS
	/* Call a remote method */
	UA_Variant input;
	UA_String argString = UA_STRING("Hello Server");
	UA_Variant_init(&input);
	UA_Variant_setScalarCopy(&input, &argString, &UA_TYPES[UA_TYPES_STRING]);
	size_t outputSize;
	UA_Variant *output;
	retval = UA_Client_call(client, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
		UA_NODEID_NUMERIC(1, 62541), 1, &input, &outputSize, &output);
	if (retval == UA_STATUSCODE_GOOD)
	{
		printf("Method call was successful, and %lu returned values available.\n",
			(unsigned long)outputSize);
		UA_Array_delete(output, outputSize, &UA_TYPES[UA_TYPES_VARIANT]);
	}
	else {
		printf("Method call was unsuccessful, and %x returned values available.\n", retval);
	}

	////	UA_Variant_clear(&input);
#endif

#ifdef UA_ENABLE_NODEMANAGEMENT
	/* Add new nodes*/
	/* New ReferenceType */
	UA_NodeId ref_id;
	UA_ReferenceTypeAttributes ref_attr = UA_ReferenceTypeAttributes_default;
	ref_attr.displayName = UA_LOCALIZEDTEXT("en-US", "NewReference");
	ref_attr.description = UA_LOCALIZEDTEXT("en-US", "References something that might or might not exist");
	ref_attr.inverseName = UA_LOCALIZEDTEXT("en-US", "IsNewlyReferencedBy");
	retval = UA_Client_addReferenceTypeNode(client,
		UA_NODEID_NUMERIC(1, 12133),
		UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
		UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
		UA_QUALIFIEDNAME(1, "NewReference"),
		ref_attr, &ref_id);
	if (retval == UA_STATUSCODE_GOOD)
		printf("Created 'NewReference' with numeric NodeID %u\n", ref_id.identifier.numeric);

	/* New ObjectType */
	UA_NodeId objt_id;
	UA_ObjectTypeAttributes objt_attr = UA_ObjectTypeAttributes_default;
	objt_attr.displayName = UA_LOCALIZEDTEXT("en-US", "TheNewObjectType");
	objt_attr.description = UA_LOCALIZEDTEXT("en-US", "Put innovative description here");
	retval = UA_Client_addObjectTypeNode(client,
		UA_NODEID_NUMERIC(1, 12134),
		UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
		UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
		UA_QUALIFIEDNAME(1, "NewObjectType"),
		objt_attr, &objt_id);
	if (retval == UA_STATUSCODE_GOOD)
		printf("Created 'NewObjectType' with numeric NodeID %u\n", objt_id.identifier.numeric);

	/* New Object */
	UA_NodeId obj_id;
	UA_ObjectAttributes obj_attr = UA_ObjectAttributes_default;
	obj_attr.displayName = UA_LOCALIZEDTEXT("en-US", "TheNewGreatNode");
	obj_attr.description = UA_LOCALIZEDTEXT("de-DE", "Hier koennte Ihre Webung stehen!");
	retval = UA_Client_addObjectNode(client,
		UA_NODEID_NUMERIC(1, 0),
		UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
		UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
		UA_QUALIFIEDNAME(1, "TheGreatNode"),
		UA_NODEID_NUMERIC(1, 12134),
		obj_attr, &obj_id);
	if (retval == UA_STATUSCODE_GOOD)
		printf("Created 'NewObject' with numeric NodeID %u\n", obj_id.identifier.numeric);

	/* New Integer Variable */
	UA_NodeId var_id;
	UA_VariableAttributes var_attr = UA_VariableAttributes_default;
	var_attr.displayName = UA_LOCALIZEDTEXT("en-US", "TheNewVariableNode");
	var_attr.description =
		UA_LOCALIZEDTEXT("en-US", "This integer is just amazing - it has digits and everything.");
	UA_Int32 int_value = 1234;
	/* This does not copy the value */
	UA_Variant_setScalar(&var_attr.value, &int_value, &UA_TYPES[UA_TYPES_INT32]);
	var_attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;
	retval = UA_Client_addVariableNode(client,
		UA_NODEID_NUMERIC(1, 0), // Assign new/random NodeID
		UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
		UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
		UA_QUALIFIEDNAME(0, "VariableNode"),
		UA_NODEID_NULL, // no variable type
		var_attr, &var_id);
	if (retval == UA_STATUSCODE_GOOD)
		printf("Created 'NewVariable' with numeric NodeID %u\n", var_id.identifier.numeric);
#endif

	UA_Client_disconnect(client);
	UA_Client_delete(client);
	return EXIT_SUCCESS;
}





// https://github.com/open62541/open62541/blob/master/examples/tutorial_client_firststeps.c

int testOpcUaClientConnection()
{
	//UA_ClientConfig cfg;
	//UA_Client *client = UA_Client_new(cfg);
	UA_Client *client = UA_Client_new(UA_ClientConfig_default);
	//UA_ClientConfig_setDefault(UA_Client_getConfig(client));
	UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://192.168.1.119:4840");
	if (retval != UA_STATUSCODE_GOOD)
	{
		UA_Client_delete(client);
		return (int)retval;
	}

	// Read the value attribute of the node. UA_Client_readValueAttribute is a // wrapper for the raw read service 
	// available as UA_Client_Service_read. UA_Variant value; // Variants can hold scalar values and arrays of any type 
	UA_Variant value; 
	UA_Variant_init(&value);

	/* NodeId of the variable holding the current time */
	const UA_NodeId nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME);
	retval = UA_Client_readValueAttribute(client, nodeId, &value);

	if (retval == UA_STATUSCODE_GOOD &&
		UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_DATETIME]))
	{
		UA_DateTime raw_date = *(UA_DateTime *)value.data;
		UA_DateTimeStruct dts = UA_DateTime_toStruct(raw_date);
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "date is: %u-%u-%u %u:%u:%u.%03u\n",
			dts.day, dts.month, dts.year, dts.hour, dts.min, dts.sec, dts.milliSec);
	}

	// Clean up
	//UA_Variant_clear(&value);
	UA_Client_disconnect(client);
	UA_Client_delete(client); // Disconnects the client internally
	return EXIT_SUCCESS;
}





UA_Variant executeMethodCall(UA_Client *client, const UA_NodeId &variableId)
{
//#ifdef UA_ENABLE_METHODCALLS
	/* Call a remote method */
	UA_Variant input;
	UA_String argString = UA_STRING("Hello Server");
	UA_Variant_init(&input);
	UA_Variant_setScalarCopy(&input, &argString, &UA_TYPES[UA_TYPES_STRING]);
	size_t outputSize;
	UA_Variant *output;
	UA_StatusCode retval = UA_Client_call(client, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
		UA_NODEID_NUMERIC(1, 62541), 1, &input, &outputSize, &output);
	bool res = (retval == UA_STATUSCODE_GOOD);
	if (res)
	{
		printf("Method call was successful, and %lu returned values available.\n",
			(unsigned long)outputSize);
		UA_Array_delete(output, outputSize, &UA_TYPES[UA_TYPES_VARIANT]);
	}
	else {
		printf("Method call was unsuccessful, and %x returned values available.\n", retval);
	}

	////	UA_Variant_clear(&input);

	return res ? *output : UA_Variant();

//#endif
}




UA_Variant browsAndReadNodeValue(UA_Client *client, const std::string uaObjectFolder, UA_NodeId &variableId)
{

	/* UA_Client* */ client = UA_Client_new(UA_ClientConfig_default);
	UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://192.168.1.119:4840");
	if (retval != UA_STATUSCODE_GOOD)
	{
		UA_Client_delete(client);
		return UA_Variant(); //(int)retval;
	}

	// Browse some objects 
	printf("Browsing nodes in objects folder:\n");
	UA_BrowseRequest bReq;
	UA_BrowseRequest_init(&bReq);
	bReq.requestedMaxReferencesPerNode = 0;
	bReq.nodesToBrowse = UA_BrowseDescription_new();
	bReq.nodesToBrowseSize = 1;
	bReq.nodesToBrowse[0].nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER); // browse objects folder 
	bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL; // return everything
	//bReq.nodesToBrowse[0].
	UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);
	printf("%-9s %-16s %-16s %-16s\n", "NAMESPACE", "NODEID", "BROWSE NAME", "DISPLAY NAME");
	for (size_t i = 0; i < bResp.resultsSize; ++i)
	{
		for (size_t j = 0; j < bResp.results[i].referencesSize; ++j)
		{
			UA_ReferenceDescription *ref = &(bResp.results[i].references[j]);
			if (ref->nodeId.nodeId.identifierType == UA_NODEIDTYPE_NUMERIC)
			{
				printf("%-9d %-16d %-16.*s %-16.*s\n", ref->nodeId.nodeId.namespaceIndex,
					ref->nodeId.nodeId.identifier.numeric, (int)ref->browseName.name.length,
					ref->browseName.name.data, (int)ref->displayName.text.length,
					ref->displayName.text.data);
			}
			else if (ref->nodeId.nodeId.identifierType == UA_NODEIDTYPE_STRING)
			{
				printf("%-9d %-16.*s %-16.*s %-16.*s\n", ref->nodeId.nodeId.namespaceIndex,
					(int)ref->nodeId.nodeId.identifier.string.length,
					ref->nodeId.nodeId.identifier.string.data,
					(int)ref->browseName.name.length, ref->browseName.name.data,
					(int)ref->displayName.text.length, ref->displayName.text.data);
			}
			/* TODO: distinguish further types */
		}
	}

	////	UA_BrowseRequest_clear(&bReq);
	////	UA_BrowseResponse_clear(&bResp);

		/* Same thing, this time using the node iterator... */
	UA_NodeId *parent = UA_NodeId_new();
	*parent = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
	UA_Client_forEachChildNodeCall(client, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
		nodeIter, (void *)parent);
	UA_NodeId_delete(parent);

}




UA_Variant readVariable(UA_Client *client, const UA_NodeId &variableId)
{
	UA_Variant value;
	//UA_Server_readValue(_impl->_server, variableId, &value);
	UA_StatusCode retval = UA_Client_readValueAttribute(client, variableId, &value);

/*
	UA_Client_readValueAttribute(UA_Client *client, const UA_NodeId nodeId, UA_Variant *outValue) {
		return __UA_Client_readAttribute(client, &nodeId, UA_ATTRIBUTEID_VALUE, outValue, &UA_TYPES[UA_TYPES_VARIANT]); }

	static UA_INLINE UA_StatusCode UA_Client_readNodeClassAttribute(UA_Client *client, const UA_NodeId nodeId, UA_NodeClass *outNodeClass) {
		return __UA_Client_readAttribute(client, &nodeId, UA_ATTRIBUTEID_NODECLASS, outNodeClass, &UA_TYPES[UA_TYPES_NODECLASS]); }

	static UA_INLINE UA_StatusCode UA_Client_readBrowseNameAttribute(UA_Client *client, const UA_NodeId nodeId, UA_QualifiedName *outBrowseName) {
		return __UA_Client_readAttribute(client, &nodeId, UA_ATTRIBUTEID_BROWSENAME, outBrowseName, &UA_TYPES[UA_TYPES_QUALIFIEDNAME]); }

	static UA_INLINE UA_StatusCode UA_Client_readDisplayNameAttribute(UA_Client *client, const UA_NodeId nodeId, UA_LocalizedText *outDisplayName) {
		return __UA_Client_readAttribute(client, &nodeId, UA_ATTRIBUTEID_DISPLAYNAME, outDisplayName, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]); }

	static UA_INLINE UA_StatusCode UA_Client_readDescriptionAttribute(UA_Client *client, const UA_NodeId nodeId, UA_LocalizedText *outDescription) {
		return __UA_Client_readAttribute(client, &nodeId, UA_ATTRIBUTEID_DESCRIPTION, outDescription, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]); }
*/


	return value;
}




void testOpcUaClientRead()
{
	//UA_NodeId _idRoot = (UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER), "Demo OPC UA");

	UA_NS0ID_ROOTFOLDER; //84 // Object
	UA_NS0ID_OBJECTSFOLDER; //85 // Object
	UA_NS0ID_TYPESFOLDER; //86 // Object


	// x.get_children()
	//var1_node = client.get_node("ns=6;s=::AsDataPV:Var1")
	//var1_value = var1_node.get_value()


	// Add server information 

	//UA_NodeId _idRoot, _idUptime, _idLoopCount;
	//_idRoot = addFolder(UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER), "Demo OPC UA");
	//_tpStart = std::chrono::system_clock::now();
	//_idLoopCount = addInt32(_idRoot, "Loop count", 0);
	//addVariable(_idRoot, "Time start", &UA_TYPES[UA_TYPES_DATETIME], UA_DateTime(_impl->_tpStart.time_since_epoch().count()));

/*
	//UA_NodeId var = resolveVariableId(objectId, command);
	//if (isNull(&var)) return;

	//UA_Variant v = readVariable(var);
	//uint32_t *value = (uint32_t *)(v.data);

	enum PointCommands
	{
		cmdNone = 0, cmdBlock =  1, cmdUnblock = 2, cmdToPlus = 3, cmdToMinus = 4
	};
	enum PointIndication
	{
		indUndefined, indIsBlocked = 1, indIsUnblocked = 2, indIsPlusPosition = 3, indIsMinusPosition = 4
	};


	PointCommands cmdValue = cmdNone;
	if (command == "block") cmdValue = cmdBlock;
	else if (command == "unblock") cmdValue = cmdUnblock;
	else if (command == "toPlus") cmdValue = cmdToPlus;
	else if (command == "toMinus") cmdValue = cmdToMinus;

	UA_NodeId idBlock = resolveVariableId(objectId, "Block"),
		idPosition = resolveVariableId(objectId, "Position");

//	PointCommands cmdValue = static_cast<PointCommands>(*value);

	switch(cmdValue)
	{
		case cmdNone: break;


		case cmdBlock:
		case cmdUnblock:
		{
			UA_Variant vBlock;
			uint32_t indBlock = static_cast<PointIndication>(cmdValue == cmdBlock ? indIsBlocked : indIsUnblocked);
			UA_Variant_setScalar(&vBlock, &indBlock, &UA_TYPES[UA_TYPES_UINT32]);
			writeVariable(idBlock, vBlock);
		}
		break;

		case cmdToPlus:
		case cmdToMinus:
		{
			PointIndication block = *(static_cast<PointIndication *>(readVariable(idBlock).data));
			if (block == indIsBlocked)
			{

			}
			else
			{
				UA_Variant vPosition;
				uint32_t position = static_cast<PointIndication>(cmdValue == cmdToPlus ? indIsPlusPosition : indIsMinusPosition);
				UA_Variant_setScalar(&vPosition, &position, &UA_TYPES[UA_TYPES_UINT32]);
				writeVariable(idPosition, vPosition);
			}
		}
		break;

	}


	//*(value) = *(value) + 1;
	//*value = 0; // release command

	//writeVariable(var, v);


	///// Find the NodeId of the status child variable 
	//UA_RelativePathElement rpe;
	//UA_RelativePathElement_init(&rpe);
	//rpe.referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT);
	//rpe.isInverse = false;
	//rpe.includeSubtypes = false;
	//rpe.targetName = UA_QUALIFIEDNAME(1, "Position");

	//UA_BrowsePath bp;
	//UA_BrowsePath_init(&bp);
	//bp.startingNode = objectId;
	//bp.relativePath.elementsSize = 1;
	//bp.relativePath.elements = &rpe;

	//UA_BrowsePathResult bpr = UA_Server_translateBrowsePathToNodeIds(_impl->_server, &bp);
	//if (bpr.statusCode != UA_STATUSCODE_GOOD || bpr.targetsSize < 1)
	//	return /*bpr.statusCode*/;


	///// Read current value 
	//UA_NodeId varId = bpr.targets[0].targetId.nodeId;
	//
	//UA_Variant varValue;
	//UA_Server_readValue(_impl->_server, varId, &varValue);
	//uint32_t *cnt = (uint32_t *)(varValue.data);
	//// *(cnt) = *(cnt)+1;

	//UA_Server_writeValue(_impl->_server, varId, varValue);

	//// Set the status value 
	//UA_BrowsePathResult_deleteMembers(&bpr);
//*/

}




const UA_DataType *toDataType(QOpcUa::Types valueType)
{
	switch (valueType) {
	case QOpcUa::Boolean:
		return &UA_TYPES[UA_TYPES_BOOLEAN];
	case QOpcUa::Int32:
		return &UA_TYPES[UA_TYPES_INT32];
	case QOpcUa::UInt32:
		return &UA_TYPES[UA_TYPES_UINT32];
	case QOpcUa::Double:
		return &UA_TYPES[UA_TYPES_DOUBLE];
	case QOpcUa::Float:
		return &UA_TYPES[UA_TYPES_FLOAT];
	case QOpcUa::String:
		return &UA_TYPES[UA_TYPES_STRING];
	case QOpcUa::LocalizedText:
		return &UA_TYPES[UA_TYPES_LOCALIZEDTEXT];
	case QOpcUa::DateTime:
		return &UA_TYPES[UA_TYPES_DATETIME];
	case QOpcUa::UInt16:
		return &UA_TYPES[UA_TYPES_UINT16];
	case QOpcUa::Int16:
		return &UA_TYPES[UA_TYPES_INT16];
	case QOpcUa::UInt64:
		return &UA_TYPES[UA_TYPES_UINT64];
	case QOpcUa::Int64:
		return &UA_TYPES[UA_TYPES_INT64];
	case QOpcUa::Byte:
		return &UA_TYPES[UA_TYPES_BYTE];
	case QOpcUa::SByte:
		return &UA_TYPES[UA_TYPES_SBYTE];
	case QOpcUa::ByteString:
		return &UA_TYPES[UA_TYPES_BYTESTRING];
	case QOpcUa::XmlElement:
		return &UA_TYPES[UA_TYPES_XMLELEMENT];
	case QOpcUa::NodeId:
		return &UA_TYPES[UA_TYPES_NODEID];
	case QOpcUa::Guid:
		return &UA_TYPES[UA_TYPES_GUID];
	case QOpcUa::QualifiedName:
		return &UA_TYPES[UA_TYPES_QUALIFIEDNAME];
	case QOpcUa::StatusCode:
		return &UA_TYPES[UA_TYPES_STATUSCODE];
	case QOpcUa::Range:
	case QOpcUa::EUInformation:
	case QOpcUa::ComplexNumber:
	case QOpcUa::DoubleComplexNumber:
	case QOpcUa::AxisInformation:
	case QOpcUa::XV:
		return &UA_TYPES[UA_TYPES_EXTENSIONOBJECT];
	default:
		//qCWarning(QT_OPCUA_PLUGINS_OPEN62541) << "Trying to convert undefined type:" << valueType;
		return nullptr;
	}
}










QVariant toQVariant(const UA_Variant &value)
{
	if (value.type == nullptr) {
		//qCWarning(QT_OPCUA_PLUGINS_OPEN62541, "Null variant received, unable to convert");
		return QVariant();
	}
	switch (value.type->typeIndex) {
	case UA_TYPES_BOOLEAN:
		return arrayToQVariant<bool, UA_Boolean>(value, QMetaType::Bool);
	case UA_TYPES_SBYTE:
		return arrayToQVariant<signed char, UA_SByte>(value, QMetaType::SChar);
	case UA_TYPES_BYTE:
		return arrayToQVariant<uchar, UA_Byte>(value, QMetaType::UChar);
	case UA_TYPES_INT16:
		return arrayToQVariant<qint16, UA_Int16>(value, QMetaType::Short);
	case UA_TYPES_UINT16:
		return arrayToQVariant<quint16, UA_UInt16>(value, QMetaType::UShort);
	case UA_TYPES_INT32:
		return arrayToQVariant<qint32, UA_Int32>(value, QMetaType::Int);
	case UA_TYPES_UINT32:
		return arrayToQVariant<quint32, UA_UInt32>(value, QMetaType::UInt);
	case UA_TYPES_INT64:
		return arrayToQVariant<int64_t, UA_Int64>(value, QMetaType::LongLong);
	case UA_TYPES_UINT64:
		return arrayToQVariant<uint64_t, UA_UInt64>(value, QMetaType::ULongLong);
	case UA_TYPES_FLOAT:
		return arrayToQVariant<float, UA_Float>(value, QMetaType::Float);
	case UA_TYPES_DOUBLE:
		return arrayToQVariant<double, UA_Double>(value, QMetaType::Double);
	case UA_TYPES_STRING:
		return arrayToQVariant<QString, UA_String>(value, QMetaType::QString);
	case UA_TYPES_BYTESTRING:
		return arrayToQVariant<QByteArray, UA_ByteString>(value, QMetaType::QByteArray);
	case UA_TYPES_LOCALIZEDTEXT:
		return arrayToQVariant<QOpcUa::QLocalizedText, UA_LocalizedText>(value, QMetaType::QString);
	case UA_TYPES_NODEID:
		return arrayToQVariant<QString, UA_NodeId>(value, QMetaType::QString);
	case UA_TYPES_DATETIME:
		return arrayToQVariant<QDateTime, UA_DateTime>(value, QMetaType::QDateTime);
	case UA_TYPES_GUID:
		return arrayToQVariant<QUuid, UA_Guid>(value, QMetaType::QUuid);
	case UA_TYPES_XMLELEMENT:
		return arrayToQVariant<QString, UA_XmlElement>(value, QMetaType::QString);
	case UA_TYPES_QUALIFIEDNAME:
		/////return arrayToQVariant<QOpcUa::QQualifiedName, UA_QualifiedName>(value);
	case UA_TYPES_STATUSCODE:
		return arrayToQVariant<QOpcUa::UaStatusCode, UA_StatusCode>(value, QMetaType::UInt);
	case UA_TYPES_EXTENSIONOBJECT:
		/////return arrayToQVariant<QVariant, UA_ExtensionObject>(value);
	default:
		//qCWarning(QT_OPCUA_PLUGINS_OPEN62541) << "Variant conversion from Open62541 for typeIndex" << value.type->typeIndex << " not implemented";
		return QVariant();
	}
}

QOpcUa::Types qvariantTypeToQOpcUaType(QMetaType::Type type)
{
	switch (type) {
	case QMetaType::Bool:
		return QOpcUa::Boolean;
	case QMetaType::UChar:
		return QOpcUa::Byte;
	case QMetaType::Char:
		return QOpcUa::SByte;
	case QMetaType::UShort:
		return QOpcUa::UInt16;
	case QMetaType::Short:
		return QOpcUa::Int16;
	case QMetaType::Int:
		return QOpcUa::Int32;
	case QMetaType::UInt:
		return QOpcUa::UInt32;
	case QMetaType::ULongLong:
		return QOpcUa::UInt64;
	case QMetaType::LongLong:
		return QOpcUa::Int64;
	case QMetaType::Double:
		return QOpcUa::Double;
	case QMetaType::Float:
		return QOpcUa::Float;
	case QMetaType::QString:
		return QOpcUa::String;
		//return QOpcUa::LocalizedText;  // TODO: unclear
	case QMetaType::QDateTime:
		return QOpcUa::DateTime;
	case QMetaType::QByteArray:
		return QOpcUa::ByteString;
	case QMetaType::QUuid:
		return QOpcUa::Guid;
		//return QOpcUa::XmlElement;
		//return QOpcUa::NodeId;
	default:
		break;
	}
	return QOpcUa::Undefined;
}

//*
UA_Variant toOpen62541Variant(const QVariant &value, QOpcUa::Types type)
{
	UA_Variant open62541value;
	UA_Variant_init(&open62541value);
	if (value.type() == QVariant::List && value.toList().size() == 0)
		return open62541value;
	QVariant temp = (value.type() == QVariant::List) ? value.toList().at(0) : value;
	QOpcUa::Types valueType = type == QOpcUa::Undefined ?
		qvariantTypeToQOpcUaType(static_cast<QMetaType::Type>(temp.type())) : type;
	const UA_DataType *dt = toDataType(valueType);
	switch (valueType) {
	case QOpcUa::Boolean:
		return arrayFromQVariant<UA_Boolean, bool>(value, dt);
	case QOpcUa::SByte:
		return arrayFromQVariant<UA_SByte, char>(value, dt);
	case QOpcUa::Byte:
		return arrayFromQVariant<UA_Byte, uchar>(value, dt);
	case QOpcUa::Int16:
		return arrayFromQVariant<UA_Int16, qint16>(value, dt);
	case QOpcUa::UInt16:
		return arrayFromQVariant<UA_UInt16, quint16>(value, dt);
	case QOpcUa::Int32:
		return arrayFromQVariant<UA_Int32, qint32>(value, dt);
	case QOpcUa::UInt32:
		return arrayFromQVariant<UA_UInt32, quint32>(value, dt);
	case QOpcUa::Int64:
		return arrayFromQVariant<UA_Int64, int64_t>(value, dt);
	case QOpcUa::UInt64:
		return arrayFromQVariant<UA_UInt64, uint64_t>(value, dt);
	case QOpcUa::Float:
		return arrayFromQVariant<UA_Float, float>(value, dt);
	case QOpcUa::Double:
		return arrayFromQVariant<UA_Double, double>(value, dt);
	case QOpcUa::DateTime:
		return arrayFromQVariant<UA_DateTime, QDateTime>(value, dt);
	case QOpcUa::String:
		return arrayFromQVariant<UA_String, QString>(value, dt);
	case QOpcUa::LocalizedText:
		return arrayFromQVariant<UA_LocalizedText, QOpcUa::QLocalizedText>(value, dt);
	case QOpcUa::ByteString:
		return arrayFromQVariant<UA_ByteString, QByteArray>(value, dt);
	case QOpcUa::NodeId:
		return arrayFromQVariant<UA_NodeId, QString>(value, dt);
	case QOpcUa::Guid:
		return arrayFromQVariant<UA_Guid, QUuid>(value, dt);
	case QOpcUa::XmlElement:
		return arrayFromQVariant<UA_XmlElement, QString>(value, dt);
	case QOpcUa::QualifiedName:
		return arrayFromQVariant<UA_QualifiedName, QOpcUa::QQualifiedName>(value, dt);
	case QOpcUa::StatusCode:
		return arrayFromQVariant<UA_StatusCode, QOpcUa::UaStatusCode>(value, dt);
	case QOpcUa::Range:
		return arrayFromQVariant<UA_ExtensionObject, QOpcUa::QRange>(value, dt);
	case QOpcUa::EUInformation:
		return arrayFromQVariant<UA_ExtensionObject, QOpcUa::QEUInformation>(value, dt);
	case QOpcUa::ComplexNumber:
		return arrayFromQVariant<UA_ExtensionObject, QOpcUa::QComplexNumber>(value, dt);
	case QOpcUa::DoubleComplexNumber:
		return arrayFromQVariant<UA_ExtensionObject, QOpcUa::QDoubleComplexNumber>(value, dt);
	case QOpcUa::AxisInformation:
		return arrayFromQVariant<UA_ExtensionObject, QOpcUa::QAxisInformation>(value, dt);
	case QOpcUa::XV:
		return arrayFromQVariant<UA_ExtensionObject, QOpcUa::QXValue>(value, dt);
	default:
		//qCWarning(QT_OPCUA_PLUGINS_OPEN62541) << "Variant conversion to Open62541 for typeIndex" << type << " not implemented";
		break;
	}
	return open62541value;
}
//*/


										//		objectId того к кому метод применяем (1. Block или Position 2. PT1) ?
										// или	objectId родителя (HMIServer-Main)
void callMethod(UA_Client* m_uaclient, /*uintptr_t handle,*/ UA_NodeId objectId, UA_NodeId methodId, QVector<QOpcUa::TypedVariant> args, bool& callResult)
{
	UA_Variant *inputArgs = nullptr;
	if (args.size())
	{
		inputArgs = static_cast<UA_Variant *>(UA_Array_new(args.size(), &UA_TYPES[UA_TYPES_VARIANT]));
		for (int i = 0; i < args.size(); ++i)
		{
			//UA_Variant var;
			//	args[i].first;
			//	args[i].second;

			// PT1
			// block / unblock

			//*
			inputArgs[i] = toOpen62541Variant
			(
				  args[i].first
				, args[i].second
			);
			//*/
		}
	}
	size_t outputSize = 0;
	UA_Variant *outputArguments;
	UA_StatusCode res = UA_Client_call(m_uaclient, objectId, methodId, args.size(), inputArgs, &outputSize, &outputArguments);
	if (res != UA_STATUSCODE_GOOD)
	{
		//qCWarning(QT_OPCUA_PLUGINS_OPEN62541, "Could not call method: 0x%X", res);
		callResult = false;
	}
	else
	{
		callResult = true;
	}

	QVariant result;
	if (outputSize > 1 && res == UA_STATUSCODE_GOOD)
	{
		QVariantList temp;
		for (size_t i = 0; i < outputSize; ++i)
		{
			temp.append(toQVariant(outputArguments[i]));
		}

		result = temp;
	}
	else if (outputSize == 1 && res == UA_STATUSCODE_GOOD)
	{
		result = toQVariant(outputArguments[0]);
	}

	UA_Array_delete(inputArgs, args.size(), &UA_TYPES[UA_TYPES_VARIANT]);
	if (outputSize > 0)
		UA_Array_delete(outputArguments, outputSize, &UA_TYPES[UA_TYPES_VARIANT]);

	//emit methodCallFinished(handle, Open62541Utils::nodeIdToQString(methodId), result, static_cast<QOpcUa::UaStatusCode>(res));
	UA_NodeId_deleteMembers(&objectId);
	UA_NodeId_deleteMembers(&methodId);
	//checkAndUpdateClientState();
}




void convertBrowseResult(UA_BrowseResult *src, quint32 referencesSize, QVector<UA_ReferenceDescription> &dst)
{
	if (!src)
		return;
	for (size_t i = 0; i < referencesSize; ++i)
	{
		UA_ReferenceDescription temp;
		temp = src->references[i];

		printf("%-9d %-16d %-16.*s %-16.*s\n"
			,		temp.nodeId.nodeId.namespaceIndex
			,		temp.nodeId.nodeId.identifier.numeric
			, (int)	temp.browseName.name.length
			,		temp.browseName.name.data
			, (int)	temp.displayName.text.length
			,		temp.displayName.text.data
		);

		/*
		temp.nodeId.nodeId = ((src->references[i].nodeId.nodeId));
		temp.nodeClass = src->references[i].nodeClass;

		/*
		temp.setRefType(static_cast<QOpcUa::ReferenceTypeId>(src->references[i].referenceTypeId.identifier.numeric));
		temp.setNodeClass(static_cast<QOpcUa::NodeClass>(src->references[i].nodeClass));
		temp.setBrowseName(QOpen62541ValueConverter::scalarToQVariant<QOpcUa::QQualifiedName, UA_QualifiedName>(
			&(src->references[i].browseName), QMetaType::Type::UnknownType).value<QOpcUa::QQualifiedName>());
		temp.setDisplayName(QOpen62541ValueConverter::scalarToQVariant<QOpcUa::QLocalizedText, UA_LocalizedText>(
			&(src->references[i].displayName), QMetaType::Type::UnknownType).value<QOpcUa::QLocalizedText>());
		*/

		dst.push_back(temp);
	}
}




UA_NodeId findChildWithBrowseName(std::string strBrowsNameToFind, UA_Client *m_uaclient, /*uintptr_t handle,*/ UA_NodeId id, UA_NodeIdType /*ReferenceTypeId*/ referenceType, UA_BrowseResultMask /*QOpcUa::NodeClasses*/ nodeClassMask)
{
	//
// https://code.woboq.org/qt5/qtopcua/src/plugins/opcua/open62541/qopen62541backend.cpp.html
//

/*
	UA_Client *m_uaclient = UA_Client_new(UA_ClientConfig_default);
	UA_StatusCode retval = UA_Client_connect(m_uaclient, "opc.tcp://192.168.1.119:4840");
	if (retval != UA_STATUSCODE_GOOD)
	{
		UA_Client_delete(m_uaclient);
		return; //(int)retval;
	}
*/

	UA_BrowseRequest request;
	UA_BrowseRequest_init(&request);
	request.nodesToBrowse = UA_BrowseDescription_new();
	request.nodesToBrowseSize = 1;
	request.nodesToBrowse->browseDirection = UA_BROWSEDIRECTION_FORWARD;
	request.nodesToBrowse->includeSubtypes = true;
	request.nodesToBrowse->nodeClassMask = static_cast<quint32>(nodeClassMask);
	request.nodesToBrowse->nodeId = id;
	request.nodesToBrowse->resultMask = UA_BROWSERESULTMASK_BROWSENAME | UA_BROWSERESULTMASK_DISPLAYNAME |
		UA_BROWSERESULTMASK_REFERENCETYPEID | UA_BROWSERESULTMASK_NODECLASS;
	request.nodesToBrowse->referenceTypeId = UA_NODEID_NUMERIC(0, static_cast<quint32>(referenceType));
	request.requestedMaxReferencesPerNode = 0; // Let the server choose a maximum value
	UA_BrowseResponse *response = UA_BrowseResponse_new();
	*response = UA_Client_Service_browse(m_uaclient, request);
	UA_BrowseRequest_deleteMembers(&request);

	QVector<UA_ReferenceDescription> ret;
	UA_StatusCode statusCode = UA_STATUSCODE_GOOD;

	while (response->resultsSize && statusCode == UA_STATUSCODE_GOOD)
	{
		UA_BrowseResponse *res = static_cast<UA_BrowseResponse *>(response);
		if (res->responseHeader.serviceResult != UA_STATUSCODE_GOOD || res->results->statusCode != UA_STATUSCODE_GOOD)
		{
			statusCode = /*static_cast<QOpcUa::UaStatusCode>*/ (res->results->statusCode);
			break;
		}

		convertBrowseResult(res->results, res->results->referencesSize, ret);

		for (auto refDesr : ret)
		{
			if ((strBrowsNameToFind.length() == (int)refDesr.browseName.name.length)
				&& (strBrowsNameToFind == uaToString(refDesr.browseName.name))
				)
			{
				return refDesr.nodeId.nodeId;
			}
		}
/*
		if (res->results->continuationPoint.length)
		{
			UA_BrowseNextRequest nextReq;
			UA_BrowseNextRequest_init(&nextReq);
			nextReq.continuationPoints = UA_ByteString_new();
			UA_ByteString_copy(&(res->results->continuationPoint), nextReq.continuationPoints);
			nextReq.continuationPointsSize = 1;
			UA_BrowseResponse_deleteMembers(res);
			*reinterpret_cast<UA_BrowseNextResponse *>(response) = UA_Client_Service_browseNext(m_uaclient, nextReq);
			UA_BrowseNextRequest_deleteMembers(&nextReq);
		}
		else
		{
			break;
		}
*/
	}
	//emit browseFinished(handle, ret, statusCode);
	//UA_BrowseResponse_delete(static_cast<UA_BrowseResponse *>(response));

	//checkAndUpdateClientState();

}


void browseChildren(UA_Client *m_uaclient, /*uintptr_t handle,*/ UA_NodeId id, UA_NodeIdType /*ReferenceTypeId*/ referenceType, UA_BrowseResultMask /*QOpcUa::NodeClasses*/ nodeClassMask)
{
	//
	// https://code.woboq.org/qt5/qtopcua/src/plugins/opcua/open62541/qopen62541backend.cpp.html
	//

/*
	UA_Client *m_uaclient = UA_Client_new(UA_ClientConfig_default);
	UA_StatusCode retval = UA_Client_connect(m_uaclient, "opc.tcp://192.168.1.119:4840");
	if (retval != UA_STATUSCODE_GOOD)
	{
		UA_Client_delete(m_uaclient);
		return; //(int)retval;
	}
*/

	UA_BrowseRequest request;
	UA_BrowseRequest_init(&request);
	request.nodesToBrowse = UA_BrowseDescription_new();
	request.nodesToBrowseSize = 1;
	request.nodesToBrowse->browseDirection = UA_BROWSEDIRECTION_FORWARD;
	request.nodesToBrowse->includeSubtypes = true;
	request.nodesToBrowse->nodeClassMask = static_cast<quint32>(nodeClassMask);
	request.nodesToBrowse->nodeId = id;
	request.nodesToBrowse->resultMask = UA_BROWSERESULTMASK_BROWSENAME | UA_BROWSERESULTMASK_DISPLAYNAME |
		UA_BROWSERESULTMASK_REFERENCETYPEID | UA_BROWSERESULTMASK_NODECLASS;
	request.nodesToBrowse->referenceTypeId = UA_NODEID_NUMERIC(0, static_cast<quint32>(referenceType));
	request.requestedMaxReferencesPerNode = 0; // Let the server choose a maximum value
	UA_BrowseResponse *response = UA_BrowseResponse_new();
	*response = UA_Client_Service_browse(m_uaclient, request);
	UA_BrowseRequest_deleteMembers(&request);

	QVector<UA_ReferenceDescription> ret;
	UA_StatusCode statusCode = UA_STATUSCODE_GOOD;

	while (response->resultsSize && statusCode == UA_STATUSCODE_GOOD)
	{
		UA_BrowseResponse *res = static_cast<UA_BrowseResponse *>(response);
		if (res->responseHeader.serviceResult != UA_STATUSCODE_GOOD || res->results->statusCode != UA_STATUSCODE_GOOD)
		{
			statusCode = /*static_cast<QOpcUa::UaStatusCode>*/ (res->results->statusCode);
			break;
		}

		convertBrowseResult(res->results, res->results->referencesSize, ret);

		for (auto refDesr : ret)
		{
			const std::string strBrowsNameToFind = "ExecuteCommand";
			if ((strBrowsNameToFind.length() == (int)refDesr.browseName.name.length)
				&& (strBrowsNameToFind == uaToString(refDesr.browseName.name))
				)
			{
				UA_NodeId objectId;
				UA_NodeId methodId = refDesr.nodeId.nodeId;

				QVector<QOpcUa::TypedVariant> args;

				QVariant valObjectName("PT1");
				QPair<QVariant, QOpcUa::Types> pairObjectName ( valObjectName, QOpcUa::Types::String );
				args.push_back ( pairObjectName /*QOpcUa::TypedVariant*/ );

				QVariant valCommand("unblock");
				QPair<QVariant, QOpcUa::Types> pairCommand( valCommand, QOpcUa::Types::String );
				args.push_back( pairCommand /*QOpcUa::TypedVariant*/);

				// args[QString] // ObjectName // 					PT1
				// args[QString] // Command name // 				unblock / block

				// Resultat  // Command 'unblock' for object 'PT1' has been execute successfully
				//		objectId того к кому метод применяем (1. Block или Position 2. PT1) ?
														// или	objectId родителя (HMIServer-Main)
				
				bool resCall = false;
				callMethod(m_uaclient, objectId, methodId, args, resCall);

				resCall = resCall;
				//m_uaclient, /*uintptr_t handle,*/ UA_NodeId objectId, UA_NodeId methodId, QVector<QOpcUa::TypedVariant> args)

			}

		}



		if (res->results->continuationPoint.length)
		{
			UA_BrowseNextRequest nextReq;
			UA_BrowseNextRequest_init(&nextReq);
			nextReq.continuationPoints = UA_ByteString_new();
			UA_ByteString_copy(&(res->results->continuationPoint), nextReq.continuationPoints);
			nextReq.continuationPointsSize = 1;
			UA_BrowseResponse_deleteMembers(res);
			*reinterpret_cast<UA_BrowseNextResponse *>(response) = UA_Client_Service_browseNext(m_uaclient, nextReq);
			UA_BrowseNextRequest_deleteMembers(&nextReq);
		}
		else
		{
			break;
		}
	}
	//emit browseFinished(handle, ret, statusCode);
	UA_BrowseResponse_delete(static_cast<UA_BrowseResponse *>(response));

	//checkAndUpdateClientState();
}



void testOpcUaBrowseObjectFolder()
{
	//
	// https://github.com/open62541/open62541/blob/master/examples/client.c
	//
	UA_Client *client = UA_Client_new(UA_ClientConfig_default);
	UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://192.168.1.119:4840");
	if (retval != UA_STATUSCODE_GOOD)
	{
		UA_Client_delete(client);
		return /*UA_Variant()*/ ; //(int)retval;
	}

    // Listing endpoints //
    UA_EndpointDescription* endpointArray = NULL;
    size_t endpointArraySize = 0;
    retval = UA_Client_getEndpoints(client, "opc.tcp://192.168.1.119:4840",
                                           &endpointArraySize, &endpointArray);
    if(retval != UA_STATUSCODE_GOOD)
	{
        UA_Array_delete(endpointArray, endpointArraySize, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION]);
        UA_Client_delete(client);
        return /*EXIT_FAILURE*/ ;
    }
    printf("%i endpoints found\n", (int)endpointArraySize);
    for(size_t i=0;i<endpointArraySize;i++) {
        printf("URL of endpoint %i is %.*s\n", (int)i,
               (int)endpointArray[i].endpointUrl.length,
               endpointArray[i].endpointUrl.data);
    }
    UA_Array_delete(endpointArray,endpointArraySize, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION]);

    // Connect to a server
    // anonymous connect would be: retval = UA_Client_connect(client, "opc.tcp://localhost:4840"); */
    retval = UA_Client_connect_username(client, "opc.tcp://localhost:4840", "user1", "password");
    if(retval != UA_STATUSCODE_GOOD) {
        UA_Client_delete(client);
        return /*EXIT_FAILURE*/ ;
    }

    // Browse some objects
    printf("Browsing nodes in objects folder:\n");
    UA_BrowseRequest bReq;
    UA_BrowseRequest_init(&bReq);
    bReq.requestedMaxReferencesPerNode = 0;
    bReq.nodesToBrowse = UA_BrowseDescription_new();
    bReq.nodesToBrowseSize = 1;
    bReq.nodesToBrowse[0].nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER); // browse objects folder
    bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL; // return everything

	UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);
    printf("%-9s %-16s %-16s %-16s\n", "NAMESPACE", "NODEID", "BROWSE NAME", "DISPLAY NAME");

    for(size_t i = 0; i < bResp.resultsSize; ++i)
	{
        for(size_t j = 0; j < bResp.results[i].referencesSize; ++j)
		{
            UA_ReferenceDescription *ref = &(bResp.results[i].references[j]);
            
			if(ref->nodeId.nodeId.identifierType == UA_NODEIDTYPE_NUMERIC)
			{
                printf("%-9d %-16d %-16.*s %-16.*s\n"
					, ref->nodeId.nodeId.namespaceIndex
					, ref->nodeId.nodeId.identifier.numeric
					, (int)ref->browseName.name.length
                    , ref->browseName.name.data
					, (int)ref->displayName.text.length
                    , ref->displayName.text.data
				);

				// https://github.com/open62541/open62541/blob/master/tests/fuzz/corpus_generator.c
				// UA_BrowseNextRequest bNextReq;

				const std::string hmiSrvMainFolder = "HMIServer-main";
				if ((hmiSrvMainFolder.length() == (int)ref->browseName.name.length)
					&& (hmiSrvMainFolder == uaToString(ref->browseName.name))
					)
				{

					browseChildren(client, ref->nodeId.nodeId, UA_NODEIDTYPE_NUMERIC, UA_BROWSERESULTMASK_ALL);
					//browseChildren( UA_NodeId id, UA_NodeIdType referenceType, UA_BrowseResultMask nodeClassMask);

					/*
					// browse next
					UA_BrowseNextRequest bNextReq;
					UA_BrowseNextRequest_init(&bNextReq);
					// normally is set to 0, to get all the nodes, but we want to test browse next
					bNextReq.releaseContinuationPoints = 0;
					//bNextReq.releaseContinuationPoints = UA_FALSE;
					bNextReq.continuationPoints = &bResp.results[i].continuationPoint;
					bNextReq.continuationPointsSize = 1;

					UA_BrowseNextResponse bNextResp = UA_Client_Service_browseNext(client, bNextReq);
					//ASSERT_GOOD();
					if (bNextResp.responseHeader.serviceResult != UA_STATUSCODE_GOOD) { UA_Client_delete(client); return ; }
					
					printf("%-9s %-16s %-16s %-16s\n", "NAMESPACE", "NODEID", "BROWSE NAME", "DISPLAY NAME");

					for (size_t ii = 0; ii < bNextResp.resultsSize; ++ii)
					{
						for (size_t jj = 0; jj < bNextResp.results[ii].referencesSize; ++jj)
						{
							UA_ReferenceDescription *ref1 = &(bNextResp.results[ii].references[jj]);

							if (ref1->nodeId.nodeId.identifierType == UA_NODEIDTYPE_NUMERIC)
							{
								printf("%-9d %-16d %-16.*s %-16.*s\n"
									,		ref1->nodeId.nodeId.namespaceIndex
									,		ref1->nodeId.nodeId.identifier.numeric
									, (int)	ref1->browseName.name.length
									,		ref1->browseName.name.data
									, (int)	ref1->displayName.text.length
									,		ref1->displayName.text.data
								);

							}
						}
					}
					*/

					/*
					UA_BrowseNextResponse_deleteMembers(&bNextResp);

					bNextResp = UA_Client_Service_browseNext(client, bNextReq);
					//ASSERT_GOOD(bNextResp.responseHeader.serviceResult);
					if (bNextResp.responseHeader.serviceResult != UA_STATUSCODE_GOOD) { UA_Client_delete(client); return ; }

					UA_BrowseNextResponse_deleteMembers(&bNextResp);

					// release continuation point. Result is then empty
					bNextReq.releaseContinuationPoints = UA_TRUE;
					bNextResp = UA_Client_Service_browseNext(client, bNextReq);
					UA_BrowseNextResponse_deleteMembers(&bNextResp);
					//ASSERT_GOOD(bNextResp.responseHeader.serviceResult);
					if (bNextResp.responseHeader.serviceResult != UA_STATUSCODE_GOOD) { UA_Client_delete(client); return ; }

					UA_BrowseRequest_deleteMembers(&bReq);
					UA_BrowseResponse_deleteMembers(&bResp);
					// already deleted by browse request
					bNextReq.continuationPoints = NULL;
					bNextReq.continuationPointsSize = 0;
					UA_BrowseNextRequest_deleteMembers(&bNextReq);
					*/

					/*
					UA_BrowseNextRequest bReq1;// = UA_BrowseNextRequest_new();
					UA_BrowseNextRequest_init(&bReq1);
					//bReq1.continuationPoints= 0;
					//bReq1.continuationPointsSize = 0;
					//bReq1.releaseContinuationPoints = 1;
					//bReq1.requestHeader. = //UA_BrowseNextRequest_new

					//bReq1.nodesToBrowse[0].nodeId = ref->nodeId.nodeId; //UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER); // browse objects folder 
					//bReq1.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL; // return everything 

					UA_BrowseNextResponse bResp1 = UA_Client_Service_browseNext(client, bReq1);
					//UA_BrowseNextResponse bResp1;
					//= UA_Client_Service_browseNext(client, bReq1);
					
					// = UA_Client_Service_browse(client, bReq1);
					//printf("%-9s %-16s %-16s %-16s\n", "NAMESPACE", "NODEID", "BROWSE NAME", "DISPLAY NAME");
					for (size_t ii = 0; ii < bResp1.resultsSize; ++ii)
					{
						for (size_t jj = 0; jj < bResp1.results[ii].referencesSize; ++jj)
						{
							UA_ReferenceDescription *ref1 = &(bResp.results[ii].references[jj]);

							if (ref1->nodeClass == UA_NODECLASS_METHOD)
							{
								int z = 0;
								printf("%-9d %-16.*s %-16.*s %-16.*s\n"
									, ref1->nodeId.nodeId.namespaceIndex
									, (int)ref1->nodeId.nodeId.identifier.string.length
									, ref1->nodeId.nodeId.identifier.string.data
									, (int)ref1->browseName.name.length
									, ref1->browseName.name.data
									, (int)ref1->displayName.text.length
									, ref1->displayName.text.data
								);
								
								//// Add a reference to the method in the objecttype //
								//UA_AddReferencesItem newItem;
								//UA_AddReferencesItem_init(&newItem);
								//newItem.sourceNodeId = *destinationNodeId;
								//newItem.referenceTypeId = rd->referenceTypeId;
								//newItem.isForward = true;
								//newItem.targetNodeId = rd->nodeId;
								//newItem.targetNodeClass = UA_NODECLASS_METHOD;
								//Operation_addReference(server, session, NULL, &newItem, &retval);
								//return retval;
								
							}

							if (ref1->nodeId.nodeId.identifierType == UA_NODEIDTYPE_NUMERIC)
							{
								printf("%-9d %-16d %-16.*s %-16.*s\n"
									, ref1->nodeId.nodeId.namespaceIndex
									, ref1->nodeId.nodeId.identifier.numeric
									, (int)ref1->browseName.name.length
									, ref1->browseName.name.data
									, (int)ref1->displayName.text.length
									, ref1->displayName.text.data
								);
							}
							else if (ref1->nodeId.nodeId.identifierType == UA_NODEIDTYPE_STRING)
							{
								printf("%-9d %-16.*s %-16.*s %-16.*s\n"
									, ref1->nodeId.nodeId.namespaceIndex
									, (int)ref1->nodeId.nodeId.identifier.string.length
									, ref1->nodeId.nodeId.identifier.string.data
									, (int)ref1->browseName.name.length
									, ref1->browseName.name.data
									, (int)ref1->displayName.text.length
									, ref1->displayName.text.data
								);
							}
						}
					}
					*/


					/*
					const UA_Node *child = //(server, childNodeId)
						ref->nodeId.nodeId;
					if (!child)
						return false;

					// Look for the reference making the child mandatory
					for (size_t i = 0; i < child->referencesSize; ++i) {
						UA_NodeReferenceKind *refs = &child->references[i];
						if (!UA_NodeId_equal(&hasModellingRuleId, &refs->referenceTypeId))
							continue;
						if (refs->isInverse)
							continue;
						for (size_t j = 0; j < refs->targetIdsSize; ++j) {
							if (UA_NodeId_equal(&mandatoryId, &refs->targetIds[j].nodeId)) {
								UA_Nodestore_release(server, child);
								return true;
							}
						}
					}
					*/


					/*
					//bool b = UA_Node_hasSubTypeOrInstances();
					const UA_Node *child = //(server, childNodeId)
											ref->nodeId.nodeId;
					if (!child)
						return false;

					// Look for the reference making the child mandatory
					for (size_t i = 0; i < child->referencesSize; ++i) {
						UA_NodeReferenceKind *refs = &child->references[i];
						if (!UA_NodeId_equal(&hasModellingRuleId, &refs->referenceTypeId))
							continue;
						if (refs->isInverse)
							continue;
						for (size_t j = 0; j < refs->targetIdsSize; ++j) {
							if (UA_NodeId_equal(&mandatoryId, &refs->targetIds[j].nodeId)) {
								UA_Nodestore_release(server, child);
								return true;
							}
						}
					}
					*/

				}
			}
			else if(ref->nodeId.nodeId.identifierType == UA_NODEIDTYPE_STRING)
			{
                printf("%-9d %-16.*s %-16.*s %-16.*s\n"
					, ref->nodeId.nodeId.namespaceIndex
                    , (int)ref->nodeId.nodeId.identifier.string.length
                    , ref->nodeId.nodeId.identifier.string.data
                    , (int)ref->browseName.name.length
					, ref->browseName.name.data
                    , (int)ref->displayName.text.length
					, ref->displayName.text.data
				);
            }
            // TODO: distinguish further types
        }
    }

    //UA_BrowseRequest_clear(&bReq);
    //UA_BrowseResponse_clear(&bResp);

    // Same thing, this time using the node iterator... 
    UA_NodeId *parent = UA_NodeId_new();
    *parent = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    
	UA_Client_forEachChildNodeCall
	(
		client
		, UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER)
		, nodeIter
		, (void *) parent
	);

    UA_NodeId_delete(parent);
}






void readValueOfLogicLevelPt1Position()
{
	UA_Client *client = UA_Client_new(UA_ClientConfig_default);
	UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://192.168.1.119:4840");
	if (retval != UA_STATUSCODE_GOOD)
	{
		UA_Client_delete(client);
		return /*UA_Variant()*/; //(int)retval;
	}

	// Listing endpoints //
	UA_EndpointDescription* endpointArray = NULL;
	size_t endpointArraySize = 0;
	retval = UA_Client_getEndpoints(client, "opc.tcp://192.168.1.119:4840",
		&endpointArraySize, &endpointArray);
	if (retval != UA_STATUSCODE_GOOD)
	{
		UA_Array_delete(endpointArray, endpointArraySize, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION]);
		UA_Client_delete(client);
		return /*EXIT_FAILURE*/;
	}
	printf("%i endpoints found\n", (int)endpointArraySize);
	for (size_t i = 0; i < endpointArraySize; i++) {
		printf("URL of endpoint %i is %.*s\n", (int)i,
			(int)endpointArray[i].endpointUrl.length,
			endpointArray[i].endpointUrl.data);
	}
	UA_Array_delete(endpointArray, endpointArraySize, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION]);

	// Connect to a server
	// anonymous connect would be: retval = UA_Client_connect(client, "opc.tcp://localhost:4840"); */
	retval = UA_Client_connect_username(client, "opc.tcp://localhost:4840", "user1", "password");
	if (retval != UA_STATUSCODE_GOOD) {
		UA_Client_delete(client);
		return /*EXIT_FAILURE*/;
	}

	// Browse some objects
	printf("Browsing nodes in objects folder:\n");
	UA_BrowseRequest bReq;
	UA_BrowseRequest_init(&bReq);
	bReq.requestedMaxReferencesPerNode = 0;
	bReq.nodesToBrowse = UA_BrowseDescription_new();
	bReq.nodesToBrowseSize = 1;
	bReq.nodesToBrowse[0].nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER); // browse objects folder
	bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL; // return everything

	UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);
	printf("%-9s %-16s %-16s %-16s\n", "NAMESPACE", "NODEID", "BROWSE NAME", "DISPLAY NAME");

	for (size_t i = 0; i < bResp.resultsSize; ++i)
	{
		for (size_t j = 0; j < bResp.results[i].referencesSize; ++j)
		{
			UA_ReferenceDescription *ref = &(bResp.results[i].references[j]);

			if (ref->nodeId.nodeId.identifierType == UA_NODEIDTYPE_NUMERIC)
			{
				printf("%-9d %-16d %-16.*s %-16.*s\n"
					, ref->nodeId.nodeId.namespaceIndex
					, ref->nodeId.nodeId.identifier.numeric
					, (int)ref->browseName.name.length
					, ref->browseName.name.data
					, (int)ref->displayName.text.length
					, ref->displayName.text.data
				);
			}
			else if (ref->nodeId.nodeId.identifierType == UA_NODEIDTYPE_STRING)
			{
				printf("%-9d %-16.*s %-16.*s %-16.*s\n"
					, ref->nodeId.nodeId.namespaceIndex
					, (int)ref->nodeId.nodeId.identifier.string.length
					, ref->nodeId.nodeId.identifier.string.data
					, (int)ref->browseName.name.length
					, ref->browseName.name.data
					, (int)ref->displayName.text.length
					, ref->displayName.text.data
				);

				const std::string hmiLogicLevelFolder = "Logical Level";
				if ((hmiLogicLevelFolder.length() == (int)ref->browseName.name.length)
					&& (hmiLogicLevelFolder == uaToString(ref->browseName.name))
					)
				{
					UA_Variant value;
					const UA_NodeId nodeId = ref->nodeId.nodeId; //UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME);
					retval = UA_Client_readValueAttribute(client, nodeId, &value);

					if (retval == UA_STATUSCODE_GOOD
						//&&  UA_Variant_hasScalarType(&value, &UA_TYPES[UA_TYPES_DATETIME])
						)
					{
						UA_Int16 int16 = *(UA_Int16 *)value.data;
						UA_Int32 int32 = *(UA_Int32 *)value.data;
						UA_Int64 int64 = *(UA_Int64 *)value.data;

						//UA_DateTime raw_date = *(UA_DateTime *)value.data;
						//UA_DateTimeStruct dts = UA_DateTime_toStruct(raw_date);
						//UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "date is: %u-%u-%u %u:%u:%u.%03u\n",
						//	dts.day, dts.month, dts.year, dts.hour, dts.min, dts.sec, dts.milliSec);
					}

				}

			}
			// TODO: distinguish further types
		}
	}


}



/*
//int
//main(int argc, char *argv[])
int performAsyncCall()
{
	UA_Client *client = UA_Client_new();
	UA_ClientConfig_setDefault(UA_Client_getConfig(client));
	UA_UInt32 reqId = 0;
	UA_String userdata = UA_STRING("userdata");

	UA_BrowseRequest bReq;
	UA_BrowseRequest_init(&bReq);
	bReq.requestedMaxReferencesPerNode = 0;
	bReq.nodesToBrowse = UA_BrowseDescription_new();
	bReq.nodesToBrowseSize = 1;
	bReq.nodesToBrowse[0].nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
	bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL; // return everything 

	UA_Client_connect_async(client, "opc.tcp://localhost:4840", onConnect, NULL);

	//Windows needs time to response
	UA_sleep_ms(100);

	// What happens if client tries to send request before connected? 
	UA_Client_sendAsyncBrowseRequest(client, &bReq, fileBrowsed, &userdata, &reqId);

	UA_DateTime startTime = UA_DateTime_nowMonotonic();
	do {
		//TODO: fix memory-related bugs if condition not checked//
		if (UA_Client_getState(client) == UA_CLIENTSTATE_SESSION) {
			// If not connected requests are not sent //
			UA_Client_sendAsyncBrowseRequest(client, &bReq, fileBrowsed, &userdata, &reqId);
		}
		// Requests are processed //
		UA_BrowseRequest_clear(&bReq);
		UA_Client_run_iterate(client, 0);
		UA_sleep_ms(100);

		// Break loop if server cannot be connected within 2s -- prevents build timeout //
		if (UA_DateTime_nowMonotonic() - startTime > 2000 * UA_DATETIME_MSEC)
			break;
	} while (reqId < 10);

	// Demo: high-level functions 
	UA_Int32 value = 0;
	UA_Variant myVariant;
	UA_Variant_init(&myVariant);

	UA_Variant input;
	UA_Variant_init(&input);

	for (UA_UInt16 i = 0; i < 5; i++) {
		if (UA_Client_getState(client) == UA_CLIENTSTATE_SESSION) {
			// writing and reading value 1 to 5 
			UA_Variant_setScalarCopy(&myVariant, &value, &UA_TYPES[UA_TYPES_INT32]);
			value++;
			UA_Client_writeValueAttribute_async(client,
				UA_NODEID_STRING(1, "the.answer"),
				&myVariant, attrWritten, NULL,
				&reqId);
			UA_Variant_clear(&myVariant);

			UA_Client_readValueAttribute_async(client,
				UA_NODEID_STRING(1, "the.answer"),
				readValueAttributeCallback, NULL,
				&reqId);

			//TODO: check the existance of the nodes inside these functions (otherwise seg faults)
#ifdef NODES_EXIST
#ifdef UA_ENABLE_METHODCALLS
			UA_String stringValue = UA_String_fromChars("World");
			UA_Variant_setScalar(&input, &stringValue, &UA_TYPES[UA_TYPES_STRING]);

			UA_Client_call_async(client,
				UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
				UA_NODEID_NUMERIC(1, 62541), 1, &input,
				methodCalled, NULL, &reqId);
			UA_String_clear(&stringValue);

#define pathSize 3
			char *paths[pathSize] = { "Server", "ServerStatus", "State" };
			UA_UInt32 ids[pathSize] = { UA_NS0ID_ORGANIZES,
			UA_NS0ID_HASCOMPONENT, UA_NS0ID_HASCOMPONENT };

			UA_Cient_translateBrowsePathsToNodeIds_async(client, paths, ids, pathSize,
				translateCalled, NULL, &reqId);
#endif // UA_ENABLE_METHODCALLS //
#endif
			//How often UA_Client_run_iterate is called depends on the number of request sent //
			UA_Client_run_iterate(client, 0);
			UA_Client_run_iterate(client, 0);
		}
	}
	UA_Client_run_iterate(client, 0);

	// Async disconnect kills unprocessed requests//
	// UA_Client_disconnect_async (client, &reqId); //can only be used when connected = true
	// UA_Client_run_iterate (client, &timedOut);
	UA_Client_disconnect(client);
	UA_Client_delete(client);

	return EXIT_SUCCESS;
}
*/







///*
//*MonitoredItem Service Set
//* ------------------------ -
//*Clients define MonitoredItems to subscribe to data and Events.Each
//* MonitoredItem identifies the item to be monitored and the Subscription to use
//* to send Notifications.The item to be monitored may be any Node Attribute.
//*/
/*
* Clients define MonitoredItems to subscribe to data and Events.Each
* MonitoredItem identifies the item to be monitored and the Subscription to use
* to send Notifications.The item to be monitored may be any Node Attribute.
*
* CreateMonitoredItems Service
* ^^^^^^^^^^^^^^^^^^^^^^^^^^^^
* Used to create and add one or more MonitoredItems to a Subscription.A
* MonitoredItem is deleted automatically by the Server when the Subscription is
* deleted.Deleting a MonitoredItem causes its entire set of triggered item
* links to be deleted, but has no effect on the MonitoredItems referenced by
* the triggered items. * /
void Service_CreateMonitoredItems(UA_Server *server, UA_Session *session,
	const UA_CreateMonitoredItemsRequest *request,
	UA_CreateMonitoredItemsResponse *response);
*/

//
// https://github.com/open62541/open62541/blob/master/tests/server/check_subscription_events.c
//

//UA_fakeSleep(UA_UInt32 duration) { //  C:\_RZD_WORK\TASKS\2019-Iter-15\OPC-Client\open62541-master\open62541-master\tests\testing-plugins\testing_clock.c


UA_Boolean notificationReceived = false;
UA_UInt32 countNotificationReceived = 0;
UA_Double publishingInterval = 500.0;


static void dataChangeHandler
							(
							UA_Client *client
							, UA_UInt32 subId
							, void *subContext
							, UA_UInt32 monId
							, void *monContext
							, UA_DataValue *value
							)
{
	notificationReceived = true;
	countNotificationReceived++;
}



void subscribeToMonitoringItem(UA_Client *client, UA_NodeId nodeIdToMonitore)
{
	UA_StatusCode retval;

	// = UA_Client_new();
	//UA_ClientConfig_setDefault(UA_Client_getConfig(client));
	//retval = UA_Client_connect(client, "opc.tcp://localhost:4840");
	//ck_assert_uint_eq(retval, UA_STATUSCODE_GOOD);
	
	//client->

	//UA_Client_recv = client->connection.recv;
	//client->connection.recv = UA_Client_recvTesting;
//*
	UA_CreateSubscriptionRequest request = UA_CreateSubscriptionRequest_default();
	UA_CreateSubscriptionResponse response = UA_Client_Subscriptions_create(client, request,
		NULL, NULL, NULL);
	retval = response.responseHeader.serviceResult;
	//ck_assert_uint_eq(response.responseHeader.serviceResult, UA_STATUSCODE_GOOD);
	UA_UInt32 subId = response.subscriptionId;
	
	// a valid UA_Client_Subscriptions_modify
	UA_ModifySubscriptionRequest modifySubscriptionRequest;
	UA_ModifySubscriptionRequest_init(&modifySubscriptionRequest);
	modifySubscriptionRequest.subscriptionId = response.subscriptionId;
	modifySubscriptionRequest.requestedPublishingInterval = response.revisedPublishingInterval;
	modifySubscriptionRequest.requestedLifetimeCount = response.revisedLifetimeCount;
	modifySubscriptionRequest.requestedMaxKeepAliveCount = response.revisedMaxKeepAliveCount;
	UA_ModifySubscriptionResponse modifySubscriptionResponse = UA_Client_Subscriptions_modify(client, modifySubscriptionRequest);
	retval = modifySubscriptionResponse.responseHeader.serviceResult;
	//ck_assert_int_eq(modifySubscriptionResponse.responseHeader.serviceResult, UA_STATUSCODE_GOOD);
	
	// an invalid UA_Client_Subscriptions_modify
	modifySubscriptionRequest.subscriptionId = 99999; // invalid
	modifySubscriptionResponse = UA_Client_Subscriptions_modify(client, modifySubscriptionRequest);
	retval = modifySubscriptionResponse.responseHeader.serviceResult;
	if (retval == UA_STATUSCODE_BADSUBSCRIPTIONIDINVALID)
	{
		int z = 0;
	}

	//ck_assert_int_eq(modifySubscriptionResponse.responseHeader.serviceResult, UA_STATUSCODE_BADSUBSCRIPTIONIDINVALID);
	
	//
	// EXAMPLE OF HOW TO MONITORE SERVER STATUS STATE CHANGES
	//
	 UA_MonitoredItemCreateRequest monRequest =
		 //    monitor the server state 
		 //UA_MonitoredItemCreateRequest_default( UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_STATE) );
		 //    monitor the server state 
		 UA_MonitoredItemCreateRequest_default( nodeIdToMonitore );

	UA_MonitoredItemCreateResult monResponse =
	UA_Client_MonitoredItems_createDataChange(client, response.subscriptionId,
		UA_TIMESTAMPSTORETURN_BOTH,
		monRequest, NULL, dataChangeHandler, NULL);
	
	monResponse.statusCode;
	//ck_assert_uint_eq(monResponse.statusCode, UA_STATUSCODE_GOOD);
	UA_UInt32 monId = monResponse.monitoredItemId;
	
	UA_fakeSleep((UA_UInt32)publishingInterval + 1);

	notificationReceived = false;
	
	////retval = UA_Client_run_iterate(client, (UA_UInt16)(publishingInterval + 1));


	//ck_assert_uint_eq(retval, UA_STATUSCODE_GOOD);
	//ck_assert_uint_eq(notificationReceived, true);
/*
	retval = UA_Client_MonitoredItems_deleteSingle(client, subId, monId);
	//ck_assert_uint_eq(retval, UA_STATUSCODE_GOOD);
	
	retval = UA_Client_Subscriptions_deleteSingle(client, subId);
	//ck_assert_uint_eq(retval, UA_STATUSCODE_GOOD);
	
	UA_Client_disconnect(client);
	UA_Client_delete(client);
*/


//*/

}


void subscribePreparationsForMonitoringItem()
{
	UA_Client *client = UA_Client_new(UA_ClientConfig_default);
	UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://192.168.1.119:4840");
	if (retval != UA_STATUSCODE_GOOD)
	{
		UA_Client_delete(client);
		return /*UA_Variant()*/; //(int)retval;
	}

	// Listing endpoints //
	UA_EndpointDescription* endpointArray = NULL;
	size_t endpointArraySize = 0;
	retval = UA_Client_getEndpoints(client, "opc.tcp://192.168.1.119:4840",
		&endpointArraySize, &endpointArray);
	if (retval != UA_STATUSCODE_GOOD)
	{
		UA_Array_delete(endpointArray, endpointArraySize, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION]);
		UA_Client_delete(client);
		return /*EXIT_FAILURE*/;
	}
	printf("%i endpoints found\n", (int)endpointArraySize);
	for (size_t i = 0; i < endpointArraySize; i++) {
		printf("URL of endpoint %i is %.*s\n", (int)i,
			(int)endpointArray[i].endpointUrl.length,
			endpointArray[i].endpointUrl.data);
	}
	UA_Array_delete(endpointArray, endpointArraySize, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION]);

	// Connect to a server
	// anonymous connect would be: retval = UA_Client_connect(client, "opc.tcp://localhost:4840"); */
	retval = UA_Client_connect_username(client, "opc.tcp://localhost:4840", "user1", "password");
	if (retval != UA_STATUSCODE_GOOD) {
		UA_Client_delete(client);
		return /*EXIT_FAILURE*/;
	}


	// Browse some objects
	printf("Browsing nodes in objects folder:\n");
	UA_BrowseRequest bReq;
	UA_BrowseRequest_init(&bReq);
	bReq.requestedMaxReferencesPerNode = 0;
	bReq.nodesToBrowse = UA_BrowseDescription_new();
	bReq.nodesToBrowseSize = 1;
	bReq.nodesToBrowse[0].nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER); // browse objects folder
	bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL; // return everything

	UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);
	printf("%-9s %-16s %-16s %-16s\n", "NAMESPACE", "NODEID", "BROWSE NAME", "DISPLAY NAME");

	for (size_t i = 0; i < bResp.resultsSize; ++i)
	{
		for (size_t j = 0; j < bResp.results[i].referencesSize; ++j)
		{
			UA_ReferenceDescription *ref = &(bResp.results[i].references[j]);
			
			const std::string objFolderName = "Logical Level";
			if ((objFolderName.length() == (int)ref->browseName.name.length)
				&& (objFolderName == uaToString(ref->browseName.name))
				)
			{
				UA_NodeId nodePt1      = findChildWithBrowseName("PT1", client,		 ref->nodeId.nodeId, UA_NODEIDTYPE_NUMERIC, UA_BROWSERESULTMASK_ALL);
				UA_NodeId nodePt1Block = findChildWithBrowseName("Block", client,	 nodePt1, UA_NODEIDTYPE_NUMERIC, UA_BROWSERESULTMASK_ALL);
				UA_NodeId nodePt1Pos   = findChildWithBrowseName("Position", client, nodePt1, UA_NODEIDTYPE_NUMERIC, UA_BROWSERESULTMASK_ALL);
				//browseChildren(client, ref->nodeId.nodeId, UA_NODEIDTYPE_NUMERIC, UA_BROWSERESULTMASK_ALL);

				UA_NodeId nodeIdToMonitore = nodePt1Block;
				subscribeToMonitoringItem(client, nodeIdToMonitore);
			}

		}

/*		UA_StatusCode  UA_Client_Subscriptions_addMonitoredEvents(UA_Client *client, const UA_UInt32 subscriptionId,
				UA_MonitoredItemCreateRequest *items, size_t itemsSize,
				UA_MonitoredEventHandlingFunction *hfs,
				void **hfContexts, UA_StatusCode *itemResults,
				UA_UInt32 *newMonitoredItemIds)
		{
			return addMonitoredEvents(client, subscriptionId, items, itemsSize, hfs,
				hfContexts, itemResults, newMonitoredItemIds);
		}

		UA_StatusCode  UA_Client_Subscriptions_addMonitoredEvent(
				UA_Client *client, UA_UInt32 subscriptionId,
				const UA_NodeId nodeId, UA_UInt32 attributeID,
				const UA_SimpleAttributeOperand *selectClauses,
				size_t selectClausesSize,
				const UA_ContentFilterElement *whereClauses,
				size_t whereClausesSize,
				const UA_MonitoredEventHandlingFunction hf,
				void *hfContext, UA_UInt32 *newMonitoredItemId)
		{

		}
*/


	}

}




//
// https://github.com/open62541/open62541/blob/master/examples/client_subscription_loop.c
//

/* ///////////////////// SILUTION FOR SERVER TIME CHANGE //////////////////////////////

UA_Boolean running = true;

static void stopHandler(int sign)
{
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Received Ctrl-C");
	running = 0;
}

static void handler_currentTimeChanged(UA_Client *client, UA_UInt32 subId, void *subContext, UA_UInt32 monId, void *monContext, UA_DataValue *value)
{
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "currentTime has changed!");
	if (UA_Variant_hasScalarType(&value->value, &UA_TYPES[UA_TYPES_DATETIME]))
	{
		UA_DateTime raw_date = *(UA_DateTime *)value->value.data;
		UA_DateTimeStruct dts = UA_DateTime_toStruct(raw_date);
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
			"date is: %02u-%02u-%04u %02u:%02u:%02u.%03u",
			dts.day, dts.month, dts.year, dts.hour, dts.min, dts.sec, dts.milliSec);
	}
}

static void deleteSubscriptionCallback(UA_Client *client, UA_UInt32 subscriptionId, void *subscriptionContext)
{
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
		"Subscription Id %u was deleted", subscriptionId);
}

static void subscriptionInactivityCallback(UA_Client *client, UA_UInt32 subId, void *subContext)
{
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Inactivity for subscription %u", subId);
}



static void stateCallback(UA_Client *client, UA_ClientState clientState)
{
	switch (clientState)
	{
	case UA_CLIENTSTATE_DISCONNECTED:
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "The client is disconnected");
		break;
//	case UA_CLIENTSTATE_WAITING_FOR_ACK:
//		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Waiting for ack");
//		break;
	case UA_CLIENTSTATE_CONNECTED:
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
			"A TCP connection to the server is open");
		break;
	case UA_CLIENTSTATE_SECURECHANNEL:
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
			"A SecureChannel to the server is open");
		break;
	case UA_CLIENTSTATE_SESSION:
	{
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "A session with the server is open");
		// A new session was created. We need to create the subscription.
		// Create a subscription
		UA_CreateSubscriptionRequest request = UA_CreateSubscriptionRequest_default();
		UA_CreateSubscriptionResponse response = UA_Client_Subscriptions_create(client, request,
			NULL, NULL, deleteSubscriptionCallback);

		if (response.responseHeader.serviceResult == UA_STATUSCODE_GOOD)
			UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
				"Create subscription succeeded, id %u", response.subscriptionId);
		else
			return;

		// Add a MonitoredItem 
		UA_MonitoredItemCreateRequest monRequest =
			UA_MonitoredItemCreateRequest_default(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME));

		UA_MonitoredItemCreateResult monResponse =
			UA_Client_MonitoredItems_createDataChange(client, response.subscriptionId,
				UA_TIMESTAMPSTORETURN_BOTH,
				monRequest, NULL, handler_currentTimeChanged, NULL);

		if (monResponse.statusCode == UA_STATUSCODE_GOOD)
			UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
				"Monitoring UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME', id %u",
				monResponse.monitoredItemId);
	}
								 break;
	case UA_CLIENTSTATE_SESSION_RENEWED:
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
			"A session with the server is open (renewed)");
		// The session was renewed. We don't need to recreate the subscription. 
		break;
	//case UA_CLIENTSTATE_SESSION_DISCONNECTED:
	//	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Session disconnected");
	//	break;
	}

	return;
}

*/ ///////////////////// SILUTION FOR SERVER TIME CHANGE //////////////////////////////


UA_Boolean running = true;
UA_Logger logger = UA_Log_Stdout;

static void stopHandler(int sign)
{
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Received Ctrl-C");
	printf("Received Ctrl-C \n");
	running = 0;
}

static void handler_currentTimeChanged(UA_Client *client, UA_UInt32 subId, void *subContext, UA_UInt32 monId, void *monContext, UA_DataValue *value)
{
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Current Tag Value has changed! \n");
	printf("Current Tag Value has changed! \n");
	if (UA_Variant_hasScalarType(&value->value, &UA_TYPES[UA_TYPES_UINT32]))
	{
		
		UA_UInt32 raw_uint_val = *(UA_UInt32 *)value->value.data;
		printf("Value changed to %u \n", raw_uint_val);

		//UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
		//	"date is: %02u-%02u-%04u %02u:%02u:%02u.%03u",
		//	dts.day, dts.month, dts.year, dts.hour, dts.min, dts.sec, dts.milliSec);
	}
}

static void deleteSubscriptionCallback(UA_Client *client, UA_UInt32 subscriptionId, void *subscriptionContext)
{
	printf( "Subscription Id %u was deleted \n", subscriptionId );
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Subscription Id %u was deleted", subscriptionId);
}

static void subscriptionInactivityCallback(UA_Client *client, UA_UInt32 subId, void *subContext)
{
	printf( "Inactivity for subscription %u \n", subId );
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Inactivity for subscription %u", subId);
}



UA_NodeId getNodeIdPt1Block(UA_Client *client)
{
	// Browse some objects
	printf("Browsing nodes in objects folder: \n");
	UA_BrowseRequest bReq;
	UA_BrowseRequest_init(&bReq);
	bReq.requestedMaxReferencesPerNode = 0;
	bReq.nodesToBrowse = UA_BrowseDescription_new();
	bReq.nodesToBrowseSize = 1;
	bReq.nodesToBrowse[0].nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER); // browse objects folder
	bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL; // return everything

	UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);
	printf("Object Filders Browsed \n");

	for (size_t i = 0; i < bResp.resultsSize; ++i)
	{
		for (size_t j = 0; j < bResp.results[i].referencesSize; ++j)
		{
			UA_ReferenceDescription *ref = &(bResp.results[i].references[j]);

			const std::string objFolderName = "Logical Level";
			if ((objFolderName.length() == (int)ref->browseName.name.length)
				&& (objFolderName == uaToString(ref->browseName.name))
				)
			{
				UA_NodeId nodePt1 = findChildWithBrowseName("PT1", client, ref->nodeId.nodeId, UA_NODEIDTYPE_NUMERIC, UA_BROWSERESULTMASK_ALL);
				UA_NodeId nodePt1Block = findChildWithBrowseName("Block", client, nodePt1, UA_NODEIDTYPE_NUMERIC, UA_BROWSERESULTMASK_ALL);
				return nodePt1Block;
			}
		}
	}
}

static void stateCallback(UA_Client *client, UA_ClientState clientState)
{
	int a = 0;

	switch (clientState)
	{
	case UA_CLIENTSTATE_DISCONNECTED: printf("The client is disconnected\n");
		//UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "The client is disconnected");
		break;
		//	case UA_CLIENTSTATE_WAITING_FOR_ACK:
		//		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Waiting for ack");
		//		break;
	case UA_CLIENTSTATE_CONNECTED: printf("A TCP connection to the server is open\n");
		//UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "A TCP connection to the server is open");
		break;
	case UA_CLIENTSTATE_SECURECHANNEL: printf("A SecureChannel to the server is open\n");
		//UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "A SecureChannel to the server is open");
		break;
	case UA_CLIENTSTATE_SESSION:
	{
		printf("A session with the server is open");
		//UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "A session with the server is open");
		// A new session was created. We need to create the subscription.
		// Create a subscription
		UA_CreateSubscriptionRequest request = UA_CreateSubscriptionRequest_default();
		UA_CreateSubscriptionResponse response = UA_Client_Subscriptions_create(client, request,
			NULL, NULL, deleteSubscriptionCallback);

		if (response.responseHeader.serviceResult == UA_STATUSCODE_GOOD) 		printf("Create subscription succeeded, id %u \n", response.subscriptionId);
		//UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Create subscription succeeded, id %u", response.subscriptionId);
		else
			return;

		//// Add a MonitoredItem 
		UA_MonitoredItemCreateRequest monRequest =
			//	UA_MonitoredItemCreateRequest_default(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME));
			UA_MonitoredItemCreateRequest_default( getNodeIdPt1Block(client) );


		UA_MonitoredItemCreateResult monResponse =
			UA_Client_MonitoredItems_createDataChange(client, response.subscriptionId,
				UA_TIMESTAMPSTORETURN_BOTH,
				monRequest, NULL, handler_currentTimeChanged, NULL);

		if (monResponse.statusCode == UA_STATUSCODE_GOOD) printf("Monitoring PT1.Blockk Value', id %u \n", monResponse.monitoredItemId);
		//UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Monitoring UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME', id %u", monResponse.monitoredItemId);
	}
	break;
	case UA_CLIENTSTATE_SESSION_RENEWED: printf("A session with the server is open (renewed) \n");
		//UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "A session with the server is open (renewed)");
		// The session was renewed. We don't need to recreate the subscription. 
		break;
		//case UA_CLIENTSTATE_SESSION_DISCONNECTED:
		//	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Session disconnected");
		//	break;
	}

	return;
}




#if 0
int performSubscribition(void) 
{
	const std::string strConnection("opc.tcp://192.168.1.119:4840");

	signal(SIGINT, stopHandler); /* catches ctrl-c */
//*
	UA_ClientConfig *cc = UA_Client_getConfig(client);
	cc->stateCallback = stateCallback;
	cc->subscriptionInactivityCallback = subscriptionInactivityCallback;

	UA_Client *client = UA_Client_new();
	UA_ClientConfig_setDefault(cc);
//*/


	UA_Client *client = UA_Client_new(UA_ClientConfig_default);
/*	//UA_ClientConfig *cc = client->;
	UA_StatusCode retval = UA_Client_connect(client, strConnection);
	if (retval != UA_STATUSCODE_GOOD)
	{
		UA_Client_delete(client);
		return ; //(int)retval;
	}
*/

	// Set stateCallback //

	//* Endless loop runAsync //
	while (running)
	{
		// if already connected, this will return GOOD and do nothing 
		// if the connection is closed/errored, the connection will be reset and then reconnected 
		// Alternatively you can also use UA_Client_getState to get the current state 
		
		UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://192.168.1.119:4840");
		
		if (retval != UA_STATUSCODE_GOOD)
		{
			UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
				"Not connected. Retrying to connect in 1 second");
			// The connect may timeout after 1 second (see above) or it may fail immediately on network errors 
			// E.g. name resolution errors or unreachable network. Thus there should be a small sleep here 
			Sleep(1000);
			//UA_sleep_ms(1000);
			continue;
		}

		//UA_Client_run_iterate(client, 1000);
	};
	//*/

	// Clean up //
	UA_Client_delete(client); /* Disconnects the client internally */

	return EXIT_SUCCESS;
}
#endif




void performSubscribition()
{


// SOLUTION WITH UA_Client_run_iterate
/*
	const std::string strConnection("opc.tcp://192.168.1.119:4840");
	UA_StatusCode retStatusCode;
	signal(SIGINT, stopHandler); // catches ctrl-c //

	//UA_Client *client = UA_Client_new();
	UA_Client *client = UA_Client_new(UA_ClientConfig_default);
	UA_ClientConfig *cc = UA_Client_getConfig(client);
	//UA_ClientConfig_setDefault(cc);

	// Set stateCallback //
	cc->stateCallback = stateCallback;
	cc->subscriptionInactivityCallback = subscriptionInactivityCallback;

	// Endless loop runAsync //
	while (running)
	{
		// if already connected, this will return GOOD and do nothing //
		// if the connection is closed/errored, the connection will be reset and then reconnected //
		// Alternatively you can also use UA_Client_getState to get the current state //
		UA_StatusCode retval = UA_Client_connect( client, strConnection.c_str() );

		if (retval != UA_STATUSCODE_GOOD)
		{
			UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
				"Not connected. Retrying to connect in 1 second");
			// The connect may timeout after 1 second (see above) or it may fail immediately on network errors //
			// E.g. name resolution errors or unreachable network. Thus there should be a small sleep here //
			//UA_sleep_ms(1000);
			//Sleep(1000);
			std::this_thread::sleep_for(std::chrono::seconds(1));

			continue;
		}

		retStatusCode = UA_Client_run_iterate(client, 1000);
	};

	// Clean up //
	UA_Client_delete(client); // Disconnects the client internally //
	//return EXIT_SUCCESS;

//*/

}






/*
UA_StatusCode
addMonitoredItems(UA_Client *client, const UA_UInt32 subscriptionId,
	UA_MonitoredItemCreateRequest *items, size_t itemsSize,
	UA_MonitoredItemHandlingFunction *hfs, void **hfContexts,
	UA_StatusCode *itemResults, UA_UInt32 *newMonitoredItemIds) {
	// Create array of wrappers and callbacks //
	dataChangeCallbackWrapper **wrappers = (dataChangeCallbackWrapper**)
		UA_alloca(sizeof(dataChangeCallbackWrapper*) * itemsSize);
	UA_Client_DeleteMonitoredItemCallback *deleteCbs = (UA_Client_DeleteMonitoredItemCallback*)
		UA_alloca(sizeof(UA_Client_DeleteMonitoredItemCallback) * itemsSize);
	UA_Client_DataChangeNotificationCallback *wrapperCbs = (UA_Client_DataChangeNotificationCallback*)
		UA_alloca(sizeof(UA_MonitoredItemHandlingFunction) * itemsSize);

	for (size_t i = 0; i < itemsSize; i++) {
		wrappers[i] = (dataChangeCallbackWrapper*)UA_malloc(sizeof(dataChangeCallbackWrapper));
		if (!wrappers[i]) {
			for (size_t j = 0; j < i; j++)
				UA_free(wrappers[j]);
			return UA_STATUSCODE_BADOUTOFMEMORY;
		}
		wrappers[i]->origCallback = (UA_MonitoredItemHandlingFunction)(uintptr_t)hfs[i];
		wrappers[i]->context = hfContexts[i];

		deleteCbs[i] = deleteMonitoredItemCallback;
		wrapperCbs[i] = dataChangeCallback;
	}

	// Prepare the request //
	UA_CreateMonitoredItemsRequest request;
	UA_CreateMonitoredItemsRequest_init(&request);
	request.subscriptionId = subscriptionId;
	request.itemsToCreateSize = itemsSize;
	request.itemsToCreate = items;

	// Process and return //
	UA_CreateMonitoredItemsResponse response =
		UA_Client_MonitoredItems_createDataChanges(client, request, (void**)wrappers,
			wrapperCbs, deleteCbs);

	UA_StatusCode retval = response.responseHeader.serviceResult;
	if (retval == UA_STATUSCODE_GOOD && response.resultsSize != itemsSize)
		retval = UA_STATUSCODE_BADINTERNALERROR;

	if (retval == UA_STATUSCODE_GOOD) {
		for (size_t i = 0; i < itemsSize; i++) {
			itemResults[i] = response.results[i].statusCode;
			newMonitoredItemIds[i] = response.results[i].monitoredItemId;
		}
	}

	UA_CreateMonitoredItemsResponse_deleteMembers(&response);
	return retval;
}
*/



/*
UA_DEPRECATED UA_StatusCode UA_EXPORT
UA_Client_Subscriptions_addMonitoredItems(UA_Client *client, const UA_UInt32 subscriptionId,
	UA_MonitoredItemCreateRequest *items, size_t itemsSize,
	UA_MonitoredItemHandlingFunction *hfs,
	void **hfContexts, UA_StatusCode *itemResults,
	UA_UInt32 *newMonitoredItemIds);

UA_DEPRECATED UA_StatusCode UA_EXPORT
UA_Client_Subscriptions_addMonitoredItem(UA_Client *client, UA_UInt32 subscriptionId,
	UA_NodeId nodeId, UA_UInt32 attributeID,
	UA_MonitoredItemHandlingFunction hf,
	void *hfContext,
	UA_UInt32 *newMonitoredItemId,
	UA_Double samplingInterval);
*/


/////////// void wouldBeNiceToHaveInServer() { ; }
/*
void
Operation_Browse(UA_Server *server, UA_Session *session, UA_UInt32 *maxrefs,
	const UA_BrowseDescription *descr, UA_BrowseResult *result) {
	// Stack-allocate a temporary cp 
	ContinuationPointEntry *cp = (ContinuationPointEntry*)UA_alloca(sizeof(ContinuationPointEntry));
	memset(cp, 0, sizeof(ContinuationPointEntry));
	cp->maxReferences = *maxrefs;
	cp->browseDescription = *descr; // Shallow copy. Deep-copy later if we persist the cp.

	UA_Boolean done = browseWithContinuation(server, session, cp, result);

	// Exit early if done or an error occurred 
	if (done || result->statusCode != UA_STATUSCODE_GOOD)
		return;

	// Persist the new continuation point 
	ContinuationPointEntry *cp2 = NULL;
	UA_StatusCode retval = UA_STATUSCODE_GOOD;
	if (session->availableContinuationPoints <= 0 ||
		!(cp2 = (ContinuationPointEntry *)UA_malloc(sizeof(ContinuationPointEntry)))) {
		retval = UA_STATUSCODE_BADNOCONTINUATIONPOINTS;
		goto cleanup;
	}
	memset(cp2, 0, sizeof(ContinuationPointEntry));
	cp2->referenceKindIndex = cp->referenceKindIndex;
	cp2->targetIndex = cp->targetIndex;
	cp2->maxReferences = cp->maxReferences;
	retval = UA_BrowseDescription_copy(descr, &cp2->browseDescription);
	if (retval != UA_STATUSCODE_GOOD)
		goto cleanup;

	// Create a random bytestring via a Guid 
	UA_Guid *ident = UA_Guid_new();
	if (!ident) {
		retval = UA_STATUSCODE_BADOUTOFMEMORY;
		goto cleanup;
	}
	*ident = UA_Guid_random();
	cp2->identifier.data = (UA_Byte*)ident;
	cp2->identifier.length = sizeof(UA_Guid);

	// Return the cp identifier 
	retval = UA_ByteString_copy(&cp2->identifier, &result->continuationPoint);
	if (retval != UA_STATUSCODE_GOOD)
		goto cleanup;

	// Attach the cp to the session 
	LIST_INSERT_HEAD(&session->continuationPoints, cp2, pointers);
	--session->availableContinuationPoints;
	return;

cleanup:
	if (cp2) {
		UA_ByteString_deleteMembers(&cp2->identifier);
		UA_BrowseDescription_deleteMembers(&cp2->browseDescription);
		UA_free(cp2);
	}
	UA_BrowseResult_deleteMembers(result);
	result->statusCode = retval;
}
*/


/////////// void wouldBeNiceToHaveInServer() { ; }
 UA_StatusCode
findChildByBrowsename(UA_Server *server, /*UA_Session *session,*/
	const UA_NodeId *searchInstance,
	const UA_QualifiedName *browseName,
	UA_NodeId *outInstanceNodeId)
 {
	UA_BrowseDescription bd;
	UA_BrowseDescription_init(&bd);
	bd.nodeId = *searchInstance;
	bd.referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_AGGREGATES);
	bd.includeSubtypes = true;
	bd.browseDirection = UA_BROWSEDIRECTION_FORWARD;
	bd.nodeClassMask = UA_NODECLASS_OBJECT | UA_NODECLASS_VARIABLE | UA_NODECLASS_METHOD;
	bd.resultMask = UA_BROWSERESULTMASK_NODECLASS | UA_BROWSERESULTMASK_BROWSENAME;

	UA_BrowseResult br;
	UA_BrowseResult_init(&br);
	UA_UInt32 maxrefs = 0;
	//Operation_Browse(server, /*session,*/ &maxrefs, &bd, &br);
	if (br.statusCode != UA_STATUSCODE_GOOD)
		return br.statusCode;

	UA_StatusCode retval = UA_STATUSCODE_GOOD;
	for (size_t i = 0; i < br.referencesSize; ++i) {
		UA_ReferenceDescription *rd = &br.references[i];
		if (rd->browseName.namespaceIndex == browseName->namespaceIndex &&
			UA_String_equal(&rd->browseName.name, &browseName->name)) {
			retval = UA_NodeId_copy(&rd->nodeId.nodeId, outInstanceNodeId);
			break;
		}
	}

	//UA_BrowseResult_deleteMembers(&br);
	//return retval;
}



#if 0
/////////////////////////////////////////////////////////////////////////////////////
// VER // performSubscribitionAsync
/////////////////////////////////////////////////////////////////////////////////////

//*

// UA_Boolean running = true;
// UA_Logger logger = UA_Log_Stdout;

 static void stopHandler(int sign)
 {
	 UA_LOG_INFO(logger, UA_LOGCATEGORY_USERLAND, "Received Ctrl-C");
	 running = 0;
 }

 static void
	 handler_currentTimeChanged(UA_Client *client, UA_UInt32 subId, void *subContext,
		 UA_UInt32 monId, void *monContext, UA_DataValue *value)
 {
	 UA_LOG_INFO(logger, UA_LOGCATEGORY_USERLAND, "currentTime has changed!");
	 if (UA_Variant_hasScalarType(&value->value, &UA_TYPES[UA_TYPES_DATETIME])) {
		 UA_DateTime raw_date = *(UA_DateTime *)value->value.data;
		 UA_DateTimeStruct dts = UA_DateTime_toStruct(raw_date);
		 UA_LOG_INFO(logger, UA_LOGCATEGORY_USERLAND, "date is: %02u-%02u-%04u %02u:%02u:%02u.%03u",
			 dts.day, dts.month, dts.year, dts.hour, dts.min, dts.sec, dts.milliSec);
	 }
 }

 static void
	 deleteSubscriptionCallback(UA_Client *client, UA_UInt32 subscriptionId, void *subscriptionContext) {
	 UA_LOG_INFO(logger, UA_LOGCATEGORY_USERLAND, "Subscription Id %u was deleted", subscriptionId);
 }

 static void
	 subscriptionInactivityCallback(UA_Client *client, UA_UInt32 subId, void *subContext) {
	 UA_LOG_INFO(logger, UA_LOGCATEGORY_USERLAND, "Inactivity for subscription %u", subId);
 }

 static void
	 stateCallback(UA_Client *client, UA_ClientState clientState) {
	 switch (clientState) {
	 case UA_CLIENTSTATE_DISCONNECTED:
		 UA_LOG_INFO(logger, UA_LOGCATEGORY_USERLAND, "The client is disconnected");
		 break;
	 case UA_CLIENTSTATE_CONNECTED:
		 UA_LOG_INFO(logger, UA_LOGCATEGORY_USERLAND, "A TCP connection to the server is open");
		 break;
	 case UA_CLIENTSTATE_SECURECHANNEL:
		 UA_LOG_INFO(logger, UA_LOGCATEGORY_USERLAND, "A SecureChannel to the server is open");
		 break;
	 case UA_CLIENTSTATE_SESSION: {
		 UA_LOG_INFO(logger, UA_LOGCATEGORY_USERLAND, "A session with the server is open");
		 /* A new session was created. We need to create the subscription. */
		 /* Create a subscription */
		 UA_CreateSubscriptionRequest request = UA_CreateSubscriptionRequest_default();
		 UA_CreateSubscriptionResponse response = UA_Client_Subscriptions_create(client, request,
			 NULL, NULL, deleteSubscriptionCallback);

		 if (response.responseHeader.serviceResult == UA_STATUSCODE_GOOD)
			 UA_LOG_INFO(logger, UA_LOGCATEGORY_USERLAND, "Create subscription succeeded, id %u", response.subscriptionId);
		 else
			 return;

		 /* Add a MonitoredItem */
		 UA_MonitoredItemCreateRequest monRequest =
			 UA_MonitoredItemCreateRequest_default(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME));

		 UA_MonitoredItemCreateResult monResponse =
			 UA_Client_MonitoredItems_createDataChange(client, response.subscriptionId,
				 UA_TIMESTAMPSTORETURN_BOTH,
				 monRequest, NULL, handler_currentTimeChanged, NULL);
		 if (monResponse.statusCode == UA_STATUSCODE_GOOD)
			 UA_LOG_INFO(logger, UA_LOGCATEGORY_USERLAND, "Monitoring UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME', id %u", monResponse.monitoredItemId);
	 }
								  break;
	 case UA_CLIENTSTATE_SESSION_RENEWED:
		 UA_LOG_INFO(logger, UA_LOGCATEGORY_USERLAND, "A session with the server is open (renewed)");
		 /* The session was renewed. We don't need to recreate the subscription. */
		 break;
	 }
	 return;
 }


 /*
 int main(void) {
	 signal(SIGINT, stopHandler); // catches ctrl-c 

	 UA_ClientConfig config = UA_ClientConfig_default;
	 // Set stateCallback 
	 config.stateCallback = stateCallback;
	 config.subscriptionInactivityCallback = subscriptionInactivityCallback;

	 UA_Client *client = UA_Client_new(config);

	 // Endless loop runAsync 
	 while (running) {
		 // if already connected, this will return GOOD and do nothing 
		 // if the connection is closed/errored, the connection will be reset and then reconnected 
		 // Alternatively you can also use UA_Client_getState to get the current state 
		 UA_StatusCode retval = UA_Client_connect(client, "opc.tcp://localhost:4840");
		 if (retval != UA_STATUSCODE_GOOD) {
			 UA_LOG_ERROR(logger, UA_LOGCATEGORY_USERLAND, "Not connected. Retrying to connect in 1 second");
			 // The connect may timeout after 1 second (see above) or it may fail immediately on network errors 
			 // E.g. name resolution errors or unreachable network. Thus there should be a small sleep here 
			 UA_sleep_ms(1000);
			 continue;
		 }

		 UA_Client_runAsync(client, 1000);
	 };

	 // Clean up //
	 UA_Client_delete(client); // Disconnects the client internally //
	 return UA_STATUSCODE_GOOD;
 }
*/

#endif // performSubscribitionAsync


 
 void performSubscribitionAsync()
 {
	 const int one_second = 1000;
	 const std::string strConnection("opc.tcp://192.168.1.119:4840");
	 signal(SIGINT, stopHandler); // catches ctrl-c 

	 UA_ClientConfig config = UA_ClientConfig_default;
	 // Set stateCallback 
	 config.stateCallback = stateCallback;
	 config.subscriptionInactivityCallback = subscriptionInactivityCallback;

	 UA_Client *client = UA_Client_new(config);

	 // Endless loop runAsync 

	 while (running)
	 {
		 // if already connected, this will return GOOD and do nothing 
		 // if the connection is closed/errored, the connection will be reset and then reconnected 
		 // Alternatively you can also use UA_Client_getState to get the current state 
		 UA_StatusCode retval = UA_Client_connect(client, strConnection.c_str() /*"opc.tcp://localhost:4840"*/);
		 if (retval != UA_STATUSCODE_GOOD)
		 {
			 UA_LOG_ERROR(logger, UA_LOGCATEGORY_USERLAND, "Not connected. Retrying to connect in 1 second");
			 // The connect may timeout after 1 second (see above) or it may fail immediately on network errors 
			 // E.g. name resolution errors or unreachable network. Thus there should be a small sleep here 
			 //UA_sleep_ms(1000);
			 std::this_thread::sleep_for(std::chrono::milliseconds(one_second));
			 continue;
		 }

		 UA_Client_runAsync(client, one_second);
	 };

	 // Clean up 
	 UA_Client_delete(client); // Disconnects the client internally
	 //return UA_STATUSCODE_GOOD;
 }




int main(int argc, char *argv[])
{
	// NodeId of the variable holding the current time
	//const UA_NodeId nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME);
	//UA_StatusCode retval = UA_Client_readValueAttribute(client, nodeId, &value);

	//int r = testOpcUaClientCode();
	//int rr = testOpcUaClientConnection();
	
	// testOpcUaClientRead();

	// testOpcUaBrowseObjectFolder();

	
	//subscribePreparationsForMonitoringItem();

	//performSubscribition();

	performSubscribitionAsync();
}



