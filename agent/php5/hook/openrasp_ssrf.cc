/*
 * Copyright 2017-2020 Baidu Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "openrasp_hook.h"
#include "openrasp_v8.h"
#include "utils/net.h"
#include "utils/url.h"
#include "hook/data/ssrf_object.h"
#include "hook/checker/v8_detector.h"
#include "hook/data/ssrf_redirect_object.h"

extern "C"
{
#ifdef PHP_WIN32
#include "win32/inet.h"
#include <winsock2.h>
#include <windows.h>
#include <Ws2tcpip.h>
#else
#include <netinet/in.h>
#if HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#include <netdb.h>
#ifdef HAVE_DNS_H
#include <dns.h>
#endif
#endif
}

void pre_global_curl_exec_ssrf(OPENRASP_INTERNAL_FUNCTION_PARAMETERS, zval *opt, zval *origin_url, zval **args);
void post_global_curl_exec_ssrf(OPENRASP_INTERNAL_FUNCTION_PARAMETERS, zval *origin_url, zval **args);

OPENRASP_HOOK_FUNCTION(curl_exec, SSRF)
{
    zval *opt = nullptr;
    MAKE_STD_ZVAL(opt);
    zval *origin_url = nullptr;
    MAKE_STD_ZVAL(origin_url);
    zval *args[2];
    if (zend_get_constant(ZEND_STRL("CURLINFO_EFFECTIVE_URL"), opt TSRMLS_CC))
    {
        if (!openrasp_check_type_ignored(SSRF TSRMLS_CC))
        {
            pre_global_curl_exec_ssrf(INTERNAL_FUNCTION_PARAM_PASSTHRU, SSRF, opt, origin_url, args);
        }
        origin_function(INTERNAL_FUNCTION_PARAM_PASSTHRU);
        if (!openrasp_check_type_ignored(SSRF_REDIRECT TSRMLS_CC))
        {
            post_global_curl_exec_ssrf(INTERNAL_FUNCTION_PARAM_PASSTHRU, SSRF_REDIRECT, origin_url, args);
        }
    }
    zval_ptr_dtor(&opt);
    zval_ptr_dtor(&origin_url);
}

void pre_global_curl_exec_ssrf(OPENRASP_INTERNAL_FUNCTION_PARAMETERS, zval *opt, zval *origin_url, zval **args)
{
    zval **zid;
    int argc = MIN(1, ZEND_NUM_ARGS());
    if (argc <= 0 ||
        zend_get_parameters_ex(argc, &zid) != SUCCESS ||
        Z_TYPE_PP(zid) != IS_RESOURCE)
    {
        return;
    }
    args[0] = *zid;
    args[1] = opt;
    if (openrasp_call_user_function(EG(function_table), nullptr, "curl_getinfo", origin_url, 2, args TSRMLS_CC))
    {
        plugin_ssrf_check(origin_url, "curl_exec" TSRMLS_CC);
    }
}

void post_global_curl_exec_ssrf(OPENRASP_INTERNAL_FUNCTION_PARAMETERS, zval *origin_url, zval **args)
{
    if (nullptr != origin_url &&
        Z_TYPE_P(origin_url) == IS_STRING &&
        Z_STRLEN_P(origin_url) > 0)
    {
        int curl_error;
        zval z_curl_error;
        if (openrasp_call_user_function(EG(function_table), nullptr, "curl_errno", &z_curl_error, 1, args TSRMLS_CC))
        {
            curl_error = Z_LVAL(z_curl_error);
            zval_dtor(&z_curl_error);
        }
        else
        {
            return;
        }

        zval effective_url;
        if (openrasp_call_user_function(EG(function_table), nullptr, "curl_getinfo", &effective_url, 2, args TSRMLS_CC))
        {
            zval *opt_http_code = nullptr;
            MAKE_STD_ZVAL(opt_http_code);
            if (zend_get_constant(ZEND_STRL("CURLINFO_HTTP_CODE"), opt_http_code TSRMLS_CC))
            {
                zval http_status;
                args[1] = opt_http_code;
                if (openrasp_call_user_function(EG(function_table), nullptr, "curl_getinfo", &http_status, 2, args TSRMLS_CC))
                {
                    if (Z_TYPE(http_status) == IS_LONG)
                    {
                        openrasp::data::SsrfRedirectObject ssrf_redirect_obj(origin_url, &effective_url, "curl_exec", curl_error, Z_LVAL(http_status));
                        openrasp::checker::V8Detector v8_detector(ssrf_redirect_obj, OPENRASP_HOOK_G(lru), OPENRASP_V8_G(isolate), OPENRASP_CONFIG(plugin.timeout.millis));
                        v8_detector.run();
                    }
                    zval_dtor(&http_status);
                }
            }
            zval_ptr_dtor(&opt_http_code);
            zval_dtor(&effective_url);
        }
    }
}

void plugin_ssrf_check(zval *file, const std::string &funtion_name TSRMLS_DC)
{
    openrasp::data::SsrfObject ssrf_obj(funtion_name, file);
    openrasp::checker::V8Detector v8_detector(ssrf_obj, OPENRASP_HOOK_G(lru), OPENRASP_V8_G(isolate), OPENRASP_CONFIG(plugin.timeout.millis));
    v8_detector.run();
}