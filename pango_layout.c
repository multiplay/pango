/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2008 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:  Michael Maclean <mgdm@php.net>                              |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_pango.h"

zend_class_entry *pango_ce_pangolayout;

/* {{{ proto pango_layout_object_new
   Creates a PangoLayout based on the CairoContext object */

PHP_FUNCTION(pango_cairo_create_layout)
{
	zval *context_zval = NULL;
	zend_class_entry *cairo_ce_cairocontext = php_cairo_get_context_ce();
	cairo_context_object *context_object;
	pango_layout_object *layout_object;

	PHP_PANGO_ERROR_HANDLING(TRUE)
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &context_zval, cairo_ce_cairocontext) == FAILURE)
	{
		PHP_PANGO_RESTORE_ERRORS(TRUE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(TRUE)

	context_object = (cairo_context_object *)zend_object_store_get_object(context_zval TSRMLS_CC);

	object_init_ex(return_value, pango_ce_pangolayout);
	layout_object = (pango_layout_object *)zend_object_store_get_object(return_value TSRMLS_CC);
	layout_object->layout = pango_cairo_create_layout(context_object->context);
	
	if(layout_object->layout == NULL) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Could not create the Pango layout");
		return;
	}
	
	/* We may want this later, so reference it and store */
	layout_object->cairo_context = context_zval;
	Z_ADDREF_P(context_zval);
}

/* }}} */

/* {{{ proto PangoLayout::__construct(CairoContext cr)
   Creates a PangoLayout based on the CairoContext object */

PHP_METHOD(PangoLayout, __construct)
{
	zval *context_zval = NULL;
	zend_class_entry *cairo_ce_cairocontext = php_cairo_get_context_ce();
	cairo_context_object *context_object;
	pango_layout_object *layout_object;

	PHP_PANGO_ERROR_HANDLING(TRUE)
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "O", &context_zval, cairo_ce_cairocontext) == FAILURE)
	{
		PHP_PANGO_RESTORE_ERRORS(TRUE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(TRUE)

	context_object = (cairo_context_object *)zend_object_store_get_object(context_zval TSRMLS_CC);

	layout_object = (pango_layout_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	layout_object->layout = pango_cairo_create_layout(context_object->context);
	
	if(layout_object->layout == NULL) {
		zend_throw_exception(pango_ce_pangoexception, "Could not create the Pango layout", 0 TSRMLS_CC);
		return;
	}
	
	/* We may want this later, so reference it and store */
	layout_object->cairo_context = context_zval;
	Z_ADDREF_P(context_zval);
}

/* }}} */

/* {{{ proto void pango_layout_set_text(PangoLayout layout, string text)
 	   proto void PangoLayout::setText(string text)
	   Sets the text of the layout. */
PHP_FUNCTION(pango_layout_set_text)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;
	const char *text;
	long text_len;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &layout_zval, pango_ce_pangolayout, &text, &text_len) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	pango_layout_set_text(layout_object->layout, text, text_len);
}

/* }}} */

/* {{{ proto string pango_layout_get_text(PangoLayout layout)
 	   proto string PangoLayout::getText(string text)
	   Gets the text currently in the layout */
PHP_FUNCTION(pango_layout_get_text)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;
	const char *text;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &layout_zval, pango_ce_pangolayout) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	if(text = pango_layout_get_text(layout_object->layout)) {
		RETURN_STRING(text, 1);
	}
	RETURN_FALSE;
}
/* }}} */

/* {{{ proto void pango_layout_set_markup(PangoLayout layout, string markup)
 	   proto void PangoLayout::setText(string markup)
	   Sets the markup of the layout. */
PHP_FUNCTION(pango_layout_set_markup)
{
	zval *layout_zval = NULL;
	pango_layout_object *layout_object;
	const char *markup;
	long markup_len;

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &layout_zval, pango_ce_pangolayout, &markup, &markup_len) == FAILURE) {
		PHP_PANGO_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)

	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	pango_layout_set_markup(layout_object->layout, markup, markup_len);
}

/* }}} */

/* {{{ proto void pango_cairo_update_layout(CairoContext cr, PangoLayout layout)
 	   proto void PangoLayout::updateLayout(CairoContext cr) 
	   Updates the private PangoContext of a PangoLayout to match the current transformation 
	   and target surface of a Cairo context. 
	   NB: PARAMS ARE REVERSED FROM NATIVE PANGO
	   */

PHP_FUNCTION(pango_cairo_update_layout)
{
	zval *layout_zval = NULL, *context_zval = NULL;
	pango_layout_object *layout_object;
	cairo_context_object *context_object;
	zend_class_entry *cairo_ce_cairocontext = php_cairo_get_context_ce();

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "OO", &layout_zval, pango_ce_pangolayout, &context_zval, cairo_ce_cairocontext) == FAILURE) {
	   PHP_PANGO_RESTORE_ERRORS(FALSE)
	   return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)	
	
	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	context_object = (cairo_context_object *)zend_object_store_get_object(context_zval TSRMLS_CC);
	pango_cairo_update_layout(context_object->context, layout_object->layout);
}
/* }}} */

/* {{{ proto void pango_cairo_show_layout(CairoContext cr, PangoLayout layout)
 	   proto void PangoLayout::showLayout(CairoContext cr) 
	   Draws a PangoLayoutLine in the specified cairo context.
	   NB: PARAMS ARE REVERSED FROM NATIVE PANGO
	   */

PHP_FUNCTION(pango_cairo_show_layout)
{
	zval *layout_zval = NULL, *context_zval = NULL;
	pango_layout_object *layout_object;
	cairo_context_object *context_object;
	zend_class_entry *cairo_ce_cairocontext = php_cairo_get_context_ce();

	PHP_PANGO_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "OO", &layout_zval, pango_ce_pangolayout, &context_zval, cairo_ce_cairocontext) == FAILURE) {
	   PHP_PANGO_RESTORE_ERRORS(FALSE)
	   return;
	}
	PHP_PANGO_RESTORE_ERRORS(FALSE)	
	
	layout_object = (pango_layout_object *)zend_object_store_get_object(layout_zval TSRMLS_CC);
	context_object = (cairo_context_object *)zend_object_store_get_object(context_zval TSRMLS_CC);
	pango_cairo_show_layout(context_object->context, layout_object->layout);
}
/* }}} */

/* {{{ Object creation/destruction functions */
static void pango_layout_object_destroy(void *object TSRMLS_DC)
{
    pango_layout_object *layout = (pango_layout_object *)object;
    zend_hash_destroy(layout->std.properties);
    FREE_HASHTABLE(layout->std.properties);

    if(layout->cairo_context) {
        Z_DELREF_P(layout->cairo_context);
        layout->cairo_context = NULL;
    }    

    if(layout->layout){
		g_object_unref(layout->layout);
    }    
    efree(object);
}

static zend_object_value pango_layout_object_new(zend_class_entry *ce TSRMLS_DC)
{
    zend_object_value retval;
    pango_layout_object *layout;
    zval *temp;

    layout = ecalloc(1, sizeof(pango_layout_object));

    layout->std.ce = ce;
	layout->layout = NULL;
	layout->cairo_context = NULL;

    ALLOC_HASHTABLE(layout->std.properties);
    zend_hash_init(layout->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
    zend_hash_copy(layout->std.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref,(void *) &temp, sizeof(zval *)); 
    retval.handle = zend_objects_store_put(layout, NULL, (zend_objects_free_object_storage_t)pango_layout_object_destroy, NULL TSRMLS_CC);
    retval.handlers = &pango_std_object_handlers;
    return retval;
}
/* }}} */

/* {{{ pango_layout_class_functions */
const zend_function_entry pango_layout_methods[] = {
	PHP_ME(PangoLayout, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME_MAPPING(setText, pango_layout_set_text, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getText, pango_layout_get_text, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(setMarkup, pango_layout_set_markup, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(updateLayout, pango_cairo_update_layout, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(showLayout, pango_cairo_show_layout, NULL, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(pango_layout)
{
	zend_class_entry layout_ce;

	INIT_CLASS_ENTRY(layout_ce, "PangoLayout", pango_layout_methods);
	pango_ce_pangolayout = zend_register_internal_class(&layout_ce TSRMLS_CC);
	pango_ce_pangolayout->create_object = pango_layout_object_new;

	return SUCCESS;
}

/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */