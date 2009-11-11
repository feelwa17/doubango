/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
*	
* This file is part of Open Source Doubango Framework.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*	
* DOUBANGO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

#ifndef _TINYXCAP_TXC_H_
#define _TINYXCAP_TXC_H_

#include "tinyxcap_config.h"

#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>
#include "sak.h"

#define TXC_AUIDS_COUNT				11
#define TXC_HTTP_DEFAULT_TIMEOUT	30000
#define TXC_HTTP_SUCCESS(request)	(200==request->status || 202==request->status)

#define TXC_IS_EMPTY_STRING(str) (!str || !strlen(str))

#define TXC_DOCUMENT_GLOBAL(auid_type) (auid_type == ietf_xcap_caps)

#define	TXC_MIME_TYPE_ELEMENT	"application/xcap-el+xml" /**< as per rfc 4825 subclause 15.2.1 */
#define	TXC_MIME_TYPE_ATTRIBUTE	"application/xcap-att+xml" /**< as per rfc 4825 subclause 15.2.2 */
#define	TXC_MIME_TYPE_NS		"application/xcap-ns+xml" /**< as per rfc 4825 subclause 15.2.3 */
#define	TXC_MIME_TYPE_ERROR		"application/xcap-error+xml" /**< as per rfc 4825 subclause 15.2.4 */
#define	TXC_MIME_TYPE_CAPS		"application/xcap-caps+xml" /**< as per rfc 4825 subclause 12.4 */
#define TXC_MIME_TYPE_RLIST		"application/resource-lists+xml" /**< as per rfc 4826 subclause 3.4.2 */
#define TXC_MIME_TYPE_RLS		"application/rls-services+xml" /**< as per rfc 4826 subclause 4.4.2 */

#define TXC_AUID_CAPS			"xcap-caps" /**< as per rfc 4825 subclause 12.1 */
#define TXC_AUID_RLIST			"resource-lists" /**< as per rfc 4826 subclause 3.4.1 */
#define TXC_AUID_RLS			"rls-services" /**< as per rfc 4826 subclause 4.4.1 */

#define TXC_DOCUMENT_CAPS		"index"		/**< as per rfc 4825 subclause 5.5 */
#define TXC_DOCUMENT_RLIST		"index"		/**< as per rfc 4826 subclause 3.4.7* */
#define TXC_DOCUMENT_RLS		"index"		/**< as per rfc 4826 subclause 4.4.7 */

#define TXC_NS_CAPS				"urn:ietf:params:xml:ns:xcap-caps" /**< as per rfc 4825 subclause 12.3*/
#define TXC_NS_RLIST			"urn:ietf:params:xml:ns:resource-lists" /**< as per rfc 4826 subclause 3.4.4 */
#define TXC_NS_RLS				"urn:ietf:params:xml:ns:rls-services" /**< as per rfc 4826 subclause 4.4.4 */

/* check context validity */
#define TXC_CONTEXT_CHECK(context, panic) \
	if(!context->txc_root) panic = xpa_invalid_root; \
	else if(!context->xui) panic = xpa_invalid_xui; \
	else panic = xpa_success;

/**@def TXC_CONTEXT_CREATE
* Helper function to easly create and initialize an XCAP context
*/
/**@def TXC_CONTEXT_SAFE_FREE
* Helper function to safely free an XCAP context
*/
#define TXC_CONTEXT_CREATE(_context)		TXC_XXX_CREATE2(_context, context)
#define TXC_CONTEXT_SAFE_FREE(_context)	TXC_XXX_SAFE_FREE2(_context, context)

/*@def TXC_REQUEST_CREATE
* Helper function to easly create and initialize an XCAP request
*/
/*@def TXC_REQUEST_SAFE_FREE
* Helper function to easly free an XCAP request
*/
/*@def TXC_REQUEST_RETURN_IF_PANIC
* Check if we should panic and leave the current section
*/
#define TXC_REQUEST_CREATE(_request)		TXC_XXX_CREATE2(_request, request)
#define TXC_REQUEST_SAFE_FREE(_request)	TXC_XXX_SAFE_FREE2(_request, request)
#define TXC_REQUEST_RETURN_IF_PANIC(request)\
	if(!TXC_PANIC_SUCCESS(request->panic)) return request;
#define TXC_REQUEST_RETURN_IF_NULL_URL(request)\
	if(!request->url) { request->panic =xpa_invalid_url;  return request; }

/*@def TXC_CONTENT_CREATE
* Helper function to easly create and initialize an XCAP content
*/
/*@def TXC_CONTENT_SAFE_FREE
* Helper function to easly free an XCAP content
*/
#define TXC_CONTENT_CREATE(_content)		TXC_XXX_CREATE2(_content, content)
#define TXC_CONTENT_SAFE_FREE(_content)	TXC_XXX_SAFE_FREE2(_content, content)

/* easyhandle */
#define TXC_EASYHANDLE_SAFE_FREE(easyhandle) \
	if(easyhandle) { curl_easy_cleanup(easyhandle); easyhandle = 0; }

/* auid */
#define TXC_AUID_CHECK(auid, panic)\
	if(!auid) panic = xpa_unsupported_auid;
#define TXC_AUID_CREATE(_auid)		TXC_XXX_CREATE2(_auid, auid)
#define TXC_AUID_SAFE_FREE(_auid)	TXC_XXX_SAFE_FREE2(_auid, auid)

#define TXC_PANIC_SUCCESS(panic) ( (panic == xpa_success) )

/* listing all possible xdm panic codes */
typedef enum txc_panic_e
{
	xpa_success = 0x00,
	xpa_unsupported_auid = -0xFF,
	xpa_invalid_root,
	xpa_invalid_xui,
	xpa_invalid_doc_name,
	xpa_invalid_url,
	xpa_invalid_proxy,
	xpa_invalid_authtype,
	xpa_libcurl_error,
	xpa_func_not_implemented,
	xpa_func_obsolete
}
txc_panic_t;

/* xdm buffer */
typedef struct txc_content_s
{
	char *data;
	char *type;
	size_t cursor;
	size_t size;
}
txc_content_t;

/* xdm operation */
typedef enum txc_oper_type_e
{
	op_create,
	op_replace, 
	op_fetch,
	op_delete
}
txc_oper_type_t;

/* xdm selection type */
typedef enum txc_selection_type_e
{
	sl_element,
	sl_document, 
	sl_attribute
}
txc_selection_type_t;

/* xdm node selector step as per rfc 4825 subclause 6.3 */
typedef enum txc_node_step_e
{
	by_name,
	by_pos,
	by_attr,
	by_pos_attr
}
txc_node_step_t;

/**
* Supported XCAP AUIDs 
*/
typedef enum xcap_auid_type_e
{
	ietf_xcap_caps,			/* RFC 4825 sublause 12 */
	ietf_resource_lists,	/* RFC 4825 sublause ... */
	ietf_rls_services,		/* */
	ietf_pres_rules,		/* */
	ietf_directory,			/* */

	oma_conv_history,		/* */
	oma_pres_rules,			/* */
	oma_directory,			/* */
	oma_deferred_list,		/* */
	oma_pres_content,		/* */
	oma_shared_groups		/* */
}
xcap_auid_type_t;

/* [auid<->content type] mapping */
typedef struct txc_auid_s
{
	xcap_auid_type_t type;
	char* name;
	char* description;
	char* content_type;
	char* document;
	int available;
}
txc_auid_t;
typedef tsk_list_t txc_auid_L_t; /* contains 'txc_auid_t' elements */
typedef txc_auid_t AUIDS_T[TXC_AUIDS_COUNT];

/* all supported auids */
static const AUIDS_T txc_auids =
{
	{ietf_xcap_caps,		TXC_AUID_CAPS,								"IETF server capabilities", TXC_MIME_TYPE_CAPS,							TXC_DOCUMENT_CAPS,			1},
	{ietf_resource_lists,	TXC_AUID_RLIST,							"IETF resource-list",		TXC_MIME_TYPE_RLIST,			TXC_DOCUMENT_RLIST,			0},
	{ietf_rls_services,		TXC_AUID_RLS,								"IETF RLS service",			TXC_MIME_TYPE_RLS,				TXC_DOCUMENT_RLS,			0},
	{ietf_pres_rules,		"pres-rules",								"IETF pres-rules",			"application/auth-policy+xml",				"pres-rules",		0},
	{ietf_directory,		"directory",								"IETF xdm directory",		"application/directory+xml",				"directory.xml",	0},

	{oma_conv_history,		"org.openmobilealliance.conv-history",		"OMA conversation history", "application/vnd.oma.im.history-list+xml",	"conv-history",		0},
	{oma_pres_rules,		"org.openmobilealliance.pres-rules",		"OMA presence rules",		"application/auth-policy+xml",				"pres-rules",		0},
	{oma_directory,			"org.openmobilealliance.xdm-directory",		"OMA xdm directory",		"application/vnd.oma.xdm-directory+xml",	"directory.xml",	0},
	{oma_deferred_list,		"org.openmobilealliance.deferred-list",		"OMA deferred list",		"application/vnd.oma.im.deferred-list+xml", "deferred-list",	0},
	{oma_pres_content,		"org.openmobilealliance.pres-content",		"OMA presence content",		"application/vnd.oma.pres-content+xml",		"oma_status-icon",	0},
	{oma_shared_groups,		"org.openmobilealliance.groups",			"OMA shared groups",		"application/vnd.oma.poc.groups+xml",		"index"}
};

/* xdm request */
typedef struct txc_request_s
{
	char* auid;	
	txc_content_t* content;
	char* url;
	char* http_accept;
	char* http_expect;
	//char http_error[CURL_ERROR_SIZE];
	txc_panic_t panic;
	
	long status;
	long timeout;
}
txc_request_t;


typedef struct txc_context_s
{
	char* xui;							/* RFC 4825 subclause 4: username*/
	char* password;						/* user password */
	char* proxy_host;					/* proxy host */
	int	  proxy_port;					/* proxy port */
	char* proxy_usr;					/* proxy user name */
	char* proxy_pwd;					/* proxy password */
	int   proxy_tunneling;				/* use proxy tunneling, default is false*/
	char* txc_root;						/* RFC 4825 subclause 6.1 */
	char* pragma;						/* HTTP pragma */
	char* user_agent;
	AUIDS_T auids;	/* contains list of all default auids */
	tsk_heap_t heap; /**< */

	CURL* easyhandle;					/* curl handle */
	int reuse_http_connection;			/* reuse the same easyhandle to send/receive data */
}
txc_context_t;

TINYXCAP_API void txc_content_init(txc_content_t *content);
TINYXCAP_API void txc_content_set(txc_content_t* content, const char *data, size_t size, const char *type);
TINYXCAP_API void txc_content_free(txc_content_t** content);

void txc_auid_init(txc_auid_t *auid);
TINYXCAP_API const txc_auid_t* txc_auid_findby_type(const AUIDS_T auids, xcap_auid_type_t type);
TINYXCAP_API const txc_auid_t* txc_auid_findby_name(const AUIDS_T auids, const char* name);
TINYXCAP_API int txc_auid_is_available(const AUIDS_T auids, xcap_auid_type_t type);
TINYXCAP_API void txc_auid_set_availability(AUIDS_T auids, xcap_auid_type_t type, int avail);
TINYXCAP_API void txc_auid_update(txc_context_t* context, xcap_auid_type_t type, const char* document);
void txc_auid_free(void **auid);

TINYXCAP_API void txc_context_init(txc_context_t* context);
TINYXCAP_API void txc_context_update_available_auids(txc_context_t *context, const tsk_list_t* avail_auids);
TINYXCAP_API void txc_context_free(txc_context_t** context);

TINYXCAP_API void txc_request_init(txc_request_t* request);
TINYXCAP_API void txc_request_free(txc_request_t** request);


TINYXCAP_API int txc_xcap_perform(txc_context_t* context, txc_request_t* request, txc_oper_type_t oper, txc_selection_type_t sel);

#endif /* _TINYXCAP_TXC_H_ */