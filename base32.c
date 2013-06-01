/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: feifengxlq <feifengxlq@gmail.com>                                                             |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_base32.h"
#include <stdint.h>
#include <string.h>
/* If you declare any globals in php_base32.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(base32)
*/

/* True global resources - no need for thread safety here */
static int le_base32;

/* {{{ base32_functions[]
 *
 * Every user visible function must have an entry in base32_functions[].
 */
const zend_function_entry base32_functions[] = {
	PHP_FE(confirm_base32_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(base32_encode, NULL)
	PHP_FE(base32_decode, NULL)
	PHP_FE_END	/* Must be the last line in base32_functions[] */
};
/* }}} */

/* {{{ base32_module_entry
 */
zend_module_entry base32_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"base32",
	base32_functions,
	PHP_MINIT(base32),
	PHP_MSHUTDOWN(base32),
	PHP_RINIT(base32),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(base32),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(base32),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_BASE32
ZEND_GET_MODULE(base32)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("base32.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_base32_globals, base32_globals)
    STD_PHP_INI_ENTRY("base32.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_base32_globals, base32_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_base32_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_base32_init_globals(zend_base32_globals *base32_globals)
{
	base32_globals->global_value = 0;
	base32_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(base32)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(base32)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(base32)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(base32)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(base32)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "base32 support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
uint8_t *php_base32_decode(const uint8_t * encoded, int length, int *ret_length)
{
	int buffer = 0;
	int bitsLeft = 0;
	int count = 0;
  	uint8_t *result;
  	int bufSize = 0;
  	const uint8_t *ptr = encoded;
  	bufSize = (length*5+7)/8;
  	//printf("bufSize:%d\n", bufSize);
  	result = (uint8_t *)safe_emalloc( bufSize * sizeof(char), 1, 1);
  	for (; count < bufSize && *ptr; ++ptr) {
    	uint8_t ch = *ptr;
    	if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '-') {
      		continue;
    	}
    	buffer <<= 5;

    	// Deal with commonly mistyped characters
    	if (ch == '0') {
      		ch = 'O';
    	} else if (ch == '1') {
      		ch = 'L';
    	} else if (ch == '8') {
      	ch = 'B';
    	}

    	// Look up one base32 digit
    	if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
      		ch = (ch & 0x1F) - 1;
    	} else if (ch >= '2' && ch <= '7') {
      		ch -= '2' - 26;
    	} else {
    		//	printf("ch:%c\n", ch);
      		return NULL;
    	}

    	buffer |= ch;
    	bitsLeft += 5;
    	if (bitsLeft >= 8) {
      		result[count++] = buffer >> (bitsLeft - 8);
      		bitsLeft -= 8;
    	}
  		//printf("count:%d\n", count);
  	}
  	if (count < bufSize) {
    	result[count] = '\000';
  	}
  	*ret_length = count;
  	//printf("ret_length:%d\n", *ret_length);
  	return result;
}
uint8_t *php_base32_encode(const uint8_t *data, int length, int *ret_length) {
  	if (length < 0 || length > (1 << 28)) {
   		return NULL;
  	}
  	int bufSize;
  	bufSize = (8*length+4)/5;
  	//printf("bufSize %d\n", bufSize);
  	uint8_t *result;
  	result = (uint8_t *)safe_emalloc(bufSize * sizeof(uint8_t), 1, 1);
  	int count = 0;
  	if (length > 0) {
    	int buffer = data[0];
    	int next = 1;
    	int bitsLeft = 8;
    	while (count < bufSize && (bitsLeft > 0 || next < length)) {
	      	if (bitsLeft < 5) {
	        	if (next < length) {
	          		buffer <<= 8;
	          		buffer |= data[next++] & 0xFF;
	          		bitsLeft += 8;
	        	} else {
	          		int pad = 5 - bitsLeft;
	          		buffer <<= pad;
	          		bitsLeft += pad;
	        	}
	      	}
		    int index = 0x1F & (buffer >> (bitsLeft - 5));
		    bitsLeft -= 5;
		    result[count++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567"[index];
    	}
  	}
  	if (count < bufSize) {
    	result[count] = '\000';
  	}
  	*ret_length = count;
  	return result;
}
/* }}} */
/**
 * base32_encode
 */
PHP_FUNCTION(base32_encode)
{
	char *str;
	unsigned char *result;
	int str_len, ret_length;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len) == FAILURE) {
		return;
	}
	result = php_base32_encode((uint8_t*)str, str_len, &ret_length);
	if (result != NULL) {
		RETVAL_STRINGL((char*)result, ret_length, 0);
	} else {
		RETURN_FALSE;
	}
}
/**
 * base32_decode
 */
PHP_FUNCTION(base32_decode)
{
	char *str;
	unsigned char *result;
	zend_bool is_num = 0;
	int str_len, ret_length;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|b", &str, &str_len, &is_num) == FAILURE) {
		return;
	}
	result = php_base32_decode((uint8_t*)str, str_len, &ret_length);
	if (result != NULL) {
		RETVAL_STRINGL((char*)result, ret_length, 0);
	} else {
		RETURN_FALSE;
	}
}

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_base32_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_base32_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "base32", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
